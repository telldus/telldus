FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_DONT_USE_QTGUI TRUE )
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-core_SRCS
	Device.cpp
	DeviceGroup.cpp
	DeviceIkea.cpp
	DeviceNexa.cpp
	DeviceSartano.cpp
	DeviceWaveman.cpp
	Controller.cpp
	Manager.cpp
	Settings.cpp
	telldus-core.cpp
	)

SET( telldus-core_HDRS
	telldus-core.h
)

SET( telldus-core_MOC_HDRS
	)
	
QT4_WRAP_CPP(  telldus-core_MOC_SRCS  ${telldus-core_MOC_HDRS} )
QT4_AUTOMOC (  ${telldus-core_SRCS} )

FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)

SET( telldus-core_LIBRARIES
	${QT_LIBRARIES}
	${FTD2XX_LIBRARY}
)


######## Configurable options for the platform  ########

#### TellStickDuo ####
IF (SUPPORT_TELLSTICK_DUO)
	ADD_DEFINITIONS( -DTELLSTICK_DUO )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		TellStickDuo.cpp
	)
ENDIF(SUPPORT_TELLSTICK_DUO)

#### Settings backend ####
IF(USE_QT_SETTINGS_BACKEND)
		SET( telldus-core_SRCS
			${telldus-core_SRCS}
			SettingsQt.cpp
		)
ELSE (USE_QT_SETTINGS_BACKEND)
	IF (APPLE)
		FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
		
		SET( telldus-core_LIBRARIES
			${telldus-core_LIBRARIES}
			${COREFOUNDATION_LIBRARY}
		)
		SET( telldus-core_SRCS
			${telldus-core_SRCS}
			SettingsCoreFoundationPreferences.cpp
		)
	ELSE (APPLE)
		SET( telldus-core_LIBRARIES
			${telldus-core_LIBRARIES}
			confuse
		)
		SET( telldus-core_SRCS
			${telldus-core_SRCS}
			SettingsConfuse.cpp
		)
		ADD_DEFINITIONS( -D_CONFUSE )
	ENDIF (APPLE)
ENDIF(USE_QT_SETTINGS_BACKEND)



######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-core_TARGET	TelldusCore )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		win/Device.cpp
	)
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSE (APPLE)
	#### Linux ####
	SET( telldus-core_TARGET	telldus-core )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		linux/Device.cpp
	)
	ADD_DEFINITIONS(
		-D_LINUX
	)
ENDIF (APPLE)



######## Configuring  ########

ADD_LIBRARY(${telldus-core_TARGET} SHARED
	${telldus-core_SRCS}
	${telldus-core_HDRS}
	${telldus-core_MOC_SRCS}
)

TARGET_LINK_LIBRARIES( ${telldus-core_TARGET}	${telldus-core_LIBRARIES} )

SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY FRAMEWORK TRUE	
)
SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY PUBLIC_HEADER ${telldus-core_HDRS}
)

INSTALL(TARGETS ${telldus-core_TARGET}
	DESTINATION lib
#	PUBLIC_HEADER       DESTINATION         ${HEADER_DIR}
	)

SET(SYSCONF_INSTALL_DIR
	"/etc"
	CACHE PATH "The ${APPLICATION_NAME} sysconfig install dir (default prefix/etc)"
	FORCE
	)

IF (UNIX)
	INSTALL(FILES
		tellstick.conf
		DESTINATION ${SYSCONF_INSTALL_DIR}
	)
ENDIF (UNIX)
