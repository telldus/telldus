FILE(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*Test.cpp" )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)

ADD_LIBRARY(TelldusCommonTests SHARED ${SRCS} )

TARGET_LINK_LIBRARIES( TelldusCommonTests TelldusCommon )
ADD_DEPENDENCIES( TelldusCommonTests TelldusCommon )

