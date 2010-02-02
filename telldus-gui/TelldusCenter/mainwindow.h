#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QHash>

class QToolBar;
class Message;

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

public slots:
	void addWidget( const QString &page, const QIcon &icon, QWidget *widget );

protected:
	virtual void closeEvent( QCloseEvent *event );

private slots:
	void slotAboutApplication();
	void slotPagesClick();

private:
	void setupMenu();
	void setupToolBar();

private:
	MainWindowPrivate *d;
};

#endif // MAINWINDOW_H
