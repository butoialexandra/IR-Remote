#include "buttons.h"

void CircleButton::drawButton() {
  tft.fillCircle(xPos, yPos, 28, colour);
  tft.drawChar(xPos - 12, yPos - 16, label, BLACK, colour, 5);
}

void CircleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
}

