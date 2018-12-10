#include "UI.h"
#include "unphone.h"

/*
 * Variables for registering sensible touch
 */
unsigned long now = 0;           // millis
unsigned long prevSigMillis = 0; // previous signal acceptance time
unsigned long sincePrevSig = 0;  // time since previous signal acceptance
uint16_t TIME_SENSITIVITY = 300; // 300 ms between touches
bool firstTime = true;           // first time in loop

// Timers for each button press by type
int lastPressedCircle = -1;      // the number pressed last, -1 by default
int lastPressedSquare = -1;      // the number pressed last, -1 by default
int lastPressedTriangle = -1;    // the number pressed last, -1 by default
int iterationsSincePrev = 0;     // iterations since previous signal acceptance

// Variables for spacing numerical buttons
int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;




void UI::handleTouch() {
  // Get the time
  now = millis();
  if(firstTime) {
    sincePrevSig = TIME_SENSITIVITY + 1;
  } else {
    sincePrevSig = now - prevSigMillis;
  }

  // reset button colour after 50 iterations
  if(iterationsSincePrev == 50) {
    if (lastPressedCircle > -1) {                           // Handle Circle Pressed                
      circleButtons[lastPressedCircle] -> resetButton();
      lastPressedCircle = -1;                        
    } else if (lastPressedSquare > -1) {                    // Handle Square Pressed
      squareButtons[lastPressedSquare] -> resetButton();
      lastPressedSquare = -1;
    } else if (lastPressedTriangle > -1) {                  // Handle Triangle Pressed
      triangleButtons[lastPressedTriangle] -> resetButton();
      lastPressedTriangle = -1;
    } else {
      power -> resetButton();                               // Power button was pressed
    }
    iterationsSincePrev = 0;                                // Reset counter
  }
 
  // retrieve the touch point
  TS_Point p = ts.getPoint();

  // no longer first time into loop
  firstTime = false;

  // only handle touches every 300ms
  if (sincePrevSig > TIME_SENSITIVITY) {
    
    // scale the point from ~0->4000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());

    if (ts.touched()) {
      Serial.println("Touched");
    // if touch has enough pressure
      if (p.z > 10 && p.z < 50) {
        
        // check whether a number button is pressed
        for (int i = 0; i < circleButtons.size(); i++) {
          if(circleButtons[i] -> isPressed(p.x, p.y)) {
            circleButtons[i] -> pressButton();
            lastPressedCircle = i;
            iterationsSincePrev = 1; // begin iterating after button press
            Serial.println(i);
          }
        }

        // check whether a number button is pressed
        for (int i = 0; i < squareButtons.size(); i++) {
          if(squareButtons[i] -> isPressed(p.x, p.y)) {
            squareButtons[i] -> pressButton();
            lastPressedSquare = i;
            iterationsSincePrev = 1; // begin iterating after button press
            Serial.println(i);
          }
        }

        // check whether a number button is pressed
        for (int i = 0; i < triangleButtons.size(); i++) {
          if(triangleButtons[i] -> isPressed(p.x, p.y)) {
            triangleButtons[i] -> pressButton();
            lastPressedTriangle = i;
            iterationsSincePrev = 1; // begin iterating after button press
            Serial.println(i);
          }
        }

       if(change -> isPressed(p.x, p.y)) {
         this -> changePage();
         Serial.println("CHANGE");
       }
        
       // check if power button is pressed
       if(power -> isPressed(p.x, p.y)){
          for (int i=0; i < 5; i++) {
            power -> pressButton();
            delay(50);
          }
          
          iterationsSincePrev = 1; // begin iterating after button press
          // prevSigMillis = now; // power on requires 5 signals back to back (within ~250ms), bypass time sensitivity
        }
      }
    }
    
    prevSigMillis = now; // successful touch, update time
  }

  // only iterate if button has been pressed
  if (iterationsSincePrev > 0) {
    iterationsSincePrev ++;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Functions for drawing the UI
 */
// Adds function buttons to the User Interface
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
    new CircleButton(50, 180, WHITE, RED, 'S', 0x100A0A1)                                  // Source
    };

  squareButtons = {
    new SquareButton(260, 110, WHITE, RED, '>', 0x1002C2D),                                // Channel Up
    new SquareButton(140, 110, WHITE, RED, '<', 0x1002C2D),                                // Channel Down
    new SquareButton(200, 70, WHITE, RED, '+', 0x1002C2D),                                 // Volume Up
    new SquareButton(200, 150, WHITE, RED, '-', 0x1002C2D),                                // Volume Down
    new SquareButton(200, 330, RED, WHITE, 'E', 0x1009293)                                 // Enter
  };

  triangleButtons = {
    new TriangleButton(130, 330, WHITE, RED, TriangleButton::Type::LEFT, 0x1007273),       // Left
    new TriangleButton(270, 330, WHITE, RED, TriangleButton::Type::RIGHT, 0x100f2f3),      // Right
    new TriangleButton(200, 260, WHITE, RED, TriangleButton::Type::UP, 0x1005253),         // Up
    new TriangleButton(200, 400, WHITE, RED, TriangleButton::Type::DOWN, 0x1005253)        // Down  
  };
  
  // Change page and redraw
  page = FUNCTIONAL;
  drawUI();
}


// Adds numerical buttons to the User Interface
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


// Draw a black screen with white edges
void UI::drawBlackScreen() {
  tft.fillScreen(BLACK);
  tft.drawLine(0,0,319,0,WHITE);
  tft.drawLine(319,0,319,479,WHITE);
  tft.drawLine(319,479,0,479,WHITE);
  tft.drawLine(0,479,0,0,WHITE);
}


// Change the current page to the other page
void UI::changePage() {
  if (page == NUMERICAL) {
    drawFunctionButtons();
  } else {
    drawNumericalButtons();
  }
}


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

