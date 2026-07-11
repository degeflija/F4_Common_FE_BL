/*
 * can.h
 *
 *  C-Ersatz für candriver.h / candriver.cpp (Klaus Schaefer C++ Rahmen)
 *  Umbau: Mai 2026
 *
 *  CubeMX übernimmt: GPIO Init, Clock, HAL_CAN_Init → hcan1
 *  can.c übernimmt:  Filter, Interrupts, Queues, Bus-Off Logik
 *
 *  Aufruf aus main.c nach MX_CAN1_Init():
 *    CAN1_Init ( &hcan1 );
 *
 *  Senden:
 *    CAN1_Send ( &hcan1, p, wait_ms );
 *
 *  Empfangen:
 *    CAN1_Receive ( &hcan1, &p, wait_ms );
 */

#ifndef CAN_H_
#define CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include <stdbool.h>

extern CAN_HandleTypeDef hcan1;   /* von CubeMX generiert in main.c */

/* -----------------------------------------------------------------------
 * CAN Paket — identisch mit CAN_Packet_t aus candriver.h
 * CAN_distributor.cpp kann damit weiter unverändert arbeiten
 * ----------------------------------------------------------------------- */
typedef struct
{
  uint16_t id;
  uint16_t dlc;
  union
  {
    uint8_t  data_b[8];
    int8_t   data_sb[8];
    uint16_t data_h[4];
    int16_t  data_sh[4];
    uint32_t data_w[2];
    int32_t  data_sw[2];
    float    data_f[2];
    double   data_d;
    uint64_t data_l;
  };
} CAN_Packet_t;

/* -----------------------------------------------------------------------
 * Konfiguration
 * ----------------------------------------------------------------------- */
#define CAN_RX_QUEUE_LEN        20
#define CAN_TX_QUEUE_LEN        20
#define CAN_BUS_OFF_RESET_MS    200     /* 200ms bis Re-Initialize nach Bus-Off */

/* -----------------------------------------------------------------------
 * Öffentliche Funktionen — alle mit CAN1_ Präfix
 * ----------------------------------------------------------------------- */
void          CAN1_Init     ( CAN_HandleTypeDef *hcan );
uint32_t      CAN1_Send     ( CAN_HandleTypeDef *hcan, CAN_Packet_t p, uint32_t wait_ms );
bool          CAN1_Receive  ( CAN_HandleTypeDef *hcan, CAN_Packet_t *p, uint32_t wait_ms );
bool          CAN1_IsLocked ( void );
QueueHandle_t CAN1_GetRxQueue ( void );   /* für CAN_distributor.cpp */

/* -----------------------------------------------------------------------
 * Kompatibilitäts-Wrapper für CAN_distributor.cpp
 * (solange dieser noch C++ ist)
 * ----------------------------------------------------------------------- */
static inline QueueHandle_t get_RX_queue ( void )
{
    return CAN1_GetRxQueue();
}

static inline uint32_t CAN_send ( CAN_Packet_t p, uint32_t wait )
{
    return CAN1_Send ( &hcan1, p, wait );
}

#ifdef __cplusplus
}
#endif

#endif /* CAN_H_ */
