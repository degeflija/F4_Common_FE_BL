/**
 * @file    my_delay.h
 * @brief   special delay macro
 * @author  Horst Rupp
 */

#ifndef MY_DELAY_H_
  #define MY_DELAY_H_

  #undef MYDEBDELAY
  #define MYDEBDELAY

  #ifdef MYDEBDELAY
    #define MDP_DEBUG_DELAY(x) vTaskDelay(x)
  #else
    #define MDP_DEBUG_DELAY(x) ((void)(x))
  #endif

#endif /* MY_DELAY_H_ */
