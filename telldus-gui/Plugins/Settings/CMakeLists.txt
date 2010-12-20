SET( Plugin_NAME "Settings" )


SET( Plugin_SRCS
	settings.cpp
	settingsplugin.cpp
)

SET( Plugin_HDRS
	settingsplugin.h
)

SET( Plugin_MOC_HDRS
	settings.h
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
