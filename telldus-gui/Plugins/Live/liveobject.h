#ifndef LIVEOBJECT_H
#define LIVEOBJECT_H

#include <QObject>
#include <QScriptEngine>
#include <QAbstractSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include "LiveMessage.h"

class LiveObject : public QObject
{
	Q_OBJECT
public:
	LiveObject( QScriptEngine *engine, QObject * parent = 0 );
	virtual ~LiveObject();

public slots:
	void activate();
	void connectToServer();
	void disconnect();
	void pingServer();
	void sendMessage(LiveMessage *message);

signals:
	void connected();
	void disconnected();
	void messageReceived(LiveMessage *message);
	void registered();

private slots:
	void p_connected();
	void p_disconnected();
	void readyRead();
	void error( QAbstractSocket::SocketError socketError );
	void stateChanged( QAbstractSocket::SocketState socketState );

private:
	void sendMessage(const LiveMessage &message);
	static QNetworkInterface interfaceFromAddress( const QHostAddress &address );

	class PrivateData;
	PrivateData *d;
};

#endif // LIVEOBJECT_H
