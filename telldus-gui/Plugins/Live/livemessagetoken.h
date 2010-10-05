#ifndef LIVEMESSAGETOKEN_H
#define LIVEMESSAGETOKEN_H

#include <QByteArray>
#include <QString>
#include <QHash>

class LiveMessageToken {
public:
	enum Type {Invalid, Int, String, Dictionary};

	LiveMessageToken();
	LiveMessageToken(const QString &value);
	QByteArray toByteArray() const;
	static LiveMessageToken parseToken(const QByteArray &string, int* start);

	Type valueType;
	int intVal;
	QString stringVal;
	QHash<QString, LiveMessageToken> dictVal;
};



#endif // LIVEMESSAGETOKEN_H
