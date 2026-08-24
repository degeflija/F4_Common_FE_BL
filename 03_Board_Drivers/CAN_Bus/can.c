/*
 * can.c
 *
 *  C-Ersatz fuer candriver.cpp (Klaus Schaefer C++ Rahmen)
 *  Umbau: Mai 2026
 *
 *  Aufgabenteilung:
 *    CubeMX / MX_CAN1_Init()  -> GPIO, Clock, HAL_CAN_Init, hcan1
 *    can.c / CAN1_Init()      -> Filter, Interrupts, Queues, Bus-Off Timer
 *
 *  Aufruf in main.c:
 *    MX_CAN1_Init();           // CubeMX generiert
 *    CAN1_Init ( &hcan1 );     // diese Datei
 *
 *  Was uebernommen wurde von candriver.cpp:
 *    - Bus-Off: locked-Flag + Timer + Re-Initialize
 *    - TX-Queue mit ISR-getriebenem Senden (Klaus' elegante Loesung)
 *    - CAN1_RX0_IRQHandler: direkte Registerlesezugriffe
 *    - CAN1_TX_IRQHandler:  ISR-getriebenes Senden aus TX-Queue
 *    - CAN1_SCE_IRQHandler: Bus-Off -> locked + Timer
 *
 *  ---------------------------------------------------------------------
 *  Korrektur August 2026 -- Dauer-Reinit-Zyklus
 *  ---------------------------------------------------------------------
 *  Symptom: FE<->Audio lief staendig in Timeouts, CAN war ca. 2x pro
 *  Sekunde fuer 50..549ms komplett tot. Breakpoint im SCE-Handler wurde
 *  erreicht, CAN1->ESR war dabei aber 0 (kein LEC, kein EWGF/EPVF/BOFF,
 *  TEC=REC=0) -- es gab also gar keinen Fehler.
 *
 *  Drei Ursachen, die sich gegenseitig am Leben hielten:
 *
 *  (1) CAN1_SCE_IRQHandler hat weder MSR noch ESR ausgewertet, sondern
 *      bedingungslos IER=0 gesetzt und s_locked=true. Damit war auch der
 *      RX-Interrupt aus -- der Bus war in BEIDEN Richtungen tot, nicht
 *      nur beim Senden.
 *
 *  (2) CAN1_configure hat neben BOFIE auch LECIE, EWGIE und EPVIE
 *      aktiviert. LEC feuert bei JEDEM einzelnen fehlerhaften Bit,
 *      EWGF schon ab Fehlerzaehler 96 -- beides Zustaende, aus denen
 *      sich bxCAN von selbst erholt und die keine Recovery brauchen.
 *      Nur Bus-Off (BOFF) braucht ein Re-Initialize.
 *
 *  (3) Kein NVIC_ClearPendingIRQ vor NVIC_EnableIRQ. HAL_CAN_DeInit()
 *      setzt CAN_MCR_RESET; laeuft dabei noch eine Uebertragung, setzt
 *      die Hardware ERRI und das NVIC-Pending-Bit rastet ein. Das
 *      anschliessende "CANx->MSR = CAN_MSR_ERRI_Msk" loescht nur das
 *      Peripherie-Flag, nicht das NVIC-Pending-Bit. NVIC_EnableIRQ
 *      springt dann sofort in den SCE-Handler -- mit sauberem ESR,
 *      weil die Peripherie zwischenzeitlich resettet wurde.
 *      -> sperren -> Timer -> CAN1_configure -> DeInit -> von vorn.
 *
 *  Massnahmen:
 *    - SCE-Handler wertet ESR aus und sperrt NUR bei echtem BOFF.
 *      Alles andere wird quittiert, gezaehlt und ignoriert.
 *    - Nur BOFIE (+ERRIE als Sammel-Enable) aktiv, kein LECIE/EWGIE/EPVIE.
 *    - NVIC-IRQs vor dem DeInit ab, Pending-Bits nach dem Konfigurieren
 *      geloescht, erst dann wieder an.
 *    - LEC wird beim Quittieren auf 0b111 ("set by software") gesetzt,
 *      damit ein alter Fehlercode nicht stehenbleibt.
 *    - Diagnosezaehler s_sce_* / s_tx_dropped_cnt, im Debugger lesbar.
 */

#include "can.h"
#include "main.h"

/* -----------------------------------------------------------------------
 * Hardware -- CAN Instanz
 * ----------------------------------------------------------------------- */
#define CANx    CAN1

/* Fehler-Interrupts, die wir tatsaechlich brauchen.
 * ERRIE ist das Sammel-Enable fuer den SCE-Interrupt, BOFIE waehlt die
 * einzige Bedingung aus, die eine Recovery erfordert.
 * LECIE / EWGIE / EPVIE bewusst NICHT -- siehe Kopfkommentar (2).        */
#define CAN1_ERROR_IT_MASK      ( CAN_IER_ERRIE | CAN_IER_BOFIE )

/* -----------------------------------------------------------------------
 * Interne Zustandsvariablen -- muessen in common_data liegen,
 * da restricted Tasks (MPU) nur auf diesen Bereich zugreifen duerfen.
 * In Klaus' candriver.cpp war CAN_driver selbst COMMON deklariert;
 * hier ersetzen einzelne COMMON-Variablen diese Rolle.
 * ----------------------------------------------------------------------- */
static COMMON CAN_HandleTypeDef *s_hcan        = NULL;
static COMMON QueueHandle_t      s_RX_queue    = NULL;
static COMMON QueueHandle_t      s_TX_queue    = NULL;
static COMMON TimerHandle_t      s_reset_timer = NULL;
static COMMON volatile bool      s_locked      = true;

/* -----------------------------------------------------------------------
 * Diagnosezaehler -- nur zum Mitlesen im Debugger, keine Funktion.
 *   s_sce_bus_off_cnt   : echte Bus-Off Ereignisse (Recovery gelaufen)
 *   s_sce_ignored_cnt   : SCE-Interrupt ohne BOFF (Warnung o. spurious)
 *   s_last_esr          : ESR-Inhalt beim letzten SCE-Interrupt
 *   s_tx_dropped_cnt    : Pakete, die waehrend der Sperre verworfen wurden
 * ----------------------------------------------------------------------- */
static COMMON volatile uint32_t  s_sce_bus_off_cnt = 0;
static COMMON volatile uint32_t  s_sce_ignored_cnt = 0;
static COMMON volatile uint32_t  s_last_esr        = 0;
static COMMON volatile uint32_t  s_tx_dropped_cnt  = 0;

/* -----------------------------------------------------------------------
 * Vorwaerts-Deklarationen
 * ----------------------------------------------------------------------- */
static void CAN1_configure ( void );
static bool CAN1_send_packet ( const CAN_Packet_t *msg );

/* -----------------------------------------------------------------------
 * Timer-Callback -- nach Bus-Off: vollstaendiges Re-Initialize
 * ----------------------------------------------------------------------- */
static void CAN1_reset_timer_callback ( TimerHandle_t xTimer )
{
    (void) xTimer;
    CAN1_configure();
}

/* -----------------------------------------------------------------------
 * CAN1_Init -- aus main.c nach MX_CAN1_Init() aufrufen
 *
 *   MX_CAN1_Init();        // CubeMX: GPIO, Clock, HAL_CAN_Init
 *   CAN1_Init ( &hcan1 );  // diese Funktion: Filter, IRQ, Queues
 * ----------------------------------------------------------------------- */
void CAN1_Init ( CAN_HandleTypeDef *hcan )
{
    s_hcan = hcan;

    s_RX_queue = xQueueCreate ( CAN_RX_QUEUE_LEN, sizeof ( CAN_Packet_t ) );
    configASSERT ( s_RX_queue != NULL );

    s_TX_queue = xQueueCreate ( CAN_TX_QUEUE_LEN, sizeof ( CAN_Packet_t ) );
    configASSERT ( s_TX_queue != NULL );

    s_reset_timer = xTimerCreate (
        "CAN1_RST",
        pdMS_TO_TICKS ( CAN_BUS_OFF_RESET_MS ),
        pdFALSE,                            /* kein Auto-Reload */
        NULL,
        CAN1_reset_timer_callback
    );
    configASSERT ( s_reset_timer != NULL );

    CAN1_configure();
}

/* -----------------------------------------------------------------------
 * CAN1_configure -- Filter, Interrupts, Start
 * Wird bei Init und nach Bus-Off Re-Initialize aufgerufen
 * Nach Bus-Off: HAL_CAN_DeInit + HAL_CAN_Init -> dann hierher
 * ----------------------------------------------------------------------- */
static void CAN1_configure ( void )
{
    /* Solange umkonfiguriert wird, darf nichts gesendet werden.           */
    s_locked = true;

    /* NVIC ZUERST abschalten.
     * HAL_CAN_DeInit() weiter unten setzt CAN_MCR_RESET. Laeuft dabei noch
     * eine Uebertragung, meldet die Hardware einen Fehler -> ERRI -> das
     * NVIC-Pending-Bit rastet ein und wuerde beim spaeteren EnableIRQ
     * sofort einen Interrupt ausloesen, obwohl die Peripherie danach
     * blitzsauber ist (ESR == 0). Genau das war der Dauerzyklus.          */
    NVIC_DisableIRQ ( CAN1_RX0_IRQn );
    NVIC_DisableIRQ ( CAN1_TX_IRQn  );
    NVIC_DisableIRQ ( CAN1_SCE_IRQn );

    /* GPIO fuer CAN1 auf AD57_FE Hardware: PA11 = RX, PA12 = TX, AF9
     * Wird bei Init und bei jedem Bus-Off Re-Initialize neu gesetzt.       */
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef g = { 0 };
    g.Mode      = GPIO_MODE_AF_PP;
    g.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    g.Pull      = GPIO_PULLUP;
    g.Alternate = GPIO_AF9_CAN1;
    g.Pin       = GPIO_PIN_11 | GPIO_PIN_12;   /* PA11=RX, PA12=TX */
    HAL_GPIO_Init( GPIOA, &g );

    /* vollstaendiges DeInit/Init mit bestehender hcan1-Konfiguration */
    HAL_CAN_DeInit  ( s_hcan );
    HAL_CAN_Init    ( s_hcan );

    /* Filter: alle IDs durchlassen -- identisch mit candriver.cpp */
    CAN_FilterTypeDef sFilterConfig = { 0 };
    sFilterConfig.FilterBank           = 0;
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh         = 0x0000;
    sFilterConfig.FilterIdLow          = 0x0000;
    sFilterConfig.FilterMaskIdHigh     = 0x0000;
    sFilterConfig.FilterMaskIdLow      = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if ( HAL_CAN_ConfigFilter ( s_hcan, &sFilterConfig ) != HAL_OK )
        Error_Handler();

    if ( HAL_CAN_Start ( s_hcan ) != HAL_OK )
        Error_Handler();

    /* Peripherie-Flags quittieren, BEVOR die Interrupts scharf werden.
     * ERRI ist rc_w1. LEC wird auf 0b111 ("set by software") gesetzt,
     * damit kein alter Fehlercode stehenbleibt.                          */
    CANx->MSR  = CAN_MSR_ERRI_Msk;
    CANx->ESR  = CAN_ESR_LEC;

    /* Interrupts in der Peripherie aktivieren */
    CANx->IER |= CAN_IT_RX_FIFO0_MSG_PENDING;
    CANx->IER |= CAN1_ERROR_IT_MASK;

    /* NVIC -- identisch mit candriver.cpp Prioritaeten.
     * Pending-Bits erst JETZT loeschen: alles, was waehrend DeInit/Init
     * aufgelaufen ist, gehoert zur alten Sitzung und ist erledigt.       */
    uint32_t pg = NVIC_GetPriorityGrouping();

    NVIC_ClearPendingIRQ ( CAN1_RX0_IRQn );
    NVIC_ClearPendingIRQ ( CAN1_TX_IRQn  );
    NVIC_ClearPendingIRQ ( CAN1_SCE_IRQn );

    NVIC_SetPriority ( CAN1_RX0_IRQn, NVIC_EncodePriority ( pg, 15, 0 ) );
    NVIC_EnableIRQ   ( CAN1_RX0_IRQn );

    NVIC_SetPriority ( CAN1_TX_IRQn,  NVIC_EncodePriority ( pg, 15, 0 ) );
    NVIC_EnableIRQ   ( CAN1_TX_IRQn  );

    NVIC_SetPriority ( CAN1_SCE_IRQn, NVIC_EncodePriority ( pg, 14, 0 ) );
    NVIC_EnableIRQ   ( CAN1_SCE_IRQn );

    s_locked = false;   /* Bus freigeben */
}

/* -----------------------------------------------------------------------
 * CAN1_send_packet -- direkte Registerlesezugriffe wie Klaus
 * ----------------------------------------------------------------------- */
static bool CAN1_send_packet ( const CAN_Packet_t *msg )
{
    uint8_t txMailbox;

    if      ( CANx->TSR & CAN_TSR_TME0 ) txMailbox = 0;
    else if ( CANx->TSR & CAN_TSR_TME1 ) txMailbox = 1;
    else if ( CANx->TSR & CAN_TSR_TME2 ) txMailbox = 2;
    else return false;

    CANx->sTxMailBox[txMailbox].TIR  &= CAN_TI0R_TXRQ;
    CANx->sTxMailBox[txMailbox].TIR  |= (uint32_t)msg->id << 21;
    CANx->sTxMailBox[txMailbox].TDTR &= 0xFFFFFFF0UL;
    CANx->sTxMailBox[txMailbox].TDTR |= msg->dlc;
    CANx->sTxMailBox[txMailbox].TDLR  = msg->data_w[0];
    CANx->sTxMailBox[txMailbox].TDHR  = msg->data_w[1];
    CANx->sTxMailBox[txMailbox].TIR  |= CAN_TI0R_TXRQ;

    return true;
}

/* -----------------------------------------------------------------------
 * CAN1_Send -- aus Task-Kontext
 *
 *  HINWEIS zum Rueckgabewert bei s_locked:
 *  Es wird weiterhin "true" gemeldet, obwohl das Paket verworfen wird.
 *  Das ist bewusst so belassen -- die Aufrufer in task_CAN_Bus_Sender.c
 *  haengen ein ASSERT ( l_Send_OK ) daran, das sonst bei jedem echten
 *  Bus-Off das System anhalten wuerde. Der Verlust ist stattdessen ueber
 *  s_tx_dropped_cnt sichtbar. Wer den ehrlichen Rueckgabewert will,
 *  aendert die eine Zeile auf "return false" UND entfernt vorher die
 *  betreffenden ASSERTs im Sender.
 * ----------------------------------------------------------------------- */
uint32_t CAN1_Send ( CAN_HandleTypeDef *hcan, CAN_Packet_t p, uint32_t wait_ms )
{
    (void) hcan;   /* hcan1 implizit ueber s_hcan */

    if ( s_locked )
    {
        s_tx_dropped_cnt++;
        return true;    /* still ignorieren -- siehe Hinweis oben */
    }

    CANx->IER &= ~CAN_IT_TX_MAILBOX_EMPTY;

    if ( CAN1_send_packet ( &p ) )
    {
        CANx->IER |= CAN_IT_TX_MAILBOX_EMPTY;
        return true;
    }

    bool ret = ( xQueueSend ( s_TX_queue, &p,
                              pdMS_TO_TICKS ( wait_ms ) ) == pdTRUE );

    CANx->IER |= CAN_IT_TX_MAILBOX_EMPTY;
    return (uint32_t) ret;
}

/* -----------------------------------------------------------------------
 * CAN1_Receive -- blockierend, aus Task-Kontext
 * ----------------------------------------------------------------------- */
bool CAN1_Receive ( CAN_HandleTypeDef *hcan, CAN_Packet_t *p, uint32_t wait_ms )
{
    (void) hcan;
    TickType_t ticks = ( wait_ms == portMAX_DELAY )
                       ? portMAX_DELAY
                       : pdMS_TO_TICKS ( wait_ms );
    return ( xQueueReceive ( s_RX_queue, p, ticks ) == pdTRUE );
}

/* -----------------------------------------------------------------------
 * CAN1_IsLocked
 * ----------------------------------------------------------------------- */
bool CAN1_IsLocked ( void )
{
    return s_locked;
}

/* -----------------------------------------------------------------------
 * CAN1_GetRxQueue -- fuer CAN_distributor.cpp Kompatibilitaet
 * ----------------------------------------------------------------------- */
QueueHandle_t CAN1_GetRxQueue ( void )
{
    return s_RX_queue;
}

/* -----------------------------------------------------------------------
 * ISR: CAN1_RX0_IRQHandler
 * ----------------------------------------------------------------------- */
void CAN1_RX0_IRQHandler ( void )
{
    CAN_Packet_t msg;
    BaseType_t   xHigherPriorityTaskWoken = pdFALSE;

    msg.id        = 0x07FFU & (uint16_t)( CANx->sFIFOMailBox[0].RIR >> 21 );
    msg.dlc       = (uint8_t)( 0x0FU & CANx->sFIFOMailBox[0].RDTR );
    msg.data_w[0] = CANx->sFIFOMailBox[0].RDLR;
    msg.data_w[1] = CANx->sFIFOMailBox[0].RDHR;

    xQueueSendFromISR ( s_RX_queue, &msg, &xHigherPriorityTaskWoken );

    CANx->RF0R |= CAN_RF0R_RFOM0;

    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}

/* -----------------------------------------------------------------------
 * ISR: CAN1_TX_IRQHandler
 * ----------------------------------------------------------------------- */
void CAN1_TX_IRQHandler ( void )
{
    CAN_Packet_t msg;
    BaseType_t   xHigherPriorityTaskWoken = pdFALSE;

    if ( xQueueReceiveFromISR ( s_TX_queue, &msg,
                                &xHigherPriorityTaskWoken ) == pdTRUE )
    {
        CAN1_send_packet ( &msg );
    }
    else
    {
        CANx->IER &= ~CAN_IT_TX_MAILBOX_EMPTY;
    }

    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}

/* -----------------------------------------------------------------------
 * ISR: CAN1_SCE_IRQHandler -- Bus-Off Erkennung
 *
 *  Wertet jetzt ESR aus, bevor irgendetwas gesperrt wird. Nur ein echtes
 *  Bus-Off (BOFF) loest die Recovery aus. Alles andere -- Warnung,
 *  Error-Passive, Einzelbitfehler oder ein spurious Interrupt aus einem
 *  stehengebliebenen NVIC-Pending-Bit -- wird quittiert, gezaehlt und
 *  ignoriert; bxCAN erholt sich davon selbst.
 * ----------------------------------------------------------------------- */
void CAN1_SCE_IRQHandler ( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    uint32_t l_esr = CANx->ESR;
    s_last_esr = l_esr;

    /* Peripherie-Flag quittieren (ERRI ist rc_w1) und LEC neutralisieren,
     * damit ein alter Fehlercode nicht stehenbleibt.                     */
    CANx->MSR = CAN_MSR_ERRI_Msk;
    CANx->ESR = CAN_ESR_LEC;

    if ( ( l_esr & CAN_ESR_BOFF ) == 0U )
    {
        /* Kein Bus-Off -> Bus bleibt in Betrieb, nichts wird gesperrt. */
        s_sce_ignored_cnt++;
        portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
        return;
    }

    /* ------------------------- echtes Bus-Off ------------------------- */

    s_sce_bus_off_cnt++;

    CANx->IER = 0;
    s_locked  = true;

    /* Stagger: Timer-Periode pro Geraet verschieden (UID-basiert: 50..549 ms).
     * Verhindert synchronen Re-Init beider Peers nach Bus-Off.
     * Liest die 96-Bit Hardware-UID direkt aus dem STM32-Register --
     * g_OWN_UNIQUE_ID war nie beschrieben (immer 0), wodurch JEDES
     * Geraet exakt 50ms erhielt und der Stagger faktisch wirkungslos war.
     * UID_BASE ist pro Chip einzigartig -> echte Streuung 50..549ms.
     * xTimerChangePeriodFromISR setzt die Periode UND startet den Timer.    */
    uint32_t l_uid = *(volatile uint32_t *)UID_BASE;
    TickType_t ticks = pdMS_TO_TICKS ( 50 + ( l_uid % 500 ) );
    xTimerChangePeriodFromISR ( s_reset_timer, ticks, &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}
