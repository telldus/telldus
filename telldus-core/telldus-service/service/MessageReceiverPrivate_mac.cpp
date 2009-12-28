#include "MessageReceiverPrivate_mac.h"

MessageReceiverPrivate::MessageReceiverPrivate(QObject *parent) : QThread(parent) {
	this->start();
}

MessageReceiverPrivate::~MessageReceiverPrivate() {
	printf("msgRecieverP cleanup\n");
	CFRunLoopStop(gRunLoop);
	printf("Stopped runloop, waiting\n");
	this->wait();
	printf("Wait done\n");
}

void MessageReceiverPrivate::run() {
	CFMutableDictionaryRef 	matchingDict;
    CFRunLoopSourceRef		runLoopSource;
    CFNumberRef				numberRef;
	kern_return_t			kr;
	long					usbVendor = 0x1781;
    long					usbProduct = 0x0c30;
	
	matchingDict = IOServiceMatching(kIOUSBDeviceClassName);	// Interested in instances of class
	// IOUSBDevice and its subclasses
	
	if (matchingDict == NULL) {
		return;
	}
	
	// Create a CFNumber for the idVendor and set the value in the dictionary
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usbVendor);
    CFDictionarySetValue(matchingDict, CFSTR(kUSBVendorID), numberRef);
    CFRelease(numberRef);
    
    // Create a CFNumber for the idProduct and set the value in the dictionary
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usbProduct);
    CFDictionarySetValue(matchingDict, CFSTR(kUSBProductID), numberRef);
    CFRelease(numberRef);
    numberRef = NULL;
	
	gNotifyPort = IONotificationPortCreate(kIOMasterPortDefault);
    runLoopSource = IONotificationPortGetRunLoopSource(gNotifyPort);
    
    gRunLoop = CFRunLoopGetCurrent();
    CFRunLoopAddSource(gRunLoop, runLoopSource, kCFRunLoopDefaultMode);
    
    // Now set up a notification to be called when a device is first matched by I/O Kit.
    kr = IOServiceAddMatchingNotification(gNotifyPort,					// notifyPort
                                          kIOFirstMatchNotification,	// notificationType
                                          matchingDict,					// matching
                                          DeviceAdded,					// callback
                                          NULL,							// refCon
                                          &gAddedIter					// notification
                                          );	
	
	
	// Iterate once to get already-present devices and arm the notification    
    DeviceAdded(NULL, gAddedIter);	
	printf("Starting RunLoop\n");
	CFRunLoopRun();
	printf("Runloop done\n");
}

void MessageReceiverPrivate::DeviceNotification(void *refCon, io_service_t service, natural_t messageType, void *messageArgument) {
	printf("DeviceNotification\n");
}

void MessageReceiverPrivate::DeviceAdded(void *refCon, io_iterator_t iterator) {
	io_service_t		usbDevice;
	kern_return_t		kr;
	
	
	while ((usbDevice = IOIteratorNext(iterator))) {
		printf("DeviceAdded\n");
		kr = IOObjectRelease(usbDevice);
	}
}
