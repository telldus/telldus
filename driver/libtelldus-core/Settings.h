#ifndef SETTINGS_H
#define SETTINGS_H

#include "Device.h"

class privateVars;

class Settings
{
public:
	Settings(void);
	std::string getSetting(const std::string &strName) const;
	int getNumberOfDevices(void) const;
	std::string getName(int intDeviceId) const;
	bool setName(int intDeviceId, const std::string &strNewName);
	std::string getProtocol(int intDeviceId) const;
	bool setProtocol(int intDeviceId, const std::string &strVendor);
	int getModel(int intDeviceId) const;
	bool setModel(int intDeviceId, int intModel);
	std::string getDeviceParameter(int intDeviceId, const std::string &strName) const;
	bool setDeviceParameter(int intDeviceId, const std::string &strName, const std::string &strValue);
	int addDevice();
	int getDeviceId(int intDeviceIndex) const;
	bool removeDevice(int intDeviceId);

	~Settings(void);
	
protected:
	std::string getStringSetting(int intDeviceId, const std::string &name, bool parameter) const;
	bool setStringSetting(int intDeviceId, const std::string &name, const std::string &value, bool parameter);
	int getIntSetting(int intDeviceId, const std::string &name, bool parameter) const;
	bool setIntSetting(int intDeviceId, const std::string &name, int value, bool parameter);

private:
	int getNextDeviceId() const;
	
	privateVars *d;
};

#endif
