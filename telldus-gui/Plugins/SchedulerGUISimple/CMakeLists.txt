SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SCHEDULER TRUE PARENT_SCOPE)

SET( Plugin_NAME "schedulerguisimple" )

SET( Plugin_PATH "com.telldus.schedulerguisimple" )
SET( Plugin_EXTRA
	ActionPoint.qml
	Button.qml
	Dialog.qml
	icon.png
	main.qml
	schedulerscripts.js
	absolute.png
	sunrise.png
	sunset.png
	on.png
	off.png
	dim.png
	bell.png
	info.png
	fuzzy.png
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
