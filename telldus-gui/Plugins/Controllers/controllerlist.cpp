#include "controllerlist.h"
#include "controller.h"
#include "tellstick.h"

#include <QDebug>

class ControllerList::PrivateData {
public:
	QList<Controller *> list;
	int callbackId;
};

ControllerList::ControllerList(QObject *parent) :
	QAbstractListModel(parent)
{
	d = new PrivateData;

	QHash<int, QByteArray> roles;
	roles[Qt::UserRole+1] = "controller";
	setRoleNames(roles);

	connect(this, SIGNAL(controllerEventSignal(int,int,int,QString)), this, SLOT(controllerEventSlot(int,int,int,QString)), Qt::QueuedConnection);
	d->callbackId = tdRegisterControllerEvent(&ControllerList::controllerEvent, this);

	const int DATA_LENGTH = 255;
	char name[DATA_LENGTH];
	int available, controllerId, type;
	while(tdController(&controllerId, &type, name, DATA_LENGTH, &available) == TELLSTICK_SUCCESS) {
		Controller *controller = loadController(controllerId, type, QString::fromUtf8(name), this);
		controller->setAvailable(available);
		connect(controller, SIGNAL(nameChanged()), this, SIGNAL(changed()));
		d->list.append(controller);
	}
}

ControllerList::~ControllerList() {
	tdUnregisterCallback(d->callbackId);
	delete d;
}

QVariant ControllerList::data(const QModelIndex &index, int role) const {
	return QVariant::fromValue(d->list.at(index.row()));
}

int ControllerList::rowCount(const QModelIndex &parent) const {
	return d->list.size();
}

void ControllerList::save() {
	for(int i = 0; i < d->list.size(); ++i) {
		d->list.at(i)->save();
	}
}

void ControllerList::controllerEventSlot(int controllerId, int changeEvent, int changeType, const QString &newValue) {
	if (changeEvent == TELLSTICK_DEVICE_STATE_CHANGED) {
		for(int i = 0; i < d->list.size(); ++i) {
			if (d->list.at(i)->id() != controllerId) {
				continue;
			}
			if (changeType == TELLSTICK_CHANGE_AVAILABLE) {
				if (newValue == "1") {
					d->list.at(i)->setAvailable(true);
				} else if (newValue == "0") {
					d->list.at(i)->setAvailable(false);
				}
			} else if (changeType == TELLSTICK_CHANGE_FIRMWARE) {
				d->list.at(i)->setFirmware(newValue);
			}
		}
		return;
	}

	if (changeEvent == TELLSTICK_DEVICE_ADDED) {
		beginInsertRows( QModelIndex(), d->list.size(), d->list.size() );
		Controller *controller = loadController(controllerId, changeType, "", this);
		controller->setAvailable(true);
		connect(controller, SIGNAL(nameChanged()), this, SIGNAL(changed()));
		d->list.append(controller);
		endInsertRows();
		return;
	}

	if (changeEvent == TELLSTICK_DEVICE_REMOVED) {
		for(int i = 0; i < d->list.size(); ++i) {
			if (d->list.at(i)->id() != controllerId) {
				continue;
			}
			beginRemoveRows( QModelIndex(), i, i );
			d->list.takeAt(i);
			endRemoveRows();
		}
		return;
	}
}

Controller *ControllerList::loadController(int id, int type, const QString &name, QObject *parent) {
	if (type == 1 || type == 2) {
		return new TellStick(id, type, "", parent);
	}
	return new Controller(id, type, "", parent);
}

void WINAPI ControllerList::controllerEvent( int controllerId, int changeEvent, int changeType, const char *newValue, int callbackId, void *context) {
	ControllerList *controllerList = reinterpret_cast<ControllerList *>(context);
	if (!controllerList) {
		return;
	}
	emit controllerList->controllerEventSignal(controllerId, changeEvent, changeType, QString::fromUtf8(newValue));
}
