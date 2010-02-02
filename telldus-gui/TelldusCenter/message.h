#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

class QLabel;
class QTimer;

class Message : public QWidget
{
	Q_OBJECT
public:
	Message( QWidget *parent = 0 );

public slots:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );

private slots:
	void hideMessage();

private:
	QLabel *p_label;
	QTimer *p_timer;
};

#endif // MESSAGE_H
