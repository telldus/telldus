#include "device.h"
#include "telldus-core.h"
#include <stdlib.h>

QHash<int, Device *> Device::devices;

Device::Device(int id)
		:p_id(id),
		p_model(0),
		p_name(""),
		p_protocol(""),
		p_modelChanged(false),
		p_nameChanged(false),
		p_protocolChanged(false)
{
	if (id > 0) {
		char *name = tdGetName(id);
		p_name = QString::fromLocal8Bit( name );
		free( name );

		p_model = tdGetModel(id);

		char *protocol = tdGetProtocol(id);
		p_protocol = QString::fromUtf8( protocol );
		free( protocol );
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

void Device::save() {
	bool deviceIsAdded = false;
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
		p_modelChanged = false;
	}

	if (p_protocolChanged) {
		tdSetProtocol(p_id, p_protocol.toUtf8());
		p_protocolChanged = false;
	}

	if (deviceIsAdded) {
		emit deviceAdded(p_id);
	}
}
