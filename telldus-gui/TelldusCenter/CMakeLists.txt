FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTUITOOLS TRUE)
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-center_SRCS
	configurationdialog.cpp
	main.cpp
	mainwindow.cpp
	scriptenvironment.cpp
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
	configurationdialog.h
	mainwindow.h
	scriptenvironment.h
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

ADD_DEFINITIONS( -DVERSION=${DISPLAYED_VERSION} )

######## Configurable options for the platform  ########



######## Platforms-specific, non configurable  ########

SET( telldus-center_TARGET	TelldusCenter )

IF (APPLE) #### Mac OS X ####
	LIST(APPEND telldus-center_LIBRARIES
		/usr/lib/libQtUiTools.a
	)
	INCLUDE_DIRECTORIES(
		/usr/include/QtUiTools
	)
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
		${CMAKE_CURRENT_BINARY_DIR}/Info.plist
	)
	LIST(APPEND telldus-center_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
	)

ELSEIF (WIN32) #### Windows ####
	ADD_DEFINITIONS( -DUNICODE )

	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/TelldusCenter.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/TelldusCenter.rc
	)

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
		${CMAKE_CURRENT_BINARY_DIR}/TelldusCenter.rc
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
	IF (APPLE)
		SET_SOURCE_FILES_PROPERTIES(${telldus-center_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/TelldusCenter.app/Contents/Resources
		)
	ELSEIF (UNIX)
		SET_SOURCE_FILES_PROPERTIES(${telldus-center_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_SOURCE_DIR}/TelldusCenter
		)
	ENDIF (APPLE)
	QT4_ADD_TRANSLATION(telldus-center_QM ${telldus-center_TS})
ENDIF (UPDATE_TRANSLATIONS)



ADD_EXECUTABLE(${telldus-center_TARGET} MACOSX_BUNDLE
	${telldus-center_SRCS}
	${telldus-center_MOC_SRCS}
	${telldus-center_RSRCS}
	${telldus-center_RESOURCES}
	${telldus-center_TS}
	${telldus-center_QM}
)
SET_SOURCE_FILES_PROPERTIES(${telldus-center_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

TARGET_LINK_LIBRARIES( ${telldus-center_TARGET}	${telldus-center_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-center_TARGET} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
)
IF (APPLE)
	SET_TARGET_PROPERTIES(${telldus-center_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	ADD_CUSTOM_COMMAND( TARGET ${telldus-center_TARGET}
		POST_BUILD
		COMMAND mkdir -p "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/script/"
	)
ENDIF (APPLE)

