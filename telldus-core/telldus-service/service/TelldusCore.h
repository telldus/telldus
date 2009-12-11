#ifndef TELLDUS_CORE_H
#define TELLDUS_CORE_H

#include <QObject>

class TelldusCorePrivate;

class TelldusCore :	public QObject {
	Q_OBJECT
public:
	TelldusCore(void);
	~TelldusCore(void);

private slots:
	void newConnection();

private:
	TelldusCorePrivate * const d;
};

#endif