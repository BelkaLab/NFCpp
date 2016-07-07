#include "stdafx.h"
#include "NFCTag.h"

NFCTag::NFCTag( SCARDHANDLE handle, int proto, NFCReader* reader, byte* ATRbytes )
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

void NFCTag::LoadKey( KeyTypes keyType, byte* keyData )
{
	reader->LoadKey( handle, proto, keyType, keyData );
}

void NFCTag::Authenticate( KeyTypes keyType, byte sector )
{
	reader->Authenticate( handle, proto, keyType, sector );
}

byte* NFCTag::Read( byte page )
{
	return reader->Read( handle, proto, page );
}

void NFCTag::Write( byte page, byte* data, int len )
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