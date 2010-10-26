
FIND_LIBRARY( QCA_LIBRARY qca )

IF (WIN32)
	GET_FILENAME_COMPONENT(QCA_PATH ${QCA_LIBRARY} PATH)
	MESSAGE(${QCA_PATH})
	INCLUDE_DIRECTORIES(
		${QCA_PATH}/../include/QtCrypto/
	)

ELSE(WIN32)
	#Maybe not so pretty, should be done better
	INCLUDE_DIRECTORIES( /usr/include/qca2/QtCrypto/ )

ENDIF (WIN32)
