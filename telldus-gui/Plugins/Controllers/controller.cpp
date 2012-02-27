#include "controller.h"
#include <telldus-core.h>
#include <QMessageBox>
#include <QDebug>

class Controller::PrivateData {
public:
	bool available, nameChanged;
	int id, type;
	QString name, serial, firmware;
};

Controller::Controller(int id, int type, const QString &name, QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
	d->id = id;
	d->type = type;
	d->available = false;
	d->nameChanged = false;
	d->name = name;

	const int DATA_LENGTH = 255;
	char buff[DATA_LENGTH];
	if (tdControllerValue(id, "serial", buff, DATA_LENGTH) == TELLSTICK_SUCCESS) {
		d->serial = QString::fromUtf8(buff);
	}
	if (tdControllerValue(id, "firmware", buff, DATA_LENGTH) == TELLSTICK_SUCCESS) {
		d->firmware = QString::fromUtf8(buff);
	}
}

Controller::~Controller() {
	delete d;
}

bool Controller::available() const {
	return d->available;
}

void Controller::setAvailable(bool available) {
	d->available = available;
	emit availableChanged();
	emit firmwareChanged();
}

QString Controller::firmware() const {
	if (!d->available) {
		return "?";
	}
	return d->firmware;
}

void Controller::setFirmware(const QString &version) {
	d->firmware = version;
	emit firmwareChanged();
}

int Controller::id() const {
	return d->id;
}

QString Controller::name() const {
	return d->name;
}

void Controller::setName(const QString &name) {
	if (name == d->name) {
		return;
	}
	d->nameChanged = true;
	d->name = name;
	emit nameChanged();
}

void Controller::save() {
	if (d->nameChanged) {
		tdSetControllerValue(d->id, "name", d->name.toUtf8());
		d->nameChanged = false;
	}
}

QString Controller::serial() const {
	return d->serial;
}

void Controller::tryRemove() {
	QMessageBox msgBox;
	msgBox.setText( tr("Are you sure you want to remove the selected controller?") );
	msgBox.setInformativeText( tr("If you connect it again at a later point it will be readded automatically.") );
	msgBox.setIcon( QMessageBox::Warning );
	msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	msgBox.setDefaultButton( QMessageBox::No );
	if ( msgBox.exec() ==  QMessageBox::Yes) {
		tdRemoveController(d->id);
	}
}

int Controller::type() const {
	return d->type;
}

