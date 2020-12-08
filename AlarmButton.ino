/*
 Name:		WiFiReciever.ino
 Created:	9/6/2020 11:37:30 AM
 Author:	Sebi
*/

#include "Settings.h"
#include "WiFiHelper.h"
#include "UdpHelper.h"

#include <NTPClient.h>
#include <sstream>

//const char* SSID = "Vodafone-D89F";
//const char* PASSWORD = "KaeteHelmutDia65";
const char* SSID = "xxx";
const char* PASSWORD = "xxx";

const int UDPPORT_MESSAGING = 4210;

// Used for Display purposes, use whatever you like
const String BOARD_NAME = "AlarmButton";

bool keepOn = false;

Settings* settings;
UdpHelper* udp;
WiFiHelper* wifi;

double debugDur = 2000;
double startUpTime = 0;

char db[256];

void setup() {
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(115200);
	Serial.println("Booting");	

	ArduinoOTA.setHostname(BOARD_NAME.c_str());
	wifi = new WiFiHelper(SSID, PASSWORD);
	udp = new UdpHelper(UDPPORT_MESSAGING);	
	
	udp->registerCommand("Help", help, "prints all available commands and their help messages");
	udp->registerCommand("Debug", debug, "Prevents this arduino from restarting");
}

void loop() {	
	ArduinoOTA.handle();	

	if (startUpTime == 0) {		
		udp->sendMessage("rdy");
		startUpTime = millis();
	}
	
	sprintf(db, "%i", WiFi.RSSI());	
	udp->sendMessage(db);
	udp->listenAndAck();

	/*if(millis() - startUpTime > debugDur){
		if (!keepOn) {
			udp->sendMessage("Alarm");			
			delay(100);
			ESP.deepSleep(0);
		}
	}*/
	delay(5000);

}

String debug(String message) {
	debugCommandMessage(message);

	keepOn = true;

	char _replyBuffer[256];
	sprintf(_replyBuffer, "Debug Succeeded. ESP not powering down");
	return _replyBuffer;
}

String help(String message) {
	debugCommandMessage(message);

	auto cmds = udp->GetCommandInfo();
	stringstream ss;
	for (int i = 0; i < cmds.size(); i++) {
		ss << i << ": " << get<0>(cmds.at(i)).c_str() <<
			"(" << get<1>(cmds.at(i)).c_str() << ")" << endl;
	}

	return ss.str().c_str();
}

void debugCommandMessage(String message) {
	Serial.printf("Execute %s \n", message.c_str());
}
