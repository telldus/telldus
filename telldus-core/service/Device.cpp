#include "Device.h"
#include "Settings.h"

class Device::PrivateData {
public:
	std::wstring model;
	std::wstring name;
	ParameterMap parameterList;
	Protocol *protocol;
	std::wstring protocolName;
	int preferredControllerId;
	int state;
	std::wstring stateValue;
};

Device::Device(int id)
	:Mutex()
{
	d = new PrivateData;
	d->protocol = 0;
	d->preferredControllerId = 0;
	d->state = 0;
}

Device::~Device(void) {
	delete d->protocol;
	delete d;
}

/**
* Get-/Set-methods
*/

int Device::getLastSentCommand(int methodsSupported){

	int lastSentCommand = Device::maskUnsupportedMethods(d->state, methodsSupported);

	if (lastSentCommand == TELLSTICK_BELL) {
		//Bell is not a state
		lastSentCommand = TELLSTICK_TURNOFF;
	}
	if (lastSentCommand == 0) {
		lastSentCommand = TELLSTICK_TURNOFF;
	}
	return lastSentCommand;

}

int Device::getMethods() const {
	Protocol *p = this->retrieveProtocol();
	if (p) {
		return p->methods();
	}
	return 0;
}

void Device::setLastSentCommand(int command, std::wstring value){
	d->state = command;
	d->stateValue = value;
}

std::wstring Device::getModel(){
	return d->model;
}

void Device::setModel(const std::wstring &model){
	if(d->protocol){
		delete(d->protocol);
		d->protocol = 0;
	}
	d->model = model;
}

std::wstring Device::getName(){
	return d->name;
}

void Device::setName(const std::wstring &name){
	d->name = name;
}

std::wstring Device::getParameter(const std::wstring &key){
	ParameterMap::iterator it = d->parameterList.find(key);
	if (it == d->parameterList.end()) {
		return L"";
	}
	return d->parameterList[key];
}

std::list<std::string> Device::getParametersForProtocol() const {
	return Protocol::getParametersForProtocol(getProtocolName());
}

void Device::setParameter(const std::wstring &key, const std::wstring &value){
	d->parameterList[key] = value;
}

int Device::getPreferredControllerId(){
	return d->preferredControllerId;
}

void Device::setPreferredControllerId(int controllerId){
	d->preferredControllerId = controllerId;
}

std::wstring Device::getProtocolName() const {
	return d->protocolName;
}

void Device::setProtocolName(const std::wstring &protocolName){
	if(d->protocol){
		delete(d->protocol);
		d->protocol = 0;
	}
	d->protocolName = protocolName;
}

std::wstring Device::getStateValue(){
	return d->stateValue;
}

int Device::getType(){
	if(d->model == L"group"){
		return TELLSTICK_TYPE_GROUP;
	}
	return TELLSTICK_TYPE_DEVICE;
}

/**
* End Get-/Set
*/

int Device::doAction(int action, unsigned char data, Controller *controller) {
	Protocol *p = this->retrieveProtocol();
	if(p){
		std::string code = p->getStringForMethod(action, data, controller);
		if (code == "") {
			return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
		}
		return controller->send(code);
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

Protocol* Device::retrieveProtocol() const {
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

int Device::maskUnsupportedMethods(int methods, int supportedMethods) {
	// Bell -> On
	if ((methods & TELLSTICK_BELL) && !(supportedMethods & TELLSTICK_BELL)) {
		methods |= TELLSTICK_TURNON;
	}

	// Execute -> On
	if ((methods & TELLSTICK_EXECUTE) && !(supportedMethods & TELLSTICK_EXECUTE)) {	//TODO ok everywhere?
		methods |= TELLSTICK_TURNON;
	}

	//Cut of the rest of the unsupported methods we don't have a fallback for
	return methods & supportedMethods;
}

int Device::methodId( const std::string &methodName ) {
	if (methodName.compare("turnon") == 0) {
		return TELLSTICK_TURNON;
	}
	if (methodName.compare("turnoff") == 0) {
		return TELLSTICK_TURNOFF;
	}
	if (methodName.compare("bell") == 0) {
		return TELLSTICK_BELL;
	}
	if (methodName.compare("dim") == 0) {
		return TELLSTICK_DIM;
	}
	if (methodName.compare("execute") == 0) {
		return TELLSTICK_EXECUTE;
	}
	return 0;
}
