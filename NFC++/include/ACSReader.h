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

	virtual void LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t* keyData );
	virtual void Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t sector );

	virtual uint8_t* Read( SCARDHANDLE handle, int proto, uint8_t page );
	virtual void Write( SCARDHANDLE handle, int proto, uint8_t Page, uint8_t* Data, int len );

	virtual uint8_t*   ParseUID( SCARDHANDLE handle, int proto );
	virtual TagType ParseATR( uint8_t* bATR, int ATRlen );

private:
	virtual uint8_t* Transmit( SCARDHANDLE handle, int proto, uint8_t* cmdBytes, int len );
};
