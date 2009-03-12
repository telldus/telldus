#include "button.h"
#include <QPainter>
#include <QTimeLine>
#include <QDebug>

class ButtonPrivate {
public:
	bool buttonFace;
	QTimeLine *timer;
	QRectF to;
	QPointF from;
	QString text;
	QBrush brush;
};

Button::Button(const QRectF &rect, const QBrush &brush, QObject *parent)
		: QObject(parent),
		QGraphicsRectItem(rect),
		d( new ButtonPrivate )
{
	d->buttonFace = true;
	d->brush = brush;

	d->timer = new QTimeLine(400, this);
	d->timer->setFrameRange(0, 100);
	connect(d->timer, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));

}

Button::~Button() {
	delete d;
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

	if (d->buttonFace) {
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(0, 0, 0, 64));
		painter->drawRoundRect(rect().translated(2, 2));

		QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
		const QColor col = d->brush.color();
		gradient.setColorAt(0, col);
		gradient.setColorAt(1, col.dark(200));
		painter->setBrush(gradient);
		painter->setPen(QPen(Qt::black, 1));
		painter->drawRoundRect(rect());

		int flags = Qt::AlignBottom | Qt::AlignLeft | Qt::TextWordWrap;
		QFont font;
		font.setPixelSize(7);
		painter->setFont(font);
		painter->setBrush(Qt::black);
		painter->setPen(Qt::black);
		painter->drawText( rect().adjusted( 10, 10, -10, -10 ), d->text);
	} else {
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(0, 0, 0, 128));
		painter->drawRoundRect(rect());
	}

}

void Button::open( QRectF to ) {
	if (!d->buttonFace) {
		return;
	}
	d->to = to;
	d->from = scenePos();
	this->setZValue(1);
	d->timer->setFrameRange(0, 100);
	d->timer->start();
}

void Button::close() {
	if (d->buttonFace) {
		return;
	}
	this->setZValue(0);
	d->timer->setFrameRange(100, 0);
	d->timer->start();
}

void Button::setText( const QString &text ) {
	d->text = text;
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent * ) {
	emit clicked();
}

void Button::updateFrame( int frame ) {
	float percent = (frame/100.0);
	float rotation = 180.0 * percent;

	if (rotation > 90) {
		d->buttonFace = false;
	} else {
		d->buttonFace = true;
	}

	float width = 50.0 + (percent * d->to.width());
	float height = 50.0 + (percent * d->to.height());
	this->setPos( d->from - percent * (d->from - d->to.topLeft()) );
	this->setRect( QRectF(-width/2, -height/2, width, height) );
	this->setTransform( QTransform().rotate(rotation, Qt::YAxis) );
}
