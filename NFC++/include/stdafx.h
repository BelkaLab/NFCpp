// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NFC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NFC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef NFC_EXPORTS
#define NFC_API __declspec(dllexport)
#else
#define NFC_API __declspec(dllimport)
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

typedef unsigned char byte;

#include <vector>
#include <string>

#include <winscard.h>

#include <exception>

std::string BytesToHex( byte* bytes, int numBytes );
byte* HexToBytes( std::string payload );
const int SCARD_INFINITE_WAIT = -1;

std::string GetScardErrMsg( LONG returnCode );

// Convert std::string to std::wstring
std::wstring s2ws( const std::string& s );

// Convert std::wstring to std::string
std::string ws2s( const std::wstring& s );

#define VIRTUAL_FUNC = 0;
