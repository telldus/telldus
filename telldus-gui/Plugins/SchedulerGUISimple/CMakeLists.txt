SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SCHEDULER TRUE PARENT_SCOPE)

SET( Plugin_NAME "schedulerguisimple" )

SET( Plugin_PATH "com.telldus.schedulerguisimple" )
SET( Plugin_EXTRA
	ActionBar.qml
	ActionPoint.qml
	actionpointscripts.js
	Button.qml
	Dialog.qml
	icon.png
	InfoBox.qml
	main.qml
	mainscripts.js
	NavButton.qml
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
	ticked.png
	unticked.png
	alwaysticked.png
	qmldir
)

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
