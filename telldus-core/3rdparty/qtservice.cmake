
QT4_GENERATE_MOC( ${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtservice.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc )

SET( qtservice_SRCS
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtservice.cpp
	${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc
)

IF (UNIX)
	QT4_GENERATE_MOC( ${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtservice_unix.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice_unix.moc )
	
	SET( qtservice_MOC_HDRS
		${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtunixsocket.h
		${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtunixserversocket.h
	)
	
	QT4_WRAP_CPP(  qtservice_MOC_SRCS  ${qtservice_MOC_HDRS} )
	
	SET( qtservice_SRCS
		${qtservice_SRCS}
		${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtservice_unix.cpp
		${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtunixsocket.cpp
		${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/qtunixserversocket.cpp
		${CMAKE_CURRENT_BINARY_DIR}/qtservice_unix.moc
		${qtservice_MOC_SRCS}
	)
	
	
ENDIF (UNIX)

INCLUDE_DIRECTORIES(
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtservice-2.6-opensource/src/
	${CMAKE_CURRENT_BINARY_DIR}
)
