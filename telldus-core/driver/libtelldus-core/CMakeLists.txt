
SET( telldus-core_SRCS
	Device.cpp
	DeviceIkea.cpp
	DeviceNexa.cpp
	DeviceSartano.cpp
	DeviceWaveman.cpp
	settings/TelldusSettings.cpp
	telldus-core.cpp
	)

IF (APPLE)
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		win/Device.cpp
		settings/TelldusSettingsCoreFoundationPreferences.cpp
	)
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSE (APPLE)
	SET( telldus-core_SRCS
		${telldus-core_SRCS}
		linux/Device.cpp
		settings/TelldusSettingsConfuse.cpp
	)
	ADD_DEFINITIONS(
		-D_LINUX
	)

ENDIF (APPLE)

ADD_LIBRARY(telldus-core SHARED
	${telldus-core_SRCS}
	)

IF (APPLE)
	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)

	TARGET_LINK_LIBRARIES(telldus-core
		${COREFOUNDATION_LIBRARY}
		ftd2xx
	)
ELSE (APPLE)
	TARGET_LINK_LIBRARIES(telldus-core
		confuse
	)
ENDIF (APPLE)

INSTALL(TARGETS telldus-core
	DESTINATION lib
	)

INSTALL(FILES
	telldus-core.h
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
ENDIF (UNIX)
