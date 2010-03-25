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

SET( Plugin_LIBRARIES
	${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusCore.lib 
)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )