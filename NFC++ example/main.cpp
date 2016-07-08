#include "NFC++.h"
#include "NFCTag.h"
#include "ACSReader.h"
#include <stdio.h>
#include <iostream>

int main()
{
	NFCHandler handler;
	std::cout << "=== NFC++.dll example usage ===" << std::endl << std::endl;

	try
	{

		std::cout << "Initializing NFCHandler.. ";
		handler.Init();
		std::cout << "Done!" << std::endl;

		std::cout << "Listing NFC Readers.. ";
		std::vector< NFCReader* > readers = handler.getReaders();
		std::cout << "Done!" << std::endl;

		if( readers.size() == 0 )
		{
			std::cout << "No reader found." << std::endl;
		}
		else
		{
			std::cout << std::endl << "Available readers: " << std::endl;
			int readerIx = 0;
			for( std::vector< NFCReader* >::iterator it = readers.begin(); it != readers.end(); ++it )
			{
				std::cout << " " << readerIx++ << ": " << (*it)->getName() << std::endl;
			}

			int rID = -1;
			while( rID < 0 || rID >= readers.size() )
			{
				std::cout << "Select a reader: ";
				std::cin >> rID;
			}

			NFCReader* reader = readers[rID];

			std::cout << std::endl << "Put a card on reader '" << reader->getName() << "'.." << std::endl;
			NFCTag* tag = NULL;
			while( tag == NULL ) 
			{
				try
				{
					tag = reader->Connect();
				}
				catch (std::runtime_error) {
					_sleep( 100 );
				}
			}
			byte numPages = 225;
			byte* t = tag->ReadAll( numPages );

			std::cout << std::endl << "[page]: b0 b1 b2 b3";
			for( int page = 0; page < numPages; page++ )
			{
				// print page number
				std::cout << std::endl << "[";
				std::cout.width( 4 );
				std::cout.fill( ' ' );
				std::cout << std::dec <<  page << "]: ";
				
				// print raw page bytes
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)t[page * 4 + 0] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)t[page * 4 + 1] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)t[page * 4 + 2] << " ";
				std::cout.width( 2 ); std::cout.fill( '0' ); std::cout << std::uppercase << std::hex << (int)t[page * 4 + 3] << " ";
				
				// print ascii content where possible, filled lock (ASCII 178) where non-printable characters are
				std::cout << " | ";
				std::cout << (t[page * 4 + 0] == 0 ? ' ' : (isprint( t[page * 4 + 0] ) ? (char)t[page * 4 + 0] : (char)178)) << " ";
				std::cout << (t[page * 4 + 1] == 0 ? ' ' : (isprint( t[page * 4 + 1] ) ? (char)t[page * 4 + 1] : (char)178)) << " ";
				std::cout << (t[page * 4 + 2] == 0 ? ' ' : (isprint( t[page * 4 + 2] ) ? (char)t[page * 4 + 2] : (char)178)) << " ";
				std::cout << (t[page * 4 + 3] == 0 ? ' ' : (isprint( t[page * 4 + 3] ) ? (char)t[page * 4 + 3] : (char)178)) << " ";

			}
			delete[] t;
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

