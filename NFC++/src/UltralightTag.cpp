#include "stdafx.h"
#include "UltralightTag.h"

// http://stackoverflow.com/questions/28228993/how-to-read-binary-blocks-of-mifare-card
uint8_t* UltralightTag::ReadAll( uint8_t numPages )
{
	const uint8_t chunkSize = 0x04;

	uint8_t* rawData = new uint8_t[numPages * chunkSize];
	for( uint8_t pageIx = 0; pageIx < numPages; pageIx++ )
	{
		uint8_t* pageContent = Read( pageIx );

		memcpy( &rawData[pageIx *chunkSize], pageContent, chunkSize );
		delete[] pageContent;
	}
	return rawData;
}

void UltralightTag::WriteAll( uint8_t* data, int len )
{
	const uint8_t chunkSize = 0x04;

	uint8_t* buffer = new uint8_t[chunkSize];
	for( int pageIx = 0; pageIx < (len / chunkSize); pageIx++ )
	{
		memcpy( buffer, &data[ pageIx * chunkSize], chunkSize );
		Write( (uint8_t)pageIx, buffer, chunkSize );
	}
	delete[] buffer;
}

void UltralightTag::Lock()
{
	Write( 0x02, new uint8_t[4]{ 0x00, 0x00, 0xFF, 0xFF }, 4 );
}