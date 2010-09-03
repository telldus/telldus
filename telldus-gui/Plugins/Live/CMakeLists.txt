SET(QT_USE_QTNETWORK TRUE)

SET( Plugin_NAME "TellStickLive" )
SET( Plugin_PATH "com.telldus.live" )

SET( Plugin_SRCS
	livemessage.cpp
	liveobject.cpp
	liveplugin.cpp
)

SET( Plugin_HDRS
	liveplugin.h
)

SET( Plugin_MOC_HDRS
	livemessage.h
	liveobject.h
)

SET(TELLSTICK_LIVE_PUBLIC_KEY	""	CACHE STRING "TellStick Live public key")
SET(TELLSTICK_LIVE_PRIVATE_KEY	""	CACHE STRING "TellStick Live private key")

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
