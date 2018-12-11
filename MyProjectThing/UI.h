// UIController.h Created on: 22-06-2018, Author: @HarryEH

#ifndef UI_H_
#define UI_H_

#include <Adafruit_GFX.h>       // core graphics library
#include <Adafruit_HX8357.h>    // tft display
#include <Adafruit_STMPE610.h>  // touch screen
#include <vector>
#include "unphone.h"
#include "buttons.h"

class UI {
  public:
    int colourDelayIterations = 50;
    enum Page {NUMERICAL, FUNCTIONAL};
    Page page;
    PowerButton *power = new PowerButton(40, 445, 0xF800, 0x07E0, 'P', 0x100BCBD);
    ChangePageButton *change = new ChangePageButton(270, 445);
    std::vector<CircleButton*> circleButtons;
    std::vector<SquareButton*> squareButtons;
    std::vector<TriangleButton*> triangleButtons;
    UI() {
      drawNumericalButtons();
    }
    void drawUI();
    char handleTouch();
    void drawFunctionButtons();
    void drawNumericalButtons();
    void drawBlackScreen();
    void changePage();
    const uint16_t BLACK =   HX8357_BLACK;
    const uint16_t BLUE =    HX8357_BLUE;
    const uint16_t RED =     HX8357_RED;
    const uint16_t GREEN =   HX8357_GREEN;
    const uint16_t CYAN =    HX8357_CYAN;
    const uint16_t MAGENTA = HX8357_MAGENTA;
    const uint16_t YELLOW =  HX8357_YELLOW;
    const uint16_t WHITE =   HX8357_WHITE;
};

#endif
