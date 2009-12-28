#include "MessageReceiver.h"
#include "TelldusCore.h"

#include "MessageReceiverPrivate_mac.h"

MessageReceiver::MessageReceiver(QObject *parent)
	:QObject(parent)
{
	
	d = new MessageReceiverPrivate(this);
	TelldusCore::logMessage("MessageReceiver");
}

MessageReceiver::~MessageReceiver(void) {
	TelldusCore::logMessage("MessageReceiver shutdown");
}

