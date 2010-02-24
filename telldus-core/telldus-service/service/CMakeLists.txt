FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTNETWORK TRUE)
INCLUDE( ${QT_USE_FILE} )

IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-service_SRCS
	TelldusCore.cpp
	Manager.cpp
	../common/Message.cpp
)
SET( telldus-service_HDRS
)
SET( telldus-service_MOC_HDRS
	TelldusCore.h
	Manager.h
	MessageReceiver.h
	Pipe.h
)
	
SET( telldus-service_LIBRARIES
	${QT_LIBRARIES}
)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/driver
	${CMAKE_CURRENT_SOURCE_DIR}/../common
)


######## Configurable options for the platform  ########

######## Platforms-specific, non configurable  ########

SET( telldus-service_TARGET	TelldusService )

IF (APPLE) #### Mac OS X ####
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
	FIND_LIBRARY(IOKIT_LIBRARY IOKit)
	
	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		${COREFOUNDATION_LIBRARY}
		${IOKIT_LIBRARY}
		TelldusCoreLib
	)
	SET( telldus-service_SRCS
		${telldus-service_SRCS}
		main_unix.cpp
		MessageReceiver_mac.cpp
		MessageReceiverPrivate_mac.cpp
	)
	SET( telldus-service_MOC_HDRS
		${telldus-service_MOC_HDRS}
		MessageReceiverPrivate_mac.h
	)
	
ELSEIF (WIN32) #### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)
	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		${LIBRARY_OUTPUT_PATH}/Release/TelldusCoreLib.lib
	)
	SET( telldus-service_SRCS
		${telldus-service_SRCS}
		MessageReceiver_win.cpp
		Pipe_win.cpp
		Socket_win.cpp
		main_win.cpp
	)

ELSE (APPLE) #### Linux ####
	SET( telldus-service_SRCS
		${telldus-service_SRCS}
		MessageReceiver_unix.cpp
		main_unix.cpp
	)
	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		telldus-core-lib
	)
ENDIF (APPLE)

######## QtService ########

IF (WIN32)
	INCLUDE( ../../3rdparty/qtservice.cmake NO_POLICY_SCOPE )
	SET( telldus-service_SRCS
		${telldus-service_SRCS}
		${qtservice_SRCS}
	)
ENDIF(WIN32)

######## Configuring  ########

QT4_WRAP_CPP(  telldus-service_MOC_SRCS  ${telldus-service_MOC_HDRS} )

ADD_EXECUTABLE(${telldus-service_TARGET}
	${telldus-service_SRCS}
	${telldus-service_MOC_SRCS}
	${telldus-service_MOC_HDRS}
)
SET_SOURCE_FILES_PROPERTIES(${telldus-service_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

TARGET_LINK_LIBRARIES( ${telldus-service_TARGET}	${telldus-service_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
IF (APPLE)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
ELSEIF (UNIX)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/TelldusCenter
	)
ENDIF (APPLE)

