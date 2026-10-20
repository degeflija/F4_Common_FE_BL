/**
 * @file    my_assert.h
 * @brief   special ASSERT macro
 * @author  Dr. Klaus Schaefer klaus.schaefer@h-da.de
 *
 * ---------------------------------------------------------------------------
 *  LINT fix September 2026  (findings A3 and A4)
 *
 *  Old version:    #define ASSERT(x) if((x)==0) asm("bkpt 0")
 *
 *  Problem 1 (A3) : On the Cortex-M4, BKPT causes a HardFault when NO
 *                   debugger is attached. DEBUG was hard-wired to 1, so
 *                   each of the about 390 ASSERT calls was also armed in
 *                   the shipped device and could hang the device in
 *                   flight.
 *
 *  Problem 2 (A4) : The bare "if" without do/while(0) binds a following
 *                   "else" to the if of the macro, not to the if of the
 *                   caller. Today no call site is affected (all 392
 *                   checked), but it stays a trap for later.
 *
 *  New version:
 *
 *    - BKPT now runs only when a debugger is really attached. So NOTHING
 *      changes while debugging -- you still stop at exactly the same
 *      place. In the field the device runs past that place instead of
 *      stopping hard.
 *
 *    - do { } while(0) makes the macro a real statement.
 *
 *    - DEBUG is now set only if it is not defined yet
 *      (the project passes -DDEBUG). The behavior stays the same.
 * ---------------------------------------------------------------------------
 *  11.10.2026  Fault on ASSERT in tasks without privilege (Horst's Variant B)
 *
 *  Problem : The check "debugger attached ?" reads the register DHCSR
 *            (0xE000EDF0). Only privileged code may do this. In the
 *            restricted tasks (MPU) the check alone caused a Bus Fault
 *            (BFSR 0x82, BFAR 0xE000EDF0) - also in flight without a
 *            debugger, and the device hung. Found on 11.10.2026 with the
 *            FE alone on the bus : ASSERT ( l_Send_OK ) in
 *            task_CAN_Bus_Sender.c.
 *
 *  Solution : Assert_Debugger_Attached () makes a difference :
 *    - privileged (interrupt, handler, main, privileged tasks) :
 *      DHCSR is read directly, as before.
 *    - without privilege : the snapshot g_Assert_Debugger_Attached is
 *      used. main() takes it from DHCSR at start-up with
 *      Assert_Capture_Debugger_State (). The variable lives in the
 *      COMMON_BLOCK, which all restricted tasks may read.
 *  Limitation : if the debugger is attached only AFTER start-up,
 *  ASSERTs in tasks without privilege do not stop.
 *
 *  The variable is defined in emergency.c of each project
 *  (AD57_FE and AD57_BL).
 * ---------------------------------------------------------------------------
 */

#ifndef MY_ASSERT_H_
#define MY_ASSERT_H_

#include <stdint.h>

//
//  Debug Halting Control and Status Register (DHCSR), ARMv7-M, fixed address.
//  Bit 0 (C_DEBUGEN) is set as long as a debugger is attached.
//  We use the bare address on purpose and not CoreDebug from core_cm4.h,
//  so this header does not force an include order.
//
//  11.10.2026 : DHCSR is now read only in privileged code, see the
//  head of this file.
//
#define   ASSERT_DHCSR_ADDRESS    0xE000EDF0UL

#ifdef __cplusplus
extern "C" {
#endif

//  Snapshot "debugger attached" from start-up, 0 = no, 1 = yes.
//  Defined in emergency.c, in the COMMON_BLOCK.
//
extern volatile uint32_t g_Assert_Debugger_Attached;

//  true if the code is running privileged right now : always in handler
//  mode ( IPSR != 0 ), in thread mode only with CONTROL.nPRIV = 0.
//  MRS on IPSR and CONTROL is allowed even without privilege.
//
static inline uint32_t Assert_Is_Privileged ( void )
{
  uint32_t l_ipsr;
  uint32_t l_control;

  __asm volatile ( "mrs %0, ipsr"    : "=r" ( l_ipsr ) );
  __asm volatile ( "mrs %0, control" : "=r" ( l_control ) );

  return ( ( l_ipsr != 0 ) || ( ( l_control & 1UL ) == 0 ) );
}

//  Call once from main(), while main() still runs privileged
//  ( before the scheduler starts ).
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
//  For the about 90 places that set BKPT directly with asm(): the same
//  protection as a separate macro, so they can be changed step by step
//  ( asm("bkpt 0");  ->  TRAP_IF_DEBUGGER(); ).
//  Do NOT use it in the fault handlers (emergency.c) -- there the hard
//  stop is wanted.
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
