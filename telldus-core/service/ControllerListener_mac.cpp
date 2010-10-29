#include "ControllerListener.h"

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>

class TellStickData {
public:
	io_object_t				notification;
	CFStringRef             serialNumber;
	UInt32					vid;
	UInt32					pid;
	EventRef				event;
};

class ControllerListener::PrivateData {
public:
	IONotificationPortRef	gNotifyPort;
	CFRunLoopRef			gRunLoop;
	io_iterator_t			gAddedIter;
	EventRef				event;
	
	static void DeviceAdded(void *refCon, io_iterator_t iterator);
	static void DeviceNotification(void *refCon, io_service_t service, natural_t messageType, void *messageArgument);
};

ControllerListener::ControllerListener(EventRef event)
:Thread()
{
	d = new PrivateData;
	d->event = event;
	this->start();
}

ControllerListener::~ControllerListener() {
	CFRunLoopStop(d->gRunLoop);
	this->wait();
	delete d;
}

void ControllerListener::run() {
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
	
	d->gNotifyPort = IONotificationPortCreate(kIOMasterPortDefault);
    runLoopSource = IONotificationPortGetRunLoopSource(d->gNotifyPort);
    
	d->gRunLoop = CFRunLoopGetCurrent();
    CFRunLoopAddSource(d->gRunLoop, runLoopSource, kCFRunLoopDefaultMode);
    
    // Now set up a notification to be called when a device is first matched by I/O Kit.
    kr = IOServiceAddMatchingNotification(d->gNotifyPort,					// notifyPort
                                          kIOFirstMatchNotification,	// notificationType
                                          matchingDict,					// matching
                                          PrivateData::DeviceAdded,					// callback
                                          this,							// refCon
                                          &d->gAddedIter					// notification
                                          );	
	
	
	// Iterate once to get already-present devices and arm the notification    
	PrivateData::DeviceAdded(this, d->gAddedIter);	
	CFRunLoopRun();
}

void ControllerListener::PrivateData::DeviceNotification(void *refCon, io_service_t service, natural_t messageType, void *messageArgument) {
	kern_return_t	kr;
    
    if (messageType != kIOMessageServiceIsTerminated) {
		return;
	}
	
	TellStickData *tsd = reinterpret_cast<TellStickData*> (refCon);
	if (!tsd) {
		return;
	}
	
	CFIndex size = CFStringGetLength(tsd->serialNumber);
	char *s = new char[size+1];
	CFStringGetCString(tsd->serialNumber, s, size+1, kCFStringEncodingASCII);
	std::string serial(s); //Copy the string to the stack
	delete[] s;
	
	ControllerChangeEventData *data = new ControllerChangeEventData;
	data->vid = tsd->vid;
	data->pid = tsd->pid;
	data->inserted = false;
	tsd->event->signal(data);
	
	// Free the data we're no longer using now that the device is going away
	CFRelease(tsd->serialNumber);
                
	kr = IOObjectRelease(tsd->notification);
        
	delete tsd;
}

void ControllerListener::PrivateData::DeviceAdded(void *refCon, io_iterator_t iterator) {
	io_service_t		usbDevice;
	kern_return_t		kr;
	
	ControllerListener *parent = reinterpret_cast<ControllerListener*> (refCon);
	
	while ((usbDevice = IOIteratorNext(iterator))) {
		TellStickData *tsd = new TellStickData;
		tsd->event = parent->d->event;
		
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
        kr = IOServiceAddInterestNotification(parent->d->gNotifyPort, usbDevice, kIOGeneralInterest, DeviceNotification, tsd, &(tsd->notification));
		
		CFIndex size = CFStringGetLength(serialNumberAsCFString);
		char *s = new char[size+1];
		CFStringGetCString(serialNumberAsCFString, s, size+1, kCFStringEncodingASCII);
		std::string serial(s); //Copy the string to the stack
		delete[] s;
		
		kr = IOObjectRelease(usbDevice);
		
		ControllerChangeEventData *data = new ControllerChangeEventData;
		data->vid = tsd->vid;
		data->pid = tsd->pid;
		data->inserted = true;
		tsd->event->signal(data);
	}
}
