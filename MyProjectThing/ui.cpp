#include "UI.h"
void UI::drawUI() {
  
  for(int i = 0; i<circleButtons.size(); i++) 
    circleButtons[i] -> drawButton();
  for(int i = 0; i<squareButtons.size(); i++) 
    squareButtons[i] -> drawButton();
  for(int i = 0; i<triangleButtons.size(); i++) 
    triangleButtons[i] -> drawButton();
//  // 
    power -> drawButton();
    change -> drawButton();
}

unsigned long now = 0;           // millis
unsigned long prevSigMillis = 0; // previous signal acceptance time
unsigned long sincePrevSig = 0;  // time since previous signal acceptance
uint16_t TIME_SENSITIVITY = 300; // 300 ms between touches
bool firstTime = true;           // first time in loop
int lastPressedCircle = -1;      // the number pressed last, -1 by default
int lastPressedSquare = -1;      // the number pressed last, -1 by default
int lastPressedTriangle = -1;    // the number pressed last, -1 by default
int iterationsSincePrev = 0;     // iterations since previous signal acceptance

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

  firstTime = false;

  // only handle touches every 300ms
  if (sincePrevSig > TIME_SENSITIVITY) {
    
    // scale the point from ~0->4000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());

    if (ts.touched()) {
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
        change -> pressButton();
       }
        
       // check if power button is pressed
       if(power -> isPressed(p.x, p.y)){
          for (int i=0; i < 5; i++) {
            power -> pressButton();
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

