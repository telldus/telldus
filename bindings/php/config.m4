PHP_ARG_ENABLE(telldus, whether to enable Telldus TellStick support,
[  --enable-telldus        Enable Telldus TellStick support])

if test "$PHP_TELLDUS" = "yes"; then
	AC_DEFINE(HAVE_TELLDUS, 1, [Whether you have Telldus TellStick])

	SEARCH_PATH="/usr/local /usr"
	SEARCH_FOR="/include/telldus-core.h"
	if test -r $PHP_TELLDUS/; then # path given as parameter
		TELLDUS_DIR=$PHP_TELLDUS
	else # search default path list
		AC_MSG_CHECKING([for telldus files in default path])
		for i in $SEARCH_PATH ; do
			if test -r $i/$SEARCH_FOR; then
				TELLDUS_DIR=$i
				AC_MSG_RESULT(found in $i)
			fi
		done
	fi
	if test -z "$TELLDUS_DIR"; then
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([Please reinstall the telldus-core distribution])
	fi
	# --enable-telldus -> add include path
	PHP_ADD_INCLUDE($TELLDUS_DIR/include)
	# --enable-telldus -> check for lib and symbol presence
	LIBNAME=telldus-core
	LIBSYMBOL=tdGetNumberOfDevices
	PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
	[
		PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TELLDUS_DIR/lib, TELLDUS_SHARED_LIBADD)
		AC_DEFINE(HAVE_TELLDUSLIB,1,[ ])
	],[
		AC_MSG_ERROR([wrong telldus lib version or lib not found])
	],[
		-L$TELLDUS_DIR/lib -lm
	])
	PHP_SUBST(TELLDUS_SHARED_LIBADD)

	PHP_NEW_EXTENSION(telldus, telldus.c, $ext_shared)
fi

