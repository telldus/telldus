PROJECT( telldus-gui )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.0 )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(PACKAGE_VERSION 2)
SET(PACKAGE_SOVERSION 2.0.0)

SET(UPDATE_TRANSLATIONS FALSE CACHE BOOL "Update source translation files (WARNING: make clean will delete the source .ts files! Danger!)")
SET(LANGUAGES sv)

SET(BUILD_LIBTELLDUS-GUI	TRUE	CACHE BOOL "Build libtelldus-gui")
SET(BUILD_TELLDUS-CENTER TRUE CACHE BOOL "Build TelldusCenter")

IF (WIN32)
	SET(TELLDUSCORE_INCLUDE_DIR	""	CACHE PATH "Path to the directory which contains telldus-core.h")
	SET(TELLDUSCORE_LIBRARY	""	CACHE FILEPATH "Path to TelldusCore.lib")
ENDIF (WIN32)

if(BUILD_LIBTELLDUS-GUI)
	ADD_SUBDIRECTORY(TelldusGui)
endif(BUILD_LIBTELLDUS-GUI)

IF (BUILD_TELLDUS-CENTER)
	ADD_SUBDIRECTORY(TelldusCenter)
	ADD_SUBDIRECTORY(Plugins)
ENDIF (BUILD_TELLDUS-CENTER)

