#include "livemessage.h"

#include <QVariant>
#include <QDebug>

class LiveMessage::PrivateData {
public:
	QList<QVariant> args;
};

LiveMessage::LiveMessage(const QString &name )
		: QObject()
{
	d = new PrivateData;
	this->append(name);
}

LiveMessage::LiveMessage(const QList<QVariant> &args )
		: QObject()
{
	d = new PrivateData;
	d->args = args;
}

LiveMessage::~LiveMessage() {
	delete d;
}

void LiveMessage::append(const QString &argument) {
	d->args.append(argument);
}

QString LiveMessage::argument(int index) const {
	if (index + 1 >= d->args.count()) {
		return "";
	}
	return d->args.at(index +1 ).toString();
}

QString LiveMessage::name() const {
	return this->argument(-1).toLower();
}

QByteArray LiveMessage::toByteArray() const {
	QByteArray retval;
	foreach(QVariant arg, d->args) {
		retval.append(QString("%1:%2").arg(arg.toString().length(), 0, 16).arg(arg.toString()));
	}
	return retval;
}

LiveMessage LiveMessage::fromByteArray(const QByteArray &ba) {
	bool ok;
	int pos = 0;
	QList<QVariant> list;
	while(1) {
		int index = ba.indexOf(':', pos);
		if (index < 0) {
			return LiveMessage("");
		}
		int length = ba.mid(pos, index - pos).toInt(&ok, 16);
		if (!ok) {
			return LiveMessage("");
		}
		pos = index + length + 1;
		if (ba.length() < pos) {
			return LiveMessage("");
		}
		QString arg = ba.mid(index+1, length);
		list.append(arg);
		if (pos >= ba.length()) {
			break;
		}
	}
	return LiveMessage(list);
}
