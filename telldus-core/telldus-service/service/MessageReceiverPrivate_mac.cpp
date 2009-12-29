#include "MessageReceiverPrivate_mac.h"

struct PrivateTellStickData {
	io_object_t				notification;
	CFStringRef             serialNumber;
	MessageReceiverPrivate	*parent;
	UInt32					vid;
	UInt32					pid;
};

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
                                          this,							// refCon
                                          &gAddedIter					// notification
                                          );	
	
	
	// Iterate once to get already-present devices and arm the notification    
    DeviceAdded(this, gAddedIter);	
	printf("Starting RunLoop\n");
	CFRunLoopRun();
	printf("Runloop done\n");
}

void MessageReceiverPrivate::DeviceNotification(void *refCon, io_service_t service, natural_t messageType, void *messageArgument) {
	kern_return_t	kr;
    
    if (messageType != kIOMessageServiceIsTerminated) {
		return;
	}
	
	PrivateTellStickData *tsd = reinterpret_cast<PrivateTellStickData*> (refCon);
	if (!tsd) {
		return;
	}
	
	CFIndex size = CFStringGetLength(tsd->serialNumber);
	char *s = new char[size+1];
	CFStringGetCString(tsd->serialNumber, s, size+1, kCFStringEncodingASCII);
	QString serial(s); //Copy the string to the stack
	delete[] s;
		
	emit tsd->parent->deviceRemoved(tsd->vid, tsd->pid, serial);
	
	// Free the data we're no longer using now that the device is going away
	CFRelease(tsd->serialNumber);
                
	kr = IOObjectRelease(tsd->notification);
        
	delete tsd;
}

void MessageReceiverPrivate::DeviceAdded(void *refCon, io_iterator_t iterator) {
	io_service_t		usbDevice;
	kern_return_t		kr;
	
	MessageReceiverPrivate *parent = reinterpret_cast<MessageReceiverPrivate*> (refCon);
	
	while ((usbDevice = IOIteratorNext(iterator))) {
		PrivateTellStickData *tsd = new PrivateTellStickData;
		tsd->parent = parent;
		
		// Get the serial number
		CFStringRef serialRef = reinterpret_cast<CFStringRef>(IORegistryEntryCreateCFProperty(  usbDevice, CFSTR("USB Serial Number" ), kCFAllocatorDefault, 0 ));
		if (serialRef == NULL) {
			//No serial number, we cannot continue. Sorry
			continue;
		}
		

		CFNumberRef vidRef = reinterpret_cast<CFNumberRef> (IORegistryEntryCreateCFProperty(usbDevice, CFSTR("idVendor"), kCFAllocatorDefault, 0));
		if (vidRef) {
			CFNumberGetValue(vidRef, kCFNumberIntType, &(tsd->vid));
			CFRelease(vidRef);
		}
		
		CFNumberRef pidRef = reinterpret_cast<CFNumberRef> (IORegistryEntryCreateCFProperty(usbDevice, CFSTR("idProduct"), kCFAllocatorDefault, 0));
		if (pidRef) {
			CFNumberGetValue(pidRef, kCFNumberIntType, &(tsd->pid));
			CFRelease(pidRef);
		}
				
		CFStringRef serialNumberAsCFString = CFStringCreateCopy(kCFAllocatorDefault, serialRef);
		tsd->serialNumber = serialNumberAsCFString;
		CFRelease(serialRef); 
		
		// Register for an interest notification of this device being removed. Use a reference to our
        // private data as the refCon which will be passed to the notification callback.
        kr = IOServiceAddInterestNotification(parent->gNotifyPort, usbDevice, kIOGeneralInterest, DeviceNotification, tsd, &(tsd->notification));
		
		CFIndex size = CFStringGetLength(serialNumberAsCFString);
		char *s = new char[size+1];
		CFStringGetCString(serialNumberAsCFString, s, size+1, kCFStringEncodingASCII);
		QString serial(s); //Copy the string to the stack
		delete[] s;
		
		kr = IOObjectRelease(usbDevice);
		
		emit parent->deviceInserted(tsd->vid, tsd->pid, serial );
	}
}
