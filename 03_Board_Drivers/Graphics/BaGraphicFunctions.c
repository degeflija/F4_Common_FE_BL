#include "BaDisplay.h"
#include "BaGraphicFunctions.h"
#include "System_Configuration.h"
#include "string.h"
#include "embedded_math.h"
#include "stdint.h"


// draw line - bresenham algorithm
void ba_graphicsDrawLine(int x1, int y1, int x2, int y2, uint32_t lineWidth, uint8_t color)
{
    if (lineWidth == 0)
    {
        return;
    }

    // limit to valid range
    if (--lineWidth > 1)
    {
        lineWidth = 1;
    }

    int x = x1;
    int y = y1;
    int d = 0;
    int hx = x2 - x1; // how many pixels on each axis
    int hy = y2 - y1;
    int xinc = 1;
    int yinc = 1;
    if (hx < 0)
    {
        xinc = -1;
        hx = -hx;
    }
    if (hy < 0)
    {
        yinc = -1;
        hy = -hy;
    }
    if (hy <= hx)
    {
        int c = 2 * hx;
        int m = 2 * hy;
        while (x != x2)
        {
            ba_displayDrawFullRect(x, y, x + lineWidth, y + lineWidth, color);
            x += xinc;
            d += m;
            if (d > hx)
            {
                y += yinc;
                d -= c;
            }
        }
    }
    else
    {
        int c = 2 * hy;
        int m = 2 * hx;
        while (y != y2)
        {
            ba_displayDrawFullRect(x, y, x + lineWidth, y + lineWidth, color);
            y += yinc;
            d += m;
            if (d > hy)
            {
                x += xinc;
                d -= c;
            }
        }
    }
    ba_displayDrawFullRect(x, y, x + lineWidth, y + lineWidth, color);
}

void ba_graphicsDrawFullRectWithIntensity(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color, uint8_t intensity)
{
    ba_displaySetWindow(x1, y1, x2, y2);

    int32_t pixelCount = (x2 - x1 + 1) * (y2 - y1 + 1);
    for (int32_t i = 0; i < pixelCount; ++i)
    {
        ba_displayPutPixelWithIntensity(color, intensity);
    }
}

// Helper function for circle drawing. Draws a dot at a given position
static void ba_graphicsPutCirclePixel(int x, int y, int pixel_size, uint8_t col)
{
    if (pixel_size == 2)
    {
        ba_displayDrawFullRect(x, y, x+1, y+1, col);
    }
    else
    {
        ba_displayDrawPixel(x, y, col);
    }
}

// Helper function for circle drawing.
static void ba_graphicsCirclePoints(int x, int y, int xOfs, int yOfs, int width, uint8_t col)
{
    ba_graphicsPutCirclePixel(xOfs+x, yOfs+y, width, col);
    ba_graphicsPutCirclePixel(xOfs+y, yOfs+x, width, col);
    ba_graphicsPutCirclePixel(xOfs+y, yOfs-x, width, col);
    ba_graphicsPutCirclePixel(xOfs+x, yOfs-y, width, col);
    ba_graphicsPutCirclePixel(xOfs-x, yOfs-y, width, col);
    ba_graphicsPutCirclePixel(xOfs-y, yOfs-x, width, col);
    ba_graphicsPutCirclePixel(xOfs-y, yOfs+x, width, col);
    ba_graphicsPutCirclePixel(xOfs-x, yOfs+y, width, col);
}

void ba_graphicsDrawCircle(int xp, int yp, uint16_t r, uint32_t width, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int deltaE = 3;
    int deltaSE = -2 * r + 5;

    if (width == 0)
    {
        return;
    }

    ba_graphicsCirclePoints(x, y, xp, yp, width, color);

    while (y > x)
    {
        if (d < 0)
        {
            d += deltaE;
            deltaSE += 2;
        }
        else
        {
            d += deltaSE;
            deltaSE += 4;
            y--;
        }
        deltaE += 2;
        x++;

        ba_graphicsCirclePoints(x, y, xp, yp, width, color);
    }
}

void ba_graphicsDrawFullCircle(int xp, int yp, uint16_t r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int deltaE = 3;
    int deltaSE = -2 * r + 5;

    ba_graphicsCirclePoints(x, y, xp, yp, 1, color);
    ba_displayDrawHLine(xp - y, xp + y, yp - x, color);

    while (y > x)
    {
        if (d < 0)
        {
            d += deltaE;
            deltaSE += 2;
        }
        else
        {
            d += deltaSE;
            deltaSE += 4;
            y--;
        }
        deltaE += 2;
        x++;

        ba_displayDrawHLine(xp - x, xp + x, yp + y, color);
        ba_displayDrawHLine(xp - x, xp + x, yp - y, color);
        ba_displayDrawHLine(xp - y, xp + y, yp + x, color);
        ba_displayDrawHLine(xp - y, xp + y, yp - x, color);
    }
}

void ba_graphicsFillOutsideCircle(int xp, int yp, uint16_t r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int deltaE = 3;
    int deltaSE = -2 * r + 5;

    int width = ba_displayGetWidth();
    int height = ba_displayGetHeight();

    ba_graphicsCirclePoints(x, y, xp, yp, 1, color);
    ba_displayDrawHLine(-1, xp - y, yp - x, color);
    ba_displayDrawHLine(xp + y, width, yp - x, color);

    while (y > x)
    {
        if (d < 0)
        {
            d += deltaE;
            deltaSE += 2;
        }
        else
        {
            d += deltaSE;
            deltaSE += 4;
            y--;
        }
        deltaE += 2;
        x++;

        ba_displayDrawHLine(-1, xp - x, yp + y, color);
        ba_displayDrawHLine(xp + x, width, yp + y, color);
        ba_displayDrawHLine(-1, xp - x, yp - y, color);
        ba_displayDrawHLine(xp + x, width, yp - y, color);
        ba_displayDrawHLine(-1, xp - y, yp + x, color);
        ba_displayDrawHLine(xp + y, width, yp + x, color);
        ba_displayDrawHLine(-1, xp - y, yp - x, color);
        ba_displayDrawHLine(xp + y, width, yp - x, color);
    }

    // fill all above and below the circle
    if (yp - r - 1 >= 0)
    {
        ba_displayDrawFullRect(0, 0, width-1, yp - r - 1, color);
    }

    if (yp + r + 1 < height)
    {
        ba_displayDrawFullRect(0, yp + r + 1, width-1, height-1, color);
    }
}

// draw a bottom flat triangle
void ba_graphicsDrawFullBottomFlatTriangle (  int topX,     int topY,
                                              int bottomX1, int bottomX2, int bottomY,
                                              uint8_t color)
{
    if (bottomX1 > bottomX2)
    {
        int t = bottomX2;
        bottomX2 = bottomX1;
        bottomX1 = t;
    }

    float dY = (float)(bottomY - topY);
    float invslope1 = ((float)(bottomX1 - topX)) / dY;
    float invslope2 = ((float)(bottomX2 - topX)) / dY;

    float curX1 = (float)topX;
    float curX2 = curX1;
    int scanlineY = topY;
    for (; scanlineY <= bottomY; scanlineY++)
    {
        int32_t x1 = (int32_t)ceilf(curX1);
        int32_t x2 = (int32_t)floorf(curX2);
        ba_displayDrawHLine(x1, x2, scanlineY, color);

        curX1 += invslope1;
        curX2 += invslope2;
    }
}

// draw a top flat triangle with (x1/y1) at top
void ba_graphicsDrawFullTopFlatTriangle(int bottomX, int bottomY,
                                        int topX1, int topX2, int topY,
                                        uint8_t color)
{
    if (topX1 > topX2)
    {
        int t = topX2;
        topX2 = topX1;
        topX1 = t;
    }

    float dY = (float)(bottomY - topY);
    float invslope1 = ((float)(topX1 - bottomX)) / dY;
    float invslope2 = ((float)(topX2 - bottomX)) / dY;

    float curX1 = (float)bottomX;
    float curX2 = curX1;
    int scanlineY = bottomY;
    for (; scanlineY >= topY; scanlineY--)
    {
        int32_t x1 = (int32_t)ceilf(curX1);
        int32_t x2 = (int32_t)floorf(curX2);
        ba_displayDrawHLine(x1, x2, scanlineY, color);

        curX1 += invslope1;
        curX2 += invslope2;
    }
}

void ba_graphicsDrawFullTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{
    // at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice
    int xs1, ys1, xs2, ys2, xs3, ys3;

    if (y1 <= y2)
    {
        if (y1 <= y3)
        {
            // y1 is the smallest
            xs1 = x1;
            ys1 = y1;

            if (y2 <= y3)
            {
                xs2 = x2;
                ys2 = y2;
                xs3 = x3;
                ys3 = y3;
            }
            else
            {
                xs2 = x3;
                ys2 = y3;
                xs3 = x2;
                ys3 = y2;
            }
        }
        else
        {
            // y3 is the smallest
            xs1 = x3;
            ys1 = y3;
            xs2 = x1;
            ys2 = y1;
            xs3 = x2;
            ys3 = y2;
        }
    }
    else // y2 < y1
    {
        if (y2 <= y3)
        {
            // y2 is the smallest
            xs1 = x2;
            ys1 = y2;

            if (y1 <= y3)
            {
                xs2 = x1;
                ys2 = y1;
                xs3 = x3;
                ys3 = y3;
            }
            else
            {
                xs2 = x3;
                ys2 = y3;
                xs3 = x1;
                ys3 = y1;
            }
        }
        else
        {
            // y3 is the smallest
            xs1 = x3;
            ys1 = y3;
            xs2 = x2;
            ys2 = y2;
            xs3 = x1;
            ys3 = y1;
        }
    }

    // here we know that v1.y <= v2.y <= v3.y
    //check for trivial case of bottom-flat triangle
    if (ys2 == ys3)
    {
        ba_graphicsDrawFullBottomFlatTriangle(xs1, ys1, xs2, xs3, ys2, color);
    }
    // check for trivial case of top-flat triangle
    else if (ys1 == ys2)
    {
        ba_graphicsDrawFullTopFlatTriangle(xs3, ys3, xs1, xs2, ys1, color);
    }
    else
    {
        // general case - split the triangle in a topflat and bottom-flat one
        int xs4 = (int)(xs1 + ((float)(ys2 - ys1) / (float)(ys3 - ys1)) * (xs3 - xs1));
        int ys4 = ys2;
        ba_graphicsDrawFullBottomFlatTriangle(xs1, ys1, xs2, xs4, ys4, color);
        ba_graphicsDrawFullTopFlatTriangle(xs3, ys3, xs2, xs4, ys4, color);
    }
}

// triangle is rotated around the center of the surrounding circle. The radius of this circle is returned
float ba_graphicsDrawFullRotatedTriangle(int x, int y, int w, int h, int16_t dir, uint8_t color, uint8_t strokeColor)
{
    float dirRad = ((float)dir) * 0.0174532925;

    // surrounding circle
    // r = axbxc / Ax4
    float Ax4 = ((float)(w*h*2));
    float b = VSQRTF( ((float)h*h) + ((float)w*w) * 0.25);
    float r = (w*b*b) / Ax4;

    float xTop = 0.0;
    float yTop = -r;
    float xBottom = w*0.5;
    float yBottom = h + yTop;

#if 1 // #ifdef BUILD_AD57_FE
    int xTopRot = (int)(xTop * COS(dirRad) - yTop * SIN(dirRad)) + x;
    int yTopRot = (int)(xTop * SIN(dirRad) + yTop * COS(dirRad)) + y;

    int xBottomLeftRot = (int)((-xBottom) * COS(dirRad) - yBottom * SIN(dirRad)) + x;
    int yBottomLeftRot = (int)((-xBottom) * SIN(dirRad) + yBottom * COS(dirRad)) + y;

    int xBottomRightRot = (int)(xBottom * COS(dirRad) - yBottom * SIN(dirRad)) + x;
    int yBottomRightRot = (int)(xBottom * SIN(dirRad) + yBottom * COS(dirRad)) + y;
//#else
//    int xTopRot = (int)(xTop * cos(dirRad) - yTop * sin(dirRad)) + x;
//    int yTopRot = (int)(xTop * sin(dirRad) + yTop * cos(dirRad)) + y;
//
//    int xBottomLeftRot = (int)((-xBottom) * cos(dirRad) - yBottom * sin(dirRad)) + x;
//    int yBottomLeftRot = (int)((-xBottom) * sin(dirRad) + yBottom * cos(dirRad)) + y;
//
//    int xBottomRightRot = (int)(xBottom * cos(dirRad) - yBottom * sin(dirRad)) + x;
//    int yBottomRightRot = (int)(xBottom * sin(dirRad) + yBottom * cos(dirRad)) + y;
#endif

    ba_graphicsDrawFullTriangle(xTopRot, yTopRot, xBottomLeftRot, yBottomLeftRot, xBottomRightRot, yBottomRightRot, color);

    if (strokeColor != color)
    {
        ba_graphicsDrawLine(xTopRot, yTopRot, xBottomLeftRot, yBottomLeftRot, 2, strokeColor);
        ba_graphicsDrawLine(xTopRot, yTopRot, xBottomRightRot, yBottomRightRot, 2, strokeColor);
        ba_graphicsDrawLine(xBottomRightRot, yBottomRightRot, xBottomLeftRot, yBottomLeftRot, 2, strokeColor);
    }

    return r;
}


int ba_graphicsDrawCharacter(int x, int y, char ascii, const BaFont* font, uint8_t fg, uint8_t bg, int transparent)
{
    if ((font == NULL) || (ascii < font->firstChar) || (ascii > font->lastChar))
    {
        return 0;
    }

    ascii -= font->firstChar;

    int width = ba_fontsGetWidthForIndex((int)ascii, font);
    ba_displaySetWindow(x, y, x + width - 1 , y + font->height - 1);

    if (font->antialiased)
    {
        uint32_t bytesPerRow = (uint32_t)font->bytesPerRow;
        uint32_t height = (uint32_t)font->height;
        const uint8_t *antialiasedFont = font->fontData.antialiasedFont;

        uint32_t curRowByteIdx = ((uint32_t)ascii) * bytesPerRow * height;
        // read lines
        for (int32_t y = 0; y < font->height; ++y)
        {
            uint32_t curLSBit = 6U;
            uint32_t curByteIdx = curRowByteIdx;
            uint8_t curByte = antialiasedFont[curByteIdx];

            for (int32_t x = 0; x < width; ++x)
            {
                uint8_t pxIntensity = (curByte >> curLSBit) & 0x03;

                if (pxIntensity != 0x00)
                {
                    ba_displayPutPixelWithIntensity(fg, pxIntensity);
                }
                else if (!transparent)
                {
                    ba_displayPutPixel(bg);
                }
                else
                {
                    ba_displaySkipPixel();
                }

                if (curLSBit == 0U)
                {
                    curLSBit = 6U;
                    ++curByteIdx;
                    curByte = font->fontData.antialiasedFont[curByteIdx];
                }
                else
                {
                    curLSBit -= 2U;
                }
            }

            // increase curByte to next row
            curRowByteIdx += bytesPerRow;
        }
    }
    else if (font->width <= 32)
    {
        uint32_t startMask = (1<<(width-1));
        uint32_t mask = 0;

        if (font->width <= 16)
        {
            const uint16_t *value = &(font->fontData.smallFont[ascii * font->height]);
            for (int32_t yn = 0; yn < font->height; yn++)
            {
                mask = startMask;

                for (int32_t xn = 0; xn < width; xn++)
                {
                    if ((value[yn] & mask) == 0x00)
                    {
                        // background
                        if (transparent)
                        {
                            ba_displaySkipPixel();
                        }
                        else
                        {
                            ba_displayPutPixel(bg);
                        }
                    }
                    else
                    {
                        // foreground
                        ba_displayPutPixel(fg);
                    }
                    mask = (mask >> 1);
                }
            }
        }
        else
        {
            const uint32_t *value = &(font->fontData.mediumFont[ascii * font->height]);
            for (int32_t yn = 0; yn < font->height; yn++)
            {
                mask = startMask;

                for (int32_t xn = 0; xn < width; xn++)
                {
                    if ((value[yn] & mask) == 0x00)
                    {
                        // background
                        if (transparent)
                        {
                            ba_displaySkipPixel();
                        }
                        else
                        {
                            ba_displayPutPixel(bg);
                        }
                    }
                    else
                    {
                        // foreground
                        ba_displayPutPixel(fg);
                    }
                    mask = (mask >> 1);
                }
            }
        }
    }
    else if (font->width > 32)
    {
        uint64_t shiftWidth = width-1;
        uint64_t startMask = (1ULL<<(shiftWidth));
        uint64_t mask = 0;

        const uint64_t *valuePt64bit = &(font->fontData.largeFont[ascii * font->height]);
        for (int32_t yn = 0; yn < font->height; ++yn)
        {
            mask = startMask;
            uint64_t value64bit = valuePt64bit[yn];
            for (int32_t xn = 0; xn < width; ++xn)
            {
                if ((value64bit & mask) == 0ULL)
                {
                    // background
                    if (transparent)
                    {
                        ba_displaySkipPixel();
                    }
                    else
                    {
                        ba_displayPutPixel(bg);
                    }
                }
                else
                {
                    // foreground
                    ba_displayPutPixel(fg);
                }
                mask = (mask >> 1ULL);
            }
        }
    }

    return width;
}

void ba_graphicsDrawString(int x, int y, const char *text, const BaFont* font, uint8_t fg, uint8_t bg, int transparent)
{
    if ((text != NULL) && (font != NULL))
    {
        uint16_t pos = x;
        while (*text != 0)
        {
            pos += ba_graphicsDrawCharacter(pos, y, *text, font, fg, bg, transparent);
            text++;
        }
    }
    else
    {
        // nothing to do
    }
}

void ba_graphicsDrawStringInBox ( int x1, int y1, int x2, int y2,
                                  const char *text, const BaFont* font,
                                  BaTextAlignment alignment,
                                  uint8_t fg, uint8_t bg, int transparent)
{
    // calculate text width and height
    int w = ba_fontsGetStringWidth(text, font);
    int h = (int)font->height;

    int x;
    int y = y1 + (y2 - y1 - h)/2;

    switch (alignment)
    {
    case BA_TEXTALIGNMENT_CENTER:
        x = x1 + (x2 - x1 - w)/2;
        break;

    case BA_TEXTALIGNMENT_RIGHT:
        x = x2 - w;
        break;

    default:
        x = x1;
        break;
    }

    ba_graphicsDrawString(x, y, text, font, fg, bg, transparent);
}

void ba_graphicsDrawStringWithBaseline(int x1, int x2, int yBaseLine, const char *text, const BaFont* font, BaTextAlignment alignment, uint8_t fg, uint8_t bg, int transparent)
{
    // calculate text width and height
    int w = ba_fontsGetStringWidth(text, font);

    int x;
    int y = yBaseLine - font->ascend;

    switch (alignment)
    {
    case BA_TEXTALIGNMENT_CENTER:
        x = x1 + (x2 - x1 - w)/2;
        break;

    case BA_TEXTALIGNMENT_RIGHT:
        x = x2 - w;
        break;

    default:
        x = x1;
        break;
    }

    ba_graphicsDrawString(x, y, text, font, fg, bg, transparent);
}

void ba_graphicsDrawIntensityImage(int32_t centerX, int32_t centerY, const BaGraphicImageData* img, uint8_t color)
{
    int32_t width = img->width;
    int32_t height = img->height;

    int32_t x1 = centerX - (width/2);
    int32_t x2 = x1 + width - 1;
    int32_t y1 = centerY - (height/2);
    int32_t y2 = y1 + height - 1;

    int32_t pixelCount = width * height;
    uint32_t curByteIdx = 0U;
    uint32_t curLSBit = 6U;
    uint8_t curByte = img->data[curByteIdx];

    ba_displaySetWindow(x1, y1, x2, y2);

    // read lines
    for (int32_t i = 0; i < pixelCount; ++i)
    {
        uint8_t pxIntensity = (curByte >> curLSBit) & 0x03;
        ba_displayPutPixelWithIntensity(color, pxIntensity);

        if (curLSBit == 0U)
        {
            curLSBit = 6U;
            curByte = img->data[++curByteIdx];
        }
        else
        {
            curLSBit -= 2U;
        }
    }
}

void ba_graphicsDrawRotatedIntensityImage(int32_t centerX, int32_t centerY, const BaGraphicImageData* img, uint8_t color, BaGraphicImageCorner startCorner, BaGraphicImageReadDir readDir)
{
    int32_t width = img->width;
    int32_t height = img->height;

    int32_t x1 = centerX - (width/2);
    int32_t x2 = x1 + width - 1;
    int32_t y1 = centerY - (height/2);
    int32_t y2 = y1 + height - 1;

    int32_t pixelCount = width * height;
    int32_t bytesPerRow = width / 4;

    ba_displaySetWindow(x1, y1, x2, y2);

    int32_t row = 0;
    int32_t col = 0;
    switch (startCorner)
    {
        case kBaGraphicImageCornerTopRight:
        {
            row = 0;
            col = width - 1;
            break;
        }
        case kBaGraphicImageCornerBottomLeft:
        {
            row = height - 1;
            col = 0;
            break;
        }
        case kBaGraphicImageCornerBottomRight:
        {
            row = height - 1;
            col = width - 1;
            break;
        }
        default:
        {
            // default is top left, row = col = 0;
            break;
        }
    }

    for (int32_t pxIdx = 0; pxIdx < pixelCount; ++pxIdx)
    {
        int32_t imgIdx = (col / 4) + (row * bytesPerRow);
        uint8_t curByte = img->data[imgIdx];
        uint8_t curLSBit = 6 - ((col % 4) * 2);
        uint8_t pxIntensity = (curByte >> curLSBit) & 0x03;

        ba_displayPutPixelWithIntensity(color, pxIntensity);

        // inc pos
        switch (startCorner)
        {
            case kBaGraphicImageCornerTopRight:
            {
                if (readDir == kBaGraphicImageReadDirCW)
                {
                    if ((++row) == height)
                    {
                        row = 0;
                        --col;
                    }
                }
                else
                {
                    if (col == 0)
                    {
                        col = width - 1;
                        ++row;
                    }
                    else
                    {
                        --col;
                    }
                }
                break;
            }
            case kBaGraphicImageCornerBottomLeft:
            {
                if (readDir == kBaGraphicImageReadDirCW)
                {
                    if (row == 0)
                    {
                        row = height - 1;
                        ++col;
                    }
                    else
                    {
                        --row;
                    }
                }
                else
                {
                    if ((++col) == width)
                    {
                        col = 0;
                        --row;
                    }
                }
                break;
            }
            case kBaGraphicImageCornerBottomRight:
            {
                if (readDir == kBaGraphicImageReadDirCW)
                {
                    if (col == 0)
                    {
                        col = width - 1;
                        --row;
                    }
                    else
                    {
                        --col;
                    }
                }
                else
                {
                    if (row == 0)
                    {
                        row = height - 1;
                        --col;
                    }
                    else
                    {
                        --row;
                    }
                }
                break;
            }
            default:
            {
                // default is top left
                if (readDir == kBaGraphicImageReadDirCW)
                {
                    if ((++col) == width)
                    {
                        col = 0;
                        ++row;
                    }
                }
                else
                {
                    if ((++row) == height)
                    {
                        row = 0;
                        ++col;
                    }
                }
                break;
            }
        }
    }
}

static void rotateVertices(const BaVertex vertices[], BaVertex rotatedVertices[], int numVertices, int16_t dir)
{
    int i;

    float dirRad = ((float)dir) * 0.0174532925;
    float cosDir = COS(dirRad);
    float sinDir = SIN(dirRad);

    for (i = 0; i < numVertices; i++)
    {
        float x = vertices[i].x;
        float y = vertices[i].y;

        rotatedVertices[i].x = (int)(x * cosDir - y * sinDir);
        rotatedVertices[i].y = (int)(x * sinDir + y * cosDir);
    }
}

void ba_graphicsDrawTriangleObject(const BaTriangleObject *object, int x, int y, int16_t dir, uint8_t color)
{
    static BaVertex rotatedVertices[TRIANGLE_OBJECT_MAX_NUM_VERTICES];
    int i;

    if (object->numVertices > TRIANGLE_OBJECT_MAX_NUM_VERTICES)
    {
        return;
    }

    rotateVertices(object->vertices, rotatedVertices, object->numVertices, dir);

    for (i = 0; i < object->numTriangles; i++)
    {
        BaTriangle t = object->triangles[i];
        ba_graphicsDrawFullTriangle(rotatedVertices[t.vi1].x + x, rotatedVertices[t.vi1].y + y,
                rotatedVertices[t.vi2].x + x, rotatedVertices[t.vi2].y + y,
                rotatedVertices[t.vi3].x + x, rotatedVertices[t.vi3].y + y,
                color);
    }
}

