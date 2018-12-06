#include "buttons.h"
#include "math.h"

void CircleButton::drawButton() {
  tft.fillCircle(xPos, yPos, radius, colour);
  tft.drawChar(xPos - 12, yPos - 16, label, BLACK, colour, 5);
}

void CircleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  colour = RED;
  this -> drawButton();
}

bool CircleButton::isPressed(int x, int y) {
 return x < (xPos + radius) && x > (xPos - radius)
     && y < (yPos + radius) && y > (yPos - radius);
}

//bool CircleButton::isPressed(int x, int y) {
//  int squaredSums = (x-xPos)^2 + (y-yPos)^2; // must be int to avoid NaN error
//  int squaredRadius = radius^2; // must be int also
//  return squaredSums < squaredRadius;
//}
