#include "tellduscoreobject.h"
#include <QDebug>

TelldusCoreObject::TelldusCoreObject( QObject * parent )
		: QObject(parent)
{
	tdInit();
	deviceEventId = tdRegisterDeviceEvent(reinterpret_cast<TDDeviceEvent>(&TelldusCoreObject::deviceEventCallback), this);
	deviceChangeEventId = tdRegisterDeviceChangeEvent(reinterpret_cast<TDDeviceChangeEvent>(&TelldusCoreObject::deviceChangeEventCallback), this);
}

TelldusCoreObject::~TelldusCoreObject() {
	tdUnregisterCallback(deviceEventId);
	tdUnregisterCallback(deviceChangeEventId);
	tdClose();
}

int TelldusCoreObject::bell(int deviceId) {
	int retval = tdBell( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::dim(int deviceId, char level) {
	int retval = tdDim( deviceId, level );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::getNumberOfDevices() const {
	return tdGetNumberOfDevices();
}

int TelldusCoreObject::getDeviceId(int index) const {
	return tdGetDeviceId( index );
}

QString TelldusCoreObject::getName(int deviceId) const {
	char *name = tdGetName(deviceId);
	QString strName = QString::fromUtf8(name);
	tdReleaseString(name);
	return strName;
}

int TelldusCoreObject::lastSentCommand( int deviceId, int methodsSupported ) {
	return tdLastSentCommand(deviceId, methodsSupported);
}

QString TelldusCoreObject::lastSentValue( int deviceId ) {
	char *value = tdLastSentValue(deviceId);
	QString strValue = QString::fromUtf8(value);
	tdReleaseString(value);
	return strValue;
}

int TelldusCoreObject::methods(int deviceId, int methodsSupported) {
	return tdMethods( deviceId, methodsSupported );
}

int TelldusCoreObject::turnOn(int deviceId) {
	int retval = tdTurnOn( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::turnOff(int deviceId) {
	int retval = tdTurnOff( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::up(int deviceId) {
	int retval = tdUp( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::down(int deviceId) {
	int retval = tdDown( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

int TelldusCoreObject::stop(int deviceId) {
	int retval = tdStop( deviceId );
	if (retval != TELLSTICK_SUCCESS) {
		triggerError(deviceId, retval);
	}
	return retval;
}

void TelldusCoreObject::triggerError(int deviceId, int errorId) {
	char *errorString = tdGetErrorString( errorId );
	QString message = QString::fromUtf8( errorString );
	tdReleaseString(errorString);
	emit errorOccurred(deviceId, errorId, message);
}


void WINAPI TelldusCoreObject::deviceChangeEventCallback(int deviceId, int eventId, int changeType, int callbackId, void *context) {
	TelldusCoreObject *parent = static_cast<TelldusCoreObject *>(context);
	if (parent) {
		emit parent->deviceChange(deviceId, eventId);
	}
}

void WINAPI TelldusCoreObject::deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context) {
	TelldusCoreObject *parent = static_cast<TelldusCoreObject *>(context);
	if (parent) {
		emit parent->deviceEvent(deviceId, method, QString::fromUtf8(data));
	}
}

