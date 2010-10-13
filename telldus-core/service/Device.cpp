#include "Device.h"
#include "Settings.h"

class Device::PrivateData {
public:
	std::wstring name;
	std::wstring model;
	std::wstring protocolName;
	ParameterMap parameterList;
	
	Protocol *protocol;
};

Device::Device(int id){
	d = new PrivateData;
	d->protocol = 0;
	//när något uppdateras, spara också till registret
	//obs, alla hantera om alla värden inte är satta
}

Device::~Device(void) {
	delete d->protocol;
	delete d;
}

/**
* Get-/Set-methods
*/
std::wstring Device::getModel(){
	return d->model;
}

void Device::setModel(const std::wstring &model){
	d->model = model;
}

std::wstring Device::getName(){
	return d->name;
}

void Device::setName(const std::wstring &name){
	d->name = name;
}

std::wstring Device::getParameter(const std::wstring &key){
	return d->parameterList[key];
}

void Device::setParameter(const std::wstring &key, const std::wstring &value){
	d->parameterList[key] = value;
}

std::wstring Device::getProtocolName(){
	return d->protocolName;
}

void Device::setProtocolName(const std::wstring &protocolName){
	d->protocolName = protocolName;
}

/**
* End Get-/Set
*/

int Device::turnOn(Controller *controller) {
	Protocol *p = this->retrieveProtocol();
	if(p){
		//p->turnOn(controller);
	}
	//TODO: Handle p (shared_ptr?)
	return 0;
}

Protocol* Device::retrieveProtocol() {
	if (d->protocol) {
		return d->protocol;
	}
	
	d->protocol = Protocol::getProtocolInstance(d->protocolName);
	if(d->protocol){
		d->protocol->setModel(d->model);
		d->protocol->setParameters(d->parameterList);
		return d->protocol;
	}

	return 0;
}