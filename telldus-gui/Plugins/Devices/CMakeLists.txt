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

SET( Plugin_LIBRARIES
	${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusGui.lib 
)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
