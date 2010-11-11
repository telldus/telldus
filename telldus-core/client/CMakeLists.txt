IF(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-core_SRCS
	CallbackDispatcher.cpp
	Client.cpp
	telldus-core.cpp
)

SET( telldus-core_HDRS
	CallbackDispatcher.h
	Client.h
)
SET( telldus-core_PUB_HDRS
	telldus-core.h
)

FIND_PACKAGE(Threads)
LIST(APPEND telldus-core_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../common
)

######## Configurable options for the platform  ########



######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-core_TARGET	TelldusCore )
	ADD_DEFINITIONS(
		-D_MACOSX
	)
	LIST(APPEND telldus-core_LIBRARIES
		TelldusCommon
	)
ELSEIF (WIN32)
	#### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	ADD_DEFINITIONS( /Zc:wchar_t- ) # Treat wchar_t as Built-in Type' = No
	SET( telldus-core_TARGET	TelldusCore )
	LIST(APPEND telldus-core_LIBRARIES
		${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/TelldusCommon.lib
	)
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/telldus-core.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/telldus-core.rc
	)
	LIST(APPEND telldus-core_SRCS
			libtelldus-core.def
			${CMAKE_CURRENT_SOURCE_DIR}/telldus-core.rc.in
			${CMAKE_CURRENT_BINARY_DIR}/telldus-core.rc
	)
	ADD_DEFINITIONS(
		-D_WINDOWS
		-DTELLDUSCORE_EXPORTS
	)
	IF (CMAKE_CL_64)
		ADD_DEFINITIONS(-D_CL64)
	ENDIF(CMAKE_CL_64)
ELSE (APPLE)
	#### Linux ####
	SET( telldus-core_TARGET	telldus-core )
	LIST(APPEND telldus-core_LIBRARIES
		telldus-common
	)

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

