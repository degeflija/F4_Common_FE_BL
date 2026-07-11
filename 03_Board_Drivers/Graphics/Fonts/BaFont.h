#ifndef __BAFONT_H
#define __BAFONT_H

#include "stdint.h"

#define true  1

typedef struct
{
  union
  {
      const uint8_t *antialiasedFont;
      const uint16_t *smallFont;   // if width <= 16
      const uint32_t *mediumFont;  // if 16 < width <= 32
      const uint64_t *largeFont;   // if 32 < width <= 64
  }
  fontData;

  char firstChar;
  char lastChar;

  char antialiased;
  uint8_t bytesPerRow;   // number of bytes per row for an antialiased font or 0 for any

  uint16_t width;        // (max)width of a character (in pixel)
  uint16_t height;       // height of a character (in pixel)
  uint16_t ascend;       // ascend of a character (in pixel)
  uint16_t descend;      // descend of a character (in pixel)

  const uint8_t *widths; // width for each character in a dynamic width font or 0 for a fix width font
}
BaFont;

// antialiased fonts
extern const BaFont BaMyriadPro20;
extern const BaFont BaMyriadPro26;
extern const BaFont BaMyriadPro33;
extern const BaFont BaMyriadPro48;
extern const BaFont BaMyriadPro56;
extern const BaFont BaMyriadProNumeric62;
extern const BaFont BaMyriadProNumeric70;
extern const BaFont BaMyriadProNumeric71;
extern const BaFont BaMyriadProNumeric73;
extern const BaFont BaMyriadProNumeric75;
extern const BaFont BaMyriadProNumeric78;
extern const BaFont BaMyriadProNumeric84;
extern const BaFont BaMyriadProNumeric125;
extern const BaFont BaMonacoFixedWidth41;


extern uint32_t ba_fontsGetWidthForChar(uint8_t c, const BaFont* font);
extern uint32_t ba_fontsGetWidthForIndex(int i, const BaFont* font);
extern uint32_t ba_fontsGetStringWidth(const char *text, const BaFont* font);
extern int ba_fontsGetVerticalCenter(int baseLine, const BaFont* font);
extern void ba_fontsTruncateStringToFit(const char* text, const BaFont* font, uint32_t availableWidth, uint8_t* buf, uint32_t bufSize);

#endif // __BAFONT_H
