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

#include "CallbackDispatcher.h"
#include "Thread.h"
#include "Event.h"
#include "EventHandler.h"

namespace TelldusCore {

	class CallbackMainDispatcher  : public Thread
	{
	public:
		CallbackMainDispatcher(void);
		~CallbackMainDispatcher(void);

		EventRef retrieveCallbackEvent();

		int registerCallback( TelldusCore::CallbackStruct::CallbackType type, void *eventFunction, void *context );
		bool unregisterCallback( int callbackId );

	protected:
		void run();

	private:
		class PrivateData;
		PrivateData *d;
		void cleanupCallbacks(void);
	};
}

#endif //CALLBACKMAINDISPATCHER_H
