#pragma once

#include "stdafx.h"
#include "Enums.h"

class NFCTag;

class NFC_API NFCReader
{
public:
	NFCReader();
	NFCReader( SCARDCONTEXT context, std::string readerName );
	~NFCReader();

	const char* GetName() const { return name; }
	SCARDCONTEXT GetContext() const { return hContext; }

	// Virtual functions to be implemented by subclasses
	virtual NFCTag* Connect() VIRTUAL_FUNC;

	virtual void LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t* keyData ) VIRTUAL_FUNC;
	virtual void Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t sector ) VIRTUAL_FUNC;

	virtual int Read( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* dest ) VIRTUAL_FUNC;
 	virtual void Write( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* data, int len ) VIRTUAL_FUNC;

	virtual int GetUID( SCARDHANDLE handle, int proto, uint8_t* dest ) VIRTUAL_FUNC;
	virtual TagType ParseATR( uint8_t* bATR, int ATRLen ) VIRTUAL_FUNC;

	// pooling thread 
	//void StartPolling();
	//void StopPooling();
	//DWORD WINAPI Pooler( LPVOID lpParam );
	//TagFoundHandler tagFound;

protected:
	int GetATR( SCARDHANDLE handle, int proto, uint8_t* dest );
	NFCTag* BuildTag( SCARDHANDLE handle, int proto, uint8_t* ATR, int ATRLen );

	SCARDCONTEXT hContext;
	const char* name;
	//bool stopPollingSignal;

private:
	virtual int Transmit( SCARDHANDLE handle, int proto, uint8_t* command, int len, uint8_t* response ) VIRTUAL_FUNC;

	HANDLE pollerThread;
};