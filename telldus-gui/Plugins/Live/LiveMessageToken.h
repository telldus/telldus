#ifndef LIVEMESSAGETOKEN_H
#define LIVEMESSAGETOKEN_H

#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <QHash>
#include <QMetaType>

class LiveMessageToken {
public:
	enum Type {Invalid, Int, String, Base64, List, Dictionary};

	LiveMessageToken();
	LiveMessageToken(const QString &value);
	LiveMessageToken(int value);
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
	LiveMessageTokenScriptWrapper(LiveMessageToken token);
	virtual ~LiveMessageTokenScriptWrapper();

	LiveMessageToken token() const;

public slots:
	void add(LiveMessageTokenScriptWrapper *token);

	int getInt(const QString &key, int defaultValue = 0) const;
	QString getString(const QString &key, const QString &defaultValue = "") const;

	int intVal() const;

	void set(const QString &key, int value);
	void set(const QString &key, const QString &value);
	void set(const QString &key, const QDateTime value);

private:
	LiveMessageToken p_token;
};

Q_DECLARE_METATYPE(LiveMessageTokenScriptWrapper*)


#endif // LIVEMESSAGETOKEN_H
