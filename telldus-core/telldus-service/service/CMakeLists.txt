FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTNETWORK TRUE)
INCLUDE( ${QT_USE_FILE} )

IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-service_SRCS
	main.cpp
	TelldusCore.cpp
	Manager.cpp
	../common/Message.cpp
	${QTSERVICE_PATH}/src/qtservice.cpp
)

SET( telldus-service_MOC_HDRS
	TelldusCore.h
	Manager.h
)
	
SET( telldus-service_LIBRARIES
	${QT_LIBRARIES}
)

######## Configurable options for the platform  ########

SET(QTSERVICE_PATH	CACHE PATH "Path to qtservice")

######## Platforms-specific, non configurable  ########

SET( telldus-service_TARGET	TelldusService )

IF (APPLE) #### Mac OS X ####

ELSEIF (WIN32) #### Windows ####
	ADD_DEFINITIONS( -DUNICODE )
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/driver
		${CMAKE_CURRENT_SOURCE_DIR}/../common
		${QTSERVICE_PATH}/src
		${CMAKE_CURRENT_BINARY_DIR}
	)
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE"
	)
	SET( telldus-service_LIBRARIES
		${telldus-service_LIBRARIES}
		${LIBRARY_OUTPUT_PATH}/Release/TelldusCoreLib.lib
	)
	SET(telldus-service_SRCS
		${telldus-service_SRCS}
		${QTSERVICE_PATH}/src/qtservice_win.cpp
		${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc
		${CMAKE_CURRENT_BINARY_DIR}/qtservice_win.moc
	)
	QT4_GENERATE_MOC( ${QTSERVICE_PATH}/src/qtservice_win.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice_win.moc )

ELSE (APPLE) #### Linux ####

ENDIF (APPLE)


######## Configuring  ########

QT4_GENERATE_MOC( ${QTSERVICE_PATH}/src/qtservice.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc )
QT4_WRAP_CPP(  telldus-service_MOC_SRCS  ${telldus-service_MOC_HDRS} )


ADD_EXECUTABLE(${telldus-service_TARGET}
	${telldus-service_SRCS}
	${telldus-service_MOC_SRCS}
)
SET_SOURCE_FILES_PROPERTIES(${telldus-service_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

TARGET_LINK_LIBRARIES( ${telldus-service_TARGET}	${telldus-service_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
IF (APPLE)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
ELSEIF (UNIX)
	SET_TARGET_PROPERTIES(${telldus-service_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/TelldusCenter
	)
ENDIF (APPLE)

