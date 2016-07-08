#include "stdafx.h"
#include "NFCTag.h"
#include "ACSReader.h"

TagType ACSReader::ParseATR( uint8_t* bATR, int ATRLen )
{
	TagType type = TagType::TagType_Unknown;

	// Parse ATR received
	std::string RIDVal, sATRStr, lATRStr, tmpVal;
	int indx;
	char buff[32];

	// Mifare cards using ISO 14443 Part 3 Supplemental Document
	if( ATRLen > 14 )
	{
		RIDVal = sATRStr = lATRStr = "";

		for( indx = 7; indx <= 11; indx++ )
		{
			sprintf_s( buff, "%02X", bATR[indx] );
			RIDVal = RIDVal + " " + buff;
		}

		for( indx = 0; indx <= 4; indx++ )
		{
			if( ((indx == '1') & (bATR[indx] / 16 == 8)) )
			{
				lATRStr = lATRStr + "8X";
				sATRStr = sATRStr + "8X";
			}
			else
			{
				if( indx == 4 )
				{
					sprintf_s( buff, "%02X", bATR[indx] );
					lATRStr = lATRStr + " " + buff;
				}
				else
				{
					sprintf_s( buff, "%02X", bATR[indx] );
					lATRStr = lATRStr + " " + buff;
					sATRStr = sATRStr + " " + buff;
				}
			}
		}

		if( RIDVal != " A0 00 00 03 06" )
		{
			switch( bATR[12] )
			{
			case 0:
				type = TagType::TagType_Unknown;
				break;
			case 1:
				type = TagType::ISO14443A_part1;
				break;
			case 2:
				type = TagType::ISO14443A_part2;
				break;
			case 3:
				type = TagType::ISO14443A_part3;
				break;
			case 5:
				type = TagType::ISO14443B_part1;
				break;
			case 6:
				type = TagType::ISO14443B_part2;
				break;
			case 7:
				type = TagType::ISO14443B_part3;
				break;
			case 9:
				type = TagType::ISO15693_part1;
				break;
			case 10:
				type = TagType::ISO15693_part2;
				break;
			case 11:
				type = TagType::ISO15693_part3;
				break;
			case 12:
				type = TagType::ISO15693_part4;
				break;
			case 13:
				type = TagType::ContactCard7816_10_IIC;
				break;
			case 14:
				type = TagType::ContactCard7816_10_ExtendedIIC;
				break;
			case 15:
				type = TagType::ContactCard7816_10_2WBP;
				break;
			case 16:
				type = TagType::ContactCard7816_10_3WBP;
				break;
			}
		}

		// Felica and Topaz Cards
		if( bATR[12] == 0x03 )
		{
			if( bATR[13] == 0xF0 )
			{
				switch( bATR[14] )
				{
				case 0x11:
					type = TagType::Felica212K;
					break;
				case 0x12:
					type = TagType::Felica424K;
					break;
				case 0x04:
					type = TagType::Topaz;
					break;
				}
			}
		}

		if( bATR[12] == 0x03 )
		{
			if( bATR[13] == 0x00 )
			{
				switch( bATR[14] )
				{
				case 0x01:
					type = TagType::MifareStandard1K;
					break;
				case 0x02:
					type = TagType::MifareStandard4K;
					break;
				case 0x03:
					type = TagType::MifareUltralightFamily;
					break;
				case 0x26:
					type = TagType::MifareMini;
					break;
				}
			}
			else
			{
				if( bATR[13] == 0xFF )
				{
					switch( bATR[14] )
					{
					case 0x09:
						type = TagType::MifareMini;
						break;
					}
				}
			}
		}

	}

	//4.2. Mifare DESFire card using ISO 14443 Part 4
	if( ATRLen == 11 )
	{
		RIDVal = "";
		for( indx = 4; indx <= 9; indx++ )
		{
			sprintf_s( buff, "%02X", bATR[indx] );
			RIDVal = RIDVal + " " + buff;
		}

		if( RIDVal == " 06 75 77 81 02 80" )
		{
			type = TagType::MifareDESFire;
		}

	}

	return type;
}

int ACSReader::GetUID( SCARDHANDLE handle, int proto, uint8_t* dest )
{
	uint8_t* command = new uint8_t[5]{ 0xFF, 0xCA, 0x00, 0x00, 0x00 };
	int responseLength = Transmit( handle, proto, command, 5, dest );
	delete[] command;

	return responseLength;
}

NFCTag* ACSReader::Connect()
{
	SCARDHANDLE handle;
	DWORD proto = 0;

	LONG retCode = SCardConnect( hContext, s2ws( name ).c_str(), SCARD_SHARE_SHARED,
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &handle, &proto );

	if( retCode == SCARD_W_REMOVED_CARD )
	{
		// no card found
		return NULL;
	}
	if( retCode != SCARD_S_SUCCESS )
		throw std::runtime_error( "Error creating a connection: " + GetScardErrMsg( retCode ) );

	//name = ws2s( tName );

	uint8_t* ATR = new uint8_t[32];
	int ATRLen = GetATR( handle, proto, ATR );

	NFCTag* tag = BuildTag( handle, proto, ATR, ATRLen );
	delete[] ATR;
	
	return tag;
}

void ACSReader::LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t* keyData )
{
	uint8_t KeyT = keyType == KeyTypes::TypeA ? (uint8_t)0x60 : (uint8_t)0x61;
	uint8_t KeyN = keyType == KeyTypes::TypeA ? (uint8_t)0x00 : (uint8_t)0x01;


	uint8_t* command = new uint8_t[11]{ 0xFF, 0x82, 0x00, KeyN, 0x06, 0, 0, 0, 0, 0, 0 };
	// add data from byte 6 onwards
	memcpy( &command[5], keyData, 6 );

	Transmit( handle, proto, command, 11, NULL );
	delete[] command;
}

void ACSReader::Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, uint8_t sector )
{
	uint8_t KeyT = keyType == KeyTypes::TypeA ? (uint8_t)0x60 : (uint8_t)0x61;
	uint8_t KeyN = keyType == KeyTypes::TypeA ? (uint8_t)0x00 : (uint8_t)0x01;


	uint8_t* command = new uint8_t[11]{ 0xFF, 0x86, 0x00, 0x00, 0x05, 0x01, 0x00, sector, KeyT, KeyN };

	Transmit( handle, proto, command, 11, NULL );
	delete[] command;
}

int ACSReader::Read( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* dest )
{
	uint8_t* command = new uint8_t[5]{ 0xFF, 0xB0, 0x00, page, 0x04 };
	int responseLength = Transmit( handle, proto, command, 5, dest );

	delete[] command;
	return responseLength;
}

void ACSReader::Write( SCARDHANDLE handle, int proto, uint8_t page, uint8_t* data, int len )
{
	if( len != 4 ) 
		throw std::invalid_argument( "Page write must be of 4 bytes" );

	uint8_t* command = new uint8_t[9]{ 0xFF, 0xD6, 0x00, page, 0x04, 0x00, 0x00, 0x00, 0x00 };
	// add data from byte 6 onwards
	memcpy( &command[5], data, 4 );

	Transmit( handle, proto, command, 9, NULL );
	delete[] command;
}

int ACSReader::Transmit( SCARDHANDLE handle, int proto, uint8_t* command, int len, uint8_t* response )
{
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = proto;
	ioRequest.cbPciLength = 8;

	DWORD rcvLenght = 32; // Use 260 to handle more intelligent smartcards
	uint8_t* rcvBytes = new uint8_t[rcvLenght];

	LONG retCode = SCardTransmit( handle,
		&ioRequest, command, len,
		&ioRequest, rcvBytes, &rcvLenght );

	if( retCode != SCARD_S_SUCCESS )
		throw std::runtime_error( "Failed querying tag: " + GetScardErrMsg( retCode ) );

	if( !(rcvBytes[rcvLenght - 2] == 0x90 && rcvBytes[rcvLenght - 1] == 0x00) )
	{
		if( rcvBytes[rcvLenght - 2] == 0x63 && rcvBytes[rcvLenght - 1] == 0x00 )
			throw std::runtime_error( "Transmit failed!" );

		if( rcvBytes[rcvLenght - 2] == 0x6A && rcvBytes[rcvLenght - 1] == 0x81 )
			throw std::runtime_error( "Transmit: operation code not supported!" );

		throw std::runtime_error( "Transmit failed, returned: " + BytesToHex( &rcvBytes[rcvLenght - 2], 2 ) );
	}

	// response length is the length of the received payload - 2 CRC bytes
	int responseLen = rcvLenght - 2;

	if( response != NULL )
		memcpy( response, rcvBytes, responseLen );
	delete[] rcvBytes;

	return responseLen;
}