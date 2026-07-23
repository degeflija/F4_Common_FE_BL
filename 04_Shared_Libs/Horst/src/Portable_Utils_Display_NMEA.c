/**
  * ###########################################################################
  * @file     Portable_Utils_Display_NMEA.c
  * @brief    New Vario
  * @author   Horst Rupp
  * @brief    Extension of Portable_Utils - see header for why this is split
  *           out from Portable_Utils.c.
  * ###########################################################################
  */

//
//  Includes
//
#include  "Portable_Utils_Display_NMEA.h"

// *****************************************************************************
//  Version from hex to str
// *****************************************************************************
//
void HexVersion2String ( char * dest, const uint32_t version_as_hex )
{
  uint8_t   buf_build[5], buf_minor[5];

  uint16_t  build =   ( version_as_hex & 0x0000000f ) +
                      ( version_as_hex & 0x000000f0 ) / 16 * 10 +
                      ( version_as_hex & 0x00000f00 ) / 16 / 16 * 100 +
                      ( version_as_hex & 0x0000f000 ) / 16 / 16 / 16 * 1000;
  uint32_t  minor = ( ( version_as_hex & 0x000f0000 ) >> 16 ) +
                    ( ( version_as_hex & 0x00f00000 ) >> 16 ) / 16 * 10;
  uint32_t  major = ( ( version_as_hex & 0x0f000000 ) >> 24 ) +
                    ( ( version_as_hex & 0xf0000000 ) >> 24 ) / 16 * 10;

  portable_itoa ( major, (char*)dest, 2 );
  PostFix_Char ('.', (char*)dest );
  portable_itoa2 ( minor, (char*)buf_minor, 2, true, '0' );
  portable_strcat ( (char*)dest, (const char*) buf_minor );
  portable_strcat ( (char*)dest, (const char*) txt_Build );
  portable_itoa2 ( build, (char*)buf_build, 4, true, ' ' );
  portable_strcat ( (char*)dest, (const char*) buf_build );

  //  #define VERSION_TXT5 "1.01 Build 15"
}

// ****************************************************************************
// Transpose coordinates on the canvas AND perform clipping  ToDo
// ****************************************************************************
//
void MapCoordinates2Canvas (  point_i16_t * StartPoint )
{
//  if ( StartPoint->x > 113 )  StartPoint->x = 113;
//  if ( StartPoint->x < -113 ) StartPoint->x = -113;
//  if ( StartPoint->y > 113 )  StartPoint->y = 113;
//  if ( StartPoint->y < -113 ) StartPoint->y = -113;

  StartPoint->x = c_center_x + StartPoint->x;
  StartPoint->y = c_center_y - StartPoint->y;
}

// ****************************************************************************
// add end delimiter, evaluate and add checksum and add CR+LF
// ****************************************************************************
ROM char HEX[]="0123456789ABCDEF";

void NMEA_append_tail ( char * buffer )
{
  PostFix_Char ( '*', (char*)buffer );

  uint8_t checksum = 0;
  //uint32_t ix = portable_strlen((const char*) buffer)-1;
  for ( uint8_t i = 1; i < portable_strlen((const char*) buffer)-1; i++ )
  {
    checksum ^= buffer[i];
  }
  PostFix_Char ( HEX[checksum >> 4], (char*)buffer );
  PostFix_Char ( HEX[checksum & 0x0f], (char*)buffer );
  portable_strcat ( (char*)buffer, (const char*) txt_CRLF );
}

// ****************************************************************************
//  EnforceExtension()
// ****************************************************************************
//
//  If p_filename ends with given extension, no action
//  If p_filename ends with no extension, append the extension given.
//  If p_filename ends with a different extension,
//        strip that extension and append the extension given.
//
void EnforceExtension ( char * p_filename, const char * p_extension )
{
  char *    l_dot;
  uint16_t  l_base_len;
  uint16_t  l_ext_len;

  ASSERT ( p_filename  != NULL );
  ASSERT ( p_extension != NULL );

  //
  //  Cut off any existing extension at the LAST '.' - strrchr, not
  //  strchr, so a name like "PRF.v2" keeps "PRF.v2" -> "PRF" as the
  //  base, not "PRF" from a first-dot match that would be wrong for
  //  names containing more than one '.'.
  //
  l_dot = portable_strrchr ( p_filename, '.' );
  if ( l_dot != NULL )
  {
    *l_dot = '\0';
  }

  l_base_len = (uint16_t) portable_strlen ( p_filename );
  l_ext_len  = (uint16_t) portable_strlen ( (char*)p_extension );

  //
  //  Guard against overflowing the destination buffer before appending.
  //
  ASSERT ( ( l_base_len + l_ext_len ) < c_filename_size );

  portable_strcpy ( &p_filename[l_base_len], (char*)p_extension );
}

// ****************************************************************************
// End of File
// ****************************************************************************
