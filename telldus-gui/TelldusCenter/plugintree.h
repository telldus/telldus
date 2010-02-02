#ifndef PLUGINTREE_H
#define PLUGINTREE_H

#include <QString>
#include <QWidget>

class PluginTreePrivate;

class PluginTree
{
public:
	PluginTree();
	~PluginTree();

	void add( const QString &path, QWidget *widget );
	QStringList pages() const;

	static QString page( const QString &path );
	static QString widgetName( const QString &path );

private:
	PluginTreePrivate *d;
};

#endif // PLUGINTREE_H
