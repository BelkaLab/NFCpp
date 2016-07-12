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
	bUID = new uint8_t[32];
	reader->GetUID( handle, proto, bUID );
	UID = BytesToHex(bUID, 32);
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