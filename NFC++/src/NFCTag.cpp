#include "stdafx.h"
#include "NFCTag.h"

NFCTag::NFCTag( SCARDHANDLE handle, int proto, NFCReader* reader, uint8_t* ATRbytes )
{
	this->handle = handle;
	this->proto = proto;
	this->reader = reader;

	// Get Tag ATR
	bATR = ATRbytes;
	ATR = BytesToHex(bATR, 32);

	// Get Tag UID
	bUID = reader->ParseUID( handle, proto );
	UID = BytesToHex(bUID, 32);
}

void NFCTag::LoadKey( KeyTypes keyType, uint8_t* keyData )
{
	reader->LoadKey( handle, proto, keyType, keyData );
}

void NFCTag::Authenticate( KeyTypes keyType, uint8_t sector )
{
	reader->Authenticate( handle, proto, keyType, sector );
}

uint8_t* NFCTag::Read( uint8_t page )
{
	return reader->Read( handle, proto, page );
}

void NFCTag::Write( uint8_t page, uint8_t* data, int len )
{
	reader->Write( handle, proto, page, data, len );
}

//NFCTag::NdefMessage NDEFRead()
//{
//	return NdefMessage.FromByteArray( ReadAll() );
//}
//
//void NFCTag::NDEFWrite( NdefMessage Message )
//{
//	WriteAll( Message.ToByteArray() );
//}

void NFCTag::Dispose()
{
	int retCode = SCardDisconnect( handle, SCARD_UNPOWER_CARD );

	if( retCode != SCARD_S_SUCCESS )
	{
		throw std::runtime_error( "Failed diconnection!" );
	}
}