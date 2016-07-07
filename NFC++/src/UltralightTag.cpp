#include "stdafx.h"
#include "UltralightTag.h"

byte* UltralightTag::ReadAll()
{
	const byte chunkSize = 0x04;
	//const byte bytesToRead = 48;
	const byte bytesToRead = 144;

	byte* rawData = new byte[bytesToRead];
	int pagesToRead = (bytesToRead / chunkSize);
	for( int i = 0; i < pagesToRead; i++ )
	{
		byte pageIndex = (byte)(i + chunkSize);
		byte* pageContent = Read( pageIndex );

		memcpy( &rawData[i *chunkSize], pageContent, chunkSize );
		delete[] pageContent;
	}
	return rawData;
}

void UltralightTag::WriteAll( byte* data, int len )
{
	const byte chunkSize = 0x04;
	byte* buffer = new byte[4];
	for( int i = 0; i < (len / 4); i++ )
	{
		memcpy( buffer, &data[ i * chunkSize], chunkSize );
		Write( (byte)(i  * 4), buffer, chunkSize );
	}
}

void UltralightTag::Lock()
{
	Write( 0x02, new byte[4]{ 0x00, 0x00, 0xFF, 0xFF }, 4 );
}