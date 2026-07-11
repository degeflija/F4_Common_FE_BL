//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaDisplay.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//!
//!

#ifndef BA_DRIVERS_DISPLAY_BADISPLAY_H_
#define BA_DRIVERS_DISPLAY_BADISPLAY_H_

// related headers

// standard libs

// other libs

// project headers
#include <BaDisplayOrientation.h>
#include "BaDeviceInfo.h"


//--------------------------------------------------------------
// addresses of FSMC for display access
// bank   = bank-1 / PSRAM-1         => BaseAdr 0x60000000
// RS-Pin = PD11=FSMC_A16 = BitNr 16 => Offset  0x00020000
//--------------------------------------------------------------
#define LCD_REG     (*((volatile unsigned short *) 0x60000000))     // RS = 0
#define LCD_RAMADR  ((uint32_t)0x60020000U)                         // RS = 1
#define LCD_RAM     (*((volatile unsigned short *) LCD_RAMADR ))    // RS = 1

//--------------------------------------------------------------
// display defines
//--------------------------------------------------------------
// total display pixel
#define  LCD_PORTRAIT_TOTAL_WIDTH     240 // pixel in X-direction (portrait modes)
#define  LCD_PORTRAIT_TOTAL_HEIGHT    320 // pixel in Y-direction (portrait modes)
#define  LCD_TOTAL_PIXEL              (LCD_PORTRAIT_TOTAL_WIDTH*LCD_PORTRAIT_TOTAL_HEIGHT)

//--------------------------------------------------------------
// display defines for display-controller R61580
//--------------------------------------------------------------

#define  LCD_R61580_FSMC_AST  ((uint32_t)9U) // AdressSetupTime
#define  LCD_R61580_FSMC_AHT  ((uint32_t)1U) // DataHoldTime
#define  LCD_R61580_FSMC_DST  ((uint32_t)3U) // DataSetupTime

//--------------------------------------------------------------
// register addresses of display-controller R61580
//--------------------------------------------------------------
#define  LCD_R61580_REG_DRIVERID    0x00   // display-id
#define  LCD_R61580_REG_POSX        0x20   // cursor-pos (X)
#define  LCD_R61580_REG_POSY        0x21   // cursor-pos (Y)
#define  LCD_R61580_REG_GRAM        0x22   // GRAM start

#define  LCD_R61580_REG_HSTART      0x50   // x-start
#define  LCD_R61580_REG_HEND        0x51   // x-end
#define  LCD_R61580_REG_VSTART      0x52   // y-start
#define  LCD_R61580_REG_VEND        0x53   // y-end

//--------------------------------------------------------------
// display defines for display-controller ST7789VI
//--------------------------------------------------------------

// 1 ~ 5.95ns
#define  LCD_ST7789VI_FSMC_AST  ((uint32_t)9U) // AdressSetupTime
#define  LCD_ST7789VI_FSMC_AHT  ((uint32_t)1U) // DataHoldTime
#define  LCD_ST7789VI_FSMC_DST  ((uint32_t)3U) // DataSetupTime

#define  LCD_ST7789VI_REG_GRAM ((uint8_t)0x2CU)

//--------------------------------------------------------------
// display defines for display-controller HX8347A
//--------------------------------------------------------------

#define  LCD_HX8347A_FSMC_AST  ((uint32_t)9U) // AdressSetupTime
#define  LCD_HX8347A_FSMC_AHT  ((uint32_t)1U) // DataHoldTime
#define  LCD_HX8347A_FSMC_DST  ((uint32_t)3U) // DataSetupTime

//--------------------------------------------------------------
// register addresses of display-controller HX8347A
//--------------------------------------------------------------
#define  LCD_HX8347A_REG_GRAM       0x22   // GRAM start




#define LCD_AVAIL_WIDTH_LANDSCAPE       ((uint32_t)285U)
#define LCD_AVAIL_HEIGHT_LANDSCAPE      ((uint32_t)227U)

#define LCD_LANDSCAPE_ORIGIN_X          ((uint32_t)17U)
#define LCD_LANDSCAPE_ORIGIN_Y          ((uint32_t)6U)

//  320 - 285 = 35 / 2 = 17 (+1)
//  240 - 227 = 13 / 2 = 6 (+1)

#define LCD_AVAIL_WIDTH_PORTRAIT        ((uint32_t)227U)
#define LCD_AVAIL_HEIGHT_PORTRAIT       ((uint32_t)285U)


#define LCD_PORTRAIT_ORIGIN_X           ((uint32_t)6U)
#define LCD_PORTRAIT_ORIGIN_Y           ((uint32_t)17U)

//  #define LCD_LANDSCAPE180_ORIGIN_X       ((uint32_t)17U)
//  #define LCD_LANDSCAPE180_ORIGIN_Y       ((uint32_t)6U)
//
//  #define LCD_PORTRAIT180_ORIGIN_X        ((uint32_t)20U)
//  #define LCD_PORTRAIT180_ORIGIN_Y        ((uint32_t)6U)

#define LCD_AVAIL_PIXEL                 ( LCD_AVAIL_WIDTH_PORTRAIT * LCD_AVAIL_HEIGHT_PORTRAIT )

#define COLOR_LIGHT_GREY      0x01
#define COLOR_DARK_GREY       0x10

#define COLOR_WHITE           0x00
#define COLOR_BLACK           0x11
#define COLOR_RED             0x22
#define COLOR_GREEN           0x33
#define COLOR_DARK_GREEN      0x44
#define COLOR_BLUE            0x55
#define COLOR_YELLOW          0x66
#define COLOR_DARK_YELLOW     0x77
#define COLOR_MAGENTA         0x88
#define COLOR_CYAN            0x99
#define COLOR_DARK_CYAN       0xAA
#define COLOR_AMBER           0xBB
#define COLOR_GREY            0xCC
#define COLOR_BROWN           0xDD
#define COLOR_HORIZON_BLUE    0xEE

#define COLOR_EARTH_1         28U
#define COLOR_EARTH_2         44U
#define COLOR_EARTH_3         72U
#define COLOR_EARTH_4         78U
#define COLOR_EARTH_5         83U
#define COLOR_EARTH_6         160U
#define COLOR_EARTH_7         163U
#define COLOR_EARTH_8         166U
#define COLOR_EARTH_9         169U
#define COLOR_EARTH_10        189U
#define COLOR_EARTH_11        213U
#define COLOR_EARTH_12        216U


#define COLOR_GREY_25PERCENT  0xF0
#define COLOR_GREY_50PERCENT  0xF1
#define COLOR_GREY_75PERCENT  0xF2

#define  BA_DISPLAY_COM22_R61580            ((uint32_t)0U)
#define  BA_DISPLAY_COM22_ST7789VI          ((uint32_t)1U)
#define  BA_DISPLAY_COM27_HX8347A           ((uint32_t)2U)


#ifdef __cplusplus
    extern "C" {
#endif

  //int ba_displayInitWithType(uint32_t type, BaDisplayOrientation orientation);

  int ba_displayInit(BaDisplayOrientation orientation);

  BaDisplayOrientation ba_displayGetOrientation(void);
  void ba_displaySetOrientation(BaDisplayOrientation orientation);

  int32_t ba_displayGetWidth(void);
  int32_t ba_displayGetHeight(void);

// set the writable area of the framebuffer to the rect (including edges)
  void ba_displaySetClipping(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
  void ba_displayResetClipping(void);

  void ba_displayClrSreen(uint8_t color);
  void ba_displayDrawPixel(int x, int y, uint8_t color);

  uint8_t ba_displayGetPixel(int x, int y);

  void ba_displayScrollSreenVertical(int32_t pixelDown, uint8_t color);

// set a window which is filled by the following ba_displayPutPixel calls row by row from left to right, top to bottom
  void ba_displaySetWindow(int x1, int y1, int x2, int y2);
  void ba_displayPutPixel(uint8_t color);
  void ba_displayPutPixelWithIntensity(uint8_t col, uint8_t intensity);

  void ba_displaySkipPixel(void);
  void ba_displayPutPixelWithTransparency(uint8_t color, uint8_t transparentColor);

  void ba_displayDrawFullRect(int x1, int y1, int x2, int y2, uint8_t color);
  void ba_displayDrawHLine(int x1, int x2, int y, uint8_t color);

//! replaces the content of the display with the last drawn buffer
  void ba_displayRefresh(void);
  bool ba_displayRefreshActive(void);

#ifdef __cplusplus
    }
#endif

#endif // BA_DRIVERS_DISPLAY_BADISPLAY_H_
