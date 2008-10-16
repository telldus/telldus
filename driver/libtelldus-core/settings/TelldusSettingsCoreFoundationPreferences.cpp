//
// C++ Implementation: telldussettingsconfuse
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "TelldusSettings.h"
#include <stdlib.h>
#include <string.h>
#include <CoreFoundation/CoreFoundation.h>

using namespace std;

class privateVars {
public:
	CFStringRef app_ID;
};

	
/*
* Constructor
*/
TelldusSettings::TelldusSettings(void)
{
	d = new privateVars();
	d->app_ID = CFSTR( "com.telldus.core" );
}

/*
* Destructor
*/
TelldusSettings::~TelldusSettings(void)
{
}

/*
* Return a setting
*/
char *TelldusSettings::getSetting(const char *strName) {
	return "";
}

/*
* Return the number of stored devices
*/
int TelldusSettings::getNumberOfDevices(void) {
	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, kCFPreferencesCurrentUser, kCFPreferencesAnyHost );
	if (!cfarray) return 0;
	CFIndex size = CFArrayGetCount( cfarray );
	int devices = 0;
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, CFSTR("device.") ) &&
			CFStringHasSuffix( key, CFSTR(".name") ) ) {
			devices++;
		}
	}	
	return devices;
}

int TelldusSettings::getDeviceId(int intDeviceIndex) {
	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, kCFPreferencesCurrentUser, kCFPreferencesAnyHost );
	if (!cfarray) return 0;
	CFIndex size = CFArrayGetCount( cfarray );
	int index = 0;
	int id = 0;
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, CFSTR("device.") ) &&
			CFStringHasSuffix( key, CFSTR(".name") ) ) {
			
			if (index == intDeviceIndex) {			 
				CFArrayRef split = CFStringCreateArrayBySeparatingStrings( 0, key, CFSTR(".") );
				if ( !split || CFArrayGetCount( split ) != 3 ) continue;

				// This code crashes!
				//CFNumberRef cfid = (CFNumberRef) CFArrayGetValueAtIndex( split, 1 );
				//if (cfid)
				//	CFNumberGetValue( cfid, kCFNumberIntType, &id);

				CFStringRef cfid = (CFStringRef) CFArrayGetValueAtIndex( split, 1 );				
				char *cp = NULL;
				CFIndex size = CFStringGetMaximumSizeForEncoding( CFStringGetLength( cfid ), kCFStringEncodingUTF8) + 1;
				cp = (char *)malloc(size);
				CFStringGetCString( cfid, cp, size, kCFStringEncodingUTF8 );
				cp = (char *)realloc( cp, strlen(cp) + 1);
				id = atoi(cp);
				
			 
				CFRelease(key);
				CFRelease(split);
				CFRelease(cfid);	
				break;
			}
			index++;
		}
		CFRelease( key );
	}
	return id;
}

/*
* Add a new device
*/
int TelldusSettings::addDevice() {
	int id = getNextDeviceId();
	setStringSetting( id, "name", "", false ); //Create a empty name so the device has an entry
	setStringSetting( id, "model", "", false );
	return id;
}

/*
* Get next available device id
*/
int TelldusSettings::getNextDeviceId() {
	int id = 0, max = 0;
	int numberOfDevices = getNumberOfDevices();
	for( int i = 0; i < numberOfDevices; i++) {
		id = getDeviceId( i );
		if (id > max) {
			max = id;
		}
	}
	max++;
	return max;
}

/*
* Remove a device
*/
bool TelldusSettings::removeDevice(int intDeviceId){
	CFStringRef filterKey = CFStringCreateWithFormat(0, NULL, CFSTR("device.%d."), intDeviceId); // The key to search for

	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, kCFPreferencesCurrentUser, kCFPreferencesAnyHost );
	if (!cfarray) return 0;
	CFIndex size = CFArrayGetCount( cfarray );
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, filterKey ) ) {
			CFPreferencesSetAppValue( key, NULL, d->app_ID ); //Remove the key
		}
	}
	
	CFPreferencesAppSynchronize( d->app_ID );
	return true;
}

//only for debug reasons
void TelldusSettings::debugLog(char* debugstring){
}

//only for debug reasons
void TelldusSettings::debugLog(int debugint){
}

char *TelldusSettings::getStringSetting(int intDeviceId, const char* name, bool parameter) {
	CFStringRef cfname = CFStringCreateWithCString( 0, name, 0 );
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("device.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("device.%d.%@"), intDeviceId, cfname);		
	}
	
	CFStringRef value;
	
	value = (CFStringRef)CFPreferencesCopyAppValue(key, d->app_ID);
	if (!value) {
		return "";
	}
	
	char *cp = NULL;
	CFIndex size = CFStringGetMaximumSizeForEncoding( CFStringGetLength( value ), kCFStringEncodingUTF8) + 1;
	cp = (char *)malloc(size);
	CFStringGetCString( value, cp, size, kCFStringEncodingUTF8 );
	cp = (char *)realloc( cp, strlen(cp) + 1);
	
	CFRelease(value);
	return cp;
}

bool TelldusSettings::setStringSetting(int intDeviceId, const char* name, const char *value, bool parameter) {
	CFStringRef cfname = CFStringCreateWithCString( 0, name, 0 );
	CFStringRef cfvalue = CFStringCreateWithCString( 0, value, 0 );
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("device.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("device.%d.%@"), intDeviceId, cfname);		
	}

	CFPreferencesSetAppValue( key, cfvalue, d->app_ID );
	CFPreferencesAppSynchronize( d->app_ID );
	return true;
}

