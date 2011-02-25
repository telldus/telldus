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
list(APPEND telldus-gui_LIBRARIES ${TELLDUSCORE_LIBRARY} )

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
	INCLUDE_DIRECTORIES(
		${TELLDUSCORE_INCLUDE_DIR}
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
			OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/TelldusCenter.app/Contents/Resources
		)
	ELSEIF (UNIX)
		SET_SOURCE_FILES_PROPERTIES(${telldus-gui_TS} PROPERTIES
			OUTPUT_LOCATION ${CMAKE_SOURCE_DIR}/TelldusCenter
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

TARGET_LINK_LIBRARIES( ${telldus-gui_TARGET}	${telldus-gui_LIBRARIES} )

SET_PROPERTY(TARGET ${telldus-gui_TARGET}
	PROPERTY FRAMEWORK TRUE
)
SET_PROPERTY(TARGET ${telldus-gui_TARGET}
	PROPERTY PUBLIC_HEADER ${telldus-gui_HDRS}
)

SET_PROPERTY(TARGET ${telldus-gui_TARGET}
	PROPERTY VERSION ${PACKAGE_VERSION}
)
SET_PROPERTY(TARGET ${telldus-gui_TARGET}
  PROPERTY SOVERSION ${PACKAGE_SOVERSION}
)

INSTALL(TARGETS ${telldus-gui_TARGET}
	DESTINATION lib
#	PUBLIC_HEADER       DESTINATION         ${HEADER_DIR}
	)
