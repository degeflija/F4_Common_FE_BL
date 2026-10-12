/**
 * @file    my_assert.h
 * @brief   special ASSERT macro
 * @author  Dr. Klaus Schaefer klaus.schaefer@h-da.de
 *
 * ---------------------------------------------------------------------------
 *  LINT-Fix September 2026  (Befunde A3 und A4)
 *
 *  Alte Fassung:   #define ASSERT(x) if((x)==0) asm("bkpt 0")
 *
 *  Problem 1 (A3) : BKPT loest auf dem Cortex-M4 einen HardFault aus, wenn
 *                   KEIN Debugger angeschlossen ist. DEBUG war fest auf 1
 *                   verdrahtet, also war jeder der rund 390 ASSERT-Aufrufe
 *                   auch im ausgelieferten Geraet scharf und konnte das
 *                   Geraet im Flug aufhaengen.
 *
 *  Problem 2 (A4) : Das nackte "if" ohne do/while(0) bindet ein nachfolgendes
 *                   "else" an das if des Makros statt an das des Aufrufers.
 *                   Heute ist keine Aufrufstelle davon betroffen (alle 392
 *                   geprueft), es bleibt aber eine Falle fuer spaeter.
 *
 *  Neue Fassung:
 *
 *    - Der BKPT wird nur noch ausgefuehrt, wenn tatsaechlich ein Debugger
 *      angeschlossen ist. Beim Debuggen aendert sich damit NICHTS -- man
 *      haelt weiterhin an genau derselben Stelle an. Im Feld laeuft das
 *      Geraet ueber die Stelle hinweg, statt hart auszusteigen.
 *
 *    - do { } while(0) macht das Makro zu einer echten Anweisung.
 *
 *    - DEBUG wird nur noch gesetzt, falls es nicht schon definiert ist
 *      (das Projekt uebergibt -DDEBUG). Verhalten bleibt unveraendert.
 * ---------------------------------------------------------------------------
 *  11.10.2026  Fault bei ASSERT in Tasks ohne Privileg (Horsts Variante B)
 *
 *  Problem : Die Frage "Debugger dran ?" liest das Register DHCSR
 *            (0xE000EDF0). Das darf nur privilegierter Code. In den
 *            restricted Tasks (MPU) loeste schon die Frage einen Bus Fault
 *            aus (BFSR 0x82, BFAR 0xE000EDF0) - auch im Flug ohne Debugger,
 *            das Geraet hing. Gefunden am 11.10.2026 mit dem FE allein am
 *            Bus : ASSERT ( l_Send_OK ) in task_CAN_Bus_Sender.c.
 *
 *  Loesung : Assert_Debugger_Attached () unterscheidet :
 *    - privilegiert (Interrupt, Handler, main, privilegierte Tasks) :
 *      DHCSR wird wie bisher direkt gelesen.
 *    - ohne Privileg : es gilt die Momentaufnahme
 *      g_Assert_Debugger_Attached, die main() beim Start mit
 *      Assert_Capture_Debugger_State () aus DHCSR holt. Die Variable liegt
 *      im COMMON_BLOCK, den alle restricted Tasks lesen duerfen.
 *  Einschraenkung : wird der Debugger erst NACH dem Start angehaengt,
 *  halten ASSERTs in Tasks ohne Privileg nicht an.
 *
 *  Die Variable ist in emergency.c des jeweiligen Projekts definiert
 *  (AD57_FE und AD57_BL).
 * ---------------------------------------------------------------------------
 */

#ifndef MY_ASSERT_H_
#define MY_ASSERT_H_

#include <stdint.h>

//
//  Debug Halting Control and Status Register (DHCSR), ARMv7-M, feste Adresse.
//  Bit 0 (C_DEBUGEN) ist gesetzt, solange ein Debugger angeschlossen ist.
//  Bewusst ueber die nackte Adresse und nicht ueber CoreDebug aus core_cm4.h,
//  damit dieser Header keine Include-Reihenfolge erzwingt.
//
//  11.10.2026 : DHCSR wird nur noch im privilegierten Code gelesen, siehe
//  Kopf dieser Datei.
//
#define   ASSERT_DHCSR_ADDRESS    0xE000EDF0UL

#ifdef __cplusplus
extern "C" {
#endif

//  Momentaufnahme "Debugger dran" vom Start, 0 = nein, 1 = ja.
//  Definiert in emergency.c, im COMMON_BLOCK.
//
extern volatile uint32_t g_Assert_Debugger_Attached;

//  true, wenn der Code gerade privilegiert laeuft : im Handler-Modus
//  ( IPSR != 0 ) immer, im Thread-Modus nur mit CONTROL.nPRIV = 0.
//  MRS auf IPSR und CONTROL ist auch ohne Privileg erlaubt.
//
static inline uint32_t Assert_Is_Privileged ( void )
{
  uint32_t l_ipsr;
  uint32_t l_control;

  __asm volatile ( "mrs %0, ipsr"    : "=r" ( l_ipsr ) );
  __asm volatile ( "mrs %0, control" : "=r" ( l_control ) );

  return ( ( l_ipsr != 0 ) || ( ( l_control & 1UL ) == 0 ) );
}

//  Einmal aus main() aufrufen, solange main() noch privilegiert laeuft
//  ( vor dem Start des Schedulers ).
//
static inline void Assert_Capture_Debugger_State ( void )
{
  g_Assert_Debugger_Attached =
      ( *( volatile uint32_t * ) ASSERT_DHCSR_ADDRESS ) & 0x00000001UL;
}

static inline uint32_t Assert_Debugger_Attached ( void )
{
  if ( Assert_Is_Privileged () )
    return ( *( volatile uint32_t * ) ASSERT_DHCSR_ADDRESS ) & 0x00000001UL;

  return g_Assert_Debugger_Attached;
}

#ifdef __cplusplus
}
#endif

#ifndef ASSERT_DEBUGGER_ATTACHED
  #define ASSERT_DEBUGGER_ATTACHED()    Assert_Debugger_Attached ()
#endif

#undef ASSERT

#ifndef DEBUG
  #define DEBUG 1
#endif

#if DEBUG==1

  #define ASSERT(x)                                                           \
            do                                                                \
            {                                                                 \
              if ( ( x ) == 0 )                                               \
              {                                                               \
                if ( ASSERT_DEBUGGER_ATTACHED() )                             \
                {                                                             \
                  __asm volatile ( "bkpt 0" );                                \
                }                                                             \
              }                                                               \
            } while ( 0 )

#else

  #define ASSERT(x)   do { ( void ) ( x ); } while ( 0 )

#endif

//
//  Fuer die rund 90 Stellen, die BKPT direkt per asm() setzen: dieselbe
//  Absicherung als eigenstaendiges Makro, damit sie schrittweise umgestellt
//  werden koennen ( asm("bkpt 0");  ->  TRAP_IF_DEBUGGER(); ).
//  In den Fault-Handlern (emergency.c) bewusst NICHT verwenden -- dort ist
//  der harte Stopp gewollt.
//
#define TRAP_IF_DEBUGGER()                                                    \
          do                                                                  \
          {                                                                   \
            if ( ASSERT_DEBUGGER_ATTACHED() )                                 \
            {                                                                 \
              __asm volatile ( "bkpt 0" );                                    \
            }                                                                 \
          } while ( 0 )

#endif /* MY_ASSERT_H_ */
