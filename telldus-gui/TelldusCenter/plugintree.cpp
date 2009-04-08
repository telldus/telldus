#include "plugintree.h"

#include <QDebug>

typedef QHash<QString, QHash<QString, QWidget *> > PluginList;

class PluginTreePrivate {
public:
	PluginList plugins;
};

PluginTree::PluginTree()
		:d( new PluginTreePrivate() )
{
}

PluginTree::~PluginTree() {
	delete d;
}

void PluginTree::add( const QString &path, QWidget *widget ) {
	QString topLevel( page(path) );
	QString subLevel( widgetName(path) );
	if (!d->plugins[topLevel].contains( subLevel )) {
		d->plugins[topLevel][subLevel] = widget;
	}
}

QString PluginTree::page( const QString &path ) {
	return path.section('.', 0, 0);
}

QString PluginTree::widgetName( const QString &path ) {
	return path.section('.', 1, 1);
}

QStringList PluginTree::pages() const {
	QStringList pages;
	for( PluginList::const_iterator it = d->plugins.begin(); it != d->plugins.end(); ++it ) {
		pages.append( it.key() );
	}
	return pages;
}
