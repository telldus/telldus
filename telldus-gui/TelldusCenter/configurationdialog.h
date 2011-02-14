#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QDir>
#include <QScriptValue>

class QScriptEngine;
class QListWidgetItem;

class ConfigurationDialog : public QDialog
{
	Q_OBJECT
public:
	explicit ConfigurationDialog(QScriptEngine *engine, QWidget *parent = 0);
	~ConfigurationDialog();

signals:
	void save();

public slots:
	QScriptValue addPage( const QString &name, const QString &file, const QString &icon );
	void open();
	void valueChanged();

private slots:
	void currentItemChanged( QListWidgetItem * current, QListWidgetItem * previous );
	void doSave();
	void okClicked();

private:
	QDir baseDir() const;
	QString currentPlugin() const;
	class PrivateData;
	PrivateData *d;
};

#endif // CONFIGURATIONDIALOG_H
