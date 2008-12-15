#ifndef TELLDUSGUI_GLOBAL_H
#define TELLDUSGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TELLDUSGUI_LIBRARY)
#  define TELLDUSGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define TELLDUSGUISHARED_EXPORT Q_DECL_IMPORT
#endif

class QWidget;

extern "C" {
	QWidget *tdDeviceWidget( QWidget *parent = 0 );
}

#endif // TELLDUSGUI_GLOBAL_H
