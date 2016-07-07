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
		std::vector< NFCReader* > r = handler.getReaders();
		std::cout << "Done!" << std::endl;

		if( r.size() == 0 )
		{
			std::cout << "No reader found" << std::endl;
		}
		else
		{
			std::cout << "Available readers: " << std::endl;
			int readerIx = 0;
			for( std::vector< NFCReader* >::iterator it = r.begin(); it != r.end(); ++it )
			{
				std::cout << " " << ++readerIx << ": " << (*it)->getName() << std::endl;
			}

			NFCReader* reader = r[0];
			NFCTag* tag = reader->Connect();
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
		std::cout << "An exception occurred: " << e.what();
		return 1;
	}

	return 0;
}

