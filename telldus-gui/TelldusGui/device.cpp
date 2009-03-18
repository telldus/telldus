#include "device.h"
#include <stdlib.h>

QHash<int, Device *> Device::devices;
int Device::callbackId = tdRegisterDeviceEvent( &Device::deviceEvent, 0);

class DevicePrivate {
public:
	int id, model, state;
	QString name, protocol;
	bool modelChanged, nameChanged, protocolChanged;
	mutable int methods;
	mutable QHash<QString, QString> settings;
};

Device::Device(int id)
{
	d = new DevicePrivate;
	d->id = id;
	d->model = 0;
	d->state = 0;
	d->name = "";
	d->protocol = "";
	d->modelChanged = false;
	d->nameChanged = false;
	d->protocolChanged = false;
	d->methods = 0;

	if (d->id > 0) {
		char *name = tdGetName(id);
		d->name = QString::fromLocal8Bit( name );
		free( name );

		d->model = tdGetModel(id);

		char *protocol = tdGetProtocol(id);
		d->protocol = QString::fromLocal8Bit( protocol );
		free( protocol );

		updateState();
	}
}

Device::~Device() {
	delete d;
}

void Device::setModel( int model ) {
	d->model = model;
	d->modelChanged = true;
}

int Device::model() {
	return d->model;
}

void Device::setName( const QString & name ) {
	if (name.compare(d->name, Qt::CaseSensitive) == 0) {
		return;
	}
	d->name = name;
	d->nameChanged = true;
}

const QString &Device::name() {
	return d->name;
}

void Device::setParameter( const QString &name, const QString &value ) {
	tdSetDeviceParameter(d->id, name.toLocal8Bit(), value.toLocal8Bit());
}

QString Device::parameter( const QString &name, const QString &defaultValue ) const {
	char *p = tdGetDeviceParameter(d->id, name.toLocal8Bit(), defaultValue.toLocal8Bit());
	QString param( p );
	free(p);
	return param;
}

void Device::setProtocol( const QString & protocol ) {
	if (protocol.compare(d->protocol, Qt::CaseSensitive) == 0) {
		return;
	}
	d->protocol = protocol;
	d->protocolChanged = true;
}

const QString &Device::protocol() {
	return d->protocol;
}

int Device::methods() const {
	if (d->methods == 0) {
		const_cast<Device*>(this)->updateMethods();
	}
	return d->methods;
}

int Device::deviceType() const {
	return tdGetDeviceType(d->id);
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
	if (d->id == 0) { //This is a new device
		d->id = tdAddDevice();
		deviceIsAdded = true;
	}

	if (d->nameChanged || deviceIsAdded) {
		tdSetName(d->id, d->name.toLocal8Bit());
		d->nameChanged = false;
	}

	if (d->modelChanged || deviceIsAdded) {
		tdSetModel(d->id, d->model);
		methodsChanged = true;
		d->modelChanged = false;
	}

	if (d->protocolChanged || deviceIsAdded) {
		tdSetProtocol(d->id, d->protocol.toLocal8Bit());
		methodsChanged = true;
		d->protocolChanged = false;
	}

	if (methodsChanged) {
		updateMethods();
	}

	if (deviceIsAdded) {
		emit deviceAdded(d->id);
	}
}

void Device::turnOff() {
	triggerEvent( tdTurnOff( d->id ) );
}

void Device::turnOn() {
	triggerEvent( tdTurnOn( d->id ) );
}

void Device::bell() {
	triggerEvent( tdBell( d->id ) );
}

int Device::lastSentCommand() const {
	return d->state;
}

void Device::updateMethods() {
	int methods = tdMethods(d->id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL | TELLSTICK_DIM);
	if (d->methods != methods) {
		bool doEmit = (d->methods > 0);
		d->methods = methods;
		if (doEmit) {
			emit methodsChanged( d->methods );
		}
	}
}

void Device::updateState() {
	int lastSentCommand = tdLastSentCommand( d->id );
	if (lastSentCommand != d->state) {
		d->state = lastSentCommand;
		emit stateChanged(d->id, d->state);
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
