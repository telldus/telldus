IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######## Non configurable options  ########
SET( common_SRCS
	)

SET( common_HDRS
)

######## Configurable options for the platform  ########


######## Platforms-specific, non configurable  ########

IF (APPLE)
	#### Mac OS X ####
	SET( telldus-common_TARGET	TelldusCommon )
	ADD_DEFINITIONS( -D_MACOSX )
ELSEIF (WIN32)
	#### Windows ####
	SET( telldus-common_TARGET	TelldusCommon )
	ADD_DEFINITIONS( -D_WINDOWS )
ELSE (APPLE)
	#### Linux ####
	SET( telldus-common_TARGET	telldus-common )
	ADD_DEFINITIONS( -D_LINUX )
ENDIF (APPLE)


######## Configuring  ########


ADD_LIBRARY(${telldus-common_TARGET} STATIC
	${telldus-common_SRCS}
	${telldus-common_HDRS}
)

TARGET_LINK_LIBRARIES( ${telldus-common_TARGET}	${telldus-common_LIBRARIES} )
