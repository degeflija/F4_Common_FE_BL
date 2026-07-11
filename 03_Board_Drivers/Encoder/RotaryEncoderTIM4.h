/**
 * ###########################################################################
 * @file     RotaryEncoderTIM4.h
 * @brief    New Vario
 * @author   Max Baetz / Horst Rupp
 * ############################################################################
 */
#ifndef __BAROTARYENCODERTIM4_H
  #define __BAROTARYENCODERTIM4_H

  // project headers
  #include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

  void      RotaryEncoderInitTIM4     ( void );
  uint32_t  GetRotaryEncoderCountTIM4 ( void );
  void      SetRotaryEncoderCountTIM4 ( uint32_t count);

#ifdef __cplusplus
}
#endif


#endif // BAROTARYENCODERTIM4
//
// =============================================================================
//  The End
// =============================================================================
