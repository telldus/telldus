#ifndef SOCKET_H
#define SOCKET_H

#include <string>

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE SOCKET_T;
#else
	typedef int SOCKET_T;
#endif

namespace TelldusCore {
	class Socket
	{
	public:
		Socket(const std::wstring &server);
		Socket(SOCKET_T hPipe);
		virtual ~Socket(void);

		std::wstring read();
		void write(const std::wstring &msg);
		
	private:
		class PrivateData;
		PrivateData *d;
	};
}
#endif //SOCKET_H
