#include "tellduscoreobject.h"
#include <telldus-core.h>
#include <QDebug>

TelldusCoreObject::TelldusCoreObject( QObject * parent )
		: QObject(parent)
{
}

TelldusCoreObject::~TelldusCoreObject() {
}

int TelldusCoreObject::bell(int deviceId) {
	return tdBell( deviceId );
}

int TelldusCoreObject::dim(int deviceId, char level) {
	return tdDim( deviceId, level );
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
	free(name);
	return strName;
}

int TelldusCoreObject::methods(int deviceId, int methodsSupported) {
	return tdMethods( deviceId, methodsSupported );
}

int TelldusCoreObject::turnOn(int deviceId) {
	return tdTurnOn( deviceId );
}

int TelldusCoreObject::turnOff(int deviceId) {
	return tdTurnOff( deviceId );
}

