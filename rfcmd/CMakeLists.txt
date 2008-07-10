cmake_minimum_required(VERSION 2.4)

PROJECT(rfcmd)

SET(rfcmd_SRCS
  rfcmd.c
)

IF(${RFCMD_DEBUG})
	ADD_DEFINITIONS( -DRFCMD_DEBUG )
ENDIF(${RFCMD_DEBUG})

INCLUDE_DIRECTORIES(
  /usr/src/linux/include
)

ADD_EXECUTABLE(rfcmd
  ${rfcmd_SRCS}
)

INSTALL(TARGETS rfcmd RUNTIME DESTINATION bin)
