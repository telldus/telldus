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
		bell( new QToolButton( this ) ),
		learn( new QToolButton( this ) )
{
//	this->setAutoFillBackground( true );
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	
	turnOff->setIcon( QIcon(":/images/state_2.png") );
	turnOff->setVisible( false );
	layout->addWidget( turnOff );

	turnOn->setIcon( QIcon(":/images/state_1.png") );
	turnOn->setVisible( false );
	layout->addWidget( turnOn );
	
	bell->setIcon( QIcon(":/images/bell.png") );
	bell->setVisible( false );
	layout->addWidget( bell );
	
	layout->addStretch();

	learn->setText( tr("Learn") );
	learn->setVisible( false );
	layout->addWidget( learn );
	
	layout->addStretch();

	connect(device, SIGNAL(methodsChanged(int)), this, SLOT(updateMethods(int)));
	connect(turnOff, SIGNAL(clicked()), device, SLOT(turnOff()));
	connect(turnOn, SIGNAL(clicked()), device, SLOT(turnOn()));
	connect(bell, SIGNAL(clicked()), device, SLOT(bell()));
	connect(learn, SIGNAL(clicked()), device, SLOT(learn()));

	updateMethods(device->methods());
}

void MethodWidget::updateMethods(int newMethods) {
	turnOff->setVisible( newMethods & TELLSTICK_TURNOFF );
	turnOn->setVisible( newMethods & TELLSTICK_TURNON );
	bell->setVisible( newMethods & TELLSTICK_BELL );
	learn->setVisible( newMethods & TELLSTICK_LEARN );
	if (newMethods == 0) {
		qDebug() << "Methods: " << newMethods;
	}
}

