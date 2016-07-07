// NFC++.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NFC++.h"
#include "ACSReader.h"

void NFCHandler::Init()
{
	if( IsInitialized() )
	{
		Release();
	}

	readers.clear();

	// Get context
	LONG retCode = SCardEstablishContext( SCARD_SCOPE_USER, NULL, NULL, &hContext );

	if( retCode != SCARD_S_SUCCESS )
	{
		throw new std::runtime_error( "Failed extablishing context: " + GetScardErrMsg( retCode ) );
	}

	// Get PC/SC readers available
	DWORD pcchReaders = 0;
	retCode = SCardListReaders( hContext, NULL, NULL, &pcchReaders );

	if( retCode == SCARD_E_NO_READERS_AVAILABLE )
	{
		return;
	}
	else if( retCode != SCARD_S_SUCCESS )
	{
		throw new std::runtime_error( "Failed listing readers: " + GetScardErrMsg( retCode ) );
	}

	// Fill reader list
	TCHAR* ReadersList = new TCHAR[pcchReaders];
	retCode = SCardListReaders( hContext, NULL, ReadersList, &pcchReaders );

	// Convert reader buffer to string
	// reader buffer is formatted like this:
	// <reader1name>\0<reader2name>\0..<readerXname>\0\0
	int idxBytes = 0;
	std::string rdrName = "";
	std::string* readersNames = new std::string[pcchReaders];
	while( ReadersList[idxBytes] != 0 )
	{
		while( ReadersList[idxBytes] != 0 )
		{
			rdrName = rdrName + (char)ReadersList[idxBytes];
			idxBytes++;
		}

		// TODO: add support for different readers
		//if( rdrName.find( "DUALi" ) == 0 )
		//	readers.push_back( new Readers.DualiReader( hContext, rdrName ) );
		//else if( rdrName.find( "ACS" ) == 0 )
		//	readers.push_back( new Readers.ACSReader( hContext, rdrName ) );
		//else if( rdrName.find( "SCM" ) == 0 || (rdrName.find( "Identive" ) == 0 && rdrName.find( "Contactless" ) > -1) )
		//	readers.push_back( new Readers.IdentiveReader( hContext, rdrName ) );

		if( rdrName.find( "ACS" ) == 0 )
			readers.push_back( new ACSReader( hContext, rdrName ) );
		else 
			throw new std::runtime_error( "Unsupported reader: " + rdrName );

		rdrName = "";
		idxBytes++;
	}

	defaultReader = 0;
}

void NFCHandler::Release()
{
	if( IsInitialized() )
	{
		int retCode = SCardReleaseContext( hContext );

		if( retCode != SCARD_S_SUCCESS )
			throw new std::runtime_error( "Failed release!" );

		hContext = NULL;

		for( std::vector< NFCReader* >::iterator it = readers.begin(); it != readers.end(); ++it )
			delete *it;
		readers.clear();
	}
}

