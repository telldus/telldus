cmake_minimum_required(VERSION 2.4)

INCLUDE_DIRECTORIES(
  /usr/src/linux/include
)

PROJECT(rfcmd)

FIND_LIBRARY(FTDI_LIBRARY ftdi)

SET(rfcmd_SRCS
  rfcmd.c
  ftdi.c
)

IF(${RFCMD_DEBUG})
	ADD_DEFINITIONS( -DRFCMD_DEBUG )
ENDIF(${RFCMD_DEBUG})

ADD_EXECUTABLE(rfcmd
  ${rfcmd_SRCS}
)

TARGET_LINK_LIBRARIES(rfcmd
 ${FTDI_LIBRARY}
)

INSTALL(TARGETS rfcmd RUNTIME DESTINATION bin)

PROJECT(find_telldus)

SET(find_telldus_SRCS
  find_telldus.c
)

ADD_EXECUTABLE(find_telldus
  ${find_telldus_SRCS}
)

TARGET_LINK_LIBRARIES(find_telldus
 ${FTDI_LIBRARY}
)

INSTALL(TARGETS find_telldus RUNTIME DESTINATION bin)
