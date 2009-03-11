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

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void tick(int frame);
	void click();

private:
	PanelPrivate *d;
};

#endif // PANEL_H
