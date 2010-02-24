#ifndef PIPE_H
#define PIPE_H

#include <QThread>
class PipePrivate;
class Socket;

class Pipe : public QThread {
	Q_OBJECT
public:
	Pipe(QObject *parent = 0);
	~Pipe(void);

	void listen(const QString &name);

signals:
	void newConnection(Socket *);
	
protected:
	void run();

private:
	PipePrivate *d;
};

#endif //PIPE_H
