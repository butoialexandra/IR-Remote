// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the 
// TestScreen::activate(true); to false to change this behaviour.

#include "unphone.h"

// Color definitions
#define RED      0xF800
#define GREEN    0x07E0
#define WHITE    0xFFFF
#define BLACK    0x0000
#define CYAN     0x07FF

int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;

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
  TestScreen::activate(true);
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
  drawButtons();
}

// draw 0-9 buttons
void drawButtons() {
  // make black screen
  tft.fillScreen(BLACK);
  tft.drawLine(0,0,319,0,WHITE);
  tft.drawLine(319,0,319,479,WHITE);
  tft.drawLine(319,479,0,479,WHITE);
  tft.drawLine(0,479,0,0,WHITE);

  // Digit buttons
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      char buttonNo = '0' + 3 * j + i + 1;
      int x = SCREEN_WIDTH / 4 * (i + 1);
      int y = SCREEN_HEIGHT / 5 * (j + 1);
      tft.fillCircle(x, y, 28, GREEN);
      tft.drawChar(x - 12, y - 16, buttonNo, BLACK, GREEN, 5);
    }    
  }
  // ZERO
  tft.fillCircle(SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 5 * 4, 28, GREEN);
  tft.drawChar(SCREEN_WIDTH / 4 * 2 - 12, SCREEN_HEIGHT / 5 * 4 - 16, '0', BLACK, GREEN, 5);
  
  /*
  // POWER BUTTON
  tft.fillCircle(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 8, 28, RED);

  
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

void loop() {
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off
  
  //TestScreen::testSequence(usbPowerOn); // run a test on all modules
}
