IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-common_SRCS
	Message.cpp
	Mutex.cpp
	Strings.cpp
	Thread.cpp
)

SET( telldus-common_HDRS
	common.h
	Message.h
	Mutex.h
	Socket.h
	Strings.h
	Thread.h
)

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	FIND_LIBRARY(ICONV_LIBRARY iconv)
	ADD_DEFINITIONS( -D_MACOSX )
	LIST(APPEND  telldus-common_SRCS
		Socket_unix.cpp
	)
	LIST(APPEND telldus-common_LIBRARIES
		${ICONV_LIBRARY}
	)
ELSEIF (WIN32)
	#### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	ADD_DEFINITIONS( -D_WINDOWS )
	LIST(APPEND  telldus-common_SRCS
		Socket_win.cpp
	)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	#### FreeBSD ####
	FIND_LIBRARY(ICONV_LIBRARY iconv)
	ADD_DEFINITIONS( -D_FREEBSD )
	LIST(APPEND  telldus-common_SRCS
		Socket_unix.cpp
	)
	LIST(APPEND telldus-common_LIBRARIES
		${ICONV_LIBRARY}
	)
ELSE (APPLE)
	#### Linux ####
	ADD_DEFINITIONS( -D_LINUX )
	LIST(APPEND  telldus-common_SRCS
		Socket_unix.cpp
	)
ENDIF (APPLE)


######## Configuring  ########

ADD_LIBRARY(TelldusCommon STATIC
	${telldus-common_SRCS}
	${telldus-common_HDRS}
)

IF (UNIX)
	SET_TARGET_PROPERTIES( TelldusCommon PROPERTIES COMPILE_FLAGS "-fPIC -fvisibility=hidden")
ENDIF (UNIX)

TARGET_LINK_LIBRARIES( TelldusCommon	${telldus-common_LIBRARIES} )
