#include "controllerlist.h"
#include "controller.h"
#include <telldus-core.h>

#include <QDebug>

class ControllerList::PrivateData {
public:
	QList<Controller *> list;
};

ControllerList::ControllerList(QObject *parent) :
	QAbstractListModel(parent)
{
	d = new PrivateData;

	QHash<int, QByteArray> roles;
	roles[Qt::UserRole+1] = "controller";
	setRoleNames(roles);

	const int DATA_LENGTH = 255;
	char name[DATA_LENGTH];
	int available, controllerId, type;
	while(tdController(&controllerId, &type, name, DATA_LENGTH, &available) == TELLSTICK_SUCCESS) {
		Controller *controller = new Controller(controllerId, type, QString::fromUtf8(name), this);
		controller->setAvailable(available);
		connect(controller, SIGNAL(nameChanged()), this, SIGNAL(changed()));
		d->list.append(controller);
	}
}

ControllerList::~ControllerList() {
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
