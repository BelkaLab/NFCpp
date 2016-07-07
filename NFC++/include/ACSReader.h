#pragma once

#include "stdafx.h"
#include "NFCReader.h"
#include "Enums.h"

class NFC_API ACSReader : public NFCReader
{
public:
	ACSReader( SCARDCONTEXT handle, std::string readerName ) :
		NFCReader( handle, readerName )
	{
	};


	// Virtual functions implementations
	virtual NFCTag* Connect();

	virtual void LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, byte* keyData );
	virtual void Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, byte sector );

	virtual byte* Read( SCARDHANDLE handle, int proto, byte page );
	virtual void Write( SCARDHANDLE handle, int proto, byte Page, byte* Data, int len );

	virtual byte*   ParseUID( SCARDHANDLE handle, int proto );
	virtual TagType ParseATR( byte* bATR, int ATRlen );

private:
	virtual byte* Transmit( SCARDHANDLE handle, int proto, byte* cmdBytes, int len );
};
