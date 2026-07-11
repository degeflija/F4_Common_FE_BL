/**
 * ###########################################################################
 * @file     RotaryEncoderTIM5.h
 * @brief    New Vario
 * @author   Max Baetz / Horst Rupp
 * ############################################################################
 */
#ifndef __BAROTARYENCODERTIM5_H
  #define __BAROTARYENCODERTIM5_H

  // project headers
  #include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

  void 		  RotaryEncoderInitTIM5     ( void );
  uint32_t 	GetRotaryEncoderCountTIM5 ( void );
  void      SetRotaryEncoderCountTIM5 ( uint32_t count);

#ifdef __cplusplus
}
#endif


#endif // BAROTARYENCODERTIM5
//
// =============================================================================
//  The End
// =============================================================================
