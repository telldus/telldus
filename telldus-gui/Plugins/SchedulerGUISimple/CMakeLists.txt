SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SCHEDULER TRUE PARENT_SCOPE)

SET( Plugin_NAME "schedulerguisimple" )

SET( Plugin_PATH "com.telldus.schedulerguisimple" )
SET( Plugin_EXTRA main.qml Dialog.qml ActionPoint.qml )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
