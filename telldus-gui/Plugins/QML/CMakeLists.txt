SET(QT_USE_QTDECLARATIVE TRUE)

SET( Plugin_NAME "QMLLoader" )


SET( Plugin_SRCS
	scriptfunctionwrapper.cpp
	qmlarray.cpp
	qmlplugin.cpp
	qmlview.cpp
)

SET( Plugin_HDRS
	qmlplugin.h
)

SET( Plugin_MOC_HDRS
	scriptfunctionwrapper.h
	qmlarray.h
	qmlview.h
)

SET( Plugin_PATH "com.telldus.qml" )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )

IF (WIN32)
	SET(QT_COMPONENTS_OUTPUT_DIR "${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Plugins/declarative")
ELSEIF (APPLE)
	SET(QT_COMPONENTS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TelldusCenter.app/Contents/Plugins/declarative")
ELSE()
	SET(QT_COMPONENTS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/TelldusCenter/Plugins/declarative")
ENDIF()
INCLUDE( ${CMAKE_SOURCE_DIR}/3rdparty/qt-components-desktop.cmake NO_POLICY_SCOPE )
