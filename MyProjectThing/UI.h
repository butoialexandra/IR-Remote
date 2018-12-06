// UIController.h Created on: 22-06-2018, Author: @HarryEH

#ifndef UI_H_
#define UI_H_

#include <Adafruit_GFX.h>       // core graphics library
#include <Adafruit_HX8357.h>    // tft display
#include <Adafruit_STMPE610.h>  // touch screen
#include "unphone.h"

class UI {
  // Colours
  const uint16_t BLACK =   HX8357_BLACK;
  const uint16_t BLUE =    HX8357_BLUE;
  const uint16_t RED =     HX8357_RED;
  const uint16_t GREEN =   HX8357_GREEN;
  const uint16_t CYAN =    HX8357_CYAN;
  const uint16_t MAGENTA = HX8357_MAGENTA;
  const uint16_t YELLOW =  HX8357_YELLOW;
  const uint16_t WHITE =   HX8357_WHITE;

  // Sizes
  const uint8_t  BOXSIZE = 40;
  const uint8_t  HALFBOX = (BOXSIZE / 2);
  const uint8_t  QUARTBOX = (BOXSIZE / 4);
  const uint8_t  PENRADIUS = 9; // orig: 3

    public:
      virtual bool handleTouch(long x, long y) = 0;
      virtual void draw() = 0;
};

#endif
