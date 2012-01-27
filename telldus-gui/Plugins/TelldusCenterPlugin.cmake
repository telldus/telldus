FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTSCRIPT TRUE)
INCLUDE( ${QT_USE_FILE} )
FIND_PACKAGE( SignTool REQUIRED )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


QT4_WRAP_CPP(  Plugin_MOC_SRCS  ${Plugin_MOC_HDRS} )
QT4_AUTOMOC (  ${Plugin_SRCS} )

IF(Plugin_RESOURCES)
	QT4_ADD_RESOURCES(Plugin_RSRCS ${Plugin_RESOURCES})
ENDIF(Plugin_RESOURCES)

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
		Plugin_PATH_relative ${Plugin_PATH}
	)
	IF (APPLE)
		SET(Plugin_PATH "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/script/${Plugin_PATH_relative}")
	ELSEIF (WIN32)
		SET(Plugin_PATH "${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Plugins/script/${Plugin_PATH_relative}")
	ELSE (APPLE)
		SET(Plugin_PATH "${CMAKE_BINARY_DIR}/TelldusCenter/Plugins/script/${Plugin_PATH_relative}")
	ENDIF (APPLE)

	LIST(APPEND translation_sources
		"${CMAKE_CURRENT_SOURCE_DIR}/__init__.js"
	)
	LIST(APPEND Plugin_FILES "${CMAKE_CURRENT_SOURCE_DIR}/__init__.js")
	#Add all extra files
	FOREACH(_FILE ${Plugin_EXTRA})
		LIST(APPEND Plugin_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_FILE}")
	ENDFOREACH(_FILE)
ENDIF(Plugin_PATH)

IF (UPDATE_TRANSLATIONS)
	QT4_CREATE_TRANSLATION( QM_FILES ${Plugin_SRCS} ${translation_sources} ${Plugin_TS} )
ELSE (UPDATE_TRANSLATIONS)
	QT4_ADD_TRANSLATION(Plugin_QM ${Plugin_TS})
	LIST(APPEND Plugin_FILES ${Plugin_QM})
ENDIF (UPDATE_TRANSLATIONS)

IF(Plugin_PATH)
	FOREACH(_FILE ${Plugin_FILES})
		GET_FILENAME_COMPONENT(_FILENAME ${_FILE} NAME)
		ADD_CUSTOM_COMMAND( OUTPUT ${Plugin_PATH}/${_FILENAME}
			COMMAND ${CMAKE_COMMAND} -E copy ${_FILE} ${Plugin_PATH}/${_FILENAME}
			DEPENDS ${_FILE}
			COMMENT "Copy ${_FILENAME} for plugin ${Plugin_NAME}"
		)
		LIST(APPEND Plugin_TARGET_FILES "${Plugin_PATH}/${_FILENAME}")
		IF (NOT APPLE)
			INSTALL(FILES ${_FILE} DESTINATION "${PLUGIN_LIB_FULL_PATH}/script/${Plugin_PATH_relative}")
		ENDIF ()
	ENDFOREACH(_FILE)
ENDIF(Plugin_PATH)

IF(Plugin_SRCS)
	ADD_LIBRARY(${Plugin_NAME} SHARED
		${Plugin_SRCS}
		${Plugin_HDRS}
		${Plugin_RSRCS}
		${Plugin_MOC_HDRS}
		${Plugin_MOC_SRCS}
		${Plugin_MOC_HDRS}
		${Plugin_FILES}
		${Plugin_TS}
		${Plugin_QM}
		${Plugin_TARGET_FILES}
	)
	TARGET_LINK_LIBRARIES( ${Plugin_NAME}	${Plugin_LIBRARIES} )

	IF (APPLE)
		SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/script
			PREFIX "../"
		)
		INSTALL(CODE "
			GET_FILENAME_COMPONENT(DESTDIR \$ENV{DESTDIR} ABSOLUTE)
			SET(app \"\${DESTDIR}/Applications/TelldusCenter.app\")
			GET_BUNDLE_AND_EXECUTABLE(\"\${app}\" bundle exe valid)
			GET_FILENAME_COMPONENT(exedir \"\${exe}\" PATH)
			SET(plugin \"\${bundle}/Contents/Plugins/script/${Plugin_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}\")

			GET_ITEM_KEY(\"\${plugin}\" pkey)
			SET(prereqs \"\")
			GET_PREREQUISITES(\${plugin} prereqs 1 0 \"\${exe}\" \"\${bundle}/Contents/Frameworks/\")
			FOREACH(pr \${prereqs})
				GET_ITEM_KEY(\"\${pr}\" rkey)

				#Don't change the path to TelldusCore
				IF (NOT \"\${rkey}\" STREQUAL \"TelldusCore\")
					#Check to see if this is ourself
					IF (NOT \${pkey} STREQUAL \${rkey})
						SET(kv \"\")
						SET_BUNDLE_KEY_VALUES(kv \"\${pr}\" \"\${pr}\" \"\${exedir}\" \"\${bundle}/Contents/Frameworks/\" 1)
						IF (NOT EXISTS \"\${\${kv}_RESOLVED_EMBEDDED_ITEM}\")
							COPY_RESOLVED_FRAMEWORK_INTO_BUNDLE(\"\${\${kv}_RESOLVED_ITEM}\" \"\${\${kv}_RESOLVED_EMBEDDED_ITEM}\")
						ENDIF ()
						EXECUTE_PROCESS(COMMAND install_name_tool
							-change \"\${pr}\" \"\${\${rkey}_EMBEDDED_ITEM}\" \"\${plugin}\"
						)
					ENDIF ()
				ENDIF ()

			ENDFOREACH()
		")
	ELSEIF (WIN32)
		SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
			PREFIX "Plugins/script/"
		)
	ELSE (APPLE)
		SET_TARGET_PROPERTIES(${Plugin_NAME} PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/TelldusCenter/Plugins/script
		)
		INSTALL(TARGETS ${Plugin_NAME}
			LIBRARY DESTINATION "${PLUGIN_LIB_FULL_PATH}/script"
		)
	ENDIF (APPLE)
	SIGN(${Plugin_NAME})
ELSE(Plugin_SRCS)
	ADD_CUSTOM_TARGET(${Plugin_NAME} ALL
		SOURCES ${Plugin_FILES} ${Plugin_TARGET_FILES}
	)
ENDIF(Plugin_SRCS)
