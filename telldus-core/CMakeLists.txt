PROJECT( telldus-core )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.0 )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

ADD_SUBDIRECTORY(driver)
ADD_SUBDIRECTORY(tdtool)
ADD_SUBDIRECTORY(rfcmd)

