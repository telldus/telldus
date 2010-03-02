#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>

class MessageReceiverPrivate;

class MessageReceiver : public QObject {
	Q_OBJECT
public:
	MessageReceiver(QObject *parent);
	~MessageReceiver(void);

signals:
	void deviceInserted(int vid, int pid, const QString &serial);
	void deviceRemoved(int vid, int pid, const QString &serial);

private:
	MessageReceiverPrivate *d;
};

#endif //MESSAGERECEIVER_H
