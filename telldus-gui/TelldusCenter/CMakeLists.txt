FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT 1)
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-center_SRCS
	main.cpp
	mainwindow.cpp
	tellduscenterapplication.cpp
	AutoUpdater.cpp
	message.cpp
	plugintree.cpp
)

SET( telldus-center_HDRS
	AutoUpdater.h
	CocoaInitializer.h
	tellduscenterplugin.h
	plugintree.h
)

SET( telldus-center_MOC_HDRS
	mainwindow.h
	tellduscenterapplication.h
	message.h
)

SET( telldus-center_RESOURCES
	TelldusCenter.icns
)

SET( telldus-center_TS
	telldus-center_sv.ts
)

SET( telldus-center_QM
	telldus-center_sv.qm
)

QT4_WRAP_CPP(  telldus-center_MOC_SRCS  ${telldus-center_MOC_HDRS} )
QT4_AUTOMOC (  ${telldus-center_SRCS} )
QT4_ADD_RESOURCES  (telldus-center_RSRCS     resource.qrc         )

SET( telldus-center_LIBRARIES
	${QT_LIBRARIES}
)

######## Configurable options for the platform  ########



######## Platforms-specific, non configurable  ########

SET( telldus-center_TARGET	TelldusCenter )

IF (APPLE) #### Mac OS X ####

ELSEIF (WIN32) #### Windows ####
	INCLUDE_DIRECTORIES(
		${TELLDUSCORE_INCLUDE_DIR}
	)
	SET(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS"
	)
	SET( telldus-center_LIBRARIES
		${telldus-center_LIBRARIES}
		"${QT_LIBRARY_DIR}/qtmain.lib"
	)
	SET(telldus-center_SRCS
		${telldus-center_SRCS}
		resource.rc
	)
ELSE (APPLE) #### Linux ####

ENDIF (APPLE)


######## Configuring  ########

# Compile translation files
ADD_CUSTOM_COMMAND(TARGET ${telldus-center_TARGET}
	PRE_BUILD
	COMMAND lupdate
	ARGS ${telldus-center_SRCS} -ts ${telldus-center_TS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(${telldus-center_TARGET} MACOSX_BUNDLE
	${telldus-center_SRCS}
	${telldus-center_MOC_SRCS}
	${telldus-center_RSRCS}
	${telldus-center_RESOURCES}
)
SET_SOURCE_FILES_PROPERTIES(${telldus-center_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

TARGET_LINK_LIBRARIES( ${telldus-center_TARGET}	${telldus-center_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-center_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
IF (APPLE)
	SET_TARGET_PROPERTIES(${telldus-center_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
ELSEIF (UNIX)
	SET_TARGET_PROPERTIES(${telldus-center_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/TelldusCenter
	)
ENDIF (APPLE)

