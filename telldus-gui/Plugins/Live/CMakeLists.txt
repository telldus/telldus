SET(QT_USE_QTNETWORK TRUE)

SET( Plugin_NAME "TelldusLive" )
SET( Plugin_PATH "com.telldus.live" )

SET( Plugin_SRCS
	LiveMessage.cpp
	LiveMessageToken.cpp
	LiveObject.cpp
	LivePlugin.cpp
)

SET( Plugin_HDRS
	LivePlugin.h
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/parsed/config.h
)

SET( Plugin_MOC_HDRS
	LiveMessageToken.h
	LiveMessage.h
	LiveObject.h
)

SET( Plugin_EXTRA
	configuration.ui
	icon.png
)

CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/parsed/config.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/parsed/)

FIND_PACKAGE(QCA REQUIRED)
SET( Plugin_LIBRARIES ${QCA_LIBRARY} )

SET(TELLDUS_LIVE_PUBLIC_KEY	""	CACHE STRING "Telldus Live! public key")
SET(TELLDUS_LIVE_PRIVATE_KEY	""	CACHE STRING "Telldus Live! private key")
SET(TELLDUS_LIVE_URI	"https://api.telldus.com/server/assign"	CACHE STRING "Telldus Live! Connection URI")

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
