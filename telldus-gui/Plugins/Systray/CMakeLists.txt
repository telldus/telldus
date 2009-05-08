SET(QT_USE_QTSCRIPT TRUE)

SET( Plugin_NAME "SystrayIcon" )

SET( Plugin_SRCS
	systrayicon.cpp
	systrayplugin.cpp
	systrayobject.cpp
)

SET( Plugin_HDRS
	systrayplugin.h
)

SET( Plugin_MOC_HDRS
	systrayicon.h
	systrayobject.h
)


FIND_LIBRARY( TELLDUSCORE_LIBRARY telldus-core )

SET( Plugin_LIBRARIES
	${TELLDUSCORE_LIBRARY}
)

INCLUDE_DIRECTORIES(
	${TELLDUSCORE_INCLUDE_DIR}
)


INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
