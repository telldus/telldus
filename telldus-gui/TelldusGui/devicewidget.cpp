#include "devicewidget.h"

#include <QApplication>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QMenu>

#include <QDebug>

#include "telldusgui.h"
#include "editdevicedialog.h"
#include "editgroupdialog.h"
#include "methodwidget.h"

DeviceWidget::DeviceWidget(QWidget *parent) :
	QWidget(parent),
	deviceView(this),
	addToolButton(this),
	removeToolButton(this),
	editToolButton(this)
{
	deviceView.setModel( &model );
	deviceView.resizeColumnsToContents();
	deviceView.resizeRowsToContents();
	connect( &deviceView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(listActivated(const QModelIndex &)) );
	connect(&model, SIGNAL(showMessage(const QString &, const QString &, const QString &)), this, SIGNAL(showMessage(const QString &, const QString &, const QString &)));
	connect(&model, SIGNAL(eventTriggered(const QString &, const QString &)), this, SIGNAL(eventTriggered(const QString &, const QString &)));
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(&deviceView);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing(0);

	QMenu *newMenu = new QMenu( this );
	QAction *newDeviceMenuAction = newMenu->addAction( tr("New device...") );
	connect(newDeviceMenuAction, SIGNAL(triggered()), this, SLOT(addDevice()));
	QAction *newGroupMenuAction = newMenu->addAction( tr("New group...") );
	connect(newGroupMenuAction, SIGNAL(triggered()), this, SLOT(addGroup()));

	addToolButton.setIcon( QIcon( ":/images/list-add.png" ) );
	addToolButton.setText( tr("New") );
	addToolButton.setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	//addToolButton.setPopupMode( QToolButton::MenuButtonPopup );
	//addToolButton.setMenu( newMenu );
	connect(&addToolButton, SIGNAL(clicked()), this, SLOT(addDevice()));
	buttonLayout->addWidget( &addToolButton );

	buttonLayout->addSpacing( 10 );

	editToolButton.setIcon( QIcon( ":/images/list-edit.png" ) );
	editToolButton.setText( tr("Edit") );
	editToolButton.setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	editToolButton.setEnabled( false );
	connect(&editToolButton, SIGNAL(clicked()), this, SLOT(editDevice()));
	buttonLayout->addWidget( &editToolButton );

	removeToolButton.setIcon( QIcon( ":/images/list-remove.png" ) );
	removeToolButton.setEnabled( false );
	removeToolButton.setText( tr("Remove") );
	removeToolButton.setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
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

void DeviceWidget::addDevice() {
	Device device(0, 0);

	EditDeviceDialog *dialog = new EditDeviceDialog(&device);
	if (dialog->exec() == QDialog::Accepted) {
		device.save();
		//Select the new row
// 		QModelIndex index = model.index(model.rowCount()-1, 0, QModelIndex());
// 		deviceView.setCurrentIndex(index);
	}

	delete dialog;
}

void DeviceWidget::addGroup() {
	/*Device *device = model.newDevice();

	EditGroupDialog *dialog = new EditGroupDialog(device, &model);
	if (dialog->exec() == QDialog::Accepted) {
		device->save();
	} else {
		delete device;
	}

	delete dialog;*/
}

void DeviceWidget::deleteDevice() {
	QMessageBox msgBox;
	msgBox.setText( tr("Are you sure you want to remove the selected device?") );
	msgBox.setInformativeText( tr("The device will be removed permanently from this application as well as all other applications.") );
	msgBox.setIcon( QMessageBox::Warning );
	msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	msgBox.setDefaultButton( QMessageBox::No );
	if ( msgBox.exec() ==  QMessageBox::Yes) {
		QModelIndex index = deviceView.currentIndex();
		Device *device = model.device(index);
		if (device) {
			device->remove();
		}
	}
}

void DeviceWidget::editDevice() {
	QModelIndex index = deviceView.currentIndex();
	Device device( model.deviceId(index), 0 );

	QDialog *dialog;
	if (device.deviceType() == TELLSTICK_TYPE_GROUP) {
		dialog = new EditGroupDialog( &device, &model );
	} else {
		dialog = new EditDeviceDialog( &device );
	}
	if (dialog->exec() == QDialog::Accepted) {
		device.save();
	}

	delete dialog;
}

void DeviceWidget::listActivated(const QModelIndex &) {
	removeToolButton.setEnabled( true );
	editToolButton.setEnabled( true );
}
