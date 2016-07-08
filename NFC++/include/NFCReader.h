#pragma once

#include "stdafx.h"
#include "Enums.h"

class NFCTag;

class NFC_API NFCReader
{
public:
	NFCReader() {}
	NFCReader( SCARDCONTEXT context, std::string readerName );
	std::string getName() const { return name; }
	SCARDCONTEXT getContext() const { return hContext; }

	// Virtual functions to be implemented by subclasses
	virtual NFCTag* Connect() VIRTUAL_FUNC;

	virtual void LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t* keyData ) VIRTUAL_FUNC;
	virtual void Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t sector ) VIRTUAL_FUNC;

	virtual uint8_t* Read( SCARDHANDLE handle, int proto, uint8_t page ) VIRTUAL_FUNC;
 	virtual void Write( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* data, int len ) VIRTUAL_FUNC;

	virtual uint8_t* ParseUID( SCARDHANDLE handle, int proto ) VIRTUAL_FUNC;
	virtual TagType ParseATR( uint8_t* bATR, int ATRLen ) VIRTUAL_FUNC;

	// pooling thread 
	//void StartPolling();
	//void StopPooling();
	//DWORD WINAPI Pooler( LPVOID lpParam );
	//TagFoundHandler tagFound;

protected:
	uint8_t* GetATR( SCARDHANDLE handle, int proto, int* ATRLen );
	NFCTag* BuildTag( SCARDHANDLE handle, int proto, uint8_t* ATR, int ATRLen );

	SCARDCONTEXT hContext;
	std::string name;
	bool stopPollingSignal;

private:
	virtual uint8_t* Transmit( SCARDHANDLE handle, int proto, uint8_t* cmdBytes, int len ) VIRTUAL_FUNC;

	HANDLE pollerThread;
};