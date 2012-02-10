SET(ENABLE_TESTING	FALSE	CACHE BOOL "Enable unit tests")

IF(ENABLE_TESTING)
	ADD_SUBDIRECTORY(common)

	ADD_EXECUTABLE(TestRunner cppunit.cpp)
	TARGET_LINK_LIBRARIES(TestRunner cppunit TelldusCommonTests)
	ADD_DEPENDENCIES(TestRunner TelldusCommonTests)

	ADD_TEST(cppunit ${CMAKE_CURRENT_BINARY_DIR}/TestRunner)
	IF (UNIX AND NOT APPLE)
		ADD_TEST(cppcheck cppcheck --quiet --error-exitcode=2 ${CMAKE_SOURCE_DIR})
	ENDIF()
ENDIF()

