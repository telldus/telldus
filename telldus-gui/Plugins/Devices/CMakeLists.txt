SET(QT_USE_QTSCRIPT TRUE)

SET( Plugin_NAME "Devices" )

SET( Plugin_SRCS
	devicesplugin.cpp
)

SET( Plugin_HDRS
)

SET( Plugin_MOC_HDRS
	devicesplugin.h
)


FIND_LIBRARY( TELLDUSGUI_LIBRARY telldus-gui )

SET( Plugin_LIBRARIES
	${TELLDUSGUI_LIBRARY}
)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
