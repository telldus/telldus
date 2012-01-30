SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)

SET(QT_USE_QTDECLARATIVE TRUE)

SET( Plugin_NAME "Sensors" )


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
	btn_action_remove.png
	header_bg.png
	icon.png
	icon_humidity.png
	icon_temp.png
	main.qml
	HeaderTitle.qml
	row_bg.png
	qmldir
	SensorValue.qml
	SensorView.qml
	SensorList.qml
)

FIND_PACKAGE(TelldusCore REQUIRED)
SET( Plugin_LIBRARIES ${TELLDUSCORE_LIBRARY} )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
