#ifndef ADAFRUITCONFIG_H
#define ADAFRUITCONFIG_H

#define IO_USERNAME     "AndrewC19"
#define IO_KEY          "a9f2b43f5dae4f7cb29f849229a78fe5"
//#define WIFI_SSID       "uos-other"
//#define WIFI_PASS       "shefotherkey05"
#define WIFI_SSID       "BTHub6-SJ7N"
#define WIFI_PASS       "6NbCepn7uGHX"

#define CONTROLLER_USERNAME      "alexandrabutoi"
#define CONTROLLER_KEY           "fe914302f62d4b588a0371d062378897"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_WiFi controller(CONTROLLER_USERNAME, CONTROLLER_KEY, WIFI_SSID, WIFI_PASS);

#endif

