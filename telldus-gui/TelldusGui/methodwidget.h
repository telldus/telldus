#ifndef METHODWIDGET_H
#define METHODWIDGET_H

#include <QWidget>

class Device;
class QToolButton;

class MethodWidget : public QWidget
{
	Q_OBJECT
public:
	MethodWidget( Device *device, QWidget *parent = 0 );

private slots:
	void updateMethods(int newMethods);

private:
	Device *device;
	QToolButton *turnOff, *turnOn, *bell, *learn;
};

#endif // METHODWIDGET_H
