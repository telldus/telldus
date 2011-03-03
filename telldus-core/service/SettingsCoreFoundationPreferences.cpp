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
#include "Settings.h"
#include "Strings.h"
#include <stdlib.h>
#include <string.h>
#include <CoreFoundation/CoreFoundation.h>

#include "../client/telldus-core.h"

class privateVars {
public:
	CFStringRef app_ID;
	CFStringRef userName;
	CFStringRef hostName;
};

class Settings::PrivateData {
public:
	CFStringRef app_ID;
	CFStringRef userName;
	CFStringRef hostName;
};
	
/*
* Constructor
*/
Settings::Settings(void)
{
	d = new PrivateData();
	d->app_ID = CFSTR( "com.telldus.core" );
	d->userName = kCFPreferencesAnyUser;
	d->hostName = kCFPreferencesCurrentHost;
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	delete d;
}

/*
* Return a setting
*/
std::wstring Settings::getSetting(const std::wstring &strName) const {
	return L"";
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void) const {
	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, d->userName, d->hostName );
	if (!cfarray) return 0;
	CFIndex size = CFArrayGetCount( cfarray );
	int devices = 0;
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, CFSTR("devices.") ) &&
			CFStringHasSuffix( key, CFSTR(".name") ) ) {
			devices++;
		}
	}	
	return devices;
}

int Settings::getDeviceId(int intDeviceIndex) const {
	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, d->userName, d->hostName );
	if (!cfarray) return 0;
	CFIndex size = CFArrayGetCount( cfarray );
	int index = 0;
	int id = 0;
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, CFSTR("devices.") ) &&
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
int Settings::addDevice() {
	int id = getNextDeviceId();
	setStringSetting( id, L"name", L"", false ); //Create a empty name so the device has an entry
	setStringSetting( id, L"model", L"", false );
	return id;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId() const {
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
int Settings::removeDevice(int intDeviceId){
	int ret = TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	CFStringRef filterKey = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d."), intDeviceId); // The key to search for

	CFArrayRef cfarray = CFPreferencesCopyKeyList( d->app_ID, d->userName, d->hostName );
	if (!cfarray) {
		return TELLSTICK_ERROR_UNKNOWN;
	}
	CFIndex size = CFArrayGetCount( cfarray );
	for (CFIndex k = 0; k < size; ++k) {
		CFStringRef key = (CFStringRef) CFArrayGetValueAtIndex(cfarray, k);
		if (CFStringHasPrefix( key, filterKey ) ) {
			CFPreferencesSetValue( key, NULL, d->app_ID, d->userName, d->hostName ); //Remove the key
			ret = TELLSTICK_SUCCESS;
		}
	}
	
	CFPreferencesSynchronize( d->app_ID, d->userName, d->hostName );
	return ret;
}

std::wstring Settings::getStringSetting(int intDeviceId, const std::wstring &wname, bool parameter) const {
	std::string name(TelldusCore::wideToString(wname));
	CFStringRef cfname = CFStringCreateWithCString( 0, name.c_str(), kCFStringEncodingUTF8 );
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.%@"), intDeviceId, cfname);		
	}
	
	CFStringRef value;
	
	value = (CFStringRef)CFPreferencesCopyValue(key, d->app_ID, d->userName, d->hostName);
	if (!value) {
		return L"";
	}
	
	char *cp = NULL;
	CFIndex size = CFStringGetMaximumSizeForEncoding( CFStringGetLength( value ), kCFStringEncodingUTF8) + 1;
	cp = (char *)malloc(size);
	CFStringGetCString( value, cp, size, kCFStringEncodingUTF8 );
	cp = (char *)realloc( cp, strlen(cp) + 1);
	
	CFRelease(value);
	std::wstring retval = TelldusCore::charToWstring(cp);
	free(cp);
	return retval;
}

int Settings::setStringSetting(int intDeviceId, const std::wstring &wname, const std::wstring &wvalue, bool parameter) {
	std::string name(TelldusCore::wideToString(wname));
	std::string value(TelldusCore::wideToString(wvalue));
	CFStringRef cfname = CFStringCreateWithCString( 0, name.c_str(), kCFStringEncodingUTF8 );
	CFStringRef cfvalue = CFStringCreateWithCString( 0, value.c_str(), kCFStringEncodingUTF8 );
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.%@"), intDeviceId, cfname);		
	}

	CFPreferencesSetValue( key, cfvalue, d->app_ID, d->userName, d->hostName );
	CFPreferencesSynchronize( d->app_ID, d->userName, d->hostName );
	return TELLSTICK_SUCCESS;
}

int Settings::getIntSetting(int intDeviceId, const std::wstring &wname, bool parameter) const {
	int retval = 0;
	std::string name(TelldusCore::wideToString(wname));
	CFStringRef cfname = CFStringCreateWithCString( 0, name.c_str(), kCFStringEncodingUTF8 );
	CFNumberRef cfvalue;
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.%@"), intDeviceId, cfname);		
	}
	
	cfvalue = (CFNumberRef)CFPreferencesCopyValue(key, d->app_ID, d->userName, d->hostName);

	// If the preference exists, use it.
	if (cfvalue) {
		try {
			// Numbers come out of preferences as CFNumber objects.
			if (!CFNumberGetValue(cfvalue, kCFNumberIntType, &retval)) {
				retval = 0;
			}
			CFRelease(cfvalue);
		} catch (std::exception e) {
			retval = 0;
		}
	}
	
	return retval;
}

int Settings::setIntSetting(int intDeviceId, const std::wstring &wname, int value, bool parameter) {
	std::string name(TelldusCore::wideToString(wname));
	CFStringRef cfname = CFStringCreateWithCString( 0, name.c_str(), kCFStringEncodingUTF8 );
	CFNumberRef cfvalue = CFNumberCreate(NULL, kCFNumberIntType, &value);
	
	CFStringRef key;
	if (parameter) {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.parameters.%@"), intDeviceId, cfname);
	} else {
		key = CFStringCreateWithFormat(0, NULL, CFSTR("devices.%d.%@"), intDeviceId, cfname);		
	}
	
	CFPreferencesSetValue( key, cfvalue, d->app_ID, d->userName, d->hostName );
	CFPreferencesSynchronize( d->app_ID, d->userName, d->hostName );
	return TELLSTICK_SUCCESS;	
}
