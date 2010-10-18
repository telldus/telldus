#include "Client.h"
#include "Socket.h"

using namespace TelldusCore;

class Client::PrivateData {
public:
	Socket eventSocket;
};

Client *Client::instance = 0;

Client::Client() {
	d = new PrivateData;
	d->eventSocket.connect(L"TelldusEvents");
}

Client::~Client(void) {
	delete d;
}

void Client::close() {
	if (Client::instance != 0) {
		delete Client::instance;
	}
}

Client *Client::getInstance() {
	if (Client::instance == 0) {
		Client::instance = new Client();
	}
	return Client::instance;
}

int Client::getIntegerFromService(const Message &msg) {
	Socket s;
	s.connect(L"TelldusClient");
	s.write(msg.data());

	std::wstring response = s.read();
	return Message::takeInt(&response);
}

bool Client::getBoolFromService(const Message &msg) {
	return getIntegerFromService(msg) == 1;
}
