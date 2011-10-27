SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)

SET(QT_USE_QTDECLARATIVE TRUE)

SET( Plugin_NAME "sensors" )


SET( Plugin_SRCS
	sensor.cpp
	sensorsplugin.cpp
	sensorvalue.cpp
)

SET( Plugin_HDRS
	sensorsplugin.h
)

SET( Plugin_MOC_HDRS
	sensor.h
	sensorvalue.h
)

SET( Plugin_PATH "com.telldus.sensors" )

SET( Plugin_EXTRA
	icon.png
	main.qml
	qmldir
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
