#include "devicewidget.h"

#include <QApplication>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

DeviceWidget::DeviceWidget(QWidget *parent) :
	QWidget(parent),
	deviceView(this),
	addToolButton(this),
	removeToolButton(this)
{
	deviceView.setModel( &model );
	connect( &deviceView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(listActivated(const QModelIndex &)) );

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(&deviceView);

	QHBoxLayout *buttonLayout = new QHBoxLayout( this );
	buttonLayout->setSpacing(0);

	addToolButton.setIcon( QIcon( ":/images/list-add.png" ) );
	buttonLayout->addWidget( &addToolButton );

	removeToolButton.setIcon( QIcon( ":/images/list-remove.png" ) );
	removeToolButton.setEnabled( false );
	connect(&removeToolButton, SIGNAL(clicked()), this, SLOT(deleteDevice()));
	buttonLayout->addWidget( &removeToolButton );

	buttonLayout->addStretch();

	layout->addLayout( buttonLayout );
}

DeviceWidget::~DeviceWidget()
{
}

void DeviceWidget::changeEvent(QEvent *e)
{
	switch(e->type()) {
	case QEvent::LanguageChange:
		//m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void DeviceWidget::deleteDevice() {
	QMessageBox msgBox;
	msgBox.setText( tr("Are you sure you want to remove the selected device?") );
	msgBox.setInformativeText( tr("The device will be removed premanently from this application as well as all other applications.") );
	msgBox.setIcon( QMessageBox::Warning );
	msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	msgBox.setDefaultButton( QMessageBox::No );
	if ( msgBox.exec() ==  QMessageBox::Yes) {
		QModelIndex index = deviceView.currentIndex();
		model.removeRow(index.row());
	}
}

void DeviceWidget::listActivated(const QModelIndex &) {
	removeToolButton.setEnabled( true );
}
