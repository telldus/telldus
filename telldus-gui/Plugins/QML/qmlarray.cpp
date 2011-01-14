#include "qmlarray.h"

#include <QDebug>

class QMLArray::PrivateData {
public:
	QList<QScriptValue> list;
};

QMLArray::QMLArray(QObject *parent) :
	QAbstractListModel(parent)
{
	d = new PrivateData;

	QHash<int, QByteArray> roles;
	roles[Qt::UserRole+1] = "modelData";
	setRoleNames(roles);
}

QMLArray::~QMLArray() {
	delete d;
}

int QMLArray::rowCount(const QModelIndex &parent) const {
	return d->list.size();
}

QVariant QMLArray::data(const QModelIndex &index, int role) const {
	return d->list.at(index.row()).toVariant();
}

void QMLArray::push(const QScriptValue &v) {
	beginInsertRows( QModelIndex(), d->list.size(), d->list.size() );
	d->list << v;
	endInsertRows();
}
