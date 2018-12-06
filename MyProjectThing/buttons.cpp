#include "buttons.h"

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
      && y < (yPos + radius) && x > (yPos - radius);
}

