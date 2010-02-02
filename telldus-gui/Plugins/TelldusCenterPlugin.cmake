FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT TRUE)
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


QT4_WRAP_CPP(  Plugin_MOC_SRCS  ${Plugin_MOC_HDRS} )
QT4_AUTOMOC (  ${Plugin_SRCS} )


SET( Plugin_LIBRARIES
	${Plugin_LIBRARIES}
	${QT_LIBRARIES}
)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

#SET( LIBRARY_OUTPUT_PATH	${LIBRARY_OUTPUT_PATH/Plugins} )

FOREACH(lang ${LANGUAGES})
	LIST(APPEND Plugin_TS "translation_${lang}.ts")
ENDFOREACH(lang)
SET(translation_sources ${Plugin_SRCS})
IF(Plugin_PATH)
	STRING(REPLACE "." "/"
		path ${Plugin_PATH}
	)
	LIST(APPEND translation_sources
		"${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script/${path}/__init__.js"
	)
	IF (APPLE)
		SET_SOURCE_FILES_PROPERTIES(${Plugin_TS} PROPERTIES
			OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script/${path}"
		)
	ELSE (APPLE)
		SET_SOURCE_FILES_PROPERTIES(${Plugin_TS} PROPERTIES
			OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script/${path}"
		)
	ENDIF (APPLE)
ENDIF(Plugin_PATH)
IF (UPDATE_TRANSLATIONS)
	QT4_CREATE_TRANSLATION( QM_FILES ${Plugin_SRCS} ${translation_sources} ${Plugin_TS} )
ELSE (UPDATE_TRANSLATIONS)
	QT4_ADD_TRANSLATION(Plugin_QM ${Plugin_TS})
ENDIF (UPDATE_TRANSLATIONS)

ADD_LIBRARY(${Plugin_NAME} SHARED
	${Plugin_SRCS}
	${Plugin_HDRS}
	${Plugin_MOC_SRCS}
	${Plugin_TS}
	${Plugin_QM}
)

TARGET_LINK_LIBRARIES( ${Plugin_NAME}	${Plugin_LIBRARIES} )

IF (APPLE)
	SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/TelldusCenter.app/Contents/Plugins/script
	)
ELSE (APPLE)
	SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script
	)
ENDIF (APPLE)
