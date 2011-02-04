SET(REQUIRE_PLUGIN_QML TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SETTINGS TRUE PARENT_SCOPE)
SET(REQUIRE_PLUGIN_SUNCALCULATOR TRUE PARENT_SCOPE)

SET( Plugin_NAME "scheduler" )

SET( Plugin_PATH "com.telldus.scheduler" )
SET( Plugin_EXTRA
	"DaylightSavingTime.js"
	"DefaultJobTypes.js"
 )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
