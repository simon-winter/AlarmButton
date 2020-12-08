// FileSystemHandler.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FS.h>

using namespace std;



class Settings {



private:
	const String PATH = "/settings.txt";
	const char SEPERATOR = '|';
	const char ENDOFSETTING = '>';


	String Load(String key, int* Out_index);

public:

	// Variable names
	static const String ALARM_TIME;
	static const String MAX_VOLUME;

	vector<pair<String, String>> _settings;
	Settings(bool* Out_loadedSuccessfull);

	/// <summary>
	/// Changes a setting for this session, no writing performed.
	/// </summary>
	void SetSetting(String key, String value);

	/// <summary>
	/// Writes all settings to file.
	/// </summary>	
	bool Save();

	/// <summary>
	/// Loads a setting
	/// </summary>
	String Load(String key);

};

#endif

