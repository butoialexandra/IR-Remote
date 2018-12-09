// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the 
// TestScreen::activate(true); to false to change this behaviour.

#include "unphone.h"
#include "buttons.h"

// Color definitions
#define RED      0xF800
#define GREEN    0x07E0
#define WHITE    0xFFFF
#define BLACK    0x0000
#define CYAN     0x07FF
#define TFT_DC   33
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750
int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;

// declare array of digit buttons
CircleButton *numbers [10];
PowerButton *power = new PowerButton(40, 445, RED, 'P', 0x100BCBD);

void setup() {
  Wire.setClock(100000); // higher rates trigger an IOExpander bug
  UNPHONE_DBG = true;
  Serial.begin(115200);  // init the serial line

  // fire up I²C, and the unPhone's IOExpander library
  Wire.begin();
  IOExpander::begin();

  checkPowerSwitch(); // check if power switch is now off & if so shutdown

  // which board version are we running?
  int version = IOExpander::getVersionNumber(), spin;
  if(version == 7) spin = 4;
  Serial.printf("starting, running on spin %d\n", spin);
  
  // show initial test screen on the LCD
  IOExpander::digitalWrite(IOExpander::BACKLIGHT, LOW);
  tft.begin(HX8357D);
  TestScreen::activate(false);
  TestScreen::init();
  IOExpander::digitalWrite(IOExpander::BACKLIGHT, HIGH);

  if(! ts.begin()) { // init the touchscreen
    D("failed to start touchscreen controller");
    TestScreen::fail("TOUCH");
    delay(3000);
  } else {
    D("touchscreen started");
  }
  
  if(!accel.begin()) // set up the accelerometer
  {
    D("Failed to start accelerometer");
    TestScreen::fail("ACCEL");
    delay(3000);
  }
  
  i2s_config(); // configure the I2S bus

  pinMode(IR_LEDS, OUTPUT); // IR_LED pin
  
  // set up the SD card
  IOExpander::digitalWrite(IOExpander::SD_CS, LOW);
  if(!SD.begin(-1)) {
    D("Card Mount Failed");
    TestScreen::fail("SD CARD");
    delay(3000);
  }
  IOExpander::digitalWrite(IOExpander::SD_CS, HIGH);

  if(! musicPlayer.begin()) { // initialise the music player
    D("Couldn't find VS1053, do you have the right pins defined?");
    TestScreen::fail("AUDIO");
    delay(3000);
  } else {
    D("VS1053 found");
  }
   
  // send a LoRaWAN message to TTN
  lmic_init();
  lmic_do_send(&sendjob);

  // draw 0-9 buttons
  drawNumericalButtons();
  drawFunctionButtons();
}

// draw function buttons
void drawFunctionButtons() {
  // make black screen
  tft.fillScreen(BLACK);
  tft.drawLine(0,0,319,0,WHITE);
  tft.drawLine(319,0,319,479,WHITE);
  tft.drawLine(319,479,0,479,WHITE);
  tft.drawLine(0,479,0,0,WHITE);

  // mute
  CircleButton *mute = new CircleButton(50, 50, WHITE, 'M', 0x1004C4D);
  mute -> drawButton();
  // channel >
  SquareButton *channelRight = new SquareButton(260, 110, WHITE, '>', 0x1002C2D);
  channelRight -> drawButton();
  // channel <
  SquareButton *channelLeft = new SquareButton(140, 110, WHITE, '<', 0x1002C2D);
  channelLeft -> drawButton();
  // volume + 
  SquareButton *volumeUp = new SquareButton(200, 70, WHITE, '+', 0x1002C2D);
  volumeUp -> drawButton();
  // volume -
  SquareButton *volumeDown = new SquareButton(200, 150, WHITE, '-', 0x1002C2D);
  volumeDown -> drawButton();
  // tune 
  CircleButton *tune = new CircleButton(50, 115, WHITE, 'T', 0x100ECED);
  tune -> drawButton();
  // left 
  TriangleButton *left = new TriangleButton(130, 330, WHITE, TriangleButton::Type::LEFT, 0x1007273);
  left -> drawButton();
  // right
  TriangleButton *right = new TriangleButton(270, 330, WHITE, TriangleButton::Type::RIGHT, 0x100f2f3);
  right -> drawButton();
  // up
  TriangleButton *up = new TriangleButton(200, 260, WHITE, TriangleButton::Type::UP, 0x1005253);
  up -> drawButton();
  // down
  TriangleButton *down = new TriangleButton(200, 400, WHITE, TriangleButton::Type::DOWN, 0x1005253);
  down -> drawButton();
  // enter 
  SquareButton *enter = new SquareButton(200, 330, RED, 'E', 0x1009293);
  enter -> drawButton();
  // back
  CircleButton *back = new CircleButton(50, 245, WHITE, 'B', 0x1002B2A);
  back -> drawButton();
  // menu

  // source
  CircleButton *source = new CircleButton(50, 180, WHITE, 'S', 0x100A0A1);
  source -> drawButton();

  power -> drawButton();
  
}

// draw 0-9 buttons
void drawNumericalButtons() {
  // make black screen
  tft.fillScreen(BLACK);
  tft.drawLine(0,0,319,0,WHITE);
  tft.drawLine(319,0,319,479,WHITE);
  tft.drawLine(319,479,0,479,WHITE);
  tft.drawLine(0,479,0,0,WHITE);

  // declare array of button positions
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

  numbers[0] = new CircleButton(positions[0][0], positions[0][1], GREEN, '0', 0x1009899);
  numbers[1] = new CircleButton(positions[1][0], positions[1][1], GREEN, '1', 0x1000809);
  numbers[2] = new CircleButton(positions[2][0], positions[2][1], GREEN, '2', 0x1008889);
  numbers[3] = new CircleButton(positions[3][0], positions[3][1], GREEN, '3', 0x1004849);
  numbers[4] = new CircleButton(positions[4][0], positions[4][1], GREEN, '4', 0x100C8C9);
  numbers[5] = new CircleButton(positions[5][0], positions[5][1], GREEN, '5', 0x1002829);
  numbers[6] = new CircleButton(positions[6][0], positions[6][1], GREEN, '6', 0x100A8A9);
  numbers[7] = new CircleButton(positions[7][0], positions[7][1], GREEN, '7', 0x1006869);
  numbers[8] = new CircleButton(positions[8][0], positions[8][1], GREEN, '8', 0x100E8E9);
  numbers[9] = new CircleButton(positions[9][0], positions[9][1], GREEN, '9', 0x1001819);
  
//  numbers[0] = new TriangleButton(positions[0][0], positions[0][1], GREEN, TriangleButton::Type::UP, 0x1009899);
//  numbers[1] = new TriangleButton(positions[1][0], positions[1][1], GREEN, TriangleButton::Type::LEFT, 0x1000809);
//  numbers[2] = new TriangleButton(positions[2][0], positions[2][1], GREEN, TriangleButton::Type::RIGHT, 0x1008889);
//  numbers[3] = new TriangleButton(positions[3][0], positions[3][1], GREEN, TriangleButton::Type::DOWN, 0x1004849);
//  numbers[4] = new TriangleButton(positions[4][0], positions[4][1], GREEN, TriangleButton::Type::UP, 0x100C8C9);
//  numbers[5] = new TriangleButton(positions[5][0], positions[5][1], GREEN, TriangleButton::Type::UP, 0x1002829);
//  numbers[6] = new TriangleButton(positions[6][0], positions[6][1], GREEN, TriangleButton::Type::UP, 0x100A8A9);
//  numbers[7] = new TriangleButton(positions[7][0], positions[7][1], GREEN, TriangleButton::Type::UP, 0x1006869);
//  numbers[8] = new TriangleButton(positions[8][0], positions[8][1], GREEN, TriangleButton::Type::UP, 0x100E8E9);
//  numbers[9] = new TriangleButton(positions[9][0], positions[9][1], GREEN, TriangleButton::Type::UP, 0x1001819);


  // draw digit buttons
  for (int i = 0; i < 10; i++) {
    numbers[i] -> drawButton();
  }

  power -> drawButton();

  /*
  // MENU
  int pos_x = SCREEN_WIDTH / 5;
  int pos_y = SCREEN_HEIGHT / 8 * 6;
  
  // < Button
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.fillTriangle(pos_x + 26, pos_y + 20, pos_x + 26, pos_y + 30, pos_x + 20, pos_y + 25, BLACK);
  // v Button
  pos_x = SCREEN_WIDTH / 5 * 2;
  pos_y = SCREEN_HEIGHT / 8 * 7;
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.fillTriangle(pos_x + 20, pos_y + 26, pos_x + 30, pos_y + 26, pos_x + 25, pos_y + 32, BLACK);
  // Enter
  pos_x = SCREEN_WIDTH / 5 * 2;
  pos_y = SCREEN_HEIGHT / 8 * 6;
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, RED);
  // > Button
  pos_x = SCREEN_WIDTH / 5 * 3;
  pos_y = SCREEN_HEIGHT / 8 * 6;
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.fillTriangle(pos_x + 26, pos_y + 20, pos_x + 26, pos_y + 30, pos_x + 32, pos_y + 25, BLACK);
  // ^ Button
  pos_x = SCREEN_WIDTH / 5 * 2;
  pos_y = SCREEN_HEIGHT / 8 * 5;
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.fillTriangle(pos_x + 20, pos_y + 26, pos_x + 30, pos_y + 26, pos_x + 25, pos_y + 20, BLACK);
  */

  /*
  int pos_x = SCREEN_WIDTH / 2- 26;
  int pos_y = SCREEN_HEIGHT / 2;

  // Volume +
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.drawChar(pos_x + 12, pos_y + 12, '+', BLACK, WHITE, 5);

  pos_y += 56;

  // Volume -
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.drawChar(pos_x + 12, pos_y + 12, '-', BLACK, WHITE, 5);

  pos_x -= 56;
  pos_y -= 28;

  // Channel <
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.drawChar(pos_x + 12, pos_y + 10, '<', BLACK, WHITE, 5);

  pos_x += 112;

  // Channel >
  tft.fillRoundRect(pos_x, pos_y, 52, 52, 8, WHITE);
  tft.drawChar(pos_x + 12, pos_y + 10, '>', BLACK, WHITE, 5);
  */

  // GO TO NEXT PAGE BUTTON
  tft.fillRoundRect(270, 420, 36, 52, 8, WHITE);
  tft.drawChar(273, 428, '>', BLACK, WHITE, 5);
}

unsigned long now = 0;           // millis
unsigned long prevSigMillis = 0; // previous signal acceptance time
unsigned long sincePrevSig = 0;  // time since previous signal acceptance
uint16_t TIME_SENSITIVITY = 300; // 300 ms between touches
bool firstTime = true;           // first time in loop
int lastPressed = -1;            // the number pressed last, -1 by default
int iterationsSincePrev = 0;     // iterations since previous signal acceptance


void loop() {
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off

  // Get the time
  now = millis();
  if(firstTime) {
    sincePrevSig = TIME_SENSITIVITY + 1;
  } else {
    sincePrevSig = now - prevSigMillis;
  }

  // reset button colour after 50 iterations
  if(iterationsSincePrev == 50) {
    if (lastPressed > -1) {                    // number button was pressed
      numbers[lastPressed] -> resetButton();
      lastPressed = -1;                        // reset last number pressed
    } else {
      power -> resetButton();                  // power button was pressed
    }
    iterationsSincePrev = 0;                   // reset counter
  }
  
  Serial.println(iterationsSincePrev);
  

  // retrieve the touch point
  TS_Point p = ts.getPoint();

  firstTime = false;

  // only handle touches every 300ms
  if (sincePrevSig > TIME_SENSITIVITY) {
    
    // scale the point from ~0->4000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
    
    // if touch has enough pressure
    if (p.z > 10 && p.z < 50) {
      
      // check whether a number button is pressed
      for (int i = 0; i < 10; i++) {
        if(numbers[i] -> isPressed(p.x, p.y)) {
          numbers[i] -> pressButton();
          lastPressed = i;
          iterationsSincePrev = 1; // begin iterating after button press
          Serial.println(i);
        }
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
    
    prevSigMillis = now; // successful touch, update time
  }

  // only iterate if button has been pressed
  if (iterationsSincePrev > 0) {
    iterationsSincePrev ++;
  }
  
}
