FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######## Non configurable options  ########
SET( telldus-gui_SRCS
	telldusgui.cpp
	devicewidget.cpp
	devicemodel.cpp
	device.cpp
	editdevicedialog.cpp
	editgroupdialog.cpp
	filtereddeviceproxymodel.cpp
	vendordevicemodel.cpp
	vendordevicetreeitem.cpp
	devicesettingarctechselflearning.cpp
	devicesettingbrateck.cpp
	devicesettinggao.cpp
	devicesettingikea.cpp
	devicesettingnexa.cpp
	devicesettingnexabell.cpp
	devicesettingrisingsun.cpp
	devicesettingsartano.cpp
	devicesettingselflearning.cpp
	devicesettingunitcode.cpp
	devicesettingupm.cpp
	devicesetting.cpp
	methodwidget.cpp
	deviceview.cpp
)

SET( telldus-gui_HDRS
	telldusgui.h
)

SET( telldus-gui_MOC_HDRS
	devicewidget.h
	devicemodel.h
	device.h
	editdevicedialog.h
	editgroupdialog.h
	filtereddeviceproxymodel.h
	vendordevicemodel.h
	devicesettingarctechselflearning.h
	devicesettingbrateck.h
	devicesettinggao.h
	devicesettingikea.h
	devicesettingnexa.h
	devicesettingnexabell.h
	devicesettingrisingsun.h
	devicesettingsartano.h
	devicesettingselflearning.h
	devicesettingunitcode.h
	devicesettingupm.h
	devicesetting.h
	methodwidget.h
	deviceview.h
)

QT4_WRAP_CPP(  telldus-gui_MOC_SRCS  ${telldus-gui_MOC_HDRS} )
QT4_AUTOMOC (  ${telldus-gui_SRCS} )
QT4_ADD_RESOURCES  (telldus-gui_RSRCS   telldusgui.qrc  data/${BRANDING}/resources.qrc         )

LIST(APPEND telldus-gui_RSRCS data/${BRANDING}/devices.xml)

SET( telldus-gui_LIBRARIES
	${QT_LIBRARIES}
)

FIND_PACKAGE(TelldusCore REQUIRED)
LIST(APPEND telldus-gui_LIBRARIES ${TELLDUSCORE_LIBRARY} )

######## Configurable options for the platform  ########



######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-gui_TARGET	TelldusGui )
	ADD_DEFINITIONS(
		-D_MACOSX
	)
	INCLUDE_DIRECTORIES(
		/usr/local/include
	)

ELSEIF (WIN32)
	#### Windows ####
	SET( telldus-gui_TARGET	TelldusGui )
	ADD_DEFINITIONS(
		-D_WINDOWS
		-DTELLDUSGUI_EXPORTS
	)
ELSE (APPLE)
	#### Linux ####
	SET( telldus-gui_TARGET	telldus-gui )
	ADD_DEFINITIONS(
		-D_LINUX
	)
ENDIF (APPLE)

######## Configuring  ########

####### i18n ########
FOREACH(lang ${LANGUAGES})
	LIST(APPEND telldus-gui_TS "TelldusGui_${lang}.ts")
ENDFOREACH(lang)
IF (UPDATE_TRANSLATIONS)
	QT4_CREATE_TRANSLATION( QM_FILES ${telldus-gui_SRCS} ${telldus-gui_TS} )
ELSE (UPDATE_TRANSLATIONS)
	IF (APPLE)
		SET_SOURCE_FILES_PROPERTIES(${telldus-gui_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Resources
		)
	ELSEIF (UNIX)
		SET_SOURCE_FILES_PROPERTIES(${telldus-gui_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/TelldusCenter
		)
	ENDIF (APPLE)
	QT4_ADD_TRANSLATION(telldus-gui_QM ${telldus-gui_TS})
ENDIF (UPDATE_TRANSLATIONS)

ADD_LIBRARY(${telldus-gui_TARGET} SHARED
	${telldus-gui_SRCS}
	${telldus-gui_HDRS}
	${telldus-gui_MOC_SRCS}
	${telldus-gui_RSRCS}
	${telldus-gui_TS}
	${telldus-gui_QM}
)
ADD_DEPENDENCIES(${telldus-gui_TARGET} TelldusCenter)

IF (UNIX)
	SET_TARGET_PROPERTIES( ${telldus-gui_TARGET} PROPERTIES COMPILE_FLAGS "-fPIC -fvisibility=hidden")
ENDIF (UNIX)

TARGET_LINK_LIBRARIES( ${telldus-gui_TARGET}	${telldus-gui_LIBRARIES} )

SET_TARGET_PROPERTIES(${telldus-gui_TARGET} PROPERTIES
	FRAMEWORK TRUE
#	PUBLIC_HEADER ${telldus-gui_HDRS}
	VERSION ${PACKAGE_VERSION}
	SOVERSION ${PACKAGE_SOVERSION}
	INSTALL_NAME_DIR "/Library/Frameworks"
)

IF (UNIX)
	INSTALL(TARGETS ${telldus-gui_TARGET}
		LIBRARY DESTINATION lib
		FRAMEWORK DESTINATION /Applications/TelldusCenter.app/Contents/Frameworks
		#	PUBLIC_HEADER       DESTINATION         ${HEADER_DIR}
	)
	IF (APPLE)
		INSTALL(CODE "
			INCLUDE(BundleUtilities)
			GET_FILENAME_COMPONENT(DESTDIR \$ENV{DESTDIR} ABSOLUTE)
			SET(lib \"\${DESTDIR}/Applications/TelldusCenter.app/Contents/Frameworks/${telldus-gui_TARGET}.framework/${telldus-gui_TARGET}\")

			GET_ITEM_KEY(\"\${lib}\" pkey)
			SET(prereqs \"\")
			GET_PREREQUISITES(\${lib} prereqs 1 0 \"\${exe}\" \"\${bundle}/Contents/Frameworks/\")
			FOREACH(pr \${prereqs})
				GET_ITEM_KEY(\"\${pr}\" rkey)

				#Don't change the path to TelldusCore
				IF (NOT \"\${rkey}\" STREQUAL \"TelldusCore\")
					#Check to see if this is ourself
					IF (NOT \${pkey} STREQUAL \${rkey})
						SET(kv \"\")
						SET_BUNDLE_KEY_VALUES(kv \"\${pr}\" \"\${pr}\" \"\${exe}\" \"\${bundle}/Contents/Frameworks/\" 0)
						EXECUTE_PROCESS(COMMAND install_name_tool
							-change \"\${pr}\" \"\${\${rkey}_EMBEDDED_ITEM}\" \"\${lib}\"
						)
					ENDIF ()
				ENDIF ()

			ENDFOREACH()
		")
	ENDIF ()
ENDIF ()
