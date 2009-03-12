#include "panel.h"
#include "button.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#endif
#include <QVector>
#include <QDebug>

class PanelPrivate {
public:
	QVector<Button*> buttons;
};

Panel::Panel(QWidget *parent)
		:QGraphicsView(parent),
		d( new PanelPrivate )
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFrameStyle(0);
	setBackgroundBrush( Qt::gray );

#ifndef QT_NO_OPENGL
	setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

	QRectF bounds(0, 0, 200, 200);

	setScene(new QGraphicsScene(bounds, this));

	Button *button = new Button(QRectF(-25, -25, 50, 50), Qt::red);
	button->setText( "Kitchen" );
	button->setPos(40, 170);
	scene()->addItem(button);
	connect(button, SIGNAL(clicked()), this, SLOT(click()));
	d->buttons.append(button);

	button = new Button(QRectF(-25, -25, 50, 50), Qt::red);
	button->setText( "Bedroom" );
	button->setPos(100, 170);
	scene()->addItem(button);
	connect(button, SIGNAL(clicked()), this, SLOT(click()));
	d->buttons.append(button);

	button = new Button(QRectF(-25, -25, 50, 50), Qt::red);
	button->setText( "Livingroom" );
	button->setPos(160, 170);
	scene()->addItem(button);
	connect(button, SIGNAL(clicked()), this, SLOT(click()));
	d->buttons.append(button);
}

Panel::~Panel() {
	delete d;
}

void Panel::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void Panel::click() {
	Button *button = qobject_cast<Button *>(sender());
	if (button) {
		foreach( Button *b, d->buttons ) {
			if (b == button) {
				b->open( QRectF(100, 90, 140, 120) );
			} else {
				b->close();
			}
		}
	}
}
