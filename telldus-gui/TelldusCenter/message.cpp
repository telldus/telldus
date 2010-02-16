#include "message.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>

Message::Message(QWidget *parent)
		:QWidget(parent)
{
	p_label = new QLabel(this);
	p_timer = new QTimer(this);
	p_timer->setInterval( 3000 );
	p_timer->setSingleShot(true);
	connect(p_timer, SIGNAL(timeout()), this, SLOT(hideMessage()));

	this->setVisible(false);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(p_label);
}

Message::~Message() {
	qDebug() << "Why am I dying?";
}

void Message::showMessage( const QString &title, const QString &message, const QString &detailedMessage ) {
	p_label->setText( message );
	this->setVisible( true );
	p_timer->start();
}

void Message::hideMessage() {
	p_label->setText("");
	this->setVisible(false);
}
