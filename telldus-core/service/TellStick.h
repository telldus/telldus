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
#include <list>

class TellStickDescriptor {
public:
	std::string serial;
	int vid, pid;
};

class TellStick : public Controller {
public:	
	TellStick(const TellStickDescriptor &d);
	virtual ~TellStick();
	
	virtual int firmwareVersion();
	bool isOpen() const;
	bool isSameAsDescriptor(const TellStickDescriptor &d) const;
	virtual int send( const std::string &message );
	
	static std::list<TellStickDescriptor> findAll();
	
protected:
	void setBaud( int baud );


private:
	static std::list<TellStickDescriptor> findAllByVIDPID( int vid, int pid );
	
	class PrivateData;
	PrivateData *d;
};

#endif
