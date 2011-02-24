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
#include "Thread.h"
#include <list>

class TellStickDescriptor {
public:
	std::string serial;
	int vid, pid;
};

class TellStick : public Controller, public TelldusCore::Thread {
public:
	TellStick(int controllerId, Event *event, const TellStickDescriptor &d);
	virtual ~TellStick();

	virtual int firmwareVersion();
	virtual int pid() const;
	bool isOpen() const;
	bool isSameAsDescriptor(const TellStickDescriptor &d) const;
	virtual int send( const std::string &message );
	bool stillConnected() const;

	static std::list<TellStickDescriptor> findAll();

protected:
	void processData( const std::string &data );
	void run();
	void setBaud( int baud );
	void stop();

private:
	static std::list<TellStickDescriptor> findAllByVIDPID( int vid, int pid );
	static std::string convertSToT(  unsigned char t0, unsigned char t1, unsigned char t2, unsigned char t3, const std::string &data );

	class PrivateData;
	PrivateData *d;
};

#endif
