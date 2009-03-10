#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QToolBar;
class Message;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

public slots:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );

protected:
	virtual void closeEvent( QCloseEvent *event );

private slots:
	void slotAboutApplication();

private:
	void setupMenu();
	void setupToolBar();

private:
	QToolBar *m_pagesBar;
	Message *p_message;
};

#endif // MAINWINDOW_H
