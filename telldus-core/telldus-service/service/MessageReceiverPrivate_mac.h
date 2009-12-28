#ifndef MESSAGERECEIVERPRIVATE_H
#define MESSAGERECEIVERPRIVATE_H

#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>

#include <QThread>

class MessageReceiverPrivate : public QThread {
	Q_OBJECT
public:
	MessageReceiverPrivate(QObject *parent = 0);
	virtual ~MessageReceiverPrivate();
	
signals:
	void deviceInserted(int vid, int pid, const QString &serial);
	void deviceRemoved(int vid, int pid, const QString &serial);
	
protected:
	virtual void run();
	
private:
	static void DeviceAdded(void *refCon, io_iterator_t iterator);
	static void DeviceNotification(void *refCon, io_service_t service, natural_t messageType, void *messageArgument);
	
	IONotificationPortRef	gNotifyPort;
	CFRunLoopRef			gRunLoop;
	io_iterator_t			gAddedIter;
};

#endif //MESSAGERECEIVERPRIVATE_H
