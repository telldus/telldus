#ifndef MANAGER_H
#define MANAGER_H

#include <QThread>

class ManagerPrivate;
class QLocalSocket;

class Manager : public QThread {
	Q_OBJECT
public:
	Manager(QLocalSocket *s, QObject *parent);
	~Manager(void);

protected:
	virtual void run();
	QVariant parseMessage(const QByteArray &message);

signals:
	void done();

private:
	ManagerPrivate *d;
};

#endif MANAGER_H
