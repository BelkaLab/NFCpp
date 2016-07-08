#include "stdafx.h"
#include "NFCReader.h"
#include "UltralightTag.h"

NFCReader::NFCReader( SCARDCONTEXT context, std::string readerName )
{
	hContext = context;
	name = readerName;

	pollerThread = NULL;
}

uint8_t* NFCReader::GetATR( SCARDHANDLE handle, int proto, int* ATRLen )
{
	TCHAR* rName = new TCHAR[256];
	DWORD rLength = 0, maxATRLen = 32, dwState = 0, proto_;
	proto_ = proto;
	uint8_t* ATRBytes = new uint8_t[maxATRLen];

	LONG retCode = SCardStatus( handle, rName, &rLength, &dwState, &proto_, &ATRBytes[0], &maxATRLen );

	if( retCode != SCARD_S_SUCCESS )
	{
		throw std::runtime_error( "Failed querying tag status: " + GetScardErrMsg( retCode ) );
	}

	uint8_t* ATR = new uint8_t[maxATRLen];
	memcpy( ATR, ATRBytes, maxATRLen );
	delete[] ATRBytes;

	(*ATRLen) = maxATRLen;
	return ATR;
}

NFCTag* NFCReader::BuildTag( SCARDHANDLE handle, int proto, uint8_t* ATR, int ATRLen )
{
	TagType type = ParseATR( ATR, ATRLen );
	switch( type )
	{
	case TagType::MifareUltralightFamily:
		return new UltralightTag( handle, proto, this, ATR );

	default:
		throw std::runtime_error( "Unsupported tag" );
	}
}

//void NFCReader::StartPolling()
//{
//	if( pollerThread == NULL )
//	{
//		DWORD   dwThreadId;
//		HANDLE  hThread;
//
//		hThread = CreateThread( NULL,                   // default security attributes
//			0,                      // use default stack size  
//			NFCReader::Pooler,    // thread function name
//			NULL,          // argument to thread function 
//			0,                      // use default creation flags 
//			&dwThreadId );   // returns the thread identifier 
//
//		if( hThread == NULL )
//		{
//			throw std::runtime_error( "Error creating Pooler thread" );
//			pollerThread = NULL;
//		}
//		else
//		{
//			pollerThread = hThread;
//		}
//	}
//}
//
//DWORD WINAPI NFCReader::Pooler( LPVOID lpParam )
//{
//	SCARD_READERSTATE* state = new SCARD_READERSTATE[1];
//	state[0].RdrName = name;
//	state[0].UserData = NULL;
//	state[0].RdrCurrState = SCARD_STATE_UNKNOWN;
//
//	LONG retCode;
//
//	retCode = SCardGetStatusChange( hContext, 100, &state, 1 );
//	if( retCode != SCARD_S_SUCCESS )
//	{
//		throw std::runtime_error( "Failed initial get status change: " + GetScardErrMsg( retCode ) );
//	}
//
//	state[0].RdrCurrState = state[0].RdrEventState;
//
//	while( !stopPollingSignal )
//	{
//		retCode = SCardGetStatusChange( hContext, 1000, state, 1 );
//
//		if( retCode == SCARD_E_TIMEOUT )
//			continue;
//		if( retCode != SCARD_S_SUCCESS )
//		{
//			throw new std::runtime_error( "Failed cycling get status change: " + GetScardErrMsg( retCode ) );
//		}
//
//		if( ((state[0].RdrEventState & SCARD_STATE_PRESENT) == SCARD_STATE_PRESENT) &&
//			((state[0].RdrEventState & SCARD_STATE_CHANGED) == SCARD_STATE_CHANGED) )
//		{
//			NFCTag tag = Connect();
//			// TODO
//			foreach( Delegate d in TagFound.GetInvocationList() )
//			{
//				ISynchronizeInvoke syncer = d.Target as ISynchronizeInvoke;
//				if( syncer != null )
//					syncer.BeginInvoke( d, new NFCTag[]{ tag } );
//				else
//					d.DynamicInvoke( tag );
//			}
//		}
//
//		state[0].RdrCurrState = State[0].RdrEventState;
//	}
//}