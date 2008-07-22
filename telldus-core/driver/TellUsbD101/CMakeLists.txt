
SET( tellusbd101_SRCS
	Device.cpp
	DeviceIkea.cpp
	DeviceNexa.cpp
	DeviceSartano.cpp
	DeviceWaveman.cpp
	settings/TelldusSettings.cpp
	TellUsbD101.cpp
	)

IF (APPLE)
	SET( tellusbd101_SRCS
		${tellusbd101_SRCS}
		win/Device.cpp
		settings/TelldusSettingsCoreFoundationPreferences.cpp
	)
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSE ()
	SET( tellusbd101_SRC
		${tellusbd101_SRCS}
		linux/Device.cpp
		settings/TelldusSettingsConfuse.cpp
	)
	ADD_DEFINITIONS(
		-D_LINUX
	)

ENDIF ()

ADD_LIBRARY(tellusbd101 SHARED
	${tellusbd101_SRCS}
	)

IF (APPLE)
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)

	TARGET_LINK_LIBRARIES(tellusbd101
		${COREFOUNDATION_LIBRARY}
		ftd2xx
	)
ENDIF (APPLE)

INSTALL(TARGETS tellusbd101 LIBRARY
	DESTINATION lib
	)

INSTALL(FILES
	TellUsbD101.h
	DESTINATION include
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
ENDIF ()