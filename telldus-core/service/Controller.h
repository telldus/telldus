#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

class Controller {
	public: 
		virtual ~Controller();

		virtual int firmwareVersion() = 0;
		virtual int send( const std::string &message ) = 0;

	protected:
		Controller();
};

#endif //CONTROLLER_H