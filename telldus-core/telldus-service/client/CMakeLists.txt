FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-client_SRCS
	main.cpp
	Manager.cpp
	../common/Message.cpp
	${CMAKE_SOURCE_DIR}/driver/libtelldus-core/Thread.cpp
)

SET( telldus-client_HDRS
	${CMAKE_SOURCE_DIR}/driver/libtelldus-core/telldus-core.h
	${CMAKE_SOURCE_DIR}/driver/libtelldus-core/Thread.h
)

SET( telldus-client_MOC_HDRS
#	Manager.h
	../common/Socket.h
)
	
SET( telldus-client_LIBRARIES
	${QT_LIBRARIES}
)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/driver
	${CMAKE_CURRENT_SOURCE_DIR}/../common
)

QT4_WRAP_CPP(  telldus-client_MOC_SRCS  ${telldus-client_MOC_HDRS} )
QT4_AUTOMOC (  ${telldus-client_SRCS} )

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########

IF (APPLE) #### Mac OS X ####
	SET( telldus-client_TARGET	TelldusCore )

ELSEIF (WIN32) #### Windows ####
	SET( telldus-client_TARGET	TelldusCore )

	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/telldus-core.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/telldus-core.rc
	)

	SET( telldus-client_SRCS
		${telldus-client_SRCS}
		../../driver/libtelldus-core/libtelldus-core.def
		../common/Socket_win.cpp
		${CMAKE_CURRENT_BINARY_DIR}/telldus-core.rc
	)
	ADD_DEFINITIONS(
		-D_WINDOWS
		-DTELLDUSCORE_EXPORTS
	)
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/driver
		${CMAKE_CURRENT_SOURCE_DIR}/../common
	)
	ADD_DEFINITIONS( -DUNICODE )
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)

ELSE (APPLE) #### Linux ####
	SET( telldus-client_TARGET	telldus-core )

ENDIF (APPLE)


######## Configuring  ########

ADD_LIBRARY(${telldus-client_TARGET} SHARED
	${telldus-client_SRCS}
	${telldus-client_HDRS}
	${telldus-client_MOC_HDRS}
	${telldus-client_MOC_SRCS}
)
SET_SOURCE_FILES_PROPERTIES(${telldus-service_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

IF (SIGN_FILE)
	#This doesn't work for some reason
	ADD_CUSTOM_COMMAND(TARGET ${telldus-client_TARGET}
		POST_BUILD 
		COMMAND signtool.exe
		ARGS sign /a /t http://timestamp.verisign.com/scripts/timstamp.dll ${telldus-client_TARGET}.dll
		WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}
		COMMENT "Signing file TelldusCore.dll"
	)
ENDIF (SIGN_FILE)


SET_PROPERTY(TARGET ${telldus-client_TARGET}
	PROPERTY FRAMEWORK TRUE	
)
SET_PROPERTY(TARGET ${telldus-client_TARGET}
	PROPERTY PUBLIC_HEADER ${telldus-client_HDRS}
)

SET_PROPERTY(TARGET ${telldus-client_TARGET}
	PROPERTY VERSION ${PACKAGE_VERSION}
)
SET_PROPERTY(TARGET ${telldus-client_TARGET}
  PROPERTY SOVERSION ${PACKAGE_SOVERSION}
)


TARGET_LINK_LIBRARIES( ${telldus-client_TARGET}	${telldus-client_LIBRARIES} )


SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
IF (APPLE)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
ELSEIF (UNIX)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/TelldusClient
	)
ENDIF (APPLE)

