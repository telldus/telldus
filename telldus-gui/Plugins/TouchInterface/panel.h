#ifndef PANEL_H
#define PANEL_H

#include <QGraphicsView>

class PanelPrivate;

class Panel : public QGraphicsView
{
	Q_OBJECT
public:
	Panel(QWidget *parent = 0);
	~Panel();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void click();

private:
	PanelPrivate *d;
};

#endif // PANEL_H
