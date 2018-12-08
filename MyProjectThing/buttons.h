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
    const int radius = 28; // needs radius to draw the circle
    char label;
    void drawButton();
    void pressButton();
    void resetButton();
    bool isPressed(int x, int y);
    CircleButton(int x, int y, uint16_t c, char l, unsigned long h)
    {
      xPos = x;
      yPos = y;
      colour = c;
      label = l;
      hexCode = h;
    }
};

class SquareButton: public Button {
  public:
    const int height = 56;
    const int width = 56;
    const int radius = 8;
    char label;
    void drawButton();
    void pressButton();
    void resetButton();
    bool isPressed(int x, int y);
    SquareButton(int x, int y, uint16_t c, char l, unsigned long h)
    {
      xPos = x;
      yPos = y;
      colour = c;
      label = l;
      hexCode = h;
    }
};

class TriangleButton: public Button {
  public:
    enum Type {UP, DOWN, LEFT, RIGHT};
    const int height = 56;
    const int width = 56;
    Type type;
    void drawButton();
    void pressButton();
    void resetButton();
    bool isPressed(int x, int y);
    TriangleButton(int x, int y, uint16_t c, Type t, unsigned long h)
    {
      xPos = x;
      yPos = y;
      colour = c;
      hexCode = h;
      type = t;
    }
};


class PowerButton: public CircleButton {
  public:
    void pressButton();
    void resetButton();
    PowerButton(int x, int y, uint16_t c, char l, unsigned long h)
    : CircleButton(x, y, c, l, h)
    {
      xPos = x;
      yPos = y;
      colour = c;
      label = l;
      hexCode = h;
    }
};
#endif
