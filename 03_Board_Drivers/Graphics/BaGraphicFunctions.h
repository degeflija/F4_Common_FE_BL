#ifndef __BAGRAPHICFUNCTIONS_H
  #define __BAGRAPHICFUNCTIONS_H

  #include "BaFont.h"
  #include "BaBasicGraphicFunctions.h"

  typedef enum
  {
    BA_TEXTALIGNMENT_LEFT,
    BA_TEXTALIGNMENT_CENTER,
    BA_TEXTALIGNMENT_RIGHT
  }
  BaTextAlignment;

  #ifdef __cplusplus
    extern "C" {
  #endif

  // valid widths are 1 or 2
  void ba_graphicsDrawLine                    (int x1, int y1, int x2, int y2, uint32_t lineWidth, uint8_t color );

  void ba_graphicsDrawFullRectWithIntensity   ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color, uint8_t intensity );

  void ba_graphicsDrawCircle                  ( int xp, int yp, uint16_t r, uint32_t width, uint16_t color );
  void ba_graphicsDrawFullCircle              ( int xp, int yp, uint16_t r, uint16_t color );
  void ba_graphicsFillOutsideCircle           ( int xp, int yp, uint16_t r, uint16_t color );

  void ba_graphicsDrawFullBottomFlatTriangle  ( int topX, int topY, int bottomX1, int bottomX2, int bottomY, uint8_t color );
  void ba_graphicsDrawFullTopFlatTriangle     ( int bottomX, int bottomY, int topX1, int topX2, int topY, uint8_t color );
  void ba_graphicsDrawFullTriangle            ( int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color );

  // triangle is rotated around the center of the surrounding circle. The radius of this circle is returned
  float ba_graphicsDrawFullRotatedTriangle    ( int x, int y, int w, int h, int16_t dir, uint8_t color, uint8_t strokeColor );

  // returns the width of the character
  int ba_graphicsDrawCharacter                ( int x, int y, char ascii, const BaFont* font, uint8_t fg, uint8_t bg, int transparent );

  void ba_graphicsDrawString                  ( int x, int y, const char *text, const BaFont* font,
                                                uint8_t fg, uint8_t bg, int transparent );

  void ba_graphicsDrawStringInBox             ( int x1, int y1, int x2, int y2, const char *text, const BaFont* font,
                                                BaTextAlignment alignment, uint8_t fg, uint8_t bg, int transparent );

  void ba_graphicsDrawStringWithBaseline      ( int x1, int x2, int yBaseLine, const char *text, const BaFont* font,
                                                BaTextAlignment alignment, uint8_t fg, uint8_t bg, int transparent );


  typedef struct
  {
      int32_t width;
      int32_t height;

      const uint8_t* data;

      // flags:
      unsigned compressed:1; // if set, the image uses run-length encoding
      unsigned dataIs2bitIntensity:1; // if set, the pixel data does not contain colors, but 2-bit intensity values
  } BaGraphicImageData;

  typedef enum
  {
      kBaGraphicImageCornerTopLeft,
      kBaGraphicImageCornerTopRight,
      kBaGraphicImageCornerBottomLeft,
      kBaGraphicImageCornerBottomRight
  } BaGraphicImageCorner;

  typedef enum
  {
      kBaGraphicImageReadDirCW,
      kBaGraphicImageReadDirCCW
  } BaGraphicImageReadDir;

  /*
   * Arbitrary triangle-based graphical objects can be created and drawn...
   *

   */

  typedef struct
  {
    int x;
    int y;
  }
  BaVertex;

  // a triangle only contains indices to vertices in an vertex array
  typedef struct
  {
    int vi1;
    int vi2;
    int vi3;
  }
  BaTriangle;

  typedef struct
  {
    int numVertices;
    const BaVertex *vertices;
    int numTriangles;
    const BaTriangle *triangles;
    // the radius of the surrounding circle (e.g. for altitude-label placement)
    uint32_t boundingRadius;
  }
  BaTriangleObject;

  // maximum allowed number of vertices of a single object
  #define TRIANGLE_OBJECT_MAX_NUM_VERTICES  100
  void ba_graphicsDrawIntensityImage(int32_t centerX, int32_t centerY, const BaGraphicImageData* img, uint8_t color);
  void ba_graphicsDrawRotatedIntensityImage(int32_t centerX, int32_t centerY, const BaGraphicImageData* img, uint8_t color, BaGraphicImageCorner startCorner, BaGraphicImageReadDir readDir);

  void ba_graphicsDrawTriangleObject(const BaTriangleObject *object, int x, int y, int16_t dir, uint8_t color);

  #ifdef __cplusplus
    }
  #endif


#endif // __BADISPLAY_H
