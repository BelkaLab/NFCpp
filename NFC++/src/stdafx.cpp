// stdafx.cpp : source file that includes just the standard includes
// NFC++.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

std::string BytesToHex( byte* bytes, int numBytes )
{
	std::string hex = "";
	char buff[32];
	for( int idx = 0; idx < numBytes; idx++ )
	{
		// format to hex, fixed lenght of 2 chars
		sprintf_s( buff, "%02X", bytes[idx] );
		hex = hex + " " + buff;
	}
	return hex;
}

byte* HexToBytes( std::string payload )
{
	while( payload.find( " " ) ) 
	{
		payload = payload.replace( payload.find( " " ), 1, "" );
	}

	if( payload.length() % 2 > 0 )
	{
		throw new std::invalid_argument( "String is not composed of hex pairs" );
		return NULL;
	}

	byte* bytes = new byte[payload.length() / 2];
	for( size_t index = 0; index < payload.length() / 2; index++ )
	{
		bytes[index] = 0xFF & strtoul( payload.substr( index * 2, 2 ).c_str(), NULL, 16 );
	}
	return bytes;
}

std::string GetScardErrMsg( LONG returnCode )
{
	switch( returnCode )
	{
	case SCARD_E_CANCELLED:
		return("The action was canceled by an SCardCancel request.");
	case SCARD_E_CANT_DISPOSE:
		return("The system could not dispose of the media in the requested manner.");
	case SCARD_E_CARD_UNSUPPORTED:
		return("The smart card does not meet minimal requirements for support.");
	case SCARD_E_DUPLICATE_READER:
		return("The reader driver didn't produce a unique reader name.");
	case SCARD_E_INSUFFICIENT_BUFFER:
		return("The data buffer for returned data is too small for the returned data.");
	case SCARD_E_INVALID_ATR:
		return("An ATR string obtained from the registry is not a valid ATR string.");
	case SCARD_E_INVALID_HANDLE:
		return("The supplied handle was invalid.");
	case SCARD_E_INVALID_PARAMETER:
		return("One or more of the supplied parameters could not be properly interpreted.");
	case SCARD_E_INVALID_TARGET:
		return("Registry startup information is missing or invalid.");
	case SCARD_E_INVALID_VALUE:
		return("One or more of the supplied parameter values could not be properly interpreted.");
	case SCARD_E_NOT_READY:
		return("The reader or card is not ready to accept commands.");
	case SCARD_E_NOT_TRANSACTED:
		return("An attempt was made to end a non-existent transaction.");
	case SCARD_E_NO_MEMORY:
		return("Not enough memory available to complete this command.");
	case SCARD_E_NO_SERVICE:
		return("The smart card resource manager is not running.");
	case SCARD_E_NO_SMARTCARD:
		return("The operation requires a smart card, but no smart card is currently in the device.");
	case SCARD_E_PCI_TOO_SMALL:
		return("The PCI receive buffer was too small.");
	case SCARD_E_PROTO_MISMATCH:
		return("The requested protocols are incompatible with the protocol currently in use with the card.");
	case SCARD_E_READER_UNAVAILABLE:
		return("The specified reader is not currently available for use.");
	case SCARD_E_READER_UNSUPPORTED:
		return("The reader driver does not meet minimal requirements for support.");
	case SCARD_E_SERVICE_STOPPED:
		return("The smart card resource manager has shut down.");
	case SCARD_E_SHARING_VIOLATION:
		return("The smart card cannot be accessed because of other outstanding connections.");
	case SCARD_E_SYSTEM_CANCELLED:
		return("The action was canceled by the system, presumably to log off or shut down.");
	case SCARD_E_TIMEOUT:
		return("The user-specified timeout value has expired.");
	case SCARD_E_UNKNOWN_CARD:
		return("The specified smart card name is not recognized.");
	case SCARD_E_UNKNOWN_READER:
		return("The specified reader name is not recognized.");
	case SCARD_F_COMM_ERROR:
		return("An internal communications error has been detected.");
	case SCARD_F_INTERNAL_ERROR:
		return("An internal consistency check failed.");
	case SCARD_F_UNKNOWN_ERROR:
		return("An internal error has been detected, but the source is unknown.");
	case SCARD_F_WAITED_TOO_LONG:
		return("An internal consistency timer has expired.");
	case SCARD_S_SUCCESS:
		return("No error was encountered.");
	case SCARD_W_REMOVED_CARD:
		return("The smart card has been removed, so that further communication is not possible.");
	case SCARD_W_RESET_CARD:
		return("The smart card has been reset, so any shared state information is invalid.");
	case SCARD_W_UNPOWERED_CARD:
		return("Power has been removed from the smart card, so that further communication is not possible.");
	case SCARD_W_UNRESPONSIVE_CARD:
		return("The smart card is not responding to a reset.");
	case SCARD_W_UNSUPPORTED_CARD:
		return("The reader cannot communicate with the card, due to ATR string configuration conflicts.");
	case SCARD_E_COMM_DATA_LOST:
		return ("A communication error with the smart card has been detected.");
	default:
		char buff[256];
		sprintf_s( buff, "Unknown error code: 0x%X", returnCode );
		return buff;
	}
}

std::wstring s2ws( const std::string& s )
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar( CP_ACP, 0, s.c_str(), slength, NULL, 0 );
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar( CP_ACP, 0, s.c_str(), slength, buf, len );
	std::wstring r( buf );
	delete[] buf;
	return r;
}

std::string ws2s( const std::wstring& s )
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte( CP_ACP, 0, s.c_str(), slength, NULL, 0, NULL, NULL );
	char* buf = new char[len];
	WideCharToMultiByte( CP_ACP, 0, s.c_str(), slength, buf, len, NULL, NULL );
	std::string r( buf );
	delete[] buf;
	return r;
}