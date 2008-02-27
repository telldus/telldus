
SET( tellusbd101_SRCS
	Device.cpp
	linux/Device.cpp
	DeviceIkea.cpp
	DeviceNexa.cpp
	DeviceSartano.cpp
	DeviceWaveman.cpp
	)

ADD_DEFINITIONS(
	-DOS_LINUX
	)

ADD_LIBRARY(tellusbd101 SHARED
	${tellusbd101_SRCS}
	${tellusbd101_MOC_SRCS}
	)

# TARGET_LINK_LIBRARIES(tellusbd101
# 	${MIDAS_LIBRARY}
# 	)

INSTALL(TARGETS tellusbd101 LIBRARY
	DESTINATION lib
	)

