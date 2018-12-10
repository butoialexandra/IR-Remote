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
    PowerButton *power = new PowerButton(40, 445, 0xF800, 'P', 0x100BCBD);
    ChangePageButton *change = new ChangePageButton(270, 445);
    std::vector<CircleButton*> circleButtons;
    std::vector<SquareButton*> squareButtons;
    std::vector<TriangleButton*> triangleButtons;
    UI(std::vector<CircleButton*> cs, std::vector<SquareButton*> ss, std::vector<TriangleButton*> ts) {
      circleButtons = cs;
      squareButtons = ss;
      triangleButtons = ts;
    }
    void drawUI();
    void handleTouch();
    void drawFunctionButtons();
};

#endif
