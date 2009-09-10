#!/bin/bash

PLUGINS="Devices SystrayIcon TelldusCorePlugin"

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


if [ ! -d /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks ]; then
	mkdir /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks
fi
if [ ! -d /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries ]; then
	mkdir /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries
fi
if [ ! -d TelldusCenter.app/Contents/Frameworks ]; then
	mkdir TelldusCenter.app/Contents/Frameworks
fi

### QtCore ###
if [ ! -d /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework ]; then
	echo "Copy framework QtCore"
	cp -R /Library/Frameworks/QtCore.framework /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/
	install_name_tool -id /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore

	strip_qt_development_files QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework
fi

### TelldusCore ###
echo "Fixing TelldusCore"
if [ ! -f /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries/libftd2xx.0.1.4.dylib ]; then
	echo "Copy libftd2xx.0.1.4.dylib"
	cp /usr/local/lib/libftd2xx.0.1.4.dylib /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries/
	install_name_tool -id /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries/libftd2xx.0.1.4.dylib /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries/libftd2xx.0.1.4.dylib
fi
install_name_tool -id /Library/Frameworks/TelldusCore.framework/TelldusCore /Library/Frameworks/TelldusCore.framework/TelldusCore
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore /Library/Frameworks/TelldusCore.framework/TelldusCore
install_name_tool -change /usr/local/lib/libftd2xx.0.1.4.dylib /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Libraries/libftd2xx.0.1.4.dylib /Library/Frameworks/TelldusCore.framework/TelldusCore

### QtGui ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtGui.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtGui.framework/QtGui"
	copy_framework QtGui
	strip_qt_development_files QtGui TelldusCenter.app/Contents/Frameworks/QtGui.framework
	install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### QtNetwork ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtNetwork.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtNetwork.framework/QtNetwork"
	copy_framework QtNetwork
	strip_qt_development_files QtNetwork TelldusCenter.app/Contents/Frameworks/QtNetwork.framework
	install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### QtScript ###
if [ ! -d TelldusCenter.app/Contents/Frameworks/QtScript.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/QtScript.framework/QtScript"
	copy_framework QtScript
	strip_qt_development_files QtScript TelldusCenter.app/Contents/Frameworks/QtScript.framework
	install_name_tool -id @executable_path/../Frameworks/QtScript.framework/Versions/4.0/QtScript ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4/QtCore ${FRAMEWORK}
fi

### TelldusGui ###
if [ -L TelldusCenter.app/Contents/Frameworks/TelldusGui.framework ]; then
	echo "Framework TelldusGui is a symlink, removing"
	rm TelldusCenter.app/Contents/Frameworks/TelldusGui.framework
fi
if [ ! -d TelldusCenter.app/Contents/Frameworks/TelldusGui.framework ]; then
	FRAMEWORK="TelldusCenter.app/Contents/Frameworks/TelldusGui.framework/TelldusGui"
	copy_framework TelldusGui
	install_name_tool -id @executable_path/../Frameworks/TelldusGui.framework/Versions/2/TelldusGui ${FRAMEWORK}
	install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore ${FRAMEWORK}
	install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui ${FRAMEWORK}
fi

### TelldusCenter ###
echo "Fixing TelldusCenter"
install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork TelldusCenter.app/Contents/MacOS/TelldusCenter
install_name_tool -change QtScript.framework/Versions/4/QtScript @executable_path/../Frameworks/QtScript.framework/Versions/4.0/QtScript TelldusCenter.app/Contents/MacOS/TelldusCenter

if [ -L TelldusCenter.app/Contents/Plugins/script/com ]; then
	echo "Scripts is a symlink, export from subversion"
	rm TelldusCenter.app/Contents/Plugins/script/com
	svn export http://svn.telldus.com/svn/tellstick/trunk/telldus-gui/TelldusCenter/Plugins/script/com TelldusCenter.app/Contents/Plugins/script/com
fi


### Plugins ###

for plugin in $PLUGINS; do
	PLUGIN_PATH="TelldusCenter.app/Contents/Plugins/script/lib${plugin}.dylib"
	echo "Fixing plugin ${plugin}"
	install_name_tool -id lib${plugin}.dylib $PLUGIN_PATH
	install_name_tool -change QtCore.framework/Versions/4/QtCore /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/Frameworks/QtCore.framework/Versions/4.0/QtCore $PLUGIN_PATH 
	install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui $PLUGIN_PATH
	install_name_tool -change QtScript.framework/Versions/4/QtScript @executable_path/../Frameworks/QtScript.framework/Versions/4.0/QtScript $PLUGIN_PATH
	TPATH=`otool -L $PLUGIN_PATH | grep "0/TelldusCore" | cut -f2 | cut -d ' ' -f1`
	if [ "$TPATH" != "" ]; then
		echo "  Plugin ${plugin} is linked to TelldusCore"
		install_name_tool -change $TPATH /Library/Frameworks/TelldusCore.framework/Versions/2.0.0/TelldusCore $PLUGIN_PATH
	fi
	TPATH=`otool -L $PLUGIN_PATH | grep "/TelldusGui " | cut -f2 | cut -d ' ' -f1`
	if [ "$TPATH" != "" ]; then
		echo "  Plugin ${plugin} is linked to TelldusGui"
		install_name_tool -change $TPATH @executable_path/../Frameworks/TelldusGui.framework/Versions/2/TelldusGui $PLUGIN_PATH
	fi
done
