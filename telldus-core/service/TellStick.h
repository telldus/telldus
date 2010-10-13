//
// C++ Interface: TellStick
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLSTICK_H
#define TELLSTICK_H

#include "Controller.h"

#include "ftd2xx.h"
#ifdef LIBFTDI
	typedef ftdi_context TellStickHandle;
#else
	typedef FT_HANDLE TellStickHandle;
#endif

class TellStick : public Controller {
public:	
	virtual ~TellStick();
	
	virtual int firmwareVersion();
	virtual int send( const std::string &message );

	int vid() const;
	int pid() const;
	std::string serial() const;
	bool open() const;
	bool stillConnected() const;

	static TellStick *findFirstDevice(int vid = 0, int pid = 0);
	static TellStick *loadBy(int vid, int pid, const std::string &serial);
	
protected:
	class TellStickDescriptor;

	TellStick(const TellStickDescriptor &d);
	void setBaud( int baud );
	TellStickHandle handle() const;
	
private:
	static TellStickDescriptor findByVIDPID( int vid, int pid );
	
	class PrivateData;
	PrivateData *d;
};

#endif
