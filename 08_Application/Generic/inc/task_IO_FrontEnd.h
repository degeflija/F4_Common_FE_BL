/**
 * @file    task_FrontEnd.h
 * @brief   New Vario
 * @author  Horst Rupp
 */
//#include  "System_Configuration.h"
//#include  "FreeRTOS_wrapper.h"
//#include  "Generic_Application_Includes.h"
//
// *****************************************************************************
//
#ifndef  __task_FrontEnd_Hb
  #define  __task_FrontEnd_H

  #include  "Generic_Common.h"
  #include  "Generic_Includes.h"
  #include  "Generic_Const.h"
  #include  "Control_Tables.h"
  #include  "Portable_Utils.h"
  #include  "Button_IO_Lib.h"

#ifdef BUILD_AD57_FE
  #include  "LogNError_OutPut_Lib.h"
#endif
  //
  //

  extern  TIM_HandleTypeDef htim4;
  extern  TIM_HandleTypeDef htim5;

  //
  void  IO_FrontEnd              ( void * nix);
  void  create_task_IO_FrontEnd  ( void );

#endif
//
// *****************************************************************************
// The End
// *****************************************************************************
