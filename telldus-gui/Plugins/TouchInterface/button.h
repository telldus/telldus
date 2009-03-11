#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>

class Button : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
public:
	Button(const QRectF &rect, const QBrush &brush, QObject *parent = 0);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * );

signals:
	void clicked();
};

#endif // BUTTON_H
