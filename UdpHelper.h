// UdpHelper.h

#ifndef _UDPHELPER_h
#define _UDPHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <WiFiUdp.h>

using namespace std;

// extern keyword means "this is defined somewhere else, you'll find it when building"
// we need to pull in the global vars from the .ino like this to access them
extern const char* SSID;
extern const char* PASSWORD;
extern const String BOARD_NAME;

// Messaging definitions. should be in sync between sender and reciever. duh.
const String DELIMITER = ":::";
const String CMD_MESSAGE_LAYOUT = "%s" + DELIMITER + "%s"; // commandKey{delimiter}commandValue

const String RPLY_MESSAGE_LAYOUT = "ACK %s";  // ACK {result}
const String ERR_MESSAGE_LAYOUT = "ERR - Didn't Work!";  // When Command not found

class UdpHelper {

public:
	UdpHelper(unsigned int port);

	/// <summary>
	/// The last recieved Command
	/// </summary>
	String lastCommandName;
	/// <summary>
	/// The last recieved Value
	/// </summary>
	String lastCommandValue;

	/// <summary>
	/// Listens to the set Port for incoming messages and trys to execute them if known.
	/// Also Replys to the sender on same port with an ACK or ERR message.
	/// </summary>	
	/// <returns>True if something was recieved</returns>
	bool listenAndAck();

	/// <summary>
	/// Register a new command, where name is the key and command is a function with string return type
	/// and taking a string as argument. Cast string to your desired value inside your command.
	/// Many functions can be mapped to the same key, execution order is registration order.
	/// </summary>
	void registerCommand(String name, function<String(String)> command, String helpText);

	void sendMessage(String message, IPAddress ip = NULL, unsigned int port = NULL);

	vector<tuple<String, String>> GetCommandInfo();

private:
	WiFiUDP udp;

	/// <summary>
	/// Name, funtionPointer, helpText
	/// </summary>
	vector<tuple<String, function<String(String)>, String>> _commands;

	int _port;

	char _replyBuffer[512];
	char _incomingPacketBuffer[512];

	/// <summary>
	/// Takes the recieved udp message and trys to extract the commandName and commandValue.
	/// Adapt to defined message layout if needed.
	/// </summary>
	void extractCommandToLocalVars(String message);
};

#endif

