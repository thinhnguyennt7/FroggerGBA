#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
    // TA-TODO: IMPLEMENT

    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
    while(*SCANLINECOUNTER > 160);
    // Write a while loop that keeps going until we're in vBlank:
    while(*SCANLINECOUNTER < 160);
  // Finally, increment the vBlank counter:
    vBlankCounter += 1;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(x, y, 240)] = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    volatile unsigned short newColor = color;
    for (int i = 0; i < height; i++) {
        DMA[3].cnt = 0;
        DMA[3].src = &newColor;
        DMA[3].dst = &videoBuffer[OFFSET(x + i, y, 240)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}

void drawFullScreenImageDMA(u16 *image) {
    drawImageDMA(0, 0, 240, 160, image);
}

void drawImageDMA(int x, int y, int width, int height, u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &image[r*width];
        DMA[3].dst = &videoBuffer[240 * (x + r) + y];
        DMA[3].cnt = width | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void fillScreenDMA(volatile u16 color) {
    // TA-TODO: IMPLEMENT
    UNUSED(color);
    // drawImageDMA(0, 0, 240, 160, color);
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}
void drawCharHorizontal(int row, int col, char ch, u16 color) {
    for(int r = 0; r < 8; r++) {
        for(int c = 0; c < 6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(row + r, col + c, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        row += 6;
    }
}

void drawStringHorizontal(int row, int col, char *str, u16 color) {
    while(*str) {
        drawCharHorizontal(row, col, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}
