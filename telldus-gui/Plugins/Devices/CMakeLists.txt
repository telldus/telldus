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

IF (WIN32)
	SET( Plugin_LIBRARIES
		${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusGui.lib
	)
ELSEIF (APPLE)
	FIND_PACKAGE(TelldusCore REQUIRED)
	SET( Plugin_LIBRARIES
		${TELLDUSCORE_LIBRARY}
		TelldusGui
	)

ELSE (WIN32)
	FIND_PACKAGE(TelldusCore REQUIRED)

	SET( Plugin_LIBRARIES
		${TELLDUSGUI_LIBRARY}
		${CMAKE_BINARY_DIR}/TelldusGui/libtelldus-gui.so
	)
ENDIF (WIN32)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
