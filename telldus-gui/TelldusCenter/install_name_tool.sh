#!/bin/bash

PLUGINS="Devices SystrayIcon TelldusCorePlugin TelldusLive"
VERSION="2.0.104"

copy_framework() {
	echo "Copy framework $1"
	mkdir TelldusCenter.app/Contents/Frameworks/$1.framework
	cp -R /Library/Frameworks/$1.framework/ TelldusCenter.app/Contents/Frameworks/$1.framework/
}

strip_qt_development_files() {
        # Strip development-files
        rm -r ${2}/${1}_debug.dSYM
        rm    ${2}/${1}_debug*
        rm    ${2}/Versions/4/${1}_debug
        rm -r ${2}/Versions/4/Headers
}

change_telldus_core() {
	TPATH=`otool -L ${1} | grep "${VERSION}/TelldusCore" | cut -f2 | cut -d ' ' -f1`
        if [ "$TPATH" != "" ]; then
                echo "* File ${1} is linked against TelldusCore"
                install_name_tool -change $TPATH "/Library/Frameworks/TelldusCore.framework/Versions/${VERSION}/TelldusCore" ${1}
        fi

}

if [ ! -d /Library/Frameworks/TelldusCore.framework/Versions/Current/Libraries ]; then
	mkdir /Library/Frameworks/TelldusCore.framework/Versions/Current/Libraries
fi
if [ ! -d TelldusCenter.app/Contents/Frameworks ]; then
	mkdir TelldusCenter.app/Contents/Frameworks
fi

### TelldusCore ###
echo "Fixing TelldusCore"
FTD2XX_VERSION="1.0.2"
if [ ! -f /Library/Frameworks/TelldusCore.framework/Versions/Current/Libraries/libftd2xx.${FTD2XX_VERSION}.dylib ]; then
	echo "Copy libftd2xx.${FTD2XX_VERSION}.dylib"
	cp /usr/local/lib/libftd2xx.${FTD2XX_VERSION}.dylib /Library/Frameworks/TelldusCore.framework/Versions/Current/Libraries/
	install_name_tool -id "/Library/Frameworks/TelldusCore.framework/Versions/${VERSION}/Libraries/libftd2xx.${FTD2XX_VERSION}.dylib" /Library/Frameworks/TelldusCore.framework/Versions/Current/Libraries/libftd2xx.${FTD2XX_VERSION}.dylib
fi
install_name_tool -id /Library/Frameworks/TelldusCore.framework/TelldusCore /Library/Frameworks/TelldusCore.framework/TelldusCore
install_name_tool -change /usr/local/lib/libftd2xx.${FTD2XX_VERSION}.dylib "/Library/Frameworks/TelldusCore.framework/Versions/${VERSION}/Libraries/libftd2xx.${FTD2XX_VERSION}.dylib" /Library/Telldus/TelldusService

### QtCore ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtCore.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtCore.framework/QtCore"
	copy_framework QtCore
	strip_qt_development_files QtCore TelldusCenter.app/Contents/Frameworks/QtCore.framework
	install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### QtGui ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtGui.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtGui.framework/QtGui"
	copy_framework QtGui
	strip_qt_development_files QtGui TelldusCenter.app/Contents/Frameworks/QtGui.framework
	install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### QtNetwork ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtNetwork.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtNetwork.framework/QtNetwork"
	copy_framework QtNetwork
	strip_qt_development_files QtNetwork TelldusCenter.app/Contents/Frameworks/QtNetwork.framework
	install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### QtScript ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtScript.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtScript.framework/QtScript"
	copy_framework QtScript
	strip_qt_development_files QtScript TelldusCenter.app/Contents/Frameworks/QtScript.framework
	install_name_tool -id @executable_path/../Frameworks/QtScript.framework/Versions/4/QtScript ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### TelldusGui ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/TelldusGui.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/TelldusGui.framework/TelldusGui"
	copy_framework TelldusGui
	install_name_tool -id @executable_path/../Frameworks/TelldusGui.framework/Versions/2/TelldusGui ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
	install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui ${FRAMEWORK}
fi

### TelldusCenter ###
echo "Fixing TelldusCenter"
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtScript.framework/Versions/4/QtScript @executable_path/../Frameworks/QtScript.framework/Versions/4/QtScript TelldusCenter.app/Contents/MacOS/TelldusCenter

### Plugins ###

for plugin in $PLUGINS; do
	PLUGIN_PATH="TelldusCenter.app/Contents/Plugins/script/${plugin}.dylib"
	echo "Fixing plugin ${plugin}"
	install_name_tool -id ${plugin}.dylib $PLUGIN_PATH
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $PLUGIN_PATH 
	install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $PLUGIN_PATH
	install_name_tool -change QtScript.framework/Versions/4/QtScript @executable_path/../Frameworks/QtScript.framework/Versions/4/QtScript $PLUGIN_PATH
	install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $PLUGIN_PATH
	change_telldus_core $PLUGIN_PATH 
	TPATH=`otool -L $PLUGIN_PATH | grep "/TelldusGui " | cut -f2 | cut -d ' ' -f1`
	if [ "$TPATH" != "" ]; then
		echo "  Plugin ${plugin} is linked to TelldusGui"
		install_name_tool -change $TPATH @executable_path/../Frameworks/TelldusGui.framework/TelldusGui $PLUGIN_PATH
	fi
done

echo "Fixing tdtool"
change_telldus_core /usr/local/bin/tdtool
