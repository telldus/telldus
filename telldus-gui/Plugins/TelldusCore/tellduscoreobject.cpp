#include "tellduscoreobject.h"
#include <telldus-core.h>
#include <QDebug>

TelldusCoreObject::TelldusCoreObject( QObject * parent )
		: QObject(parent)
{
	tdInit();
	tdRegisterDeviceChangeEvent(reinterpret_cast<TDDeviceChangeEvent>(deviceChangeEvent), this);
}

TelldusCoreObject::~TelldusCoreObject() {
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
	QString strName = QString::fromLocal8Bit(name);
	tdReleaseString(name);
	return strName;
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

void TelldusCoreObject::triggerError(int deviceId, int errorId) {
	char *errorString = tdGetErrorString( errorId );
	QString message = QString::fromLocal8Bit( errorString );
	free(errorString);
	emit errorOccurred(deviceId, errorId, message);
}


void TelldusCoreObject::deviceChangeEvent(int deviceId, int eventId, int changeType, int callbackId, void *object) {
	TelldusCoreObject *parent = static_cast<TelldusCoreObject *>(object);
	if (parent) {
		emit parent->deviceChange(deviceId, eventId);
	}
}



