#pragma once

#include "stdafx.h"
#include "Enums.h"
#include "NFCReader.h"

class NFCTag
{
public:
	NFCTag() {}
	NFCTag( SCARDHANDLE handle, int Proto, NFCReader* reader, byte* ATRbytes );

	void LoadKey( KeyTypes keyType, byte* keyData );
	void Authenticate( KeyTypes keyType, byte sector );

	byte* Read( byte page );

	void Write( byte page, byte* data, int len );

	virtual byte* ReadAll() VIRTUAL_FUNC;
	virtual void WriteAll( byte* data, int len ) VIRTUAL_FUNC;

	//virtual void NDEFFormat();
	//NdefMessage NDEFRead();

	//void NDEFWrite( NdefMessage Message );

	virtual void Lock() VIRTUAL_FUNC;

	void Dispose();

protected:
	SCARDHANDLE handle;
	int proto;
	NFCReader* reader;

	byte* bATR;
	std::string ATR;
	byte* bUID;
	std::string UID;
};