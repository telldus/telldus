#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QStackedWidget>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>


#include "../TelldusGui/telldusgui.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_pagesBar(0)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	statusBar()->setSizeGripEnabled(true);
	setupMenu();
	setupToolBar();

	//QStackedWidget *centralWidget = new QStackedWidget(this);
	QWidget *centralWidget = tdDeviceWidget(this);
	setCentralWidget(centralWidget);

	setWindowTitle( tr("Telldus Center") );
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenu()
{
	menuBar()->clear();

	// File
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	// Help
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addSeparator();
	helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	helpMenu->addAction(tr("About &Telldus Center"), this, SLOT(slotAboutApplication()));
}

void MainWindow::setupToolBar()
{
	setUnifiedTitleAndToolBarOnMac(true);
	m_pagesBar = addToolBar(tr("Pages"));
	m_pagesBar->setIconSize(QSize(32, 32));

	QActionGroup *ag = new QActionGroup(this);

	QAction *actionDevices = new QAction( QIcon(":/images/devices.png"), tr("Devices"), this );
	actionDevices->setCheckable( true );
	actionDevices->setChecked( true );
	ag->addAction(actionDevices);
	m_pagesBar->addActions( ag->actions() );
}

void MainWindow::slotAboutApplication() {
	QMessageBox::about(this, tr("About Telldus Center"),
					   tr("<h2>Telldus Center 0.1</h2>"
						  "<p>Copyright &copy; 2008 Telldus Technologies AB<p>"
						  "<p>Telldus Center is a configuration utility for Telldus TellStick&reg;</p>"));
}
