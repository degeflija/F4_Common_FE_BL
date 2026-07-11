//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaBasicGraphicFunctions.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//! ...
//!

#ifndef BA_SHARED_GRAPHIC_BABASICGRAPHICFUNCTIONS_H_
  #define BA_SHARED_GRAPHIC_BABASICGRAPHICFUNCTIONS_H_

  // project headers
  #include "stdint.h"

  #ifdef __cplusplus
    extern "C" {
  #endif

  void ba_graphicsClrScreen(uint8_t color);

  void ba_graphicsDrawPixel(int32_t x, int32_t y, uint8_t color);

  void ba_graphicsDrawFullRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color);
  void ba_graphicsDrawRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t lineWidth, uint8_t color);

  void ba_graphicsDrawHLine(int32_t x1, int32_t x2, int32_t y, uint32_t lineWidth, uint8_t color);
  void ba_graphicsDrawVLine(int32_t y1, int32_t y2, int32_t x, uint32_t lineWidth, uint8_t color);

  uint32_t ba_graphicsDrawBasicText(int32_t x, int32_t y, const uint8_t* text, int32_t scale, uint8_t fg, uint8_t bg, int transparent);
  void ba_graphicsDrawBasicTextAtLine(int32_t x1, int32_t x2, int32_t y, const uint8_t* text, int32_t scale, uint8_t fg, uint8_t bg, int transparent);
  void ba_graphicsDrawBasicTextInBox(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint8_t* text, int32_t scale, uint8_t fg, uint8_t bg, int transparent);

  #ifdef __cplusplus
    }
  #endif

#endif // BA_SHARED_GRAPHIC_BABASICGRAPHICFUNCTIONS_H_
