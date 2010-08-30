SET( Plugin_NAME "Devices" )

SET( Plugin_SRCS
	devicesplugin.cpp
)

SET( Plugin_HDRS
)

SET( Plugin_MOC_HDRS
	devicesplugin.h
)

SET( Plugin_PATH "com.telldus.gui" )

FIND_LIBRARY( TELLDUSGUI_LIBRARY telldus-gui )

FIND_PACKAGE(TelldusCore REQUIRED)

SET( Plugin_LIBRARIES
	${TELLDUSGUI_LIBRARY}
	${TELLDUSCORE_LIBRARY}
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
