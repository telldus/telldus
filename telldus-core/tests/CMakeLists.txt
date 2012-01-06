ADD_SUBDIRECTORY(common)

ADD_EXECUTABLE(TestRunner cppunit.cpp)
TARGET_LINK_LIBRARIES(TestRunner cppunit TelldusCommonTests)
ADD_DEPENDENCIES(TestRunner TelldusCommonTests)

ADD_TEST(Tests ${CMAKE_CURRENT_BINARY_DIR}/TestRunner)

