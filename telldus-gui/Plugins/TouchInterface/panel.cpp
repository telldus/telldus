#include "panel.h"
#include "button.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#endif
#include <QTimeLine>
#include <QVector>
#include <QDebug>

class PanelPrivate {
public:
	QVector<Button*> buttons;
	Button *animatedButton;
	QRectF from, to;
	QTimeLine *timer;
};

Panel::Panel(QWidget *parent)
		:QGraphicsView(parent),
		d( new PanelPrivate )
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFrameStyle(0);
	setBackgroundBrush( Qt::blue );

#ifndef QT_NO_OPENGL
	setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

	QRectF bounds(0, 0, 200, 200);

	setScene(new QGraphicsScene(bounds, this));

	Button *button = new Button(QRectF(-25, -25, 50, 50), Qt::red);
	button->setPos(50, 150);
	scene()->addItem(button);
	connect(button, SIGNAL(clicked()), this, SLOT(click()));
	d->buttons.append(button);

	button = new Button(QRectF(-25, -25, 50, 50), Qt::red);
	button->setPos(125, 150);
	scene()->addItem(button);
	connect(button, SIGNAL(clicked()), this, SLOT(click()));
	d->buttons.append(button);

	d->timer = new QTimeLine(400, this);
	d->timer->setFrameRange(0, 100);
	connect(d->timer, SIGNAL(frameChanged(int)), this, SLOT(tick(int)));

}

Panel::~Panel() {
	delete d;
}

void Panel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(0, 0, 0, 64));
	painter->drawRoundRect(rect().translated(2, 2));
}

void Panel::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void Panel::tick( int frame) {
	if (!d->animatedButton) {
		d->timer->stop();
		return;
	}

	float percent = (frame/100.0);
	QPointF from = d->from.topLeft(), to = d->to.topLeft();
	QPointF rect = from - percent * (from - to);
	d->animatedButton->setPos( rect );

	QTransform trans;
	trans = trans.rotate(180 * percent, Qt::YAxis);

	trans = trans.scale(percent * d->to.width() / d->animatedButton->rect().width() + 1, percent * d->to.height() / d->animatedButton->rect().height() + 1);

	d->animatedButton->setTransform( trans );
}

void Panel::click() {
	d->animatedButton = qobject_cast<Button *>(sender());
	if (d->animatedButton) {
		QPointF pos = d->animatedButton->pos();
		QRectF rect = d->animatedButton->rect();
		d->from = QRectF(pos, rect.size());
		d->to = QRectF(100, 100, 125, 125);
		d->timer->start();
	}
}
