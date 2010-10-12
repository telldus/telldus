IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-service_SRCS
	ClientCommunicationHandler.cpp
	Event.cpp
	Settings.cpp
	TelldusMain.cpp
	DeviceManager.cpp
	Device.cpp
	Protocol.cpp
	Controller.cpp
)
SET( telldus-service_HDRS
	ClientCommunicationHandler.h
	ConnectionListener.h
	Event.h
	EventHandler.h
	Settings.h
	TelldusMain.h
	DeviceManager.h
	Device.h
	Protocol.h
	Controller.h
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
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)
	LIST(APPEND telldus-service_LIBRARIES
		${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusCommon.lib
	)
	LIST(APPEND telldus-service_SRCS
		ConnectionListener_win.cpp
		Event_win.cpp
		EventHandler_win.cpp
		main_win.cpp
		SettingsWinRegistry.cpp
		TelldusWinService_win.cpp
	)
	LIST(APPEND telldus-service_HDRS
		TelldusWinService_win.h
	)

ELSE (APPLE) #### Linux ####
	FIND_LIBRARY(CONFUSE_LIBRARY confuse)
	ADD_DEFINITIONS( -D_CONFUSE )

	SET( telldus-service_TARGET	telldusd )
	LIST(APPEND telldus-service_SRCS
		ConnectionListener_unix.cpp
		Event_unix.cpp
		EventHandler_unix.cpp
		main_unix.cpp
		SettingsConfuse.cpp
	)
	LIST(APPEND telldus-service_LIBRARIES
		${CONFUSE_LIBRARY}
		telldus-common
	)
ENDIF (APPLE)

######## Configuring  ########


ADD_EXECUTABLE(${telldus-service_TARGET}
	${telldus-service_SRCS}
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

