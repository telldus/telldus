#ifndef XPLMESSAGE_H
#define XPLMESSAGE_H

#ifdef _WINDOWS
	#ifdef qtxpl_EXPORTS
		#define QTXPL_EXPORT __declspec(dllexport)
	#else
		#define QTXPL_EXPORT __declspec(dllimport)
	#endif
#else
	#define QTXPL_EXPORT
#endif

#include <QObject>
#include <QMetaType>

class QTXPL_EXPORT xPLMessage : public QObject
{
	Q_OBJECT

	Q_ENUMS( Identifier );
public:
	enum Identifier { xplcmnd, xplstat, xpltrig };

	xPLMessage( const xPLMessage & ); //Copy constructor
	xPLMessage& operator=( xPLMessage ); //Copy assignment operator

	xPLMessage(Identifier identifier);
	xPLMessage();
	virtual ~xPLMessage();

	static xPLMessage *createMessageFromString( const QString &message );

public slots:
	void addBodyItem( const QString &key, const QString &value );
	void addHeadItem( const QString &key, const QString &value );

	QString bodyItem( const QString &key ) const;
	QString headItem( const QString &key ) const;
	QString messageSchemeIdentifier() const;
	QString source() const;
	QString target() const;

	void setMessageSchemeIdentifier( const QString &msi );
	void setSource( const QString &value );
	void setTarget( const QString &value );

	QString toString() const;

private:
	class xPLMessagePrivate;
	xPLMessagePrivate *d;
};

Q_DECLARE_METATYPE(xPLMessage*)

#endif // XPLMESSAGE_H
