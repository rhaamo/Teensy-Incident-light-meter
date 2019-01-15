#ifndef OLED_FUNCTIONS_H
#define OLED_FUNCTIONS_H

#include <Arduino.h>
#include "TeensyView.h"

// Comes from LightMeter
extern const float isoTable[];
extern uint8_t isoTableSize;

/*
 * http://javl.github.io/image2cpp/
 * logo.png
 * 128x32, bg black, invert colors, code output "Arduino code, single bitmap", draw vertical
 */
static uint8_t screenlogo [LCDMEMORYSIZE] = {
	// 'logo, 128x32px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x04, 0x04, 0x04, 0x04, 0xfc, 0x04, 0x04, 0x04, 0x04, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 
	0x20, 0xc0, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0xc0, 0x20, 0xc0, 0xc0, 0x60, 0x20, 0x20, 0x40, 
	0x80, 0x00, 0x00, 0xc0, 0x20, 0x20, 0x20, 0x60, 0x40, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0xec, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 
	0x20, 0xe0, 0x80, 0x00, 0xfc, 0xc0, 0x60, 0x20, 0x40, 0xc0, 0x00, 0x00, 0x20, 0x20, 0xfc, 0x20, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xc0, 0x40, 0x20, 0x60, 0xc0, 0xc0, 0x20, 0x20, 
	0x40, 0x80, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0xc0, 0x00, 0x20, 0x20, 0xfc, 0x20, 
	0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0xc0, 0x00, 0xc0, 0x80, 0x40, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1a, 0x12, 0x12, 0x11, 
	0x09, 0x08, 0x0f, 0x1a, 0x12, 0x12, 0x11, 0x09, 0x08, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x0f, 0x00, 0x00, 0x08, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00, 0x07, 0x18, 0x10, 0x18, 0x87, 0x3f, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 
	0x0c, 0xc7, 0x3f, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x18, 0x07, 0x00, 0x00, 0x00, 0x0f, 0x18, 
	0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x00, 0x00, 
	0x18, 0x07, 0x00, 0x00, 0x0f, 0x1a, 0x12, 0x12, 0x11, 0x09, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x18, 
	0x00, 0x00, 0x0f, 0x1a, 0x12, 0x12, 0x11, 0x09, 0x08, 0x00, 0x1f, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

class OLEDFunctions : public TeensyView {
public:
  OLEDFunctions(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi)
      : TeensyView(rst, dc, cs, sck,
                   mosi){}; //(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck,
                            //uint8_t mosi)
  void drawByte(uint8_t x, uint8_t y,
                uint8_t c) //, uint8_t color, uint8_t mode);
  {
    uint8_t color = WHITE;
    uint8_t mode = NORM;
    for (int i = 0; i < 8;
         i++) { // 8 is the LCD's page height (see datasheet for explanation)
      if (c & 0x1) {
        pixel(x, y + 7 - i, color, mode);
      } else {
        pixel(x, y + 7 - i, !color, mode);
      }
      c >>= 1;
    }
  };
  void drawLogo(void) {
    drawBitmap(screenlogo);
    display();
  };
  void drawSeperator1(void) { line(0, 8, 127, 8); };
  void drawRightArrow(void) {
    drawByte(122, 0, 0x10);
    drawByte(123, 0, 0x10);
    drawByte(124, 0, 0x7C);
    drawByte(125, 0, 0x38);
    drawByte(126, 0, 0x10);
  };
  void drawLeftArrow(void) {
    drawByte(0, 0, 0x10);
    drawByte(1, 0, 0x38);
    drawByte(2, 0, 0x7C);
    drawByte(3, 0, 0x10);
    drawByte(4, 0, 0x10);
  };
  void drawLeftBracket(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 2, yIn + 0, 0x7F);
    drawByte(xIn + 3, yIn + 0, 0xFF);
    drawByte(xIn + 4, yIn + 0, 0xC0);
    drawByte(xIn + 5, yIn + 0, 0xC0);

    drawByte(xIn + 0, yIn + 8, 0x20);
    drawByte(xIn + 1, yIn + 8, 0x70);
    drawByte(xIn + 2, yIn + 8, 0xFF);
    drawByte(xIn + 3, yIn + 8, 0xDF);

    drawByte(xIn + 2, yIn + 16, 0xF0);
    drawByte(xIn + 3, yIn + 16, 0xF8);
    drawByte(xIn + 4, yIn + 16, 0x18);
    drawByte(xIn + 5, yIn + 16, 0x18);
  };
  void drawRightBracket(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0xC0);
    drawByte(xIn + 1, yIn + 0, 0xC0);
    drawByte(xIn + 2, yIn + 0, 0xFF);
    drawByte(xIn + 3, yIn + 0, 0x7F);

    drawByte(xIn + 2, yIn + 8, 0xDF);
    drawByte(xIn + 3, yIn + 8, 0xFF);
    drawByte(xIn + 4, yIn + 8, 0x70);
    drawByte(xIn + 5, yIn + 8, 0x20);

    drawByte(xIn + 0, yIn + 16, 0x18);
    drawByte(xIn + 1, yIn + 16, 0x18);
    drawByte(xIn + 2, yIn + 16, 0xF8);
    drawByte(xIn + 3, yIn + 16, 0xF0);
  };
  void eraseArrows(void) {
    rectFill(0, 0, 7, 8, BLACK, NORM);   // x,y,w,h,c,m
    rectFill(121, 0, 7, 8, BLACK, NORM); // x,y,w,h,c,m
  };
  void eraseMenu(void) {
    rectFill(7, 0, 115, 8, BLACK, NORM); // x,y,w,h,c,m
  };
  void eraseBracketArea(void) {
    rectFill(31, 10, 91, 21, BLACK, NORM); // x,y,w,h,c,m
  };
  void eraseTypeArea(void) {
    rectFill(0, 10, 25, 21, BLACK, NORM); // x,y,w,h,c,m
  };
  void eraseLowerArea(void) {
    rectFill(0, 9, 128, 22, BLACK, NORM); // x,y,w,h,c,m
  };
  void eraseOuterLowerArea(void) {
    rectFill(0, 9, 24, 30, BLACK, NORM); // x,y,w,h,c,m
  };
  void drawMenu(const char *name, bool lArrow, bool rArrow) {
    setCursor(10, 0);
    print(name);
    // Force widget
    batteryStyle1(116, 0, (float)lipo.soc() / 100);
    if (lipo.current(AVG) >= -1) {
      drawPlug(106, 0);
    } else {
      drawByte(106, 0, 0x00);
      drawByte(107, 0, 0x00);
      drawByte(108, 0, 0x00);
      drawByte(109, 0, 0x00);
      drawByte(110, 0, 0x00);
      drawByte(111, 0, 0x00);
      drawByte(112, 0, 0x00);
      drawByte(113, 0, 0x00);
      drawByte(114, 0, 0x00);
    }
    // if( lArrow ) drawLeftArrow();
    // if( rArrow ) drawRightArrow();
    drawSeperator1();
  };
  bool drawBatteryWidget(void) {
    bool returnVar = false;
    int tempSOC = lipo.soc();
    int tempChargerState;
    if (lipo.current(AVG) >= -1) {
      tempChargerState = 1;
    } else {
      tempChargerState = 0;
    }
    if (lastSOC != tempSOC) {
      returnVar = true;
      lastSOC = tempSOC;
      // redraw
      batteryStyle1(116, 0, (float)lipo.soc() / 100);
    }
    if (lastChargerState != tempChargerState) {
      returnVar = true;
      lastChargerState = tempChargerState;
      // redraw
      if (lastChargerState == 1) {
        drawPlug(106, 0);
      } else {
        drawByte(106, 0, 0x00);
        drawByte(107, 0, 0x00);
        drawByte(108, 0, 0x00);
        drawByte(109, 0, 0x00);
        drawByte(110, 0, 0x00);
        drawByte(111, 0, 0x00);
        drawByte(112, 0, 0x00);
        drawByte(113, 0, 0x00);
        drawByte(114, 0, 0x00);
      }
    }
    return returnVar;
  };
  void drawMenu(const char *name, uint16_t isoValue, bool lArrow, bool rArrow) {
    drawMenu(name, lArrow, rArrow);
    setCursor(60, 0);
    print("ISO");
    setCursor(77, 0);
    print(":");
    setCursor(81, 0);
    print(isoValue);
  };
  void drawISOScale(uint8_t isoSetting) {
    uint8_t x1, x2, x3;
    eraseLowerArea();
    if (isoTable[isoSetting] < 1000) {
      x2 = 53;
      x3 = x2 + 28;
    } else {
      x2 = 48;
      x3 = x2 + 37;
    }
    setFontType(1);
    setCursor(x2, 15);
    print(isoTable[isoSetting], 0);
    setFontType(0);
    if (isoSetting < (isoTableSize - 1)) {
      // draw next higher value
      setCursor(x3, 19);
      print(isoTable[isoSetting + 1], 0);
    }
    if (isoSetting > 0) {
      if (isoTable[isoSetting - 1] < 1000) {
        x1 = x2 - 20;
      } else {
        x1 = x2 - 26;
      }
      // draw next lower value
      setCursor(x1, 19);
      print(isoTable[isoSetting - 1], 0);
    }
  };
  void drawExposureStyle1(float exposureTime, uint8_t xIn, uint8_t yIn) {
    if (exposureTime < 1) {
      setCursor(xIn, yIn);
      print("1");
      line(xIn + 4, yIn + 10, xIn + 9, yIn + 2, WHITE, NORM);
      setCursor(xIn + 10, yIn + 5);
      print(1 / exposureTime, 0);
    } else {
      setCursor(xIn, yIn + 3);
      print(exposureTime, 1);
    }
  };
  void drawExposureStyle2(float exposureTime, uint8_t xIn, uint8_t yIn) {
    setCursor(xIn, yIn);
    print("1");
    line(xIn + 4, yIn + 10, xIn + 9, yIn + 2, WHITE, NORM);
    setCursor(xIn + 10, yIn + 5);
    print(1 / exposureTime, 0);
  };
  void drawFNumStyle1(float fNumIn, uint8_t xIn, uint8_t yIn) {
    setCursor(xIn, yIn + 4);
    print("F");
    setFontType(1);
    uint16_t whole = (uint16_t)fNumIn; // f. xx
    float partial = (fNumIn - whole);  // f.   .yy
    if (partial > 0) {
      // If f. is xx.yy
      uint8_t offset = 0;
      if (whole >= 10) {
        offset = 10; // two chars
      }

      setCursor(xIn + 8, yIn);
      print(whole);
      setCursor(xIn + 18 + offset, yIn);
      print(partial * 10, 0);
      pixel(xIn + 15 + offset, yIn + 9, WHITE, NORM);
      pixel(xIn + 15 + offset, yIn + 10, WHITE, NORM);
      pixel(xIn + 16 + offset, yIn + 9, WHITE, NORM);
      pixel(xIn + 16 + offset, yIn + 10, WHITE, NORM);
    } else {
      // f. is xx
      setCursor(xIn + 8, yIn);
      print(whole);
    }
    setFontType(0);
  };
  void drawFNumStyle2(float fNumIn, uint8_t xIn,
                      uint8_t yIn) // always 1 decimal
  {
    setCursor(xIn, yIn);
    print("F");
    setFontType(1);
    line(xIn + 4, yIn + 10, xIn + 9, yIn + 2, WHITE, NORM);
    // long whole = (long)fNumIn;
    // float partial = ( fNumIn - (long)fNumIn );
    // Print 1 dec
    setCursor(xIn + 10, yIn + 5);
    print(fNumIn);
    // print( whole );
    // setCursor(xIn + 20,yIn + 5);
    // print( partial * 10, 0 );
    // pixel(xIn + 17,yIn + 15, WHITE,NORM);
    // pixel(xIn + 17,yIn + 16, WHITE,NORM);
    // pixel(xIn + 18,yIn + 15, WHITE,NORM);
    // pixel(xIn + 18,yIn + 16, WHITE,NORM);
    setFontType(0);
  };
  void holdStyle1(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0xF8);
    drawByte(xIn + 1, yIn + 0, 0x20);
    drawByte(xIn + 2, yIn + 0, 0x20);
    drawByte(xIn + 3, yIn + 0, 0xF8);

    drawByte(xIn + 5, yIn + 0, 0x30);
    drawByte(xIn + 6, yIn + 0, 0x48);
    drawByte(xIn + 7, yIn + 0, 0x48);
    drawByte(xIn + 8, yIn + 0, 0x30);

    drawByte(xIn + 10, yIn + 0, 0x78);
    drawByte(xIn + 11, yIn + 0, 0x08);
    drawByte(xIn + 12, yIn + 0, 0x08);

    drawByte(xIn + 14, yIn + 0, 0x78);
    drawByte(xIn + 15, yIn + 0, 0x48);
    drawByte(xIn + 16, yIn + 0, 0x30);
  };
  void aveStyle1(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0x80);
    drawByte(xIn + 1, yIn + 0, 0x80);
    drawByte(xIn + 2, yIn + 0, 0xF8);
    drawByte(xIn + 3, yIn + 0, 0x80);
    drawByte(xIn + 4, yIn + 0, 0x80);

    drawByte(xIn + 5, yIn + 0, 0x78);
    drawByte(xIn + 6, yIn + 0, 0x50);
    drawByte(xIn + 7, yIn + 0, 0x28);

    drawByte(xIn + 9, yIn + 0, 0x48);
    drawByte(xIn + 10, yIn + 0, 0x78);
    drawByte(xIn + 11, yIn + 0, 0x48);

    drawByte(xIn + 13, yIn + 0, 0x70);
    drawByte(xIn + 14, yIn + 0, 0x88);
    drawByte(xIn + 15, yIn + 0, 0xA8);
    drawByte(xIn + 16, yIn + 0, 0x30);
  };
  void batteryStyle1(uint8_t xIn, uint8_t yIn, float percent) {
    drawByte(xIn + 0, yIn + 0, 0xFE);
    drawByte(xIn + 1, yIn + 0, 0x82);
    drawByte(xIn + 2, yIn + 0, 0x82);
    drawByte(xIn + 3, yIn + 0, 0x82);
    drawByte(xIn + 4, yIn + 0, 0x82);
    drawByte(xIn + 5, yIn + 0, 0x82);
    drawByte(xIn + 6, yIn + 0, 0x82);
    drawByte(xIn + 7, yIn + 0, 0x82);
    drawByte(xIn + 8, yIn + 0, 0x82);
    drawByte(xIn + 9, yIn + 0, 0xEE);
    drawByte(xIn + 10, yIn + 0, 0x38);
    // line(xIn, yIn + 1, xIn + (percent * 10), yIn + 1);
    line(xIn + 2, yIn + 2, xIn + 2 + (percent * 6), yIn + 2);
    line(xIn + 2, yIn + 3, xIn + 2 + (percent * 6), yIn + 3);
    line(xIn + 2, yIn + 4, xIn + 2 + (percent * 6), yIn + 4);
    // line(xIn, yIn + 5, xIn + (percent * 10), yIn + 5);
    // rectFill(80 , 25, 84, 30);
  };
  void drawHeart(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0x3C);
    drawByte(xIn + 1, yIn + 0, 0x7F);
    drawByte(xIn + 2, yIn + 0, 0xFF);
    drawByte(xIn + 3, yIn + 0, 0xFF);
    drawByte(xIn + 4, yIn + 0, 0xFF);
    drawByte(xIn + 5, yIn + 0, 0xFF);
    drawByte(xIn + 6, yIn + 0, 0x7F);
    drawByte(xIn + 7, yIn + 0, 0x3F);
    drawByte(xIn + 8, yIn + 0, 0x3F);
    drawByte(xIn + 9, yIn + 0, 0x7F);
    drawByte(xIn + 10, yIn + 0, 0xFF);
    drawByte(xIn + 11, yIn + 0, 0xFF);
    drawByte(xIn + 12, yIn + 0, 0xFF);
    drawByte(xIn + 13, yIn + 0, 0xFF);
    drawByte(xIn + 14, yIn + 0, 0x7F);
    drawByte(xIn + 15, yIn + 0, 0x3C);

    drawByte(xIn + 0, yIn + 8, 0x00);
    drawByte(xIn + 1, yIn + 8, 0x00);
    drawByte(xIn + 2, yIn + 8, 0x80);
    drawByte(xIn + 3, yIn + 8, 0xC0);
    drawByte(xIn + 4, yIn + 8, 0xE0);
    drawByte(xIn + 5, yIn + 8, 0xF0);
    drawByte(xIn + 6, yIn + 8, 0xF8);
    drawByte(xIn + 7, yIn + 8, 0xFC);
    drawByte(xIn + 8, yIn + 8, 0xFC);
    drawByte(xIn + 9, yIn + 8, 0xF8);
    drawByte(xIn + 10, yIn + 8, 0xF0);
    drawByte(xIn + 11, yIn + 8, 0xE0);
    drawByte(xIn + 12, yIn + 8, 0xC0);
    drawByte(xIn + 13, yIn + 8, 0x80);
    drawByte(xIn + 14, yIn + 8, 0x00);
    drawByte(xIn + 15, yIn + 8, 0x00);
  };
  void drawPlug(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0x28);
    drawByte(xIn + 1, yIn + 0, 0x28);
    drawByte(xIn + 2, yIn + 0, 0x7C);
    drawByte(xIn + 3, yIn + 0, 0x44);
    drawByte(xIn + 4, yIn + 0, 0x44);
    drawByte(xIn + 5, yIn + 0, 0x38);
    drawByte(xIn + 6, yIn + 0, 0x10);
    drawByte(xIn + 7, yIn + 0, 0x10);
    drawByte(xIn + 8, yIn + 0, 0x10);
  };
  void drawNo(uint8_t xIn, uint8_t yIn) {
    drawByte(xIn + 0, yIn + 0, 0x38);
    drawByte(xIn + 1, yIn + 0, 0x44);
    drawByte(xIn + 2, yIn + 0, 0x8A);
    drawByte(xIn + 3, yIn + 0, 0x92);
    drawByte(xIn + 4, yIn + 0, 0xA2);
    drawByte(xIn + 5, yIn + 0, 0x44);
    drawByte(xIn + 6, yIn + 0, 0x38);
  };

private:
  int16_t lastSOC;
  int16_t lastChargerState = 0;
};

#endif