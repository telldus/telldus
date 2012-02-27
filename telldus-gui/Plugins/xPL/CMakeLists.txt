SET(QT_USE_QTNETWORK TRUE)

SET( Plugin_NAME "xPLPlugin" )
SET( Plugin_PATH "com.telldus.xpl" )

SET( Plugin_SRCS
	xplplugin.cpp
)

SET( Plugin_HDRS
	xplplugin.h
)

FIND_LIBRARY( QTXPL_LIBRARY qtxpl )

SET(QTXPL_INCLUDE_DIR	""	CACHE PATH "Path to the directory which contains header files for qtxpl")

INCLUDE_DIRECTORIES(
	${QTXPL_INCLUDE_DIR}
)

SET( Plugin_LIBRARIES
	${QTXPL_LIBRARY}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
