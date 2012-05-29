#ifndef XPLINSTANCE_H
#define XPLINSTANCE_H

#include <QObject>
#include "xpldevice.h"
#include "xplmessage.h"

const int XPL_PORT = 3865;

class QTimer;
class xPLInstancePrivate;

class QTXPL_EXPORT xPLInstance : public QObject
{
	Q_OBJECT

	Q_ENUMS( OperationMode );

	Q_PROPERTY( OperationMode operationMode READ operationMode );
public:
	enum OperationMode { Client, Disconnected, Hub };

	xPLInstance( const xPLDevice &device, QObject *parent = 0 );
	xPLInstance( const QString &vendor, const QString &device, QObject *parent = 0 );
	~xPLInstance();

public slots:
	bool attatched() const;

	OperationMode operationMode() const;

	void sendMessage( const xPLMessage &message ) const;
	void sendMessage( xPLMessage * message ) const;
	void sendMessage( const xPLMessage &message, const xPLDevice &device ) const;

signals:
	void messageReceived(xPLMessage *message);
	void attachedToNetwork();

private slots:
	void processPendingDatagrams();
	void sendHeartbeat();
	void poll();

private:
	void processHeartBeat( const xPLMessage &message );
	void processMessage( const xPLMessage &message );
	void init();
	int bindToPort();
	void setAttatched( bool attatched );

	xPLInstancePrivate *d;
};

#endif // XPLINSTANCE_H
