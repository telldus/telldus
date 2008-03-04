#ifndef TELLDUSSETTINGS_H
#define TELLDUSSETTINGS_H

#include "../Device.h"
//#include <vector>

class privateVars;

class TelldusSettings
{
public:
	TelldusSettings(void);
	int getNumberOfDevices(void);
	Device* getDevice(int intDeviceId);
	Device* getDevice(int intDeviceId, int intDongleIndex);
	char* getName(int intDeviceId);
	bool setName(int intDeviceId, const char* strNewName);
	char* getVendor(int intDeviceId);
	bool setVendor(int intDeviceId, const char* strVendor);
	char* getModel(int intDeviceId);
	bool setModel(int intDeviceId, const char* strModel);
	char* getArgument(int intDeviceId, const char *strName);
	bool setArgument(int intDeviceId, const char *strName, const char *strValue);
	int addDevice();
	int getDeviceId(int intDeviceIndex);
	bool removeDevice(int intDeviceId);
	void debugLog(char* debugstring);
	void debugLog(int debugint);
	int getNumberOfArguments(int intDeviceId);

	~TelldusSettings(void);
	
protected:
	char *getStringSetting(int intDeviceId, const char* name, bool parameter);
	bool setStringSetting(int intDeviceId, const char* name, const char *value, bool parameter);

private:
	int getNextDeviceId();
	
	privateVars *d;
};

#endif
