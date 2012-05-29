#include "xplmessage.h"

#include <QStringList>
#include <QMap>
#include <QDebug>

class xPLMessage::xPLMessagePrivate {
public:
	xPLMessage::Identifier identifier;
	int hop;
	QString source, target, msi;
	QMap<QString, QString> headExtra, body;
};

xPLMessage::xPLMessage( const xPLMessage &other )
	: QObject(),
	d( new xPLMessagePrivate( *other.d ))
{
	//do nothing
}

xPLMessage& xPLMessage::operator=( xPLMessage other ) {
	std::swap( this->d, other.d );
	return *this;
}


xPLMessage::xPLMessage(Identifier i)
		:QObject(),
		d(new xPLMessagePrivate)
{
	d->hop = 1;
	d->identifier = i;
	d->target = "*";
}

xPLMessage::xPLMessage()
		:QObject(),
		d(new xPLMessagePrivate)
{

	d->hop = 1;
	d->identifier = xplcmnd;
	d->target = "*";
}

xPLMessage::~xPLMessage() {
	delete d;
}


void xPLMessage::addBodyItem( const QString &key, const QString &value ) {
	d->body[key] = value;
}

void xPLMessage::addHeadItem( const QString &key, const QString &value ) {
	if (key == "hop") {
		d->hop = value.toInt();
	} else if (key == "source") {
		setSource(value);
	} else if (key == "target") {
		setTarget(value);
	} else {
		d->headExtra[key] = value;
	}
}

QString xPLMessage::bodyItem( const QString &key ) const {
	if (d->body.contains(key)) {
		return d->body[key];
	}
	return "";
}

QString xPLMessage::headItem( const QString &key ) const {
	if (d->headExtra.contains(key)) {
		return d->headExtra[key];
	}
	return "";
}

QString xPLMessage::messageSchemeIdentifier() const {
	return d->msi;
}

QString xPLMessage::source() const {
	return d->source;
}

QString xPLMessage::target() const {
	return d->target;
}

void xPLMessage::setMessageSchemeIdentifier( const QString &messageSchemeIdentifier ) {
	d->msi = messageSchemeIdentifier;
}

void xPLMessage::setSource( const QString &value ) {
	d->source = value;
}

void xPLMessage::setTarget( const QString &value ) {
	d->target = value;
}

QString xPLMessage::toString() const {
	QString message;

	switch( d->identifier ) {
		case xplcmnd:
			message += "xpl-cmnd\n";
			break;
		case xplstat:
			message += "xpl-stat\n";
			break;
		case xpltrig:
			message += "xpl-trig\n";
			break;
		default:
			return "";
	}

	message += "{\n";
	message += QString("hop=%1\n").arg(d->hop);
	message += QString("source=%1\n").arg(d->source);
	message += QString("target=%1\n").arg(d->target);
	for( QMap<QString, QString>::const_iterator it = d->headExtra.begin(); it != d->headExtra.end(); ++it ) {
		message += QString("%1=%2\n").arg(it.key()).arg(it.value());
	}
	message += "}\n";
	message += d->msi + "\n";
	message += "{\n";
	for( QMap<QString, QString>::const_iterator it = d->body.begin(); it != d->body.end(); ++it ) {
		message += QString("%1=%2\n").arg(it.key()).arg(it.value());
	}
	message += "}\n";

	return message;
}

xPLMessage *xPLMessage::createMessageFromString( const QString &message ) {
	QStringList lines = message.split('\n', QString::SkipEmptyParts);
	QStringList::const_iterator it = lines.begin();

	Identifier i;

	if (*it == "xpl-cmnd") {
		i = xplcmnd;
	} else if (*it == "xpl-stat") {
		i = xplstat;
	} else if (*it == "xpl-trig") {
		i = xpltrig;
	} else {
		return 0;
	}
	++it;

	if (*it != "{") {
		return 0;
	}
	++it;

	xPLMessage *msg = new xPLMessage(i);
	for(; it != lines.end() && *it != "}"; ++it) {
		msg->addHeadItem( (*it).section('=', 0, 0), (*it).section('=', 1) );
	}
	if (it == lines.end()) {
		delete msg;
		return 0;
	}
	++it;

	msg->setMessageSchemeIdentifier( *it );
	++it;

	if (*it != "{") {
		delete msg;
		return 0;
	}
	++it;

	for(; it != lines.end() && *it != "}"; ++it) {
		msg->addBodyItem( (*it).section('=', 0, 0), (*it).section('=', 1) );
	}

	return msg;
}

