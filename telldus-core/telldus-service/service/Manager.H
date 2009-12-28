#ifndef MANAGER_H
#define MANAGER_H

#include "message.h"

class ManagerPrivate;
class QLocalSocket;

class Manager : public QObject {
	Q_OBJECT
public:
	Manager(QLocalSocket *s, QObject *parent);
	~Manager(void);

protected:
	QVariant parseMessage(const QByteArray &message);

signals:
	void done();

private slots:
	void dataArrived();
	
private:
	ManagerPrivate *d;
};

#endif MANAGER_H
