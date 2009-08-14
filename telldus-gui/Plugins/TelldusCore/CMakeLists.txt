SET( Plugin_NAME "TelldusCorePlugin" )

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

FIND_LIBRARY( TELLDUSCORE_LIBRARY telldus-core )

SET( Plugin_LIBRARIES
	${TELLDUSCORE_LIBRARY}
)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
