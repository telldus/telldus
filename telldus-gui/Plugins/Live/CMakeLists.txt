SET(QT_USE_QTNETWORK TRUE)

SET( Plugin_NAME "TelldusLive" )
SET( Plugin_PATH "com.telldus.live" )

SET( Plugin_SRCS
	livemessage.cpp
	liveobject.cpp
	liveplugin.cpp
)

SET( Plugin_HDRS
	liveplugin.h
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/parsed/config.h
)

SET( Plugin_MOC_HDRS
	livemessage.h
	liveobject.h
)

CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/parsed/config.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/parsed/)

SET(TELLDUS_LIVE_PUBLIC_KEY	""	CACHE STRING "Telldus Live! public key")
SET(TELLDUS_LIVE_PRIVATE_KEY	""	CACHE STRING "Telldus Live! private key")
SET(TELLDUS_LIVE_HOST	"live.telldus.se"	CACHE STRING "Telldus Live! Connection Host")
SET(TELLDUS_LIVE_PORT	"50000"	CACHE STRING "Telldus Live! Connection Host")

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
