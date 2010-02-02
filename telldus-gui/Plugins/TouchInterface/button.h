#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>

class ButtonPrivate;

class Button : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
public:
	Button(const QRectF &rect, const QBrush &brush, QObject *parent = 0);
	virtual ~Button();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * );

	void open( QRectF to );
	void close();

	void setText( const QString &text );

signals:
	void clicked();

private slots:
	void updateFrame( int frame );

private:
	ButtonPrivate *d;
};

#endif // BUTTON_H
