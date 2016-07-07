#include "stdafx.h"
#include "NFCTag.h"
#include "ACSReader.h"

TagType ACSReader::ParseATR( byte* bATR, int ATRLen )
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

byte* ACSReader::ParseUID( SCARDHANDLE handle, int proto )
{
	return Transmit( handle, proto, new byte[5]{ 0xFF, 0xCA, 0x00, 0x00, 0x00 }, 5 );
}

NFCTag* ACSReader::Connect()
{
	SCARDHANDLE handle;
	DWORD proto = 0;

	int ATRLen;
	LONG retCode = SCardConnect( hContext, s2ws( name ).c_str(), SCARD_SHARE_SHARED,
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &handle, &proto );

	if( retCode != SCARD_S_SUCCESS )
		throw new std::runtime_error( "Error creating a connection: " + GetScardErrMsg( retCode ) );

	//name = ws2s( tName );
	byte* ATR = GetATR( handle, proto, &ATRLen );

	return BuildTag( handle, proto, ATR, ATRLen );
}

void ACSReader::LoadKey( SCARDHANDLE handle, int proto, KeyTypes keyType, byte* keyData )
{
	//if( keyData.Length != 6 )
	//	throw new std::invalid_argument( "Keys must be 6 byte long" );

	byte KeyT = keyType == KeyTypes::TypeA ? (byte)0x60 : (byte)0x61;
	byte KeyN = keyType == KeyTypes::TypeA ? (byte)0x00 : (byte)0x01;

	Transmit( handle, proto, new byte[11]{ 0xFF, 0x82, 0x00, KeyN, 0x06,
		keyData[0], keyData[1], keyData[2], keyData[3], keyData[4], keyData[5] }, 11 );
	return;
}

void ACSReader::Authenticate( SCARDHANDLE handle, int proto, KeyTypes keyType, byte sector )
{
	byte KeyT = keyType == KeyTypes::TypeA ? (byte)0x60 : (byte)0x61;
	byte KeyN = keyType == KeyTypes::TypeA ? (byte)0x00 : (byte)0x01;

	Transmit( handle, proto, new byte[11]{ 0xFF, 0x86, 0x00, 0x00, 0x05,
		0x01, 0x00, sector, KeyT, KeyN }, 11 );
	return;
}

byte* ACSReader::Read( SCARDHANDLE handle, int proto, byte page )
{
	return Transmit( handle, proto, new byte[5]{ 0xFF, 0xB0, 0x00, page, 0x04 }, 5 );
}

void ACSReader::Write( SCARDHANDLE handle, int proto, byte page, byte* data, int len )
{
	if( len != 4 ) 
		throw new std::invalid_argument( "Page write must be of 4 bytes" );

	byte* buffer = new byte[9]{ 0xFF, 0xD6, 0x00, page, 0x04, 0x00, 0x00, 0x00, 0x00 };
	// add data from byte 5 on
	memcpy( (void*)buffer[5], data, 4 );
	Transmit( handle, proto, buffer, 9 );
}

byte* ACSReader::Transmit( SCARDHANDLE handle, int proto, byte* cmdBytes, int len )
{
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = proto;
	ioRequest.cbPciLength = 8;

	DWORD rcvLenght = 32; // Use 260 to handle more intelligent smartcards
	byte* rcvBytes = new byte[rcvLenght];

	LONG retCode = SCardTransmit( handle,
		&ioRequest, cmdBytes, len,
		&ioRequest, rcvBytes, &rcvLenght );

	if( retCode != SCARD_S_SUCCESS )
		throw new std::runtime_error( "Failed querying tag: " + GetScardErrMsg( retCode ) );

	if( !(rcvBytes[rcvLenght - 2] == 0x90 && rcvBytes[rcvLenght - 1] == 0x00) )
	{
		if( rcvBytes[rcvLenght - 2] == 0x63 && rcvBytes[rcvLenght - 1] == 0x00 )
			throw new std::runtime_error( "Operation failed!" );

		if( rcvBytes[rcvLenght - 2] == 0x6A && rcvBytes[rcvLenght - 1] == 0x81 )
			throw new std::runtime_error( "Operation not supported!" );

		char buff[256];
		sprintf_s( buff, "Operation returned %02X %02X", rcvBytes[rcvLenght - 2], rcvBytes[rcvLenght - 1] );
		throw new std::runtime_error( buff );
	}

	byte* returnBytes = new byte[rcvLenght - 2];
	memcpy( returnBytes, rcvBytes, rcvLenght - 2 );

	return returnBytes;
}