#include "buttons.h"
#include "math.h"

bool pressSuccess = false; // whether the button press has been successful

void CircleButton::drawButton() {
  tft.fillCircle(xPos, yPos, radius, colour);
  tft.drawChar(xPos - 12, yPos - 16, label, BLACK, colour, 5);
}

void SquareButton::drawButton() {
  tft.fillRoundRect(xPos, yPos, height, width, radius, colour);
  tft.drawChar(xPos + 14, yPos + 12, label, BLACK, colour, 5);
}

void TriangleButton::drawButton() {
  switch(type) {
    case TriangleButton::Type::LEFT : tft.fillTriangle(xPos - width/2, yPos, xPos + width/2, yPos - height/2, xPos + width/2, yPos + height/2, colour); break;
    case TriangleButton::Type::RIGHT : tft.fillTriangle(xPos + width/2, yPos, xPos - width/2, yPos - height/2, xPos - width/2, yPos + height/2, colour); break;
    case TriangleButton::Type::UP : tft.fillTriangle(xPos - width/2, yPos + height/2, xPos, yPos - height/2, xPos + width/2, yPos + height/2, colour); break;
    case TriangleButton::Type::DOWN : tft.fillTriangle(xPos - width/2, yPos - height/2, xPos, yPos + height/2, xPos + width/2, yPos - height/2, colour); break;
    default : tft.fillTriangle(xPos - width/2, yPos, xPos + width/2, yPos - height/2, xPos + width/2, yPos + height/2, colour); break;
  }
}

// Press button
void CircleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  colour = RED;
  this -> drawButton();
}

void SquareButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  colour = RED;
  this -> drawButton();
}

void TriangleButton::pressButton() {
  IRsend irsend;
  irsend.sendPanasonic(PanasonicAddress, hexCode);
  colour = RED;
  this -> drawButton();
}

void PowerButton::pressButton() {
  IRsend irsend;
  colour = GREEN;
  this -> drawButton();
  // simulate holding down the power button
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

bool TriangleButton::isPressed(int x, int y) {
 return x < (xPos + width/2) && x > (xPos - width/2)
     && y < (yPos + height/2) && y > (yPos - height/2);
}


// Reset buttons
void CircleButton::resetButton() {
  colour = GREEN;
  this -> drawButton();
}

void SquareButton::resetButton() {
  colour = WHITE;
  this -> drawButton();
}

void TriangleButton::resetButton() {
  colour = WHITE;
  this -> drawButton();
}

void PowerButton::resetButton() {
  colour = RED;
  this -> drawButton();
}
