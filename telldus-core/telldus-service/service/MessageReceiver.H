#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>
#include "Windows.h"

class MessageReceiverPrivate;

class MessageReceiver : public QObject {
	Q_OBJECT
public:
	MessageReceiver(QObject *parent);
	~MessageReceiver(void);

	static LRESULT MyWindowProc(HWND, UINT, WPARAM, LPARAM);

signals:
	void deviceInserted(int vid, int pid, const QString &serial);
	void deviceRemoved(int vid, int pid, const QString &serial);

private:
	MessageReceiverPrivate *d;
	static MessageReceiver *instance;
};

#endif //MESSAGERECEIVER_H
