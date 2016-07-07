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
			byte* t = tag->ReadAll();
			for( int i = 0; i < 144; i++ )
			{
				std::cout.width( 2 );
				std::cout.fill( '0' );
				std::cout << std::uppercase << std::hex << (int)t[i] << " ";
				if( i % 8 == 7 ) 
					std::cout << std::endl;
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

