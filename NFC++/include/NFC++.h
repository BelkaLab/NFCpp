#include "stdafx.h"
#include "NFC++-version.h"
#include "NFCReader.h"

#pragma warning( push )
#pragma warning( disable: 4251 ) // Warning 4251 is safe to be disabled here, as std::vector< NFCReader* > readers
								 // is never exposed directly via any NFCHandler method.

// This class is exported from the NFC++.dll
class NFC_API NFCHandler
{
public:
	NFCHandler() :
		hContext( NULL )
	{
	}

	void Init();
	void Release();

	bool IsInitialized() const { return hContext != NULL; }

	int GetReadersCount() const
	{
		return (int)readers.size();
	}

	NFCReader* GetReader( int i ) const;
	NFCReader* GetReaderByName( const char* name ) const;

private:
	SCARDCONTEXT hContext;
	std::vector< NFCReader* > readers;
};

#pragma warning( pop )
