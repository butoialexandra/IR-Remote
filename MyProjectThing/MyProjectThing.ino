// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the 
// TestScreen::activate(true); to false to change this behaviour.


#include <WebServer.h> // simple webserver
#include "HTTPClient.h"  // ESP32 library for making HTTP requests
#include <Update.h>       // OTA update library
#include "unphone.h"
#include "UI.h"
#include <vector>
#include "adafruitConfig.h"

// OTA Stuff
int doCloudGet(HTTPClient *, String, String); // helper for downloading 'ware
void doOTAUpdate();                           // main OTA logic
int currentVersion = 1;
String gitID = "AndrewC19";
bool otaUpdate = false;

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

bool sendAdafruit = false;

UI *ui;

// Adafruit feeds for each button, storing the frequency of button presses
AdafruitIO_Feed *buttonFeeds [10] = {io.feed("0Button"),
                                    io.feed("1Button"),
                                    io.feed("2Button"),
                                    io.feed("3Button"),
                                    io.feed("4Button"),
                                    io.feed("5Button"),
                                    io.feed("6Button"),
                                    io.feed("7Button"),
                                    io.feed("8Button"),
                                    io.feed("9Button")
                                   };
                               

// Array storing number of clicks for each numerical button 0 - 9
int clickCount [10];

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
  
  ui = new UI();
  
  if (sendAdafruit || otaUpdate) {
    // connect to io.adafruit.com
    Serial.print("Connecting to internet");
    io.connect();
  
    // wait for a connection
    while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(500);
    }

    if (otaUpdate)
      doOTAUpdate();
    
    // we are connected
    Serial.println();
    Serial.println(io.statusText());
    resetClickCount(); // clear the click count
  }
}


void loop() {
  // Always handle button press and check power
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off
  char buttonPress = ui -> handleTouch(); // handle the pressed button
  
  // Send data to Adafruit
  if (sendAdafruit) {
    io.run();
    // send analytics on number buttons to Adafruit.io

    if (isdigit(buttonPress)) {
      int buttonNumber = (int) buttonPress - 48; // Subtract 48 as casts to ASCII code
      clickCount[buttonNumber] ++;
      buttonFeeds[buttonNumber] -> save(clickCount[buttonNumber]);
    }
//    switch(buttonPress) {
//      case '0': clickCount[0] ++; button0 -> save(clickCount[0]); break;
//      case '1': clickCount[1] ++; button1 -> save(clickCount[1]); break;
//      case '2': clickCount[2] ++; button2 -> save(clickCount[2]); break;
//      case '3': clickCount[3] ++; button3 -> save(clickCount[3]); break;
//      case '4': clickCount[4] ++; button4 -> save(clickCount[4]); break;
//      case '5': clickCount[5] ++; button5 -> save(clickCount[5]); break;
//      case '6': clickCount[6] ++; button6 -> save(clickCount[6]); break;
//      case '7': clickCount[7] ++; button7 -> save(clickCount[7]); break;
//      case '8': clickCount[8] ++; button8 -> save(clickCount[8]); break;
//      case '9': clickCount[9] ++; button9 -> save(clickCount[9]); break;
//    }
  }
}


void resetClickCount() {
  for (int i=0; i<10; i++) {
    clickCount[i] = 0;
    buttonFeeds[i] -> save(clickCount[i]);
  }
}


// OTA over-the-air update stuff ///////////////////////////////////////////
void doOTAUpdate() {             // the main OTA logic
  // materials for doing an HTTP GET on github from the BinFiles/ dir
  HTTPClient http; // manage the HTTP request process
  int respCode;    // the response code from the request (e.g. 404, 200, ...)
  int highestAvailableVersion = -1;  // version of latest firmware on server

  // do a GET to read the version file from the cloud
  Serial.println("checking for firmware updates...");
  respCode = doCloudGet(&http, gitID, "version");
  if(respCode == 200) // check response code (-ve on failure)
    highestAvailableVersion = atoi(http.getString().c_str());
  else
    Serial.printf("couldn't get version! rtn code: %d\n", respCode);
  http.end(); // free resources

  // do we know the latest version, and does the firmware need updating?
  if(respCode != 200) {
    Serial.printf("cannot update\n\n");
    return;
  } else if(currentVersion >= highestAvailableVersion) {
    Serial.printf("firmware is up to date\n\n");
    return;
  }

  // ok, we need to do a firmware update...
  Serial.printf(
    "upgrading firmware from version %d to version %d\n",
    currentVersion, highestAvailableVersion
  );

  // do a GET for the .bin
  String binName = String(highestAvailableVersion);
  binName += ".bin";
  respCode = doCloudGet(&http, gitID, binName);
  int updateLength = http.getSize(); // if isn't big enough refuse to update
  if(respCode == 200) {              // check response code (-ve on failure)
    Serial.printf(".bin code/size: %d; %d\n\n", respCode, updateLength);
    if(updateLength < 174992) {      // the size of the Blink example sketch
      Serial.println("update size is too small! refusing to try OTA update");
      return;
    }
  } else {
    Serial.printf("failed to get a .bin! return code is: %d\n", respCode);
    http.end(); // free resources
    return;
  }

  // write the new version of the firmware to flash
  WiFiClient stream = http.getStream();
  if(Update.begin(updateLength)) {
    Serial.printf("starting OTA may take a minute or two...\n");
    Update.writeStream(stream);
    if(Update.end()) {
      Serial.printf("update done, now finishing...\n");
      if(Update.isFinished()) {
        Serial.printf("update successfully finished; rebooting...\n\n");
        ESP.restart();
      } else {
        Serial.printf("update didn't finish correctly :(\n");
      }
    } else {
      Serial.printf("an update error occurred, #: %d\n" + Update.getError());
    }
  } else {
    Serial.printf("not enough space to start OTA update :(\n");
  }
  stream.flush();
}


// helper for downloading from cloud firmware server via HTTP GET
int doCloudGet(HTTPClient *http, String gitID, String fileName) {
  // build up URL from components; for example:
  // http://com3505.gate.ac.uk/repos/com3505-labs-2018-adalovelace/BinFiles/2.bin
  String baseUrl =
    "http://com3505.gate.ac.uk/repos/";
  String url =
    baseUrl + "com3505-labs-2018-" + gitID + "/BinFiles/" + fileName;

  // make GET request and return the response code
  http->begin(url);
  http->addHeader("User-Agent", "ESP32");
  return http->GET();
}
