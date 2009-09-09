SET( Plugin_NAME "SystrayIcon" )

SET( Plugin_SRCS
	systrayplugin.cpp
	systrayobject.cpp
)

SET( Plugin_HDRS
	systrayplugin.h
)

SET( Plugin_MOC_HDRS
	systrayobject.h
)

SET( Plugin_PATH "com.telldus.systray" )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
