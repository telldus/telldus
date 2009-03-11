#include "button.h"
#include <QPainter>
#include <QDebug>

Button::Button(const QRectF &rect, const QBrush &brush, QObject *parent)
		: QObject(parent),
		QGraphicsRectItem(rect)
{
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(0, 0, 0, 64));
	painter->drawRoundRect(rect().translated(2, 2));

	painter->setBrush(Qt::black);
	painter->setPen(Qt::black);
	painter->drawText( rect().adjusted( 10, 10, -10, -10 ), "Hej");
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent * ) {
	emit clicked();
}
