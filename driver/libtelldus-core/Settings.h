#ifndef SETTINGS_H
#define SETTINGS_H

#include "Device.h"

class privateVars;

class Settings
{
public:
	Settings(void);
	std::string getSetting(const std::string &strName);
	int getNumberOfDevices(void);
	std::string getName(int intDeviceId);
	bool setName(int intDeviceId, const std::string &strNewName);
	std::string getProtocol(int intDeviceId);
	bool setProtocol(int intDeviceId, const std::string &strVendor);
	int getModel(int intDeviceId);
	bool setModel(int intDeviceId, int intModel);
	std::string getDeviceParameter(int intDeviceId, const std::string &strName);
	bool setDeviceParameter(int intDeviceId, const std::string &strName, const std::string &strValue);
	int addDevice();
	int getDeviceId(int intDeviceIndex);
	bool removeDevice(int intDeviceId);

	~Settings(void);
	
protected:
	std::string getStringSetting(int intDeviceId, const std::string &name, bool parameter);
	bool setStringSetting(int intDeviceId, const std::string &name, const std::string &value, bool parameter);
	int getIntSetting(int intDeviceId, const std::string &name, bool parameter);
	bool setIntSetting(int intDeviceId, const std::string &name, int value, bool parameter);

private:
	int getNextDeviceId();
	
	privateVars *d;
};

#endif
