/**
  * ############################################################################
  * @file     Timer_Lib.h
  * @brief    New Vario
  * @author   Horst Rupp
  * @brief    This library contains all portable basic functions.
  * ############################################################################
  */

#ifndef __Timer_Lib_H
	#define __Timer_Lib_H
	#include  "Generic_Includes.h"
  #include  "Generic_Common.h"

  #ifdef MDP_TIMER_LIB

    #define c_length_timer_array  10

    enum
    {
      cc_timer_0 = 0,
      cc_timer_1,
      cc_timer_2,
      cc_timer_3,
      cc_timer_4,
      cc_timer_5,
      cc_timer_6,
      cc_timer_7,
      cc_timer_8,
      cc_timer_9,
    };

    extern uint32_t Timer_Array_Current_Value[];
    extern uint32_t Timer_Array_Last_Value[];

    // *****************************************************************************
    //
    void  Timer_Set               ( uint8_t index );
    void  Timer_Get_Current_Value ( uint8_t index );

  #endif

#endif

	// ************************************************************************** **
// End of File
// ****************************************************************************
