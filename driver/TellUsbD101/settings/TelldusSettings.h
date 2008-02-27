#ifndef TELLDUSSETTINGS_H
#define TELLDUSSETTINGS_H

#include "../Device.h"
#include <vector.h>

class privateVars;

class TelldusSettings
{
public:
	TelldusSettings(void);
	int getNumberOfDevices(void);
	Device* getDevice(int intDeviceId);
	Device* getDevice(int intDeviceId, int intDongleIndex);
	char* getName(int intDeviceId);
	bool setName(int intDeviceId, char* strNewName);
	char* getVendor(int intDeviceId);
	bool setVendor(int intDeviceId, char* strVendor);
	char* getModel(int intDeviceId);
	bool setModel(int intDeviceId, char* strModel);
	int* getArguments(int intDeviceId);
	bool setArguments(int intDeviceId, vector <int> vArguments);
	int addDevice();
	int getDeviceId(int intDeviceIndex);
	bool removeDevice(int intDeviceId);
	void debugLog(char* debugstring);
	void debugLog(int debugint);
	int getNumberOfArguments(int intDeviceId);

	~TelldusSettings(void);
	
protected:
	char *getStringSetting(int intDeviceId, const char* name);
	bool setStringSetting(int intDeviceId, const char* name, const char *value);

private:
	int getNextDeviceId();
	
	privateVars *d;
};

#endif
