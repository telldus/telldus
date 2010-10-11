#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
public:
	Settings(void);
	virtual ~Settings(void);

	std::wstring getSetting(const std::wstring &strName) const;
	int getNumberOfDevices(void) const;
	std::wstring getName(int intDeviceId) const;
	bool setName(int intDeviceId, const std::wstring &strNewName);
	std::wstring getProtocol(int intDeviceId) const;
	bool setProtocol(int intDeviceId, const std::wstring &strVendor);
	std::wstring getModel(int intDeviceId) const;
	bool setModel(int intDeviceId, const std::wstring &strModel);
	std::wstring getDeviceParameter(int intDeviceId, const std::wstring &strName) const;
	bool setDeviceParameter(int intDeviceId, const std::wstring &strName, const std::wstring &strValue);
	bool setDeviceState( int intDeviceId, int intDeviceState, const std::wstring &strDeviceStateValue );
	int getDeviceState( int intDeviceId ) const;
	std::wstring getDeviceStateValue( int intDeviceId ) const;

	int addDevice();
	int getDeviceId(int intDeviceIndex) const;
	bool removeDevice(int intDeviceId);
	
protected:
	std::wstring getStringSetting(int intDeviceId, const std::wstring &name, bool parameter) const;
	bool setStringSetting(int intDeviceId, const std::wstring &name, const std::wstring &value, bool parameter);
	int getIntSetting(int intDeviceId, const std::wstring &name, bool parameter) const;
	bool setIntSetting(int intDeviceId, const std::wstring &name, int value, bool parameter);

private:
	int getNextDeviceId() const;
	
	class PrivateData;
	PrivateData *d;
};

#endif
