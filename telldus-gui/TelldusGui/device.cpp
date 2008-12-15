#include "device.h"
#include "telldus-core.h"

QHash<int, Device *> Device::devices;

Device::Device(int id)
		:p_id(id)
{
}

Device::~Device() {
}

Device *Device::getDevice( int id ) {

	if (devices.contains(id)) {
		return devices[id];
	}
	Device *device = new Device(id);
	devices[id] = device;
	return device;
}

Device *Device::newDevice( ) {
	return new Device(0);
}

void Device::save() {
	if (p_id == 0) { //This is a new device
		p_id = tdAddDevice();
		emit deviceAdded(p_id);
	}
}
