#ifndef CLIENT_H
#define CLIENT_H

namespace TelldusCore {
	class Client{
	public:
		Client();
		~Client(void);

	private:
		class PrivateData;
		PrivateData *d;
		
	};
}

#endif //CLIENT_H
