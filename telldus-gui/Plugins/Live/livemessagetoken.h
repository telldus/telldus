#ifndef LIVEMESSAGETOKEN_H
#define LIVEMESSAGETOKEN_H

#include <QByteArray>
#include <QString>
#include <QHash>
#include <QMetaType>

class LiveMessageToken {
public:
	enum Type {Invalid, Int, String, List, Dictionary};

	LiveMessageToken();
	LiveMessageToken(const QString &value);
	QByteArray toByteArray() const;
	static LiveMessageToken parseToken(const QByteArray &string, int* start);

	Type valueType;
	int intVal;
	QString stringVal;
	QList<LiveMessageToken> listVal;
	QHash<QString, LiveMessageToken> dictVal;
};

class LiveMessageTokenScriptWrapper : public QObject {
	Q_OBJECT
public:
	LiveMessageTokenScriptWrapper();
	virtual ~LiveMessageTokenScriptWrapper();

	LiveMessageToken token() const;

public slots:
	void add(LiveMessageTokenScriptWrapper *token);

	void set(const QString &key, int value);
	void set(const QString &key, const QString &value);
	
private:
	LiveMessageToken p_token;
};

Q_DECLARE_METATYPE(LiveMessageTokenScriptWrapper*)


#endif // LIVEMESSAGETOKEN_H
