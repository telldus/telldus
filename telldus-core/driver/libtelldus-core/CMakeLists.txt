SET( telldus-core_SRCS
	Device.cpp
	DeviceIkea.cpp
	DeviceNexa.cpp
	DeviceSartano.cpp
	DeviceWaveman.cpp
	settings/TelldusSettings.cpp
	telldus-core.cpp
	)

SET( telldus-core_HDRS
	telldus-core.h
)
	
IF (APPLE)
	SET( telldus-core_TARGET	TelldusCore )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		win/Device.cpp
		settings/TelldusSettingsCoreFoundationPreferences.cpp
	)
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSE (APPLE)
	SET( telldus-core_TARGET	telldus-core )
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		linux/Device.cpp
		settings/TelldusSettingsConfuse.cpp
	)
	ADD_DEFINITIONS(
		-D_LINUX
	)

ENDIF (APPLE)

ADD_LIBRARY(${telldus-core_TARGET} SHARED
	${telldus-core_SRCS}
	${telldus-core_HDRS}
	)
	
SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY PUBLIC_HEADER {$telldus-core_HDRS}
)


IF (APPLE)
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
	FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)

	TARGET_LINK_LIBRARIES(${telldus-core_TARGET}
		${COREFOUNDATION_LIBRARY}
		${FTD2XX_LIBRARY}
	)
	SET_PROPERTY(TARGET	${telldus-core_TARGET}
		PROPERTY FRAMEWORK TRUE	
	)

ELSE (APPLE)
	TARGET_LINK_LIBRARIES(${telldus-core_TARGET}
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

