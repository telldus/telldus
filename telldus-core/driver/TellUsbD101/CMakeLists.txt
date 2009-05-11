
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET( tellusbd101_SRCS
	TellUsbD101.cpp
	TellUsbD101.def
)

SET( tellusbd101_HDRS
	TellUsbD101.h
)

SET( tellusbd101_TARGET	TellUsbD101 )
	ADD_DEFINITIONS(
		-D_WINDOWS
		-DTellUsbD101_EXPORTS
)

SET( tellusbd101_LIBRARIES
	"${LIBRARY_OUTPUT_PATH}/Release/TelldusCore.lib"
)


######## Configuring  ########

ADD_LIBRARY(${tellusbd101_TARGET} SHARED
	${tellusbd101_SRCS}
	${tellusbd101_HDRS}
)

TARGET_LINK_LIBRARIES( ${tellusbd101_TARGET}	${tellusbd101_LIBRARIES} )

SET_PROPERTY(TARGET ${tellusbd101_TARGET}
	PROPERTY PUBLIC_HEADER ${tellusbd101_HDRS}
)

