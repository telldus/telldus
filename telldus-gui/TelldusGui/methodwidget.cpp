#include "methodwidget.h"
#include "device.h"

#include <telldus-core.h>

#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

class MethodWidget::PrivateData {
public:
	Device *device;
	QToolButton *turnOff, *turnOn, *bell, *learn;
};

MethodWidget::MethodWidget( Device *device, QWidget *parent )
		:QWidget(parent)
{
	d = new PrivateData;
	d->turnOff = new QToolButton( this );
	d->turnOn = new QToolButton( this );
	d->bell = new QToolButton( this );
	d->learn = new QToolButton( this );

//	this->setAutoFillBackground( true );
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	
	d->turnOff->setIcon( QIcon(":/images/state_2.png") );
	d->turnOff->setVisible( false );
	layout->addWidget( d->turnOff );

	d->turnOn->setIcon( QIcon(":/images/state_1.png") );
	d->turnOn->setVisible( false );
	layout->addWidget( d->turnOn );
	
	d->bell->setIcon( QIcon(":/images/bell.png") );
	d->bell->setVisible( false );
	layout->addWidget( d->bell );
	
	layout->addStretch();

	d->learn->setText( tr("Learn") );
	d->learn->setVisible( false );
	layout->addWidget( d->learn );
	
	layout->addStretch();

	connect(device, SIGNAL(methodsChanged(int)), this, SLOT(updateMethods(int)));
	connect(d->turnOff, SIGNAL(clicked()), device, SLOT(turnOff()));
	connect(d->turnOn, SIGNAL(clicked()), device, SLOT(turnOn()));
	connect(d->bell, SIGNAL(clicked()), device, SLOT(bell()));
	connect(d->learn, SIGNAL(clicked()), device, SLOT(learn()));

	updateMethods(device->methods());
}

MethodWidget::~MethodWidget() {
	delete d;
}

void MethodWidget::updateMethods(int newMethods) {
	d->turnOff->setVisible( newMethods & TELLSTICK_TURNOFF );
	d->turnOn->setVisible( newMethods & TELLSTICK_TURNON );
	d->bell->setVisible( newMethods & TELLSTICK_BELL );
	d->learn->setVisible( newMethods & TELLSTICK_LEARN );
	if (newMethods == 0) {
		qDebug() << "Methods: " << newMethods;
	}
}

