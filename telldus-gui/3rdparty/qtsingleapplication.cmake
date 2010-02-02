
SET( qtsingleapplication_MOC_HDRS
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/qtsingleapplication.h
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/qtlocalpeer.h
)

QT4_WRAP_CPP(  qtsingleapplication_MOC_SRCS  ${qtsingleapplication_MOC_HDRS} )
QT4_AUTOMOC (  ${qtsingleapplication_SRCS} )

SET( qtsingleapplication_SRCS
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/qtsingleapplication.cpp
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/qtlocalpeer.cpp
# 	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/qtlocalserver.cpp
	${qtsingleapplication_MOC_SRCS}
)

INCLUDE_DIRECTORIES(
	${CMAKE_HOME_DIRECTORY}/3rdparty/qtsingleapplication/src/
)
