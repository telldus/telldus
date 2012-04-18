SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
#SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)

SET(QT_USE_QTDECLARATIVE TRUE)

SET( Plugin_NAME "Controllers" )


SET( Plugin_SRCS
	controller.cpp
	controllerlist.cpp
	controllersplugin.cpp
	tellstick.cpp
)

SET( Plugin_HDRS
	controllersplugin.h
)

SET( Plugin_MOC_HDRS
	controller.h
	controllerlist.h
	tellstick.h
)

SET( Plugin_PATH "com.telldus.controllers" )

SET( Plugin_EXTRA
	btn_action_remove.png
	ControllerView.qml
	header_bg.png
	HeaderTitle.qml
	icon.png
	main.qml
	qmldir
	row_bg.png
	tellstick.png
	tellstick_duo.png
)

FIND_PACKAGE(TelldusCore REQUIRED)
SET( Plugin_LIBRARIES ${TELLDUSCORE_LIBRARY} )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
