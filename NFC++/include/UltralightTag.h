#pragma once

#include "stdafx.h"
#include "NFCReader.h"
#include "NFCTag.h"

class UltralightTag : public NFCTag
{
public:
	UltralightTag( SCARDCONTEXT handle, int proto, NFCReader* reader, byte* ATRbytes ):
		NFCTag( handle, proto, reader, ATRbytes ) 
	{
	}

	//void NDEFFormat()
	//{
	//	byte[] check;

	//	check = Read( 0x02 );
	//	if( check[2] != 0x00 || check[3] != 0x00 )
	//		throw new Exception( "Format failure: tag is locked!" );

	//	check = Read( 0x03 );
	//	if( check[0] != 0x00 || check[1] != 0x00 || check[2] != 0x00 || check[3] != 0x00 )
	//		throw new Exception( "Format failure: tag is formatted!" );

	//	//Write(0x03, new byte[] { 0xE1, 0x10, 0x06, 0x00 }); // CC for Ultralight
	//	Write( 0x03, new byte[]{ 0xE1, 0x10, 0x12, 0x00 } ); // CC for NTAG203/UltralightC

	//	Write( 0x04, new byte[]{ 0x03, 0x00, 0xFE, 0x00 } ); // Empty NDEF record plus terminator
	//}

	byte* ReadAll();

	void WriteAll( byte* data, int len );

	void Lock();
};
