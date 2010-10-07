IF(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-core_SRCS
	telldus-core.cpp
	)

SET( telldus-core_HDRS
)
SET( telldus-core_PUB_HDRS
	telldus-core.h
)


OPTION(USE_FTD2XX_RATHER_THEN_LIBFTDI "Link against ftd2xx even if libftdi is the preferred one" OFF)
IF (APPLE OR WIN32 OR USE_FTD2XX_RATHER_THEN_LIBFTDI)
	FIND_LIBRARY(FTD2XX_LIBRARY ftd2xx)
	SET( telldus-core_LIBRARIES
		${telldus-core_LIBRARIES}
		${FTD2XX_LIBRARY}
	)
	ADD_DEFINITIONS( -DLIBFTD2XX )
ELSE (APPLE OR WIN32 OR USE_FTD2XX_RATHER_THEN_LIBFTDI)
	FIND_LIBRARY(FTDI_LIBRARY ftdi)
	SET( telldus-core_LIBRARIES
		${telldus-core_LIBRARIES}
		${FTDI_LIBRARY}
	)
	ADD_DEFINITIONS( -DLIBFTDI )
ENDIF (APPLE OR WIN32 OR USE_FTD2XX_RATHER_THEN_LIBFTDI)


######## Configurable options for the platform  ########



######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-core_TARGET	TelldusCore )
	ADD_DEFINITIONS(
		-D_MACOSX
	)
ELSEIF (WIN32)
	#### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET( telldus-core_TARGET	TelldusCore )
	#IF (NOT CMAKE_CL_64)
		LIST(APPEND telldus-core_SRCS
			libtelldus-core.def
		)
	#ENDIF(NOT CMAKE_CL_64)
	ADD_DEFINITIONS(
		-D_WINDOWS
		-DTELLDUSCORE_EXPORTS
	)
ELSE (APPLE)
	#### Linux ####
	SET( telldus-core_TARGET	telldus-core )
	ADD_DEFINITIONS(
		-D_LINUX
	)
ENDIF (APPLE)



######## Configuring  ########

ADD_LIBRARY(${telldus-core_TARGET} SHARED
	${telldus-core_SRCS}
	${telldus-core_HDRS}
	${telldus-core_PUB_HDRS}
)

TARGET_LINK_LIBRARIES( ${telldus-core_TARGET}	${telldus-core_LIBRARIES} )

SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY FRAMEWORK TRUE	
)
SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY PUBLIC_HEADER ${telldus-core_PUB_HDRS}
)

SET_PROPERTY(TARGET ${telldus-core_TARGET}
	PROPERTY VERSION ${PACKAGE_VERSION}
)
SET_PROPERTY(TARGET ${telldus-core_TARGET}
  PROPERTY SOVERSION ${PACKAGE_SOVERSION}
)

IF (NOT LIBRARY_DIR)
	SET(LIBRARY_DIR "lib")
ENDIF (NOT LIBRARY_DIR)

IF (UNIX)
	INSTALL(TARGETS ${telldus-core_TARGET}
		LIBRARY         DESTINATION   ${LIBRARY_DIR}
		ARCHIVE         DESTINATION   ${LIBRARY_DIR}
		FRAMEWORK       DESTINATION   "/Library/Frameworks"
		PUBLIC_HEADER   DESTINATION   include
	)
ENDIF (UNIX)

IF (UNIX AND NOT APPLE)
	SET(SYSCONF_INSTALL_DIR "/etc" CACHE PATH "The sysconfig install dir (default prefix/etc)" )
	SET(STATE_INSTALL_DIR	"/var/state" CACHE PATH "The directory to store state information of the devices" )
	SET(UDEV_RULES_DIR	"/etc/udev/rules.d" CACHE PATH "The directory where udev store its rules" )
	
	INSTALL(FILES tellstick.conf
		DESTINATION ${SYSCONF_INSTALL_DIR}
	)
	INSTALL(FILES telldus-core.conf
		DESTINATION ${STATE_INSTALL_DIR}
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE
	)
	INSTALL(FILES 99-tellstick.rules
		DESTINATION ${UDEV_RULES_DIR}
	)
ENDIF (UNIX AND NOT APPLE)
