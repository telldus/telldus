#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QTableView>

class DeviceView : public QTableView
{
	Q_OBJECT
public:
	DeviceView( QWidget *parent = 0);
	virtual void setModel ( QAbstractItemModel * model );

private slots:
	void rowsUpdated ( const QModelIndex & parent, int start, int end );
};

#endif // DEVICEVIEW_H
