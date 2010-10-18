#ifndef CLIENT_H
#define CLIENT_H

namespace TelldusCore {
	class Client{
	public:
		~Client(void);

		static Client *getInstance();
		static void close();

	private:
		Client();

		class PrivateData;
		PrivateData *d;
		static Client *instance;
	};
}

#endif //CLIENT_H
