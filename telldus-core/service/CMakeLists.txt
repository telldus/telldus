IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-service_SRCS
	ClientCommunicationHandler.cpp
	Controller.cpp
	ControllerManager.cpp
	Device.cpp
	DeviceManager.cpp
	Event.cpp
	Settings.cpp
	TelldusMain.cpp
)
SET( telldus-service_protocol_SRCS
	Protocol.h
	Protocol.cpp
	ProtocolBrateck.h
	ProtocolBrateck.cpp
	ProtocolEverflourish.h
	ProtocolEverflourish.cpp
	ProtocolFuhaote.h
	ProtocolFuhaote.cpp
	ProtocolIkea.h
	ProtocolIkea.cpp
	ProtocolNexa.h
	ProtocolNexa.cpp
	ProtocolRisingSun.h
	ProtocolRisingSun.cpp
)
SET( telldus-service_HDRS
	ClientCommunicationHandler.h
	ConnectionListener.h
	Controller.h
	ControllerManager.h
	Device.h
	DeviceManager.h
	Event.h
	EventHandler.h
	Settings.h
	TelldusMain.h
	TellStick.h
)
FIND_PACKAGE(Threads)
LIST(APPEND telldus-service_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../common
)

######## Configurable options for the platform  ########

######## Platforms-specific, non configurable  ########

IF (APPLE) #### Mac OS X ####
	SET( telldus-service_TARGET	TelldusService )
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
	FIND_LIBRARY(IOKIT_LIBRARY IOKit)
	
	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		${COREFOUNDATION_LIBRARY}
		${IOKIT_LIBRARY}
		TelldusCommon
	)
	LIST(APPEND telldus-service_SRCS
		main_unix.cpp
		Event_unix.cpp
		EventHandler_unix.cpp
		ConnectionListener_unix.cpp
	)
	
ELSEIF (WIN32) #### Windows ####
	SET( telldus-service_TARGET	TelldusService )
	FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)
	ADD_DEFINITIONS( -DUNICODE -DLIBFTD2XX )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)
	LIST(APPEND telldus-service_LIBRARIES
		${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusCommon.lib
		${FTD2XX_LIBRARY}
	)
	LIST(APPEND telldus-service_SRCS
		ConnectionListener_win.cpp
		Event_win.cpp
		EventHandler_win.cpp
		main_win.cpp
		SettingsWinRegistry.cpp
		TelldusWinService_win.cpp
		TellStick_ftd2xx.cpp
	)
	LIST(APPEND telldus-service_HDRS
		TelldusWinService_win.h
	)

ELSE (APPLE) #### Linux ####
	FIND_LIBRARY(CONFUSE_LIBRARY confuse)
	FIND_LIBRARY(FTDI_LIBRARY ftdi)
	ADD_DEFINITIONS( -D_CONFUSE -DLIBFTDI )

	SET( telldus-service_TARGET	telldusd )
	LIST(APPEND telldus-service_SRCS
		ConnectionListener_unix.cpp
		Event_unix.cpp
		EventHandler_unix.cpp
		main_unix.cpp
		SettingsConfuse.cpp
		TellStick_libftdi.cpp
	)
	LIST(APPEND telldus-service_LIBRARIES
		${CONFUSE_LIBRARY}
		${FTDI_LIBRARY}
		telldus-common
	)
ENDIF (APPLE)

######## Configuring  ########

SOURCE_GROUP("Protocol Files" FILES ${telldus-service_protocol_SRCS})

ADD_EXECUTABLE(${telldus-service_TARGET}
	${telldus-service_SRCS}
	${telldus-service_protocol_SRCS}
	${telldus-service_HDRS}
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
ENDIF (APPLE)

