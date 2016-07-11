#include "stdafx.h"
#include "NFCReader.h"

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

	const std::vector< NFCReader* > GetReaders() const {
		return readers;
	}
	
private:
	SCARDCONTEXT hContext;
	std::vector< NFCReader* > readers;
};
