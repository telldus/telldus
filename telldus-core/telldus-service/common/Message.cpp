
#include "Message.h"

/*class MesssagePrivate {
public:
};*/

Message::Message()
	:QByteArray()
{
}

Message::Message(const QByteArray &functionName)
	:QByteArray()
{
	this->addArgument(functionName);
}

Message::~Message(void) {
	//delete d;
}

void Message::addArgument(const QByteArray &value) {
	this->append(QByteArray::number(value.length()));
	this->append(":");
	this->append(value);
}

void Message::addArgument(int value) {
	this->append("i");
	this->append(QByteArray::number(value));
	this->append("e");
}

void Message::addArgument(const QVariant &value) {
	if (value.type() == QVariant::Int) {
		this->addArgument(value.toInt());
	} else {
		this->addArgument(value.toByteArray());
	}
}

void Message::addArgument(const char *value) {
	this->addArgument(QByteArray(value));
}



QVariant Message::takeFirst(QByteArray *message) {
	if (message->length() == 0) {
		return QVariant();
	}
	QChar first = message->at(0);
	if (first.isDigit()) { //String
		int index = message->indexOf(':');
		int length = message->left(index).toInt();
		QByteArray retval(message->mid(index+1, length));
		message->remove(0, index+length+1);
		return retval;
	}
	if (first == 'i') {
		int index = message->indexOf('e');
		int value = message->mid(1, index - 1).toInt();
		message->remove(0, index+1);
		return value;
	}
	return QVariant();
}