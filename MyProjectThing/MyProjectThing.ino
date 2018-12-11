// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the 
// TestScreen::activate(true); to false to change this behaviour.

#include "unphone.h"
#include "UI.h"
#include <vector>
#include "adafruitConfig.h"

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

bool sendAdafruit = true;

UI *pageOne;
AdafruitIO_Feed *button0 = io.feed("0Button");
AdafruitIO_Feed *button1 = io.feed("1Button");
AdafruitIO_Feed *button2 = io.feed("2Button");
AdafruitIO_Feed *button3 = io.feed("3Button");
AdafruitIO_Feed *button4 = io.feed("4Button");
AdafruitIO_Feed *button5 = io.feed("5Button");
AdafruitIO_Feed *button6 = io.feed("6Button");
AdafruitIO_Feed *button7 = io.feed("7Button");
AdafruitIO_Feed *button8 = io.feed("8Button");
AdafruitIO_Feed *button9 = io.feed("9Button");

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
  //drawNumericalButtons();
  //drawFunctionButtons();
  
  pageOne = new UI();
  
  if (sendAdafruit) {
    // connect to io.adafruit.com
    Serial.print("Connecting to Adafruit IO");
    io.connect();
  
    // wait for a connection
    while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
  
    // we are connected
    Serial.println();
    Serial.println(io.statusText());
    pageOne -> colourDelayIterations = 5; // Accounting for delay to Adafruit
  }
}

int clickCount [10] = {0}; // Counts of each numerical button being pressed, send this to Adafruit


void loop() {
  // Always handle button press and check power
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off
  char buttonPress = pageOne -> handleTouch(); // check for touch
  
  // Send data to Adafruit
  if (sendAdafruit) {
    io.run();
    // send analytics on number buttons to Adafruit.io
    switch(buttonPress) {
      case '0': clickCount[0] ++; button0 -> save(clickCount[0]); break;
      case '1': clickCount[1] ++; button1 -> save(clickCount[1]); break;
      case '2': clickCount[2] ++; button2 -> save(clickCount[2]); break;
      case '3': clickCount[3] ++; button3 -> save(clickCount[3]); break;
      case '4': clickCount[4] ++; button4 -> save(clickCount[4]); break;
      case '5': clickCount[5] ++; button5 -> save(clickCount[5]); break;
      case '6': clickCount[6] ++; button6 -> save(clickCount[6]); break;
      case '7': clickCount[7] ++; button7 -> save(clickCount[7]); break;
      case '8': clickCount[8] ++; button8 -> save(clickCount[8]); break;
      case '9': clickCount[9] ++; button9 -> save(clickCount[9]); break;
    }
  }
}
