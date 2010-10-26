#include "LiveMessageToken.h"

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
		return QString("i%1s").arg(intVal, 0, 16).toLocal8Bit();

	} else if (valueType == List) {
		QByteArray retVal("l");
		foreach(LiveMessageToken token, listVal) {
			retVal.append(token.toByteArray());
		}
		retVal.append("s");
		return retVal;

	} else if (valueType == Dictionary) {
		QByteArray retVal("h");
		QHashIterator<QString, LiveMessageToken> it(dictVal);
		while(it.hasNext()) {
			it.next();
			LiveMessageToken key(it.key());
			retVal.append(key.toByteArray());
			retVal.append(it.value().toByteArray());
		}
		retVal.append("s");
		return retVal;
	}
	//String or Base64 encoded string
	QByteArray retVal;
	QByteArray string = stringVal.toUtf8();
	if (valueType == Base64) {
		string = string.toBase64();
		retVal.append('u');
	}
	retVal.append(QByteArray::number(string.length(),16));
	retVal.append(':');
	retVal.append(string);
	return retVal;
}

LiveMessageToken LiveMessageToken::parseToken(const QByteArray &string, int* start) {
	LiveMessageToken d;
	if ((*start) >= string.length()) {
		return d;
	}
	if (string[(*start)] == 'i') { //Int
		++(*start);
		int index = string.indexOf('s', (*start));
		if (index < 0) {
			qDebug() << "Error in" << string;
			return d;
		}
		bool ok;
		d.intVal = string.mid((*start), index-(*start)).toInt(&ok, 16);
		if (!ok) {
			return d;
		}
		d.valueType = LiveMessageToken::Int;
		(*start) = index + 1;
		return d;

	} else if (string[(*start)] == 'h') { //Dictionary
		++(*start);
		while((*start) < string.length() && string[(*start)] != 's') {
			LiveMessageToken keyToken(LiveMessageToken::parseToken(string, start));
			if (keyToken.valueType == LiveMessageToken::Invalid) {
				break;
			}
			LiveMessageToken valueToken(LiveMessageToken::parseToken(string, start));
			if (valueToken.valueType == LiveMessageToken::Invalid) {
				break;
			}
			d.valueType = LiveMessageToken::Dictionary;
			d.dictVal[keyToken.stringVal] = valueToken;
		}
		++(*start);
	} else { //String
		int index = string.indexOf(':', (*start));
		if (index < 0) {
			qDebug() << "Error in" << string;
			return d;
		}
		bool ok;
		int length = string.mid((*start), index - (*start)).toInt(&ok, 16);
		if (!ok) {
			return d;
		}
		d.stringVal = QString::fromUtf8(string.mid(index+1, length));
		d.valueType = LiveMessageToken::String;
		(*start) = index + length + 1;
	}
	return d;
}

LiveMessageTokenScriptWrapper::LiveMessageTokenScriptWrapper()
	:QObject()
{
}

LiveMessageTokenScriptWrapper::LiveMessageTokenScriptWrapper(LiveMessageToken token)
	:QObject(), p_token(token)
{
}

LiveMessageTokenScriptWrapper::~LiveMessageTokenScriptWrapper() {
}

LiveMessageToken LiveMessageTokenScriptWrapper::token() const {
	return p_token;
}

void LiveMessageTokenScriptWrapper::add(LiveMessageTokenScriptWrapper *t) {
	p_token.valueType = LiveMessageToken::List;
	p_token.listVal << t->p_token;
}

int LiveMessageTokenScriptWrapper::getInt(const QString &key, int defaultValue) {
	if (p_token.valueType != LiveMessageToken::Dictionary) {
		return defaultValue;
	}
	if (!p_token.dictVal.contains(key)) {
		return defaultValue;
	}
	return p_token.dictVal[key].intVal;
}

int LiveMessageTokenScriptWrapper::intVal() const {
	return p_token.intVal;
}

void LiveMessageTokenScriptWrapper::set(const QString &key, int value) {
	p_token.valueType = LiveMessageToken::Dictionary;
	LiveMessageToken token;
	token.valueType = LiveMessageToken::Int;
	token.intVal = value;
	p_token.dictVal[key] = token;
}

void LiveMessageTokenScriptWrapper::set(const QString &key, const QString &value) {
	p_token.valueType = LiveMessageToken::Dictionary;
	LiveMessageToken token;
	token.valueType = LiveMessageToken::Base64;
	token.stringVal = value;
	p_token.dictVal[key] = token;
}
