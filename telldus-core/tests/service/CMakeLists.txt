FILE(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*Test.cpp" )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

ADD_LIBRARY(TelldusServiceTests SHARED ${SRCS} )

TARGET_LINK_LIBRARIES( TelldusServiceTests TelldusServiceStatic )
ADD_DEPENDENCIES( TelldusServiceTests ${telldus-service_TARGET} )

