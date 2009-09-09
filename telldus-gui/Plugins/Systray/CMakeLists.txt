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

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
