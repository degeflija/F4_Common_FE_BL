/**
  * ###########################################################################
  * @file     Portable_Utils_Display_NMEA.h
  * @brief    New Vario
  * @author   Horst Rupp
  * @brief    Extension of Portable_Utils, for the functions that need
  *           FE/BL-only application context (canvas center, text tables,
  *           filename size) - HexVersion2String() needs txt_Build,
  *           MapCoordinates2Canvas() needs c_center_x/c_center_y,
  *           NMEA_append_tail() needs txt_CRLF, EnforceExtension() needs
  *           c_filename_size, all defined in Generic_Const.h /
  *           Translator_Tables.h.
  *
  *           Used by AD57_FE_GIT and AD57_BL_GIT only - F4_GenBL_GIT and
  *           F4_P_Util_GIT have no display and no persisted filenames, so
  *           they never call these. Candidate for the F4_Common_FE_BL
  *           common-tier repo, alongside Portable_Utils.c/.h itself.
  * ###########################################################################
  */

  #include  "Generic_Includes.h"
  #include  "Translator_Tables.h"   // for txt_Build, txt_CRLF - NOT pulled in by Generic_Includes.h
  #include  "Portable_Utils.h"

#ifndef _Portable_Utils_Display_NMEA_H
  #define _Portable_Utils_Display_NMEA_H

  #ifdef __cplusplus
    extern "C" {
  #endif

  void      HexVersion2String         ( char * dest,
                                        const uint32_t version_as_hex );
  void      MapCoordinates2Canvas     ( point_i16_t * StartPoint );
  void      NMEA_append_tail          ( char * buffer );
  void      EnforceExtension          ( char * p_filename,
                                        const char * p_extension );

  #ifdef __cplusplus
  }
  #endif

#endif // _Portable_Utils_Display_NMEA_H

// ****************************************************************************
// End of File
// ****************************************************************************
