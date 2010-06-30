#ifndef SOCKET_H
#define SOCKET_H

#include "Message.h"
#include <QObject>

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE SOCKET_T;
	#define BUFSIZE 512
#endif

class Socket : public QObject {
	Q_OBJECT
public:
	Socket(SOCKET_T socket);
	Socket();
	~Socket(void);

	void connectToServer(const QString &serverName);
	void disconnect();
	void write(const TelldusService::Message &msg);
	void writeOverlapped(const TelldusService::Message &msg);
	std::string readOverlapped();
	std::string read();

	bool connected() const;

signals:
	void disconnected();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //SOCKET_H
