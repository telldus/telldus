SET(QT_USE_QTNETWORK TRUE)

SET( Plugin_NAME "xPLPlugin" )

SET( Plugin_SRCS
	xplplugin.cpp
)

SET( Plugin_HDRS
	xplplugin.h
)

FIND_LIBRARY( QTXPL_LIBRARY qtxpl )

SET( Plugin_LIBRARIES
	${QTXPL_LIBRARY}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
