#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolButton>
#include "devicemodel.h"
#include "deviceview.h"


class DeviceWidget : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(DeviceWidget)
public:
	explicit DeviceWidget(QWidget *parent = 0);
	virtual ~DeviceWidget();

signals:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

protected:
	virtual void changeEvent(QEvent *e);

private slots:
	void listActivated(const QModelIndex &);
	void addDevice();
	void addGroup();
	void deleteDevice();
	void editDevice();
	void emitError();

private:
	DeviceModel model;
	DeviceView deviceView;
	QToolButton addToolButton, removeToolButton, editToolButton;
};

#endif // DEVICEWIDGET_H
