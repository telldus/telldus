/*
 *  CallbackMainDispatcher.h
 *  telldus-core
 *
 *  Created by Stefan Persson on 2012-02-23.
 *  Copyright 2012 Telldus Technologies AB. All rights reserved.
 *
 */

#ifndef CALLBACKMAINDISPATCHER_H
#define CALLBACKMAINDISPATCHER_H

#include "client/CallbackDispatcher.h"
#include "common/Thread.h"

namespace TelldusCore {

	class CallbackMainDispatcher
	{
	public:
		CallbackMainDispatcher(void);
		~CallbackMainDispatcher(void);

		void execute(TelldusCore::CallbackStruct::CallbackType type, EventData *eventData);

		int registerCallback( TelldusCore::CallbackStruct::CallbackType type, void *eventFunction, void *context );
		int unregisterCallback( int callbackId );

	private:
		class PrivateData;
		PrivateData *d;
	};
}

#endif //CALLBACKMAINDISPATCHER_H
