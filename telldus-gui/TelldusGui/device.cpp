#include "device.h"
#include <stdlib.h>

QHash<int, Device *> Device::devices;
int Device::callbackId = tdRegisterDeviceEvent( &Device::deviceEvent, 0);

Device::Device(int id)
		:p_id(id),
		p_model(0),
		p_state(0),
		p_name(""),
		p_protocol(""),
		p_modelChanged(false),
		p_nameChanged(false),
		p_protocolChanged(false),
		p_methods(0)
{
	if (id > 0) {
		char *name = tdGetName(id);
		p_name = QString::fromLocal8Bit( name );
		free( name );

		p_model = tdGetModel(id);

		char *protocol = tdGetProtocol(id);
		p_protocol = QString::fromLocal8Bit( protocol );
		free( protocol );

		updateState();
	}
}

Device::~Device() {
}

void Device::setModel( int model ) {
	p_model = model;
	p_modelChanged = true;
}

int Device::model() {
	return p_model;
}

void Device::setName( const QString & name ) {
	if (name.compare(p_name, Qt::CaseSensitive) == 0) {
		return;
	}
	p_name = name;
	p_nameChanged = true;
}

const QString &Device::name() {
	return p_name;
}

void Device::setParameter( const QString &name, const QString &value ) {
	tdSetDeviceParameter(p_id, name.toLocal8Bit(), value.toLocal8Bit());
}

QString Device::parameter( const QString &name, const QString &defaultValue ) const {
	char *p = tdGetDeviceParameter(p_id, name.toLocal8Bit(), defaultValue.toLocal8Bit());
	QString param( p );
	free(p);
	return param;
}

void Device::setProtocol( const QString & protocol ) {
	if (protocol.compare(p_protocol, Qt::CaseSensitive) == 0) {
		return;
	}
	p_protocol = protocol;
	p_protocolChanged = true;
}

const QString &Device::protocol() {
	return p_protocol;
}

int Device::methods() const {
	if (p_methods == 0) {
		const_cast<Device*>(this)->updateMethods();
	}
	return p_methods;
}

int Device::deviceType() const {
	return tdGetDeviceType(p_id);
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

bool Device::deviceLoaded( int id ) {
	return devices.contains(id);
}

void Device::save() {
	bool deviceIsAdded = false, methodsChanged = false;
	if (p_id == 0) { //This is a new device
		p_id = tdAddDevice();
		deviceIsAdded = true;
	}

	if (p_nameChanged) {
		tdSetName(p_id, p_name.toLocal8Bit());
		p_nameChanged = false;
	}

	if (p_modelChanged) {
		tdSetModel(p_id, p_model);
		methodsChanged = true;
		p_modelChanged = false;
	}

	if (p_protocolChanged) {
		tdSetProtocol(p_id, p_protocol.toLocal8Bit());
		methodsChanged = true;
		p_protocolChanged = false;
	}

	if (methodsChanged) {
		updateMethods();
	}

	if (deviceIsAdded) {
		emit deviceAdded(p_id);
	}
}

void Device::turnOff() {
	triggerEvent( tdTurnOff( p_id ) );
}

void Device::turnOn() {
	triggerEvent( tdTurnOn( p_id ) );
}

void Device::bell() {
	triggerEvent( tdBell( p_id ) );
}

int Device::lastSentCommand() const {
	return p_state;
}

void Device::updateMethods() {
	int methods = tdMethods(p_id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL | TELLSTICK_DIM);
	if (p_methods != methods) {
		bool doEmit = (p_methods > 0);
		p_methods = methods;
		if (doEmit) {
			emit methodsChanged( p_methods );
		}
	}
}

void Device::updateState() {
	int lastSentCommand = tdLastSentCommand( p_id );
	if (lastSentCommand != p_state) {
		p_state = lastSentCommand;
		emit stateChanged(p_id, p_state);
	}
}

void Device::triggerEvent( int messageId ) {
	if (messageId == TELLSTICK_SUCCESS) {
		//Update the last sent command
		updateState();
	} else {
		char *message = tdGetErrorString( messageId );
		emit showMessage( "", message, "" );
		free( message );
	}
}

void Device::deviceEvent(int deviceId, int, const char *, int, void *) {
	if (Device::deviceLoaded( deviceId )) {
		Device *device = Device::getDevice( deviceId );
		if (device) {
			device->updateState();
		}
	}
}
