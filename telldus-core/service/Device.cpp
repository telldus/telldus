#include "Device.h"

class Device::PrivateData {
public:
	
};

Device::Device(){

}

Device::~Device(void) {
	//delete d->devices;
	delete d;
}
