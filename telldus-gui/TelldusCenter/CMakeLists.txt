FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTUITOOLS TRUE)
IF (APPLE)
	#This is a workaround for plugins that depends on libraries and will not copy these correctly
	SET(QT_USE_QTDECLARATIVE TRUE)
ENDIF()
INCLUDE( ${QT_USE_FILE} )

FIND_PACKAGE( SignTool REQUIRED )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 NEW)
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
	ADD_DEFINITIONS( -DPLUGIN_LIB_PATH=${CMAKE_INSTALL_PREFIX}/${PLUGIN_LIB_FULL_PATH} )
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
	IF (UNIX)
		SET_SOURCE_FILES_PROPERTIES(${telldus-center_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/TelldusCenter
		)
	ENDIF ()
	QT4_ADD_TRANSLATION(telldus-center_QM ${telldus-center_TS})
	IF (APPLE)
		INSTALL(FILES ${telldus-center_QM} DESTINATION /Applications/TelldusCenter.app/Contents/Resources)
	ENDIF ()
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
SIGN(${telldus-center_TARGET})

IF (UNIX)
	IF (GENERATE_MAN)
		ADD_CUSTOM_COMMAND(
			TARGET ${telldus-center_TARGET}
			POST_BUILD
			
			COMMAND xsltproc --nonet --param man.charmap.use.subset "0" /usr/share/sgml/docbook/stylesheet/xsl/docbook-xsl/manpages/docbook.xsl ${CMAKE_CURRENT_SOURCE_DIR}/tellduscenter.man.xml 
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating man file TelldusCenter.1"
		)
		INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelldusCenter.1 DESTINATION share/man/man1)
	ENDIF (GENERATE_MAN)
	IF (NOT APPLE)
		INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/tellduscenter.xpm DESTINATION share/pixmaps)
		INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tellduscenter.desktop DESTINATION share/applications)
	ENDIF ()
ENDIF (UNIX)

IF (WIN32)
	ADD_CUSTOM_COMMAND( TARGET ${telldus-center_TARGET}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${telldus-center_QM} ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR} VERBATIM
		COMMENT "Copying translation for TelldusCenter"
	)
ENDIF()

INSTALL(TARGETS ${telldus-center_TARGET}
	RUNTIME	DESTINATION bin
	BUNDLE DESTINATION "/Applications"
)

IF (APPLE)
	INSTALL(
		DIRECTORY ${QT_QTGUI_LIBRARY}/Resources/qt_menu.nib
		DESTINATION /Applications/TelldusCenter.app/Contents/Resources
	)
	INSTALL(
		FILES qt.conf
		DESTINATION /Applications/TelldusCenter.app/Contents/Resources
	)
	INSTALL(CODE "
		INCLUDE(BundleUtilities)
		GET_FILENAME_COMPONENT(DESTDIR \$ENV{DESTDIR} ABSOLUTE)
		SET(app \"\${DESTDIR}/Applications/TelldusCenter.app\")
		GET_BUNDLE_AND_EXECUTABLE(\"\${app}\" bundle exe valid)
		FIXUP_BUNDLE(\"\${bundle}\" \"\" \"\")
	")
ENDIF ()

