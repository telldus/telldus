PROJECT( telldus-core )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.0 )

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(PACKAGE_VERSION 2)
SET(PACKAGE_SOVERSION 2.0.0)

SET(DISPLAYED_VERSION 2.0.0_beta1)

SET(BUILD_LIBTELLDUS-CORE	TRUE	CACHE BOOL "Build libtelldus-core")
SET(SUPPORT_TELLSTICK_DUO FALSE CACHE BOOL "Include support for TellStick Duo")
SET(SUPPORT_USB TRUE CACHE BOOL "Build support for usb-devices")
SET(USE_QT_SETTINGS_BACKEND FALSE CACHE BOOL "Use QT as settings backend instead of the native one")

SET(BUILD_RFCMD		TRUE	CACHE BOOL "Build rfcmd")
SET(BUILD_RFCMD_WITH_LIBFTDI	TRUE	CACHE	BOOL "Enable support for using rfcmd with libftdi (does not need kernel-driver)")
SET(BUILD_TDTOOL	TRUE	CACHE BOOL "Build tdtool")

ADD_SUBDIRECTORY(driver)

if(BUILD_TDTOOL)
	ADD_SUBDIRECTORY(tdtool)
endif(BUILD_TDTOOL)

if(BUILD_RFCMD)
	ADD_SUBDIRECTORY(rfcmd)
endif(BUILD_RFCMD)

