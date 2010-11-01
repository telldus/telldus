PROJECT(tdadmin)

cmake_policy(SET CMP0005 NEW)

SET (tdadmin_DESCRIPTION
	"a command line utility to edit devices and controllers for Telldus TellStick"
)

SET(tdadmin_SRCS
	main.cpp
)

ADD_EXECUTABLE(tdadmin
	${tdadmin_SRCS}
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/driver
)

ADD_DEFINITIONS( -DVERSION="${DISPLAYED_VERSION}" )

IF (WIN32)
	FIND_LIBRARY(TELLDUSCORE_LIBRARY TelldusCore)
	TARGET_LINK_LIBRARIES(tdadmin
		${TELLDUSCORE_LIBRARY}
	)
ELSEIF (APPLE)
	TARGET_LINK_LIBRARIES(tdadmin
		TelldusCore
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(tdadmin
		${CMAKE_BINARY_DIR}/client/libtelldus-core.so
	)
ENDIF (WIN32)

IF (UNIX)
	IF (GENERATE_MAN)
		ADD_CUSTOM_COMMAND(
			TARGET tdadmin
			POST_BUILD
			COMMAND help2man -n ${tdadmin_DESCRIPTION} ./tdadmin > tdadmin.1
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating man file tdadmin.1"
		)
		INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tdadmin.1 DESTINATION share/man/man1)
	ENDIF (GENERATE_MAN)
ENDIF (UNIX)

INSTALL(TARGETS tdadmin RUNTIME DESTINATION sbin)
