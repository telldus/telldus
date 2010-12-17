SET(QT_USE_QTDECLARATIVE TRUE)

SET( Plugin_NAME "QMLLoader" )


SET( Plugin_SRCS
	qmlplugin.cpp
)

SET( Plugin_HDRS
	qmlplugin.h
)

SET( Plugin_PATH "com.telldus.qml" )

INCLUDE( ../TelldusCenterPlugin.cmake NO_POLICY_SCOPE )
