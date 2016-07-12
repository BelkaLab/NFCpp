#include "stdafx.h"
#include "UltralightTag.h"

void UltralightTag::LoadKey( KeyTypes keyType, uint8_t* keyData )
{
	reader->LoadKey( handle, proto, keyType, keyData );
}

void UltralightTag::Authenticate( KeyTypes keyType, uint8_t sector )
{
	reader->Authenticate( handle, proto, keyType, sector );
}

int UltralightTag::Read( uint8_t page, uint8_t* dest )
{
	return reader->Read( handle, proto, page, dest );
}

void UltralightTag::Write( uint8_t page, uint8_t* data, int len )
{
	reader->Write( handle, proto, page, data, len );
}

// http://stackoverflow.com/questions/28228993/how-to-read-binary-blocks-of-mifare-card
int UltralightTag::ReadAll( uint8_t numPages, uint8_t* dest )
{
	const uint8_t chunkSize = 0x04;

	uint8_t* pageContent = new uint8_t[chunkSize];
	int readBytes = 0;
	for( uint8_t pageIx = 0; pageIx < numPages; pageIx++ )
	{
		readBytes += Read( pageIx, pageContent );
		memcpy( &dest[pageIx *chunkSize], pageContent, chunkSize );
	}
	delete[] pageContent;

	return readBytes;
}

void UltralightTag::WriteAll( uint8_t* data, int len )
{
	const uint8_t chunkSize = 0x04;

	uint8_t* pageContent = new uint8_t[chunkSize];
	for( int pageIx = 0; pageIx < (len / chunkSize); pageIx++ )
	{
		memcpy( pageContent, &data[ pageIx * chunkSize], chunkSize );
		Write( (uint8_t)pageIx, pageContent, chunkSize );
	}
	delete[] pageContent;
}

void UltralightTag::Lock()
{
	uint8_t* command = new uint8_t[4]{ 0x00, 0x00, 0xFF, 0xFF };
	Write( 0x02, command, 4 );
	delete[] command;
}