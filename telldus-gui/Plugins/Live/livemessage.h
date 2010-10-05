#ifndef LIVEMESSAGE_H
#define LIVEMESSAGE_H

#include <QObject>
#include <QMetaType>
class LiveMessageToken;
class LiveMessageTokenScriptWrapper;

class LiveMessage : public QObject
{
	Q_OBJECT
public:
	LiveMessage(const QString &name);
	virtual ~LiveMessage();

	QByteArray toByteArray() const;

	static LiveMessage fromByteArray(const QByteArray &ba);

public slots:
	void append(const QString &argument);
	void append(const LiveMessageToken &argument);
	void appendToken(LiveMessageTokenScriptWrapper *argument);
	QByteArray argument(int index) const;
	QString name() const;

private:
	LiveMessage(const QList<LiveMessageToken> &args);
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(LiveMessage*)

#endif // LIVEMESSAGE_H
