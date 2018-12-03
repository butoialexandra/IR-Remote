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
  tft.fillScreen(HX8357_BLACK);
  tft.drawLine(0,0,319,0,HX8357_WHITE);
  tft.drawLine(319,0,319,479,HX8357_WHITE);
  tft.drawLine(319,479,0,479,HX8357_WHITE);
  tft.drawLine(0,479,0,0,HX8357_WHITE);
 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      char buttonNo = '0' + 3 * j + i + 1;
      int x = SCREEN_WIDTH / 4 * (i + 1);
      int y = SCREEN_HEIGHT / 5 * (j + 1);
      tft.fillCircle(x, y, 28, GREEN);
      tft.drawChar(x - 12, y - 16, buttonNo, BLACK, GREEN, 5);
    }    
  }
  tft.fillCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 4, 28, RED);
  tft.drawChar(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 5 * 4 - 16, '0', BLACK, RED, 5);
}

void loop() {
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off
  
  //TestScreen::testSequence(usbPowerOn); // run a test on all modules
}
