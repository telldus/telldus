#ifndef METHODWIDGET_H
#define METHODWIDGET_H

#include <QWidget>

class Device;

class MethodWidget : public QWidget
{
	Q_OBJECT
public:
	MethodWidget( Device *device, QWidget *parent = 0 );
	~MethodWidget();

private slots:
	void updateMethods(int newMethods);
	void dim();

private:
	class PrivateData;
	PrivateData *d;
};

#endif // METHODWIDGET_H
