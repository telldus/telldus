#include "livemessagetoken.h"

#include <QDebug>

LiveMessageToken::LiveMessageToken() {
	valueType = Invalid; 
}

LiveMessageToken::LiveMessageToken(const QString &value) {
	valueType = String;
	stringVal = value;
}

QByteArray LiveMessageToken::toByteArray() const {
	if (valueType == Int) {
		return QString("i%1e").arg(intVal).toLocal8Bit();
	} else if (valueType == Dictionary) {
		QByteArray retVal("h");
		QHashIterator<QString, LiveMessageToken> it(dictVal);
		while(it.hasNext()) {
			it.next();
			LiveMessageToken key(it.key());
			retVal.append(key.toByteArray());
			retVal.append(it.value().toByteArray());
		}
		retVal.append("e");
		return retVal;
	}
	return QString("%1:%2").arg(stringVal.length(), 0, 16).arg(stringVal).toLocal8Bit();
}

LiveMessageToken LiveMessageToken::parseToken(const QByteArray &string, int* start) {
	LiveMessageToken d;
	if ((*start) >= string.length()) {
		return d;
	}
	if (string[(*start)] == 'i') { //Int
		qDebug() << "INT!";
	} else { //String
		int index = string.indexOf(':', (*start));
		if (index < 0) {
			qDebug() << "Error in" << string;
			return d;
		}
		qDebug() << "String" << index << string;
		bool ok;
		int length = string.mid((*start), index - (*start)).toInt(&ok, 16);
		if (!ok) {
			return d;
		}
		d.stringVal = string.mid(index+1, length);
		d.valueType = LiveMessageToken::String;
		(*start) = index + length + 1;

		qDebug() << "String found:" << d.stringVal;
	}
	return d;
}