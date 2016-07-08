#include "stdafx.h"
#include "UltralightTag.h"

// http://stackoverflow.com/questions/28228993/how-to-read-binary-blocks-of-mifare-card
byte* UltralightTag::ReadAll( byte numPages )
{
	const byte chunkSize = 0x04;

	byte* rawData = new byte[numPages * chunkSize];
	for( byte pageIx = 0; pageIx < numPages; pageIx++ )
	{
		byte* pageContent = Read( pageIx );

		memcpy( &rawData[pageIx *chunkSize], pageContent, chunkSize );
		delete[] pageContent;
	}
	return rawData;
}

void UltralightTag::WriteAll( byte* data, int len )
{
	const byte chunkSize = 0x04;

	byte* buffer = new byte[chunkSize];
	for( int pageIx = 0; pageIx < (len / chunkSize); pageIx++ )
	{
		memcpy( buffer, &data[ pageIx * chunkSize], chunkSize );
		Write( (byte)pageIx, buffer, chunkSize );
	}
	delete[] buffer;
}

void UltralightTag::Lock()
{
	Write( 0x02, new byte[4]{ 0x00, 0x00, 0xFF, 0xFF }, 4 );
}