#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>

#ifdef _WINDOWS
#include "Windows.h"
#endif

class MessageReceiverPrivate;

class MessageReceiver : public QObject {
	Q_OBJECT
public:
	MessageReceiver(QObject *parent);
	~MessageReceiver(void);

#ifdef _WINDOWS
	static LRESULT MyWindowProc(HWND, UINT, WPARAM, LPARAM);
#endif

signals:
	void deviceInserted(int vid, int pid, const QString &serial);
	void deviceRemoved(int vid, int pid, const QString &serial);

private:
	MessageReceiverPrivate *d;
#ifdef _WINDOWS
	static MessageReceiver *instance;
#endif
};

#endif //MESSAGERECEIVER_H
