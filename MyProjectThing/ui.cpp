#include "UI.h"

/*
 * Variables for registering sensible touch; only register if > 300ms since last
 */
unsigned long now = 0;           // millis
unsigned long prevSigMillis = 0; // previous signal acceptance time
unsigned long sincePrevSig = 0;  // time since previous signal acceptance
uint16_t TIME_SENSITIVITY = 250; // 300 ms between touches
bool firstTime = false;           // first time in loop

/*
 * Variables for calculating button positions
 */
int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;


/*
 * Handles touch on UI, returning the label of the button pressed
 */
char UI::handleTouch() {
  char buttonPressed = ' '; // by default no sensible button is pressed
  
  // work out time since last touch
  now = millis();
  if(firstTime) {
    sincePrevSig = TIME_SENSITIVITY + 1;
  } else {
    sincePrevSig = now - prevSigMillis;
  }
  
  // retrieve the touch point
  TS_Point p = ts.getPoint();

  // no longer first time into loop
  firstTime = false;

  // only handle touch if longer than 300 ms since last touch
  if (sincePrevSig > TIME_SENSITIVITY) {
    
    // scale the point from ~0->4000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());

    if (ts.touched()) {
      if (p.z > 10 && p.z < 50) {                                        // if touch has enough pressure
        for (int i = 0; i < circleButtons.size(); i++) {                 // check whether a circle button is pressed
          if(circleButtons[i] -> isPressed(p.x, p.y)) {
            circleButtons[i] -> pressButton();
            buttonPressed = circleButtons[i] -> label;
          }
        }
        for (int i = 0; i < squareButtons.size(); i++) {                 // check whether a square button is pressed
          if(squareButtons[i] -> isPressed(p.x, p.y)) {
            squareButtons[i] -> pressButton();
            buttonPressed = squareButtons[i] -> label;
          }
        }
        for (int i = 0; i < triangleButtons.size(); i++) {               // check whether a triangle button is pressed
          if(triangleButtons[i] -> isPressed(p.x, p.y)) {
            triangleButtons[i] -> pressButton();
          }
        }
       if(change -> isPressed(p.x, p.y)) {                               // check whether change page button is pressed
         this -> changePage();
       }
       if(power -> isPressed(p.x, p.y)){                                 // check whether power button is pressed
          power -> pressButton();
        }
      }
    }
    prevSigMillis = now;                                                 // successful touch, update time
  }
  return buttonPressed;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////                         Functions for drawing the User Interface                              //////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*
 *  Draw the function buttons page
 */
void UI::drawFunctionButtons() {
  
  // Clear current buttons
  circleButtons.clear();
  squareButtons.clear();
  triangleButtons.clear();

  // Create the new buttons of each shape
  circleButtons = {
    new CircleButton(50, 50, WHITE, RED, 'M', 0x1004C4D),                                  // Mute
    new CircleButton(50, 115, WHITE, RED, 'T', 0x100ECED),                                 // Tune
    new CircleButton(50, 245, WHITE, RED, 'B', 0x1002B2A),                                 // Back
    new CircleButton(50, 180, WHITE, RED, 'S', 0x100A0A1),                                 // Source
    new CircleButton(50, 310, WHITE, RED, 'G', 0x190E170)                                  // Guide
    };

  squareButtons = {
    new SquareButton(260, 110, WHITE, RED, '>', 0x1002C2D),                                // Channel Up
    new SquareButton(140, 110, WHITE, RED, '<', 0x100ACAD),                                // Channel Down
    new SquareButton(200, 70, WHITE, RED, '+', 0x1000405),                                 // Volume Up
    new SquareButton(200, 150, WHITE, RED, '-', 0x1008485),                                // Volume Down
    new SquareButton(200, 330, RED, WHITE, 'E', 0x1009293)                                 // Enter
  };

  triangleButtons = {
    new TriangleButton(130, 330, WHITE, RED, TriangleButton::Type::LEFT, 0x1007273),       // Left
    new TriangleButton(270, 330, WHITE, RED, TriangleButton::Type::RIGHT, 0x100f2f3),      // Right
    new TriangleButton(200, 260, WHITE, RED, TriangleButton::Type::UP, 0x1005253),         // Up
    new TriangleButton(200, 400, WHITE, RED, TriangleButton::Type::DOWN, 0x100d2d3)        // Down  
  };
  
  // Change page and redraw
  page = FUNCTIONAL;
  drawUI();
}


/*
 * Draw the numerical buttons page
 */
void UI::drawNumericalButtons() {

  // Empty the current buttons
  circleButtons.clear();
  squareButtons.clear();
  triangleButtons.clear();
  
   // Calculate equidistant button positions (like mobile phone)
  int positions[10][2];

  // positions for 0 button
  positions[0][0] = SCREEN_WIDTH / 4 * 2;
  positions[0][1] = SCREEN_HEIGHT / 5 * 4;

  // positions for 1-9 buttons
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int buttonNo = 3 * j + i + 1;
      positions[buttonNo][0] = SCREEN_WIDTH / 4 * (i + 1);
      positions[buttonNo][1] = SCREEN_HEIGHT / 5 * (j + 1);
    }
   }
   
  // Create the new buttons
  circleButtons = {
    new CircleButton(positions[0][0], positions[0][1], GREEN, RED, '0', 0x1009899),      // 0
    new CircleButton(positions[1][0], positions[1][1], GREEN, RED, '1', 0x1000809),      // 1
    new CircleButton(positions[2][0], positions[2][1], GREEN, RED, '2', 0x1008889),      // 2
    new CircleButton(positions[3][0], positions[3][1], GREEN, RED, '3', 0x1004849),      // 3
    new CircleButton(positions[4][0], positions[4][1], GREEN, RED, '4', 0x100C8C9),      // 4
    new CircleButton(positions[5][0], positions[5][1], GREEN, RED, '5', 0x1002829),      // 5
    new CircleButton(positions[6][0], positions[6][1], GREEN, RED, '6', 0x100A8A9),      // 6
    new CircleButton(positions[7][0], positions[7][1], GREEN, RED, '7', 0x1006869),      // 7
    new CircleButton(positions[8][0], positions[8][1], GREEN, RED, '8', 0x100E8E9),      // 8
    new CircleButton(positions[9][0], positions[9][1], GREEN, RED, '9', 0x1001819)       // 9
  };

  // Switch page and redraw
  page = NUMERICAL;
  drawUI();
}


/*
 * Draw the blank screen/background
 */
void UI::drawBlackScreen() {
  tft.fillScreen(BLACK);
  tft.drawLine(0,0,319,0,WHITE);
  tft.drawLine(319,0,319,479,WHITE);
  tft.drawLine(319,479,0,479,WHITE);
  tft.drawLine(0,479,0,0,WHITE);
}


/*
 * Switch which page is displayed on the User Interface
 */
void UI::changePage() {
  if (page == NUMERICAL) {
    drawFunctionButtons();
  } else {
    drawNumericalButtons();
  }
}


/*
 * Draw each button onto the User Interface
 */
void UI::drawUI() {
  drawBlackScreen();
  for(int i = 0; i<circleButtons.size(); i++) 
    circleButtons[i] -> drawButton();
  for(int i = 0; i<squareButtons.size(); i++) 
    squareButtons[i] -> drawButton();
  for(int i = 0; i<triangleButtons.size(); i++) 
    triangleButtons[i] -> drawButton();
  power -> drawButton();
  change -> drawButton();
}

