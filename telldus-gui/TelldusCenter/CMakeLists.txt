FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTNETWORK TRUE)
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

SET( telldus-center_LANGS
	sv
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

######## QtSingleApplication ########

INCLUDE( ../3rdparty/qtsingleapplication.cmake NO_POLICY_SCOPE )
SET( telldus-center_SRCS
	${telldus-center_SRCS}
	${qtsingleapplication_SRCS}
)

######## Configuring  ########

####### i18n ########
FOREACH(lang ${LANGUAGES})
	LIST(APPEND telldus-center_TS "${telldus-center_TARGET}_${lang}.ts")
ENDFOREACH(lang)
IF (UPDATE_TRANSLATIONS)
	QT4_CREATE_TRANSLATION( QM_FILES ${telldus-center_SRCS} ${telldus-center_TS} )
ELSE (UPDATE_TRANSLATIONS)
	QT4_ADD_TRANSLATION(QM_FILES ${telldus-center_TS})
ENDIF (UPDATE_TRANSLATIONS) 
ADD_CUSTOM_TARGET (translations_target DEPENDS ${QM_FILES})



ADD_EXECUTABLE(${telldus-center_TARGET} MACOSX_BUNDLE
	${telldus-center_SRCS}
	${telldus-center_MOC_SRCS}
	${telldus-center_RSRCS}
	${telldus-center_RESOURCES}
	${telldus-center_TS}
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

