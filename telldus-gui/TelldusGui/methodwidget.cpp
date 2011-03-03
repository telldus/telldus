#include "methodwidget.h"
#include "device.h"

#include <telldus-core.h>

#include <QHBoxLayout>
#include <QToolButton>
#include <QVariant>
#include <QDebug>

class MethodWidget::PrivateData {
public:
	Device *device;
	QToolButton *turnOff, *turnOn, *bell, *learn,
				*dim25, *dim50, *dim75,
				*execute, *up, *down, *stop;
};

MethodWidget::MethodWidget( Device *device, QWidget *parent )
		:QWidget(parent)
{
	d = new PrivateData;
	d->device = device;
	d->turnOff = new QToolButton( this );
	d->dim25 = new QToolButton( this );
	d->dim50 = new QToolButton( this );
	d->dim75 = new QToolButton( this );
	d->turnOn = new QToolButton( this );
	d->bell = new QToolButton( this );
	d->learn = new QToolButton( this );
	d->execute = new QToolButton( this );
	d->up = new QToolButton( this );
	d->down = new QToolButton( this );
	d->stop = new QToolButton( this );

//	this->setAutoFillBackground( true );
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	d->turnOff->setIcon( QIcon(":/images/state_2.png") );
	d->turnOff->setVisible( false );
	layout->addWidget( d->turnOff );

	d->dim25->setIcon( QIcon(":/images/state_dimmed_25.png") );
	d->dim50->setIcon( QIcon(":/images/state_dimmed_50.png") );
	d->dim75->setIcon( QIcon(":/images/state_dimmed_75.png") );
	d->dim25->setProperty("dimvalue", 64);
	d->dim50->setProperty("dimvalue", 128);
	d->dim75->setProperty("dimvalue", 192);
	d->dim25->setVisible( false );
	d->dim50->setVisible( false );
	d->dim75->setVisible( false );
	layout->addWidget( d->dim25 );
	layout->addWidget( d->dim50 );
	layout->addWidget( d->dim75 );

	d->turnOn->setIcon( QIcon(":/images/state_1.png") );
	d->turnOn->setVisible( false );
	layout->addWidget( d->turnOn );

	d->bell->setIcon( QIcon(":/images/bell.png") );
	d->bell->setVisible( false );
	layout->addWidget( d->bell );

	d->execute->setIcon( QIcon(":/images/execute.png") );
	d->execute->setVisible( false );
	layout->addWidget( d->execute );

	d->up->setIcon( QIcon(":/images/up.png") );
	d->up->setVisible( false );
	layout->addWidget( d->up );

	d->down->setIcon( QIcon(":/images/down.png") );
	d->down->setVisible( false );
	layout->addWidget( d->down );

	d->stop->setIcon( QIcon(":/images/state_2.png") );	//TODO
	d->stop->setVisible( false );
	layout->addWidget( d->stop );

	layout->addStretch();

	d->learn->setText( tr("Learn") );
	d->learn->setVisible( false );
	layout->addWidget( d->learn );

	connect(device, SIGNAL(methodsChanged(int)), this, SLOT(updateMethods(int)), Qt::QueuedConnection);
	connect(d->turnOff, SIGNAL(clicked()), device, SLOT(turnOff()));
	connect(d->dim25, SIGNAL(clicked()), this, SLOT(dim()));
	connect(d->dim50, SIGNAL(clicked()), this, SLOT(dim()));
	connect(d->dim75, SIGNAL(clicked()), this, SLOT(dim()));
	connect(d->turnOn, SIGNAL(clicked()), device, SLOT(turnOn()));
	connect(d->bell, SIGNAL(clicked()), device, SLOT(bell()));
	connect(d->learn, SIGNAL(clicked()), device, SLOT(learn()));
	connect(d->execute, SIGNAL(clicked()), device, SLOT(execute()));
	connect(d->up, SIGNAL(clicked()), device, SLOT(up()));
	connect(d->down, SIGNAL(clicked()), device, SLOT(down()));
	connect(d->stop, SIGNAL(clicked()), device, SLOT(stop()));


	updateMethods(device->methods());
}

MethodWidget::~MethodWidget() {
	delete d;
}

void MethodWidget::updateMethods(int newMethods) {
	d->turnOff->setVisible( newMethods & TELLSTICK_TURNOFF );
	d->dim25->setVisible( newMethods & TELLSTICK_DIM );
	d->dim50->setVisible( newMethods & TELLSTICK_DIM );
	d->dim75->setVisible( newMethods & TELLSTICK_DIM );
	d->turnOn->setVisible( newMethods & TELLSTICK_TURNON );
	d->bell->setVisible( newMethods & TELLSTICK_BELL );
	d->learn->setVisible( newMethods & TELLSTICK_LEARN );
	d->execute->setVisible( newMethods & TELLSTICK_EXECUTE );
	d->up->setVisible( newMethods & TELLSTICK_UP );
	d->down->setVisible( newMethods & TELLSTICK_DOWN );
	d->stop->setVisible( newMethods & TELLSTICK_STOP );
}

void MethodWidget::dim() {
	QObject *o = qobject_cast<QObject *>(sender());
	if (!o) {
		return;
	}
	d->device->dim(o->property("dimvalue").toChar().toAscii());
}
