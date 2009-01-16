#include "methodwidget.h"
#include "device.h"

#include <telldus-core.h>

#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

MethodWidget::MethodWidget( Device *device, QWidget *parent )
		:QWidget(parent),
		turnOff( new QToolButton( this ) ),
		turnOn( new QToolButton( this ) ),
		bell( new QToolButton( this ) )
{
//	this->setAutoFillBackground( true );
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	turnOff->setIcon( QIcon(":/images/turn-off.png") );
	turnOff->setVisible( false );
	layout->addWidget( turnOff );

	turnOn->setIcon( QIcon(":/images/turn-on.png") );
	turnOn->setVisible( false );
	layout->addWidget( turnOn );

	bell->setIcon( QIcon(":/images/bell.png") );
	bell->setVisible( false );
	layout->addWidget( bell );

	connect(device, SIGNAL(methodsChanged(int)), this, SLOT(updateMethods(int)));
	connect(turnOff, SIGNAL(clicked()), device, SLOT(turnOff()));
	connect(turnOn, SIGNAL(clicked()), device, SLOT(turnOn()));
	connect(bell, SIGNAL(clicked()), device, SLOT(bell()));

	updateMethods(device->methods());
}

void MethodWidget::updateMethods(int newMethods) {
	turnOff->setVisible( newMethods & TELLSTICK_TURNOFF );
	turnOn->setVisible( newMethods & TELLSTICK_TURNON );
	bell->setVisible( newMethods & TELLSTICK_BELL );
	if (newMethods == 0) {
		qDebug() << "Methods: " << newMethods;
	}
}

