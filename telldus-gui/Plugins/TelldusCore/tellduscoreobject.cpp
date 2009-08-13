#include "tellduscoreobject.h"
#include <telldus-core.h>
#include <QDebug>

TelldusCoreObject::TelldusCoreObject( QObject * parent )
		: QObject(parent)
{
}

TelldusCoreObject::~TelldusCoreObject() {
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
