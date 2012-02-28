SET(ENABLE_TESTING	FALSE	CACHE BOOL "Enable unit tests")

SET(cpplint_filters
	-whitespace/tab,-whitespace/parens,-whitespace/line_length,-whitespace/labels
)

FUNCTION(ADD_SOURCES TARGET PATH)
	GET_TARGET_PROPERTY(SOURCES ${TARGET} SOURCES)
	FOREACH(SOURCE ${SOURCES})
		LIST(APPEND L ${PATH}/${SOURCE})
	ENDFOREACH()
	ADD_TEST(StyleGuidelines-${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/cpplint.py --filter=${cpplint_filters} ${L})
ENDFUNCTION()

IF(ENABLE_TESTING)
	ADD_SUBDIRECTORY(common)

	ADD_EXECUTABLE(TestRunner cppunit.cpp)
	TARGET_LINK_LIBRARIES(TestRunner cppunit TelldusCommonTests)
	ADD_DEPENDENCIES(TestRunner TelldusCommonTests)

	ADD_SOURCES(TelldusCommon ${CMAKE_SOURCE_DIR}/common)
	ADD_SOURCES(${telldus-core_TARGET} ${CMAKE_SOURCE_DIR}/client)
	ADD_SOURCES(${telldus-service_TARGET} ${CMAKE_SOURCE_DIR}/service)

	ADD_TEST(cppunit ${CMAKE_CURRENT_BINARY_DIR}/TestRunner)
	IF (UNIX AND NOT APPLE)
		ADD_TEST(cppcheck cppcheck --quiet --error-exitcode=2 ${CMAKE_SOURCE_DIR})
	ENDIF()
ENDIF()

