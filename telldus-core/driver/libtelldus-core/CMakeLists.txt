FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_DONT_USE_QTGUI TRUE )
INCLUDE( ${QT_USE_FILE} )

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
	TellStickDuo.cpp
	)

SET( telldus-core_HDRS
	telldus-core.h
)

SET( telldus-core_MOC_HDRS
	)
	
QT4_WRAP_CPP(  telldus-core_MOC_SRCS  ${telldus-core_MOC_HDRS} )
QT4_AUTOMOC (  ${telldus-core_SRCS}                     )
	
IF (APPLE)
	SET( telldus-core_TARGET	TelldusCore )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		win/Device.cpp
#		SettingsCoreFoundationPreferences.cpp
		SettingsQt.cpp
	)
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSE (APPLE)
	SET( telldus-core_TARGET	telldus-core )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		linux/Device.cpp
		SettingsConfuse.cpp
	)
	ADD_DEFINITIONS(
		-D_LINUX
	)

ENDIF (APPLE)

ADD_LIBRARY(${telldus-core_TARGET} SHARED
	${telldus-core_SRCS}
	${telldus-core_HDRS}
	${telldus-core_MOC_SRCS}
	)
	
SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY PUBLIC_HEADER {$telldus-core_HDRS}
)

FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)

IF (APPLE)
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)

	TARGET_LINK_LIBRARIES(${telldus-core_TARGET}
		${COREFOUNDATION_LIBRARY}
		${QT_LIBRARIES}
		${FTD2XX_LIBRARY}
	)
	SET_PROPERTY(TARGET	${telldus-core_TARGET}
		PROPERTY FRAMEWORK TRUE	
	)

ELSE (APPLE)
	TARGET_LINK_LIBRARIES(${telldus-core_TARGET}
		${QT_LIBRARIES}
		${FTD2XX_LIBRARY}
		confuse
	)
ENDIF (APPLE)

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

