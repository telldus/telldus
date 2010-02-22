#ifndef PIPE_H
#define PIPE_H

#include <QThread>
class PipePrivate;

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE SOCKET_T;
#endif

class Pipe : public QThread {
	Q_OBJECT
public:
	Pipe(QObject *parent = 0);
	~Pipe(void);

	void listen(const QString &name);

signals:
	void newConnection(SOCKET_T socket);
	
protected:
	void run();

private:
	PipePrivate *d;
};

#endif //PIPE_H
