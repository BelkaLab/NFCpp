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

	virtual void LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, byte* keyData ) VIRTUAL_FUNC;
	virtual void Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, byte sector ) VIRTUAL_FUNC;

	virtual byte* Read( SCARDHANDLE handle, int proto, byte page ) VIRTUAL_FUNC;
 	virtual void Write( SCARDHANDLE handle, int proto, byte page, byte* data, int len ) VIRTUAL_FUNC;

	virtual byte* ParseUID( SCARDHANDLE handle, int proto ) VIRTUAL_FUNC;
	virtual TagType ParseATR( byte* bATR, int ATRLen ) VIRTUAL_FUNC;

	// pooling thread 
	//void StartPolling();
	//void StopPooling();
	//DWORD WINAPI Pooler( LPVOID lpParam );
	//TagFoundHandler tagFound;

protected:
	byte* GetATR( SCARDHANDLE handle, int proto, int* ATRLen );
	NFCTag* BuildTag( SCARDHANDLE handle, int proto, byte* ATR, int ATRLen );

	SCARDCONTEXT hContext;
	std::string name;
	bool stopPollingSignal;

private:
	virtual byte* Transmit( SCARDHANDLE handle, int proto, byte* cmdBytes, int len ) VIRTUAL_FUNC;

	HANDLE pollerThread;
};