#include "editgroupdialog.h"
#include "device.h"
#include "devicemodel.h"

#include <QVBoxLayout>
#include <QListView>
#include <QToolButton>
#include <QApplication>
#include <QDialogButtonBox>
#include <QDebug>

EditGroupDialog::EditGroupDialog(Device *d, DeviceModel *m, QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags),
		device(d),
		model(model)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *mainLayout = new QHBoxLayout;

	QListView *availableListView = new QListView( this );
	availableListView->setModel( model );
	mainLayout->addWidget( availableListView );

	QVBoxLayout *buttonsLayout = new QVBoxLayout;
	buttonsLayout->addStretch();

	QToolButton *addToolButton = new QToolButton( this );
	addToolButton->setIcon( qApp->style()->standardIcon( QStyle::SP_ArrowRight) );
	addToolButton->setEnabled( false );
	buttonsLayout->addWidget( addToolButton );

	QToolButton *removeToolButton = new QToolButton( this );
	removeToolButton->setIcon( qApp->style()->standardIcon( QStyle::SP_ArrowLeft) );
	removeToolButton->setEnabled( false );
	buttonsLayout->addWidget( removeToolButton );

	buttonsLayout->addStretch();
	mainLayout->addLayout( buttonsLayout );

	QListView *addedListView = new QListView( this );
	mainLayout->addWidget( addedListView );

	layout->addLayout( mainLayout );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	buttonBox->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Cancel );
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	layout->addWidget(buttonBox);

}

EditGroupDialog::~EditGroupDialog() {
}


void EditGroupDialog::okClicked() {

//	if (!item || !item->isDevice()) {
//		QMessageBox msgBox;
//		msgBox.setText( tr("You must choose a device") );
//		msgBox.setInformativeText( tr("Please select the device you have.") );
//		msgBox.setIcon( QMessageBox::Critical );
//		msgBox.setStandardButtons( QMessageBox::Ok );
//		msgBox.exec();
//		return;
//	}
	this->accept();
}
