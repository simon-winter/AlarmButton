// WiFiHelper.h

#ifndef _WIFIHELPER_h
#define _WIFIHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

using namespace std;
class WiFiHelper
{
public:
	WiFiHelper(string ssid, string password);
};


#endif

