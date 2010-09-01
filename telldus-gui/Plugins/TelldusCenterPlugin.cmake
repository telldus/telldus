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
	IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/translation_${lang}.ts" OR UPDATE_TRANSLATIONS)
		LIST(APPEND Plugin_TS "translation_${lang}.ts")
	ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/translation_${lang}.ts" OR UPDATE_TRANSLATIONS)
ENDFOREACH(lang)
SET(translation_sources ${Plugin_SRCS})

IF(Plugin_PATH)
	STRING(REPLACE "." "/"
		path ${Plugin_PATH}
	)
	IF (APPLE)
		SET(Plugin_PATH "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/script/${path}")
	ELSE (APPLE)
		SET(Plugin_PATH "${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Plugins/script/${path}")
	ENDIF (APPLE)
	
	LIST(APPEND translation_sources
		"${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script/${path}/__init__.js"
	)
	LIST(APPEND Plugin_FILES "${CMAKE_SOURCE_DIR}/TelldusCenter/Plugins/script/${path}/__init__.js")
	
ENDIF(Plugin_PATH)

IF (UPDATE_TRANSLATIONS)
	QT4_CREATE_TRANSLATION( QM_FILES ${Plugin_SRCS} ${translation_sources} ${Plugin_TS} )
ELSE (UPDATE_TRANSLATIONS)
	QT4_ADD_TRANSLATION(Plugin_QM ${Plugin_TS})
	LIST(APPEND Plugin_FILES ${Plugin_QM})
ENDIF (UPDATE_TRANSLATIONS)

IF(Plugin_SRCS)
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
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/script
			PREFIX "../"
		)
	ELSE (APPLE)
		SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
			PREFIX "Plugins/script/"
		)
	ENDIF (APPLE)
ELSE(Plugin_SRCS)
	ADD_CUSTOM_TARGET(${Plugin_NAME} ALL
		SOURCES ${Plugin_FILES}
	)
ENDIF(Plugin_SRCS)

IF(Plugin_PATH)
	ADD_CUSTOM_COMMAND( TARGET ${Plugin_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${Plugin_PATH}
		COMMENT "Creating plugin directory ${Plugin_NAME}"
	)
	FOREACH(_FILE ${Plugin_FILES})
		GET_FILENAME_COMPONENT(_FILENAME ${_FILE} NAME)
		ADD_CUSTOM_COMMAND( TARGET ${Plugin_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${_FILE} ${Plugin_PATH}
			COMMENT "Copy ${_FILENAME} for plugin ${Plugin_NAME}"
		)
	ENDFOREACH(_FILE)
ENDIF(Plugin_PATH)

