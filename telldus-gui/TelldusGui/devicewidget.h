#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolButton>
#include "devicemodel.h"


class DeviceWidget : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(DeviceWidget)
public:
	explicit DeviceWidget(QWidget *parent = 0);
	virtual ~DeviceWidget();

protected:
	virtual void changeEvent(QEvent *e);

private slots:
	void listActivated(const QModelIndex &);
	void addDevice();
	void deleteDevice();

private:
	DeviceModel model;
	QTableView deviceView;
	QToolButton addToolButton, removeToolButton, editToolButton;
};

#endif // DEVICEWIDGET_H
