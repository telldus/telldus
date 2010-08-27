#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QStackedLayout>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>

#include "tellduscenterapplication.h"
#include "tellduscenterplugin.h"
#include "message.h"
#include "plugintree.h"

class MainWindowPrivate {
public:
	QToolBar *pagesBar;
	Message *message;
	QStackedLayout *stackedLayout;
	PluginTree pluginTree;
	QActionGroup *pagesActionGroup;
};

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	d = new MainWindowPrivate;
	d->pagesActionGroup = new QActionGroup( this );
	d->message = new Message(this);
	connect(qApp, SIGNAL(showMessage(QString,QString,QString)), d->message, SLOT(showMessage(QString,QString,QString)));

	setAttribute(Qt::WA_DeleteOnClose, true);

	// Restore size and position
	QSettings settings;
	resize(settings.value("Size", size()).toSize());
	move(settings.value("Pos", pos()).toPoint());

	statusBar()->setSizeGripEnabled(true);
	setupMenu();

	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	centralWidget->setLayout(layout);

	layout->addWidget(d->message);

	d->stackedLayout = new QStackedLayout;
	layout->addLayout(d->stackedLayout);

	setCentralWidget(centralWidget);

	setupToolBar();

	setWindowTitle( tr("Telldus Center") );
}

MainWindow::~MainWindow()
{
	delete d;
}

void MainWindow::closeEvent( QCloseEvent *event ) {
	QSettings settings;
	settings.setValue("Size", size());
	settings.setValue("Pos", pos());

	this->hide();
	event->ignore();
}

void MainWindow::setupMenu() {
	menuBar()->clear();

	// File
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QAction *quit = fileMenu->addAction(tr("&Quit"), qApp, SLOT(quit()));
	quit->setMenuRole( QAction::QuitRole );

	// Help
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addSeparator();
	QAction *aboutQt = helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	aboutQt->setMenuRole( QAction::AboutQtRole );
	QAction *aboutTelldusCenter = helpMenu->addAction(tr("About &Telldus Center"), this, SLOT(slotAboutApplication()));
	aboutTelldusCenter->setMenuRole( QAction::AboutRole );
}

void MainWindow::setupToolBar()
{
	setUnifiedTitleAndToolBarOnMac(true);
	d->pagesBar = addToolBar(tr("Pages"));
	d->pagesBar->setIconSize(QSize(32, 32));
}

void MainWindow::addWidget( const QString &page, const QIcon &icon, QWidget *widget ) {
	QString topLevel = d->pluginTree.page( page );

	bool pageExists = d->pluginTree.pages().contains( topLevel );
	d->pluginTree.add( page, widget );
	if (!pageExists) {
		QAction *action = new QAction( icon, page, this );
		action->setCheckable( true );
		action->setChecked( false );

		int index = d->stackedLayout->addWidget( widget );
		action->setData( index );

		connect(action, SIGNAL(triggered()), this, SLOT(slotPagesClick()));
		d->pagesActionGroup->addAction( action );
		//toolbarIcons.insert(page);
		d->pagesBar->addAction( action );
	}

	if (d->pagesActionGroup->actions().count() == 1) { //First icon
		d->pagesActionGroup->actions().first()->setChecked( true );
	}
}

void MainWindow::slotAboutApplication() {
	QMessageBox::about(this, tr("About Telldus Center"),
					   tr("<center><img src=':/images/TelldusCenter_128.png' width=128 height=128 /><h2>Telldus Center 2.0.3</h2>"
						  "<p>Copyright &copy; 2010 Telldus Technologies AB<p></center>"));
}

void MainWindow::slotPagesClick() {
	QAction *action = qobject_cast<QAction *>(sender());
	if (action) {
		d->stackedLayout->setCurrentIndex(action->data().toInt());
	}
}
