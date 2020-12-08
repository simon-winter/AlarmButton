// 
// 
// 

#include "UdpHelper.h"

UdpHelper::UdpHelper(unsigned int port)
{
	udp.begin(port);
	_port = port;
}

bool UdpHelper::listenAndAck()
{
	int packetSize = udp.parsePacket();
	if (packetSize) // recieved something
	{
		Serial.printf("\nReceived %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
		int len = udp.read(_incomingPacketBuffer, 255);
		if (len > 0)
		{
			_incomingPacketBuffer[len] = '\0';
		}
		Serial.printf("UDP packet contents: %s\n", _incomingPacketBuffer);

		extractCommandToLocalVars(_incomingPacketBuffer);
		// search for command(s) and execute if found.
		String result = ERR_MESSAGE_LAYOUT;
		for (int i = 0; i < _commands.size(); i++)
		{
			if (get<0>(_commands.at(i)).equals(lastCommandName)) {
				result = get<1>(_commands.at(i))(lastCommandValue);  // execute command with string args				
			}
		}

		// send Ack -- Populate message layout
		sprintf(_replyBuffer, RPLY_MESSAGE_LAYOUT.c_str(), result.c_str());
		sendMessage(_replyBuffer, udp.remoteIP(), _port);

		return true;
	}
	return false;
}

void UdpHelper::registerCommand(String name, std::function<String(String args)> command, String helpText)
{
	_commands.push_back(make_tuple(name, command, helpText));
	Serial.printf("Registered UDP Command: %s\n", name.c_str());
}

/// <summary>
/// Sends a udp message. If no ip is supplied it will be broad casted over Router to all devices (255.255.255.255)
/// </summary>
void UdpHelper::sendMessage(String message, IPAddress ip, unsigned int port)
{
	char buffer[512];
	sprintf(buffer, "%s", message.c_str());

	if (ip == (IPAddress)NULL) {
		ip.fromString("255.255.255.255");
	}
	if (port == NULL) {
		port = _port;
	}
	Serial.printf("Sending: %s \n", buffer);
	udp.beginPacket(ip, port);
	udp.write(buffer);
	udp.endPacket();
}

vector<tuple<String, String>> UdpHelper::GetCommandInfo()
{
	vector<tuple<String, String>> res;
	for (int i = 0; i < _commands.size(); i++) {
		auto command = _commands.at(i);
		res.push_back(make_tuple(
			get<0>(command), get<2>(command)));
	}
	return res;
}

void UdpHelper::extractCommandToLocalVars(String message) {
	lastCommandName = message.substring(
		0,
		message.indexOf(DELIMITER));
	Serial.printf("Extracted Command: %s\n", lastCommandName.c_str());

	if (message.indexOf(DELIMITER) != -1) {
		lastCommandValue = message.substring(
			message.indexOf(DELIMITER) + DELIMITER.length(),
			message.length());
	}
	else {
		lastCommandValue = "";
	}
	Serial.printf("Extracted Value: %s\n", lastCommandValue.c_str());
}

