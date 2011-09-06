SET( Plugin_NAME "TelldusCorePlugin" )
SET( Plugin_PATH "com.telldus.core" )

SET( Plugin_SRCS
	tellduscoreplugin.cpp
	tellduscoreobject.cpp
)

SET( Plugin_HDRS
	tellduscoreplugin.h
)

SET( Plugin_MOC_HDRS
	tellduscoreobject.h
)

SET( Plugin_EXTRA
	Device.js
	DeviceCommands.js
)

FIND_PACKAGE(TelldusCore REQUIRED)
SET( Plugin_LIBRARIES ${TELLDUSCORE_LIBRARY} )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
