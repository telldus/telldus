PROJECT(tdtool)

cmake_policy(SET CMP0005 NEW)

SET (tdtool_DESCRIPTION
	"a command line utility to control a Telldus TellStick"
)

SET(tdtool_SRCS
  main.cpp
)

ADD_EXECUTABLE(tdtool
  ${tdtool_SRCS}
)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/driver
)

ADD_DEFINITIONS( -DVERSION="${DISPLAYED_VERSION}" )

IF (WIN32)
	FIND_LIBRARY(TELLDUSCORE_LIBRARY TelldusCore)
	TARGET_LINK_LIBRARIES(tdtool
		${TELLDUSCORE_LIBRARY}
	)
ELSEIF (APPLE)	
	TARGET_LINK_LIBRARIES(tdtool
		TelldusCore
	)
ELSE (WIN32)	
	TARGET_LINK_LIBRARIES(tdtool
		telldus-core
	)
ENDIF (WIN32)

IF (UNIX)
	IF (GENERATE_MAN)
		ADD_CUSTOM_COMMAND(
			TARGET tdtool
			POST_BUILD
			COMMAND help2man -n ${tdtool_DESCRIPTION} ./tdtool > tdtool.1
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating man file tdtool.1"
		)
		INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tdtool.1 DESTINATION share/man/man1)
	ENDIF (GENERATE_MAN)
ENDIF (UNIX)

INSTALL(TARGETS tdtool RUNTIME DESTINATION bin)
