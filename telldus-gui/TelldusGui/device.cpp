#include "device.h"
// #include <stdlib.h>

#include <QHash>
#include <QDebug>

class Device::DevicePrivate {
public:
	int id, state, supportedMethods;
	QString name, protocol, model, stateValue;
	bool modelChanged, nameChanged, protocolChanged;
	mutable int methods;
	mutable QHash<QString, QString> settings;
	int callbackId, deviceChangeCallbackId;
};

Device::Device(int id, int supportedMethods, QObject *parent)
	: QObject(parent)
{
	d = new DevicePrivate;
	d->id = id;
	d->supportedMethods = supportedMethods;
	d->modelChanged = false;
	d->nameChanged = false;
	d->protocolChanged = false;

	d->callbackId = tdRegisterDeviceEvent(reinterpret_cast<TDDeviceEvent>(&Device::deviceEvent), this);
	d->deviceChangeCallbackId = tdRegisterDeviceChangeEvent( reinterpret_cast<TDDeviceChangeEvent>(&Device::deviceChangeEvent), this);

	if (id > 0) {
		d->state = tdLastSentCommand(id, supportedMethods);
		char *value = tdLastSentValue(id);
		d->stateValue = QString::fromUtf8(value);
		tdReleaseString(value);

		char *name = tdGetName(id);
		d->name = QString::fromUtf8( name );
		tdReleaseString( name );

		d->methods = tdMethods(id, supportedMethods);

		d->model = tdGetModel(id);

		char *protocol = tdGetProtocol(id);
		d->protocol = QString::fromUtf8( protocol );
		tdReleaseString( protocol );
	}

	connect(this, SIGNAL(deviceChanged(int,int,int)), this, SLOT(deviceChangedSlot(int,int,int)), Qt::QueuedConnection);
}

Device::~Device() {
	tdUnregisterCallback(d->callbackId);
	tdUnregisterCallback(d->deviceChangeCallbackId);
	delete d;
}

int Device::id() const {
	return d->id;
}

void Device::setModel( const QString &model ) {
	d->model = model;
	d->modelChanged = true;
}

QString Device::model() const {
	return d->model;
}

void Device::setName( const QString & name ) {
	if (name.compare(d->name, Qt::CaseSensitive) == 0) {
		return;
	}
	d->name = name;
	d->nameChanged = true;
}

QString &Device::name() const {
	return d->name;
}

void Device::setParameter( const QString &name, const QString &value ) {
	d->settings[name] = value;
}

QString Device::parameter( const QString &name, const QString &defaultValue ) const {
	if (!d->settings.contains(name)) {
		char *p = tdGetDeviceParameter(d->id, name.toUtf8(), defaultValue.toUtf8());
		d->settings[name] = p;
		tdReleaseString(p);
	}
	return d->settings[name];
}

void Device::setProtocol( const QString & protocol ) {
	if (protocol.compare(d->protocol, Qt::CaseSensitive) == 0) {
		return;
	}
	d->protocol = protocol;
	d->protocolChanged = true;
}

QString &Device::protocol() const {
	return d->protocol;
}

int Device::methods() const {
	return d->methods;
}

int Device::deviceType() const {
	return tdGetDeviceType(d->id);
}

void Device::save() {
	bool deviceIsAdded = false;
	if (d->id == 0) { //This is a new device
		d->id = tdAddDevice();
		if (d->id < 0) {
			return;
		}
		deviceIsAdded = true;
	}

	if (d->nameChanged || deviceIsAdded) {
		d->nameChanged = false;
		tdSetName(d->id, d->name.toUtf8());
	}

	if (d->modelChanged || deviceIsAdded) {
		tdSetModel(d->id, d->model.toUtf8());
		d->modelChanged = false;
	}

	if (d->protocolChanged || deviceIsAdded) {
		tdSetProtocol(d->id, d->protocol.toUtf8());
		d->protocolChanged = false;
	}

	//Save all parameters
	for( QHash<QString, QString>::const_iterator it = d->settings.begin(); it != d->settings.end(); ++it) {
		QByteArray name(it.key().toUtf8() );
		QByteArray value(it.value().toUtf8() );
		tdSetDeviceParameter(d->id, name.constData(), value.constData());
	}
	d->settings.clear();
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

void Device::dim(unsigned char level) {
	triggerEvent( tdDim( d->id, level ) );
}


void Device::learn() {
	triggerEvent( tdLearn( d->id ) );
}

int Device::lastSentCommand() const {
	return d->state;
}

QString Device::lastSentValue() const {
	return d->stateValue;
}

void Device::remove(int deviceId) {
	tdRemoveDevice(deviceId);
}

void Device::remove() {
	Device::remove(d->id);
}

void Device::deviceChangedSlot(int deviceId, int eventId, int changeType) {
	if (eventId == TELLSTICK_DEVICE_REMOVED) {
		emit deviceRemoved(deviceId);
	}
	if (eventId != TELLSTICK_DEVICE_CHANGED) {
		return;
	}
	switch( changeType ) {
		case TELLSTICK_CHANGE_NAME: {
			if (d->nameChanged) {
				break;
			}
			char *name = tdGetName(d->id);
			d->name = QString::fromUtf8( name );
			tdReleaseString( name );
			emit nameChanged( d->id, d->name );
			break;
		}

		case TELLSTICK_CHANGE_PROTOCOL:
		case TELLSTICK_CHANGE_MODEL:
			int methods = tdMethods(d->id, d->supportedMethods);
			if (methods != d->methods) {
				d->methods = methods;
				emit methodsChanged( d->methods );
			}
			break;
	}
}

void Device::triggerEvent( int messageId ) {
	if (messageId != TELLSTICK_SUCCESS) {
// 		char *message = tdGetErrorString( messageId );
// 		emit showMessage( "", message, "" );
// 		tdReleaseString( message );
	}
}

void WINAPI Device::deviceEvent(int deviceId, int method, const char *data, int, void *context) {
	Device *device = reinterpret_cast<Device *>(context);
	if (!device) {
		return;
	}
	if (device->id() != deviceId) {
		return;
	}
	device->d->state = method;
	device->d->stateValue = data;
	emit device->stateChanged(deviceId);
}

void WINAPI Device::deviceChangeEvent(int deviceId, int eventId, int changeType, int, void *context) {
	Device *device = reinterpret_cast<Device *>(context);
	if (!device) {
		return;
	}
	if (device->id() != deviceId) {
		return;
	}
	emit device->deviceChanged(deviceId, eventId, changeType);
}

