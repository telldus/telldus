#ifndef TELLSTICK_H
#define TELLSTICK_H

#include "controller.h"

class TellStick : public Controller
{
	Q_OBJECT

public:
	explicit TellStick(int id = 0, int type = 1, const QString &name = "", QObject *parent = 0);
	virtual ~TellStick();

	//bool available() const;
	virtual void setAvailable(bool available);

	/*QString firmware() const;
	void setFirmware(const QString &version);

	int id() const;

	QString name() const;
	void setName(const QString &name);

	void save();

	QString serial() const;

	Q_INVOKABLE void tryRemove();

	int type() const;*/

	Q_INVOKABLE virtual void upgrade();

protected:
	virtual bool isUpgradable() const;

private slots:
	void aquireTellStick();
	void enterBootloader();
	void downloadFirmware();
	void rebootTellStick();
	void connectTellStick();

private:
	char getCh();
	void send(uchar ch);

	static QByteArray readHex(const QString &filename, int maxAddress);
	static int parseHex( const QByteArray &characters, int start, int length );

	class PrivateData;
	PrivateData *d;
};

#endif // TELLSTICK_H
