#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QVariant>

//class MessagePrivate;
namespace TelldusService {
	class Message : public QByteArray {
	public:
		Message();
		Message(const QByteArray &);
		~Message(void);
	
		void addArgument(const QByteArray &);
		void addArgument(int);
		void addArgument(const QVariant &);
		void addArgument(const char *);
	
		static QVariant takeFirst(QByteArray *);
	
		//MessagePrivate *d;
	};
}

#endif //MESSAGE_H
