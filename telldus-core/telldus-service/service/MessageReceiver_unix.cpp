#include "MessageReceiver.h"
#include "TelldusCore.h"


MessageReceiver::MessageReceiver(QObject *parent)
	:QObject(parent)
{
	
/*	d = new MessageReceiverPrivate(this);
	connect(d, SIGNAL(deviceInserted(int, int, const QString &)), this, SIGNAL(deviceInserted(int, int, const QString &)));
	connect(d, SIGNAL(deviceRemoved(int, int, const QString &)), this, SIGNAL(deviceRemoved(int, int, const QString &)));*/
	TelldusCore::logMessage("MessageReceiver");
}

MessageReceiver::~MessageReceiver(void) {
	TelldusCore::logMessage("MessageReceiver shutdown");
}

