#pragma once

#include "stdafx.h"
#include "NFCReader.h"
#include "NFCTag.h"

class UltralightTag : public NFCTag
{
public:
	UltralightTag( SCARDHANDLE handle, int proto, NFCReader* reader, uint8_t* ATRbytes ):
		NFCTag( handle, proto, reader, ATRbytes ) 
	{
	}

	//void NDEFFormat()
	//{
	//	uint8_t[] check;

	//	check = Read( 0x02 );
	//	if( check[2] != 0x00 || check[3] != 0x00 )
	//		throw new Exception( "Format failure: tag is locked!" );

	//	check = Read( 0x03 );
	//	if( check[0] != 0x00 || check[1] != 0x00 || check[2] != 0x00 || check[3] != 0x00 )
	//		throw new Exception( "Format failure: tag is formatted!" );

	//	//Write(0x03, new uint8_t[] { 0xE1, 0x10, 0x06, 0x00 }); // CC for Ultralight
	//	Write( 0x03, new uint8_t[]{ 0xE1, 0x10, 0x12, 0x00 } );  // CC for NTAG203/UltralightC

	//	Write( 0x04, new uint8_t[]{ 0x03, 0x00, 0xFE, 0x00 } );  // Empty NDEF record plus terminator
	//}

	void LoadKey( KeyTypes keyType, uint8_t* keyData );
	void Authenticate( KeyTypes keyType, uint8_t sector );

	int Read( uint8_t page, uint8_t* dest );
	void Write( uint8_t page, uint8_t* data, int len );

	int ReadAll( uint8_t numPages, uint8_t* dest );

	void WriteAll( uint8_t* data, int len );

	void Lock();
};
