// buttons.h

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Adafruit_GFX.h>       // core graphics library
#include <Adafruit_HX8357.h>    // tft display
#include <Adafruit_STMPE610.h>  // touch screen
#include <IRremote.h>
#include "unphone.h"

class Button {
  protected:
    const unsigned int PanasonicAddress = 0x4004;
    unsigned long hexCode; // keep the hex code private

    const uint16_t BLACK =   HX8357_BLACK;
    const uint16_t BLUE =    HX8357_BLUE;
    const uint16_t RED =     HX8357_RED;
    const uint16_t GREEN =   HX8357_GREEN;
    const uint16_t CYAN =    HX8357_CYAN;
    const uint16_t MAGENTA = HX8357_MAGENTA;
    const uint16_t YELLOW =  HX8357_YELLOW;
    const uint16_t WHITE =   HX8357_WHITE;

  public:
    int xPos, yPos;
    bool pressed;
    uint16_t colour;
    void drawButton();
    void pressButton(Button);
};

class CircleButton: public Button {
  public:
    int radius; // needs radius to draw the circle
    char label;
    void drawButton();
    void pressButton();
    CircleButton(int x, int y, int r, uint16_t c, char l, unsigned long h)
    {
      xPos = x;
      yPos = y;
      radius = r;
      colour = c;
      label = l;
      hexCode = h;
    }
};
#endif
