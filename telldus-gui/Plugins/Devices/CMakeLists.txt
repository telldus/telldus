FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######## Non configurable options  ########
SET( Devices_SRCS
	devicesplugin.cpp
)

SET( Devices_HDRS
)

SET( Devices_MOC_HDRS
	devicesplugin.h
)

QT4_WRAP_CPP(  Devices_MOC_SRCS  ${Devices_MOC_HDRS} )
QT4_AUTOMOC (  ${Devices_SRCS} )

FIND_LIBRARY( TELLDUSGUI_LIBRARY telldus-gui )

SET( Devices_LIBRARIES
	${QT_LIBRARIES}
	${TELLDUSGUI_LIBRARY}
)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########


######## Configuring  ########

ADD_LIBRARY(Devices SHARED
	${Devices_SRCS}
	${Devices_HDRS}
	${Devices_MOC_SRCS}
)

TARGET_LINK_LIBRARIES( Devices	${Devices_LIBRARIES} )

SET_PROPERTY(TARGET Devices
	PROPERTY VERSION ${PACKAGE_VERSION}
)
SET_PROPERTY(TARGET Devices
  PROPERTY SOVERSION ${PACKAGE_SOVERSION}
)
