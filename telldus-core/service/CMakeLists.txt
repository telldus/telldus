IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET (telldusd_DESCRIPTION
	"background service for Telldus TellStick, must be running to control TellStick"
)

ADD_DEFINITIONS( -DVERSION="${DISPLAYED_VERSION}" )

######## Non configurable options  ########
SET( telldus-service_SRCS
	ClientCommunicationHandler.cpp
	Controller.cpp
	ControllerManager.cpp
	ControllerMessage.cpp
	Device.cpp
	DeviceManager.cpp
	Event.cpp
	Sensor.cpp
	Settings.cpp
	TelldusMain.cpp
	TellStick.cpp
	EventUpdateManager.cpp
)
SET( telldus-service_protocol_SRCS
	Protocol.h
	Protocol.cpp
	ProtocolBrateck.h
	ProtocolBrateck.cpp
	ProtocolComen.h
	ProtocolComen.cpp
	ProtocolEverflourish.h
	ProtocolEverflourish.cpp
	ProtocolFuhaote.h
	ProtocolFuhaote.cpp
	ProtocolGroup.h
	ProtocolGroup.cpp
	ProtocolHasta.h
	ProtocolHasta.cpp
	ProtocolIkea.h
	ProtocolIkea.cpp
	ProtocolNexa.h
	ProtocolNexa.cpp
	ProtocolRisingSun.h
	ProtocolRisingSun.cpp
	ProtocolSartano.h
	ProtocolSartano.cpp
	ProtocolScene.h
	ProtocolScene.cpp
	ProtocolSilvanChip.h
	ProtocolSilvanChip.cpp
	ProtocolUpm.h
	ProtocolUpm.cpp
	ProtocolWaveman.h
	ProtocolWaveman.cpp
	ProtocolX10.h
	ProtocolX10.cpp
	ProtocolYidong.h
	ProtocolYidong.cpp
)
SET( telldus-service_HDRS
	ClientCommunicationHandler.h
	ConnectionListener.h
	Controller.h
	ControllerListener.h
	ControllerManager.h
	ControllerMessage.h
	Device.h
	DeviceManager.h
	Event.h
	EventHandler.h
	Sensor.h
	Settings.h
	TelldusMain.h
	TellStick.h
	EventUpdateManager.h
)
FIND_PACKAGE(Threads REQUIRED)
LIST(APPEND telldus-service_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../common
)

######## Configurable options for the platform  ########

######## Platforms-specific, non configurable  ########

IF (APPLE) #### Mac OS X ####
	SET(DEFAULT_FTDI_ENGINE "ftd2xx")
	SET( telldus-service_TARGET	TelldusService )
	ADD_DEFINITIONS( -D_MACOSX )
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
	FIND_LIBRARY(IOKIT_LIBRARY IOKit)

	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		${COREFOUNDATION_LIBRARY}
		${IOKIT_LIBRARY}
		TelldusCommon
	)
	LIST(APPEND telldus-service_SRCS
		main_mac.cpp
		ConnectionListener_unix.cpp
		ControllerListener_mac.cpp
		Event_unix.cpp
		EventHandler_unix.cpp
		SettingsCoreFoundationPreferences.cpp
	)

ELSEIF (WIN32) #### Windows ####
	SET(DEFAULT_FTDI_ENGINE "ftd2xx")
	SET( telldus-service_TARGET	TelldusService )
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)
	LIST(APPEND telldus-service_LIBRARIES
		TelldusCommon
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
	SET(DEFAULT_FTDI_ENGINE "libftdi")
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
		TelldusCommon
	)
ENDIF (APPLE)

SET(FTDI_ENGINE ${DEFAULT_FTDI_ENGINE} CACHE STRING "Which FTDI engine to use. This could be either 'libftdi' or 'ftd2xx'")
IF (FTDI_ENGINE STREQUAL "ftd2xx")
	FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)
	ADD_DEFINITIONS( -DLIBFTD2XX )
	LIST(APPEND telldus-service_SRCS TellStick_ftd2xx.cpp )
	LIST(APPEND telldus-service_LIBRARIES ${FTD2XX_LIBRARY})
ELSE (FTDI_ENGINE STREQUAL "ftd2xx")
	FIND_LIBRARY(FTDI_LIBRARY ftdi)
	ADD_DEFINITIONS( -DLIBFTDI )
	LIST(APPEND telldus-service_SRCS TellStick_libftdi.cpp )
	LIST(APPEND telldus-service_LIBRARIES ${FTDI_LIBRARY})
ENDIF (FTDI_ENGINE STREQUAL "ftd2xx")

######## Configuring  ########

SOURCE_GROUP("Protocol Files" FILES ${telldus-service_protocol_SRCS})

ADD_EXECUTABLE(${telldus-service_TARGET}
	${telldus-service_SRCS}
	${telldus-service_protocol_SRCS}
	${telldus-service_HDRS}
)
ADD_DEPENDENCIES(${telldus-service_TARGET} TelldusCommon)
SET_SOURCE_FILES_PROPERTIES(${telldus-service_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

TARGET_LINK_LIBRARIES( ${telldus-service_TARGET}	${telldus-service_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
IF (APPLE)
	SET(TELLDUS_SERVICE_TARGET_PATH "/Library/Telldus" CACHE STRING "Path to install TelldusService")
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	INSTALL(TARGETS ${telldus-service_TARGET}
		RUNTIME DESTINATION ${TELLDUS_SERVICE_TARGET_PATH}
	)
	INSTALL(CODE "
		INCLUDE(GetPrerequisites)
		GET_FILENAME_COMPONENT(DESTDIR \$ENV{DESTDIR} ABSOLUTE)
		GET_PREREQUISITES(\"${TELLDUS_SERVICE_TARGET_PATH}/${telldus-service_TARGET}\" prereqs 1 0 \"\$\" \"\$\")
		FOREACH(pr \${prereqs})
			GET_FILENAME_COMPONENT(lib \${pr} NAME)
			FILE(INSTALL \${pr} DESTINATION ${TELLDUS_SERVICE_TARGET_PATH})
			EXECUTE_PROCESS(COMMAND install_name_tool
				-change \"\${pr}\" \"${TELLDUS_SERVICE_TARGET_PATH}/\${lib}\" \"\${DESTDIR}/${TELLDUS_SERVICE_TARGET_PATH}/${telldus-service_TARGET}\"
			)
		ENDFOREACH ()
	")
	INSTALL(FILES com.telldus.service.plist DESTINATION /Library/LaunchDaemons)
ENDIF (APPLE)

IF (UNIX)
	IF (GENERATE_MAN)
		ADD_CUSTOM_COMMAND(
			TARGET ${telldus-service_TARGET}
			POST_BUILD
			COMMAND help2man -n ${telldusd_DESCRIPTION} ./telldusd > telldusd.1
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating man file telldusd.1"
		)
		INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/telldusd.1 DESTINATION share/man/man1)
	ENDIF (GENERATE_MAN)
ENDIF (UNIX)

IF (UNIX AND NOT APPLE)
	INSTALL(TARGETS ${telldus-service_TARGET} RUNTIME DESTINATION sbin)
	SET(SYSCONF_INSTALL_DIR "/etc" CACHE PATH "The sysconfig install dir (default prefix/etc)" )
	SET(STATE_INSTALL_DIR	"/var/state" CACHE PATH "The directory to store state information of the devices" )

	INSTALL(FILES tellstick.conf
		DESTINATION ${SYSCONF_INSTALL_DIR}
	)
	INSTALL(FILES telldus-core.conf
		DESTINATION ${STATE_INSTALL_DIR}
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE
	)
ENDIF (UNIX AND NOT APPLE)
