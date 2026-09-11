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
//  Inhaltlich identisch zu UNDER_DEBUGGER() aus System_Configuration.h
//  (F4_GenBL / F4_P_Util).
//
#ifndef ASSERT_DEBUGGER_ATTACHED
  #define ASSERT_DEBUGGER_ATTACHED()                                          \
            ( ( *( volatile uint32_t * ) 0xE000EDF0UL ) & 0x00000001UL )
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
