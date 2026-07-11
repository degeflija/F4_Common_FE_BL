#include "BaDisplayTest.h"

void ba_displayTestDrawCrosstalkPattern(uint8_t colA, uint8_t colB, uint8_t colC)
{
    int32_t maxX = ba_displayGetWidth() - 1;
    int32_t maxY = ba_displayGetHeight() - 1;

    int32_t col1StartX = 0;
    int32_t col1EndX = maxX/2 - 1;
    int32_t col2StartX = col1EndX + 1;
    int32_t col2EndX = col2StartX + ((maxX*2)/6) - 1;
    int32_t col3StartX = col2EndX + 1;
    int32_t col3EndX = maxX;

    int32_t row1StartY = 0;
    int32_t row1EndY = maxY/2 - 1;
    int32_t row2StartY = row1EndY + 1;
    int32_t row2EndY = row2StartY + ((maxY*2)/6) - 1;
    int32_t row3StartY = row2EndY + 1;
    int32_t row3EndY = maxY;

    ba_graphicsDrawFullRect(col1StartX, row1StartY, col1EndX, row1EndY, colA);
    ba_graphicsDrawFullRect(col1StartX, row2StartY, col1EndX, row2EndY, colC);
    ba_graphicsDrawFullRect(col1StartX, row3StartY, col1EndX, row3EndY, colA);

    ba_graphicsDrawFullRect(col2StartX, row1StartY, col2EndX, row1EndY, colC);
    ba_graphicsDrawFullRect(col2StartX, row2StartY, col2EndX, row2EndY, colB);
    ba_graphicsDrawFullRect(col2StartX, row3StartY, col2EndX, row3EndY, colC);

    ba_graphicsDrawFullRect(col3StartX, row1StartY, col3EndX, row1EndY, colA);
    ba_graphicsDrawFullRect(col3StartX, row2StartY, col3EndX, row2EndY, colC);
    ba_graphicsDrawFullRect(col3StartX, row3StartY, col3EndX, row3EndY, colA);

    ba_displayRefresh();
}

void ba_displayTestDrawGrayScalePattern(void)
{
    int32_t maxX = ba_displayGetWidth() - 1;
    int32_t maxY = ba_displayGetHeight() - 1;

    int32_t width = maxX / 5;

    ba_graphicsDrawFullRect(0, 0, width - 1, maxY, COLOR_BLACK);

    ba_displayRefresh();

    ba_graphicsDrawFullRect(width, 0, (2*width) - 1, maxY, COLOR_GREY_25PERCENT);

    ba_displayRefresh();

    ba_graphicsDrawFullRect((2*width), 0, (3*width) - 1, maxY, COLOR_GREY_50PERCENT);

    ba_displayRefresh();

    ba_graphicsDrawFullRect((3*width), 0, (4*width) - 1, maxY, COLOR_GREY_75PERCENT);

    ba_displayRefresh();

    ba_graphicsDrawFullRect((4*width), 0, maxX, maxY, COLOR_WHITE);

    ba_displayRefresh();
}
