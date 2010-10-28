#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "Device.h"
#include "ControllerManager.h"
#include "EventUpdateManager.h"

class DeviceManager
{
public:
	DeviceManager(ControllerManager *controllerManager, EventRef deviceUpdateEvent);
	~DeviceManager(void);
	int getNumberOfDevices(void);
	int addDevice();
	void connectTellStickController(int vid, int pid, std::wstring serial);
	void disconnectTellStickController(int vid, int pid, std::wstring serial);
	int getDeviceId(int deviceIndex);
	int getDeviceLastSentCommand(int deviceId, int methodsSupported);
	int setDeviceLastSentCommand(int deviceId, int command,  std::wstring value);
	int getDeviceMethods(int deviceId, int methodsSupported);
	std::wstring getDeviceModel(int deviceId);
	int setDeviceModel(int deviceId, std::wstring model);
	std::wstring getDeviceName(int deviceId);
	int setDeviceName(int deviceId, std::wstring name);
	std::wstring getDeviceParameter(int deviceId, std::wstring name, std::wstring defauleValue);
	int setDeviceParameter(int deviceId, std::wstring name, std::wstring value);
	std::wstring getDeviceProtocol(int deviceId);
	int setDeviceProtocol(int deviceId, std::wstring name);
	std::wstring getDeviceStateValue(int deviceId);
	int getDeviceType(int deviceId);
	int getPreferredControllerId(int deviceId);
	int doAction(int deviceId, int action, unsigned char data);
	int removeDevice(int deviceId);
	int sendRawCommand(std::wstring command, int reserved);

	void handleControllerMessage(const ControllerEventData &event);

private:
	bool triggerDeviceStateChange(int deviceId, int intDeviceState, const std::wstring &strDeviceStateValue );
	void fillDevices(void);

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H
