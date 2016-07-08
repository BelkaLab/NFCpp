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

	virtual int Read( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* dest );
	virtual void Write( SCARDHANDLE handle, int proto, uint8_t Page, uint8_t* Data, int len );

	virtual int GetUID( SCARDHANDLE handle, int proto, uint8_t* dest );
	virtual TagType ParseATR( uint8_t* bATR, int ATRlen );

private:
	virtual int Transmit( SCARDHANDLE handle, int proto, uint8_t* command, int len, uint8_t* response );
};
