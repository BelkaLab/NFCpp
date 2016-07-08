#pragma once

#include "stdafx.h"
#include "Enums.h"
#include "NFCReader.h"

class NFCTag
{
public:
	NFCTag() {}
	NFCTag( SCARDHANDLE handle, int Proto, NFCReader* reader, uint8_t* ATRbytes );

	void LoadKey( KeyTypes keyType, uint8_t* keyData );
	void Authenticate( KeyTypes keyType, uint8_t sector );

	int Read( uint8_t page, uint8_t* dest );

	void Write( uint8_t page, uint8_t* data, int len );

	virtual int ReadAll( uint8_t numPages, uint8_t* dest ) VIRTUAL_FUNC;
	virtual void WriteAll( uint8_t* data, int len ) VIRTUAL_FUNC;

	//virtual void NDEFFormat();
	//NdefMessage NDEFRead();

	//void NDEFWrite( NdefMessage Message );

	virtual void Lock() VIRTUAL_FUNC;

	void Dispose();

protected:
	SCARDHANDLE handle;
	int proto;
	NFCReader* reader;

	uint8_t* bATR;
	std::string ATR;
	uint8_t* bUID;
	std::string UID;
};