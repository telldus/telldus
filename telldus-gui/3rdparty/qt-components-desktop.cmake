FIND_PACKAGE( Qt4 REQUIRED )

SET(BASE_PATH "${CMAKE_SOURCE_DIR}/3rdparty/qt-components-desktop")
FILE(GLOB SRCS ${BASE_PATH}/src/*.cpp)
FILE(GLOB HDRS ${BASE_PATH}/src/*.h)

FILE(GLOB_RECURSE QML ${BASE_PATH}/components/*.qml)
FILE(GLOB_RECURSE JS ${BASE_PATH}/components/*.js)
FILE(GLOB_RECURSE PNG ${BASE_PATH}/components/*.png)
FILE(GLOB_RECURSE QMLDIR ${BASE_PATH}/components/qmldir)
SET(EXTRA_FILES ${QML} ${JS} ${PNG} ${QMLDIR})

IF (QT_COMPONENTS_OUTPUT_DIR)
	SET(QT_COMPONENTS_OUTPUT_DIR "${QT_COMPONENTS_OUTPUT_DIR}/QtDesktop")
ELSE()
	SET(QT_COMPONENTS_OUTPUT_DIR "QtDesktop")
ENDIF()

STRING(LENGTH "${BASE_PATH}/components" BASE_LENGTH)
FOREACH(_FILE ${EXTRA_FILES})
	STRING(LENGTH ${_FILE} _FILE_LENGTH)
	MATH(EXPR _TOP_LENGTH "${_FILE_LENGTH}-${BASE_LENGTH}")
	STRING(SUBSTRING ${_FILE} ${BASE_LENGTH} ${_TOP_LENGTH} _OUT_FILE)
	GET_FILENAME_COMPONENT(_FILENAME ${_FILE} NAME)
	SET(_OUTFILEPATH ${QT_COMPONENTS_OUTPUT_DIR}${_OUT_FILE})
	LIST(APPEND SRCS ${_OUTFILEPATH})
	ADD_CUSTOM_COMMAND(
		OUTPUT ${_OUTFILEPATH}
		DEPENDS ${_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy ${_FILE} ${_OUTFILEPATH}
		COMMENT "Copy ${_FILENAME} to destination"
	)
ENDFOREACH(_FILE)

QT4_WRAP_CPP( MOC_SRCS  ${HDRS} )
QT4_AUTOMOC ( ${SRCS} )

SET(LIBRARIES ${QT_LIBRARIES})

IF (APPLE)
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
	LIST(APPEND LIBRARIES ${CARBON_LIBRARY})
ENDIF ()

ADD_LIBRARY(styleplugin SHARED
	${SRCS}
	${MOC_SRCS}
)
TARGET_LINK_LIBRARIES( styleplugin ${LIBRARIES} )

IF (WIN32)
	SET_TARGET_PROPERTIES(styleplugin PROPERTIES
		PREFIX "Plugins/declarative/QtDesktop/plugin/"
	)
ELSEIF(APPLE)
	ADD_CUSTOM_COMMAND(TARGET styleplugin POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${QT_COMPONENTS_OUTPUT_DIR}/plugin/
		COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/libstyleplugin.dylib
			${QT_COMPONENTS_OUTPUT_DIR}/plugin/
		COMMENT "Copy plugin to destination bundle"
	)
	INSTALL(CODE "
		GET_FILENAME_COMPONENT(DESTDIR \"\$ENV{DESTDIR}\" ABSOLUTE)
		SET(app \"\${DESTDIR}/Applications/TelldusCenter.app\")
		GET_BUNDLE_AND_EXECUTABLE(\"\${app}\" bundle exe valid)
		GET_FILENAME_COMPONENT(exedir \"\${exe}\" PATH)
		SET(plugin \"\${bundle}/Contents/Plugins/declarative/QtDesktop/plugin/libstyleplugin${CMAKE_SHARED_LIBRARY_SUFFIX}\")

		GET_ITEM_KEY(\"\${plugin}\" pkey)
		SET(prereqs \"\")
		GET_PREREQUISITES(\${plugin} prereqs 1 0 \"\${exe}\" \"\${bundle}/Contents/Frameworks/\")
		FOREACH(pr \${prereqs})
			GET_ITEM_KEY(\"\${pr}\" rkey)

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

		ENDFOREACH()
	")
ELSE()
	SET_TARGET_PROPERTIES(styleplugin PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${QT_COMPONENTS_OUTPUT_DIR}/plugin
	)
ENDIF()

IF(UNIX AND NOT APPLE)
	INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/TelldusCenter/Plugins/declarative" DESTINATION "${PLUGIN_LIB_FULL_PATH}")
ENDIF()
