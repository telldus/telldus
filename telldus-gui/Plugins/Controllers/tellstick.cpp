#include "tellstick.h"
#include <telldus-core.h>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QDebug>

#ifdef _WINDOWS
#include <windows.h>
#endif
#include "ftd2xx.h"

inline void msleep( const int msec) {
#ifdef _WINDOWS
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}

class TellStick::PrivateData {
public:
	FT_HANDLE ftHandle;
};

TellStick::TellStick(int id, int type, const QString &name, QObject *parent) :
	Controller(id, type, name, parent)
{
	d = new PrivateData;
}

TellStick::~TellStick() {
	delete d;
}

void TellStick::setAvailable(bool available) {
	Controller::setAvailable(available);
	if (!available && upgradeStep() == 0) {
		//Doing firmware upgrade
		setUpgradeStep(1);
		QTimer::singleShot(1000, this, SLOT(aquireTellStick())); //Allow for widget updates
	}
}

bool TellStick::isUpgradable() const {
	QString fw = firmware();
	if (fw == "?") {
		return false;
	}
	int firmware = fw.toInt();
	if (type() == 1) {
		//TellStick
		if (firmware <= 3) {
			return false;
		}
		if (firmware < 6) {
			return true;
		}

	} else if (type() == 2) {
		//TellStick Duo
		if (firmware < 10) {
			return true;
		}
	}
	return false;
}

void TellStick::aquireTellStick() {
	char *tempSerial = new char[serial().size()+1];
#ifdef _WINDOWS
	strcpy_s(tempSerial, serial().size()+1, serial().toLocal8Bit());
#else
	strcpy(tempSerial, serial().toLocal8Bit());
	int pid = 0x0C30;
	if (type() == 2) {
		pid = 0x0C31;
	}
	FT_SetVIDPID(0x1781, pid);
#endif
	FT_STATUS ftStatus = FT_OpenEx(tempSerial, FT_OPEN_BY_SERIAL_NUMBER, &d->ftHandle);
	delete tempSerial;
	if (ftStatus != FT_OK) {
		return;
	}
	//open = true;

	if (type() == 2) {
		FT_SetBaudRate(d->ftHandle, 115200);
	} else {
		FT_SetBaudRate(d->ftHandle, 9600);
	}
	FT_SetFlowControl(d->ftHandle, FT_FLOW_NONE, 0, 0);
	FT_SetTimeouts(d->ftHandle,1000,0);

	setUpgradeStep(2);
	QTimer::singleShot(0, this, SLOT(enterBootloader()));
}

void TellStick::enterBootloader() {
	FT_Purge(d->ftHandle, FT_PURGE_RX | FT_PURGE_TX);

	FT_STATUS ftStatus = FT_SetBitMode(d->ftHandle, 0xff, 0x20);
	for (int i = 0; i < 100; ++i) {
		msleep(1);
		QApplication::processEvents();
	}
	ftStatus = FT_SetBitMode(d->ftHandle, 0xf0, 0x20);

	char buf = 0;
	while(1) { //TODO: Make it possible to bail!
		QApplication::processEvents();
		buf = getCh();
		if (buf == 'g') {
			break;
		}
	}
	send('r');
	setUpgradeStep(3);
	QTimer::singleShot(1000, this, SLOT(downloadFirmware()));
}

void TellStick::downloadFirmware() {
	QString filename = "TellStick";
	int bootloaderStart = 0x3A00;
	if (type() == 2) {
		filename = "TellStickDuo";
		bootloaderStart = 0x7A00;
	}
	QString path;
	//if (QApplication::arguments().count() > 1) {
	//	path = QApplication::arguments().at(1);
	//} else {
		path = QString(":/firmware/%1.hex").arg(filename);
	//}

	QByteArray data = readHex(path, bootloaderStart);
	int bytesLeft = 0, i = 0;
	char byte;

	while (i < data.length()) {
		QApplication::processEvents();
		byte = getCh();
		QApplication::processEvents();
		if (byte == 'b') {
			bytesLeft = data.length() - i;
			if (bytesLeft > 0xFF) {
				bytesLeft = 0xFF;
			}
			send(bytesLeft);
		} else if (byte == 'd') {
			send(data[i]);
			--bytesLeft;
			++i;
			this->setUpgradeProgress( (qreal)i/(qreal)data.length()*100.0 );
		}
	}
	setUpgradeStep(4);
	QTimer::singleShot(0, this, SLOT(rebootTellStick()));
}

void TellStick::rebootTellStick() {
	FT_STATUS ftStatus = FT_SetBitMode(d->ftHandle, 0xff, 0x20);
	for (int i = 0; i < 100; ++i) {
		msleep(1);
		QApplication::processEvents();
	}
	ftStatus = FT_SetBitMode(d->ftHandle, 0xf0, 0x20);
	FT_Close(d->ftHandle);
	QApplication::processEvents();
	setUpgradeStep(5);
	QTimer::singleShot(0, this, SLOT(connectTellStick()));
}

void TellStick::connectTellStick() {
	int pid = 0x0C30;
	if (type() == 2) {
		pid = 0x0C31;
	}
	tdConnectTellStickController(0x1781, pid, this->serial().toUtf8());
	setUpgradeStep(-1);
	emit upgradableChanged();
	emit upgradeDone();
}

char TellStick::getCh() {
	char buf = 0;
	DWORD dwBytesRead = 0;
	FT_Read(d->ftHandle, &buf, sizeof(buf), &dwBytesRead);
	return buf;
}

void TellStick::send(uchar ch) {
	DWORD bytesWritten;
	FT_Write(d->ftHandle, &ch, sizeof(ch), &bytesWritten);
}

QByteArray TellStick::readHex(const QString &filename, int maxAddress) {
	QByteArray data;

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return "";
	}
	while( !file.atEnd() ) {
		QByteArray fileLine = file.readLine();

		if (fileLine[0] != ':' || fileLine.length() < 11) {
			// skip line if not hex line entry,or not minimum length ":BBAAAATTCC"
			continue;
		}

		int byteCount = parseHex(fileLine, 1, 2);
		int startAddress = parseHex(fileLine, 3, 4);
		int recordType = parseHex(fileLine, 7, 2);

		if (recordType == 1) {
			//End of file, break
			break;
		}
		if (recordType == 2) {
			//Not implemented yet
		} else if (recordType == 4) {
			//Extended Linear Address Record not supported
			break;
		} else if (recordType == 0) { //Data record
			if (fileLine.length() < (11+ (2*byteCount))) {
				// skip if line isn't long enough for bytecount.
				continue;
			}
			//Protect us from overwriting the bootloader
			if (startAddress >= maxAddress) {
				continue;
			}
			//Pad with empty data when needed
			if ((startAddress > data.size())) {
				while (startAddress > data.size()) {
					data.append((char)0xFF);
				}
			}

			for (int lineByte = 0; lineByte < byteCount; lineByte++) {
				unsigned char hex = (unsigned char)parseHex(fileLine, 9 + (2 * lineByte), 2);
				data.append(hex);
			}
		}
	}
	for (int i = 0; i < 64; ++i) { //At least 64-bytes extra so the last block will be written to the memory
		data.append((char)0xff);
	}
	return data;
}

int TellStick::parseHex(const QByteArray &characters, int start, int length) {
	int integer = 0;

	for (int i = 0; i < length; i++) {
		integer *= 16;
		switch(characters[start + i]) {
		case '1':
			integer += 1;
			break;

		case '2':
			integer += 2;
			break;

		case '3':
			integer += 3;
			break;

		case '4':
			integer += 4;
			break;

		case '5':
			integer += 5;
			break;

		case '6':
			integer += 6;
			break;

		case '7':
			integer += 7;
			break;

		case '8':
			integer += 8;
			break;

		case '9':
			integer += 9;
			break;

		case 'A':
		case 'a':
			integer += 10;
			break;

		case 'B':
		case 'b':
			integer += 11;
			break;

		case 'C':
		case 'c':
			integer += 12;
			break;

		case 'D':
		case 'd':
			integer += 13;
			break;

		case 'E':
		case 'e':
			integer += 14;
			break;

		case 'F':
		case 'f':
			integer += 15;
			break;
		}
	}
	return integer;
}

void TellStick::upgrade() {
	setUpgradeStep(0);
	int pid = 0x0C30;
	if (type() == 2) {
		pid = 0x0C31;
	}
	tdDisconnectTellStickController(0x1781, pid, serial().toUtf8()); //TODO: Make dynamic for TellStick Duo
}
