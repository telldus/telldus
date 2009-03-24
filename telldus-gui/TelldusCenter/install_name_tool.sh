#!/bin/bash

# Change QtGui
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui TelldusCenter.app/Contents/Frameworks/QtGui.framework/QtGui
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2/Frameworks/QtCore.framework/Versions/4/QtCore TelldusCenter.app/Contents/Frameworks/QtGui.framework/QtGui

# Change QtScript
install_name_tool -id @executable_path/../Frameworks/QtScript.framework/Versions/4.0/QtScript TelldusCenter.app/Contents/Frameworks/QtScript.framework/QtScript
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2/Frameworks/QtCore.framework/Versions/4/QtCore TelldusCenter.app/Contents/Frameworks/QtScript.framework/QtScript

# Change TelldusGui
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2/Frameworks/QtCore.framework/Versions/4.0/QtCore TelldusCenter.app/Contents/Frameworks/TelldusGui.framework/TelldusGui
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui TelldusCenter.app/Contents/Frameworks/TelldusGui.framework/TelldusGui

# Change TelldusCenter
install_name_tool -change /Users/micke/Documents/dev/telldus-pub/trunk/telldus-core/build/driver/libtelldus-core/TelldusCore.framework/Versions/2/TelldusCore /Library/Frameworks/TelldusCore.framework/Versions/2/TelldusCore TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2/Frameworks/QtCore.framework/Versions/4.0/QtCore TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtScript.framework/Versions/4/QtScript @executable_path/../Frameworks/QtScript.framework/Versions/4.0/QtScript TelldusCenter.app/Contents/MacOS/TelldusCenter

# Change plugin Devices
install_name_tool -change TelldusGui.framework/Versions/1/TelldusGui @executable_path/../Frameworks/TelldusGui.framework/Versions/1/TelldusGui TelldusCenter.app/Contents/Plugins/libDevices.dylib
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2/Frameworks/QtCore.framework/Versions/4.0/QtCore TelldusCenter.app/Contents/Plugins/libDevices.dylib
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui TelldusCenter.app/Contents/Plugins/libDevices.dylib
