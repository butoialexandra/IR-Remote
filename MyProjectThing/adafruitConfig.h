#ifndef ADAFRUITCONFIG_H
#define ADAFRUITCONFIG_H

#define IO_USERNAME     "AndrewC19"
#define IO_KEY          "a9f2b43f5dae4f7cb29f849229a78fe5"
#define WIFI_SSID       "uos-other"
#define WIFI_PASS       "shefotherkey05"
//#define WIFI_SSID       "BTHub6-SJ7N"
//#define WIFI_PASS       "6NbCepn7uGHX"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#endif

