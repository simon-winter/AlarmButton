// 
// 
// 

#include "Settings.h"

const String Settings::ALARM_TIME = "AlarmTime";
const String Settings::MAX_VOLUME = "MaxVolume";

Settings::Settings(bool* Out_loadedSuccessfull)
{
	if (!SPIFFS.begin()) {
		*Out_loadedSuccessfull = false;
		return;
	}

	// open/creates file
	File file = SPIFFS.open(PATH, "r");
	if (!file) {
		Serial.println("Error opening file for writing");
		*Out_loadedSuccessfull = false;
		return;
	}
	while (file.available()) {
		String setting = file.readStringUntil(ENDOFSETTING);
		String key = setting.substring(0, setting.indexOf(SEPERATOR));
		String value = setting.substring(setting.indexOf(SEPERATOR) + 1, setting.length());

		_settings.push_back(make_pair(key, value));
	}

	file.close();
	*Out_loadedSuccessfull = true;
}

void Settings::SetSetting(String key, String value)
{
	int foundIndex;
	if (Load(key, &foundIndex).equals("")) {
		_settings.push_back(make_pair(key, value));
	}
	else {
		_settings.at(foundIndex).second = value;
	}
}

bool Settings::Save()
{
	if (!SPIFFS.begin()) {
		return false;
	}

	// open/creates file
	File file = SPIFFS.open(PATH, "w");
	if (!file) {
		Serial.println("Error opening file for writing");
		return false;
	}

	char writeBuffer[256];
	for (int i = 0; i < _settings.size(); i++)
	{
		String name = _settings.at(i).first;
		String value = _settings.at(i).second;

		int length = sprintf(writeBuffer, "%s%c%s%c", name.c_str(), SEPERATOR, value.c_str(), ENDOFSETTING);
		file.write(writeBuffer, length);
		Serial.printf("Written Setting: %.*s \n", length, writeBuffer);
	}

	file.close();
	return true;
}

String Settings::Load(String key)
{
	int foundIndex;
	String result = Load(key, &foundIndex);

	return result;
}

String Settings::Load(String key, int* Out_index)
{
	*Out_index = -1;

	// search for setting
	String result = "";
	for (int i = 0; i < _settings.size(); i++)
	{
		if (_settings.at(i).first.equals(key)) {
			result = _settings.at(i).second;
			*Out_index = i;
		}
		return result;
	}
}
