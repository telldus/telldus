#include "livemessage.h"
#include "livemessagetoken.h"

#include <QVariant>
#include <QDebug>

class LiveMessage::PrivateData {
public:
	QList<LiveMessageToken> args;
};

LiveMessage::LiveMessage(const QString &name )
		: QObject()
{
	d = new PrivateData;
	this->append(name);
}

LiveMessage::LiveMessage(const QList<LiveMessageToken> &args )
		: QObject()
{
	d = new PrivateData;
	d->args = args;
}

LiveMessage::~LiveMessage() {
	delete d;
}

void LiveMessage::append(const QString &argument) {
	this->append(LiveMessageToken(argument));
}

void LiveMessage::append(const LiveMessageToken &argument) {
	d->args.append(argument);
}

void LiveMessage::appendToken(LiveMessageTokenScriptWrapper *argument) {
	this->append(argument->token());
}

LiveMessageToken LiveMessage::arg(int index) const {
	if (index + 1 >= d->args.count()) {
		return "";
	}
	return d->args.at(index +1 );
}

LiveMessageTokenScriptWrapper *LiveMessage::argument(int index) const {
	return new LiveMessageTokenScriptWrapper(arg(index));
}


QString LiveMessage::name() const {
	return this->arg(-1).stringVal.toLower();
}

QByteArray LiveMessage::toByteArray() const {
	QByteArray retval;
	foreach(LiveMessageToken arg, d->args) {
		retval.append(arg.toByteArray());
	}
	return retval;
}


LiveMessage LiveMessage::fromByteArray(const QByteArray &ba) {
	int start = 0;
	QList<LiveMessageToken> list;
	while (start < ba.length()) {
		LiveMessageToken d = LiveMessageToken::parseToken(ba, &start);
		if (d.valueType == LiveMessageToken::Invalid) {
			break;
		}
		list.append(d);
	}
	return LiveMessage(list);
}
