IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-common_SRCS
	Message.cpp
	Mutex.cpp
	Thread.cpp
)

SET( telldus-common_HDRS
	Message.h
	Mutex.h
	Socket.h
	Thread.h
)

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-common_TARGET	TelldusCommon )
	ADD_DEFINITIONS( -D_MACOSX )
	LIST(APPEND  telldus-common_SRCS
		Socket_unix.cpp
	)
ELSEIF (WIN32)
	#### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET( telldus-common_TARGET	TelldusCommon )
	ADD_DEFINITIONS( -D_WINDOWS )
	LIST(APPEND  telldus-common_SRCS
		Socket_win.cpp
	)
ELSE (APPLE)
	#### Linux ####
	SET( telldus-common_TARGET	telldus-common )
	ADD_DEFINITIONS( -D_LINUX )
	LIST(APPEND  telldus-common_SRCS
		Socket_unix.cpp
	)
ENDIF (APPLE)


######## Configuring  ########


ADD_LIBRARY(${telldus-common_TARGET} STATIC
	${telldus-common_SRCS}
	${telldus-common_HDRS}
)

SET_TARGET_PROPERTIES( ${telldus-common_TARGET} PROPERTIES COMPILE_FLAGS -fPIC)

TARGET_LINK_LIBRARIES( ${telldus-common_TARGET}	${telldus-common_LIBRARIES} )
