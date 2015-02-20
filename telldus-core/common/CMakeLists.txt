IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-common_SRCS
	Event.cpp
	Message.cpp
	Mutex.cpp
	Strings.cpp
	Thread.cpp
)

SET( telldus-common_HDRS
	common.h
	Event.h
	EventHandler.h
	Message.h
	Mutex.h
	Socket.h
	Strings.h
	Thread.h
)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	FIND_LIBRARY(ICONV_LIBRARY iconv)
	ADD_DEFINITIONS( -D_MACOSX )
	LIST(APPEND  telldus-common_SRCS
		Event_unix.cpp
		EventHandler_unix.cpp
		Socket_unix.cpp
		stdlibc_workaround.cpp #Remove this when we drop support for 10.5
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
		Event_win.cpp
		EventHandler_win.cpp
		Socket_win.cpp
	)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	#### FreeBSD ####
	string(REGEX MATCH "(([0-9]+)\\.([0-9]+))-([A-Z0-9])+" FREEBSD "${CMAKE_SYSTEM_VERSION}")
	set( FREEBSD_RELEASE "${CMAKE_MATCH_1}" )
	ADD_DEFINITIONS( -D_FREEBSD )
	LIST(APPEND  telldus-common_SRCS
		Event_unix.cpp
		EventHandler_unix.cpp
		Socket_unix.cpp
	)

	# FreeBSD 10 has iconv built in to libc
	# However, if user has libiconv package installed, clang will find that header first,
	# and we'd get a link error. This fix will at least let it build; we cannot
	# force the user to NOT have libiconv installed.
	#
	# "proper" fix would be to force clang/gcc to use system header. How?
	IF( (FREEBSD_RELEASE LESS 10) OR (EXISTS "/usr/local/include/iconv.h"))
		IF(NOT (FREEBSD_RELEASE LESS 10))
			# Note that building the freebsd port with this may or may not fail,
			# as the port does not define a dependency on libiconv package.
			# When building with poudriere, this is not an issue since it will not be installed.
			MESSAGE(WARNING "building with libiconv from package instead of base")
		ENDIF()
		FIND_LIBRARY(ICONV_LIBRARY iconv)
		LIST(APPEND telldus-common_LIBRARIES
			${ICONV_LIBRARY}
		)
	ENDIF ()
ELSE (APPLE)
	#### Linux ####
	ADD_DEFINITIONS( -D_LINUX )
	LIST(APPEND  telldus-common_SRCS
		Event_unix.cpp
		EventHandler_unix.cpp
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
