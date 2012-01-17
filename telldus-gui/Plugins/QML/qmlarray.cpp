#include "qmlarray.h"
#include <QMetaMethod>

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

QVariant QMLArray::data(const QModelIndex &index, int role) const {
	return d->list.at(index.row()).toVariant();
}

QVariant QMLArray::get(int index) const {
	return this->data(this->index(index), Qt::DisplayRole);
}

void QMLArray::push(const QScriptValue &v) {
	beginInsertRows( QModelIndex(), d->list.size(), d->list.size() );
	d->list << v;
	endInsertRows();
}

void QMLArray::remove(int index) {
	beginRemoveRows( QModelIndex(), index, index );
	d->list.takeAt(index);
	endRemoveRows();
}

void QMLArray::removeLater(int index) {
	int methodIndex = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature("remove(int)"));
	QMetaMethod method = this->metaObject()->method(methodIndex);
	method.invoke(this, Qt::QueuedConnection, Q_ARG(int, index));
}

int QMLArray::rowCount(const QModelIndex &parent) const {
	return d->list.size();
}
