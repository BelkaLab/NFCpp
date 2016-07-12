#pragma once

#include "stdafx.h"
#include "Enums.h"
#include "NFCReader.h"

class NFCTag
{
public:
	NFCTag() {}
	NFCTag( SCARDHANDLE handle, int Proto, NFCReader* reader, uint8_t* ATRbytes );

	virtual void LoadKey( KeyTypes keyType, uint8_t* keyData ) VIRTUAL_FUNC;
	virtual void Authenticate( KeyTypes keyType, uint8_t sector ) VIRTUAL_FUNC;

	virtual int  Read ( uint8_t page, uint8_t* dest ) VIRTUAL_FUNC;
	virtual void Write( uint8_t page, uint8_t* data, int len ) VIRTUAL_FUNC;

	virtual int  ReadAll ( uint8_t numPages, uint8_t* dest ) VIRTUAL_FUNC;
	virtual void WriteAll( uint8_t numPages, uint8_t* data ) VIRTUAL_FUNC;

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