#include "NFC++.h"
#include "NFCTag.h"
#include "ACSReader.h"
#include <stdio.h>
#include <iostream>

int main()
{
	NFCHandler handler;
	std::cout << "=== NFC++.dll example usage ===" << std::endl;
	std::cout << "=== Library version: " << NFCPP_VERSION << " ===" << std::endl << std::endl;

	try
	{

		std::cout << "Initializing NFCHandler.. ";
		handler.Init();
		std::cout << "Done!" << std::endl;

		if( handler.GetReadersCount() == 0 )
		{
			std::cout << "No reader found." << std::endl;
		}
		else
		{
			std::cout << std::endl << "Available readers: " << std::endl;
			for( int readerIx = 0; readerIx < handler.GetReadersCount(); readerIx++ )
			{
				std::cout << " " << readerIx << ": " << handler.GetReader( readerIx )->GetName() << std::endl;
			}

			int rID = -1;
			while( rID < 0 || rID >= handler.GetReadersCount() )
			{
				std::cout << "Select a reader: ";
				std::cin >> rID;
			}

			NFCReader* reader = handler.GetReader( rID );

			std::cout << std::endl << "Put a card on reader '" << reader->GetName() << "'.." << std::endl;
			NFCTag* tag = NULL;
			bool unsupported = false; // used to debounce "Unsupported card" message
			while( tag == NULL ) 
			{
				try
				{
					tag = reader->Connect();
					unsupported = false;
				}
				catch (std::runtime_error) {
					if( unsupported == false )
					{
						unsupported = true;
						std::cout << "Unsupported card" << std::endl;
					}
				}
				Sleep( 100 );
			}
			uint8_t numPages = 225;
			uint8_t* data = new uint8_t[numPages * 4];
			int readLen = tag->ReadAll( numPages, data );

			std::cout << std::endl << "[page]: b0 b1 b2 b3";
			for( int page = 0; page < numPages; page++ )
			{
				// print page number
				std::cout << std::endl << "[";
				std::cout.width( 4 );
				std::cout.fill( ' ' );
				std::cout << std::dec <<  page << "]: ";
				
				// print raw page bytes
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)data[page * 4 + 0] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)data[page * 4 + 1] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)data[page * 4 + 2] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)data[page * 4 + 3] << " ";
				
				// print ascii content where possible, filled lock (ASCII 178) where non-printable characters are
				std::cout << " | ";
				std::cout << (data[page * 4 + 0] == 0 ? ' ' : (isprint( data[page * 4 + 0] ) ? (char)data[page * 4 + 0] : (char)178)) << " ";
				std::cout << (data[page * 4 + 1] == 0 ? ' ' : (isprint( data[page * 4 + 1] ) ? (char)data[page * 4 + 1] : (char)178)) << " ";
				std::cout << (data[page * 4 + 2] == 0 ? ' ' : (isprint( data[page * 4 + 2] ) ? (char)data[page * 4 + 2] : (char)178)) << " ";
				std::cout << (data[page * 4 + 3] == 0 ? ' ' : (isprint( data[page * 4 + 3] ) ? (char)data[page * 4 + 3] : (char)178)) << " ";

			}
			delete[] data;
		}

		handler.Release();

		std::cout << std::endl;
		system( "pause" );
	}
	catch( std::exception e )
	{
		std::cout << "An exception occurred: " << e.what() << std::endl;
		system( "pause" );

		return 1;
	}

	return 0;
}

