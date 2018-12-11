#include "buttons.h"

bool pressSuccess = false; // whether the button press has been successful


void Button::changeHex(unsigned long h) {
  hexCode = h;
}

void CircleButton::drawButton() {
  tft.fillCircle(xPos, yPos, radius, currentColour);
  tft.drawChar(xPos - 12, yPos - 16, label, BLACK, currentColour, 5);
}

void SquareButton::drawButton() {
  tft.fillRoundRect(xPos - width/2, yPos - height/2, height, width, radius, currentColour);
  tft.drawChar(xPos - width/2 + 14, yPos - height/2 + 12, label, BLACK, currentColour, 5);
}

void TriangleButton::drawButton() {
  switch(type) {
    case TriangleButton::Type::LEFT : tft.fillTriangle(xPos - width/2, yPos, xPos + width/2, yPos - height/2, xPos + width/2, yPos + height/2, currentColour); break;
    case TriangleButton::Type::RIGHT : tft.fillTriangle(xPos + width/2, yPos, xPos - width/2, yPos - height/2, xPos - width/2, yPos + height/2, currentColour); break;
    case TriangleButton::Type::UP : tft.fillTriangle(xPos - width/2, yPos + height/2, xPos, yPos - height/2, xPos + width/2, yPos + height/2, currentColour); break;
    case TriangleButton::Type::DOWN : tft.fillTriangle(xPos - width/2, yPos - height/2, xPos, yPos + height/2, xPos + width/2, yPos - height/2, currentColour); break;
  }
}

void ChangePageButton::drawButton() {
  tft.fillRoundRect(xPos - width/2, yPos - height/2, height, width, radius, WHITE);
  tft.drawChar(xPos - width/2 + 14, yPos - height/2 + 12, label, BLACK, WHITE, 5);
}

// Press button
void CircleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  currentColour = inactiveColour;
  this -> drawButton();
}

void SquareButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  currentColour = inactiveColour;
  this -> drawButton();
}

void TriangleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  currentColour = inactiveColour;
  this -> drawButton();
}

void PowerButton::pressButton() {
  IRsend irsend;
  currentColour = inactiveColour;
  this -> drawButton();
  irsend.sendPanasonic(PanasonicAddress, hexCode);
}

// Check if a button isPressed
bool CircleButton::isPressed(int x, int y) {
 return x < (xPos + radius) && x > (xPos - radius)
     && y < (yPos + radius) && y > (yPos - radius);
}

bool SquareButton::isPressed(int x, int y) {
 return x < (xPos + width/2) && x > (xPos - width/2)
     && y < (yPos + height/2) && y > (yPos - height/2);
}

bool ChangePageButton::isPressed(int x, int y) {
 return x < (xPos + width/2) && x > (xPos - width/2)
     && y < (yPos + height/2) && y > (yPos - height/2);
}

bool TriangleButton::isPressed(int x, int y) {
 return x < (xPos + width/2) && x > (xPos - width/2)
     && y < (yPos + height/2) && y > (yPos - height/2);
}


// Reset buttons
void CircleButton::resetButton() {
  currentColour = activeColour;
  this -> drawButton();
}

void SquareButton::resetButton() {
  currentColour = activeColour;
  this -> drawButton();
}

void TriangleButton::resetButton() {
  currentColour = activeColour;
  this -> drawButton();
}

void PowerButton::resetButton() {
  currentColour = activeColour;
  this -> drawButton();
}
