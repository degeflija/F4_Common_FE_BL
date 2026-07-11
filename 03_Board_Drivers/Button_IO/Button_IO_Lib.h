/**
 * ###########################################################################
 * @file     Button_IO_Lib.h
 * @brief    New Vario
 * @author   Horst Rupp
 * ############################################################################
 */
//
//  ****************************************************************************
//
// Includes
//
#ifndef __Button_IO_Lib_H
  #define __Button_IO_Lib_H

  #include  "Generic_Common.h"
  #include  "Generic_Includes.h"
  //#include  "Control_Tables.h"
  #include  "Portable_Utils.h"

  enum
    {
      c_BtnSEL = 0,
      c_BtnESC,
      c_Btn3,
      c_Btn4,
      c_Btn5,
    };

//
// #######################################################
//
#ifdef __cplusplus
 extern "C" {
#endif

 bool Is_Button_Pushed ( uint8_t button_no, uint8_t no_of_interations );

#ifdef __cplusplus
}
#endif


#endif // __Button_IO_Lib_H
//
// =============================================================================
//  The End
// =============================================================================
