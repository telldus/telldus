#ifndef CONTROLLERLIST_H
#define CONTROLLERLIST_H

#include <QAbstractListModel>
#include <QScriptValue>

class ControllerList : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(int length READ rowCount)
public:
	explicit ControllerList(QObject *parent = 0);
	~ControllerList();

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
	void changed();

public slots:
	void save();

private:
	class PrivateData;
	PrivateData *d;

};

#endif // CONTROLLERLIST_H
