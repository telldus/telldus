#include "tellduscoreobject.h"
#include <QDateTime>
#include <QDebug>

TelldusCoreObject::TelldusCoreObject( QObject * parent )
		: QObject(parent)
{
	tdInit();
	deviceEventId = tdRegisterDeviceEvent(reinterpret_cast<TDDeviceEvent>(&TelldusCoreObject::deviceEventCallback), this);
	deviceChangeEventId = tdRegisterDeviceChangeEvent(reinterpret_cast<TDDeviceChangeEvent>(&TelldusCoreObject::deviceChangeEventCallback), this);
	sensorEventId = tdRegisterSensorEvent(reinterpret_cast<TDSensorEvent>(&TelldusCoreObject::sensorEventCallback), this);
}

TelldusCoreObject::~TelldusCoreObject() {
	tdUnregisterCallback(deviceEventId);
	tdUnregisterCallback(deviceChangeEventId);
	tdUnregisterCallback(sensorEventId);
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

QVariant TelldusCoreObject::sensor() const {
	const int DATA_LENGTH = 20;
	char protocol[DATA_LENGTH], model[DATA_LENGTH];
	int sensorId = 0, dataTypes = 0;


	if (tdSensor(protocol, DATA_LENGTH, model, DATA_LENGTH, &sensorId, &dataTypes) != TELLSTICK_SUCCESS) {
		return 0;
	}

	QVariantMap retval;

	retval["protocol"] = protocol;
	retval["model"] = model;
	retval["sensorId"] = sensorId;
	retval["dataTypes"] = dataTypes;

	return retval;
}

QVariant TelldusCoreObject::sensorValue(const QString &protocol, const QString &model, int id, int dataType) const {
	const int DATA_LENGTH = 20;
	char value[DATA_LENGTH];
	time_t timestamp = 0;

	tdSensorValue(protocol.toUtf8(), model.toUtf8(), id, dataType, value, DATA_LENGTH, (int *)&timestamp);
	QVariantMap retval;
	retval["value"] = value;
	retval["timestamp"] = QDateTime::fromTime_t(timestamp);
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

void WINAPI TelldusCoreObject::sensorEventCallback(const char *protocol, const char *model, int id, int dataType, const char *value, int timestamp, int callbackId, void *context) {
	TelldusCoreObject *parent = static_cast<TelldusCoreObject *>(context);
	if (parent) {
		emit parent->sensorEvent(QString::fromUtf8(protocol), QString::fromUtf8(model), id, dataType, QString::fromUtf8(value), QDateTime::fromTime_t(timestamp));
	}
}
