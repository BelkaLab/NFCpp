#include "stdafx.h"
#include "NFCReader.h"

// This class is exported from the NFC++.dll
class NFC_API NFCHandler
{
public:
	NFCHandler() :
		hContext( NULL ),
		defaultReader( 0 )
	{
	}

	void Init();
	void Release();

	bool IsInitialized() { return hContext != NULL; }
	int defaultReader = 0;

	const std::vector< NFCReader* > getReaders() const {
		return readers;
	}
	
private:
	SCARDCONTEXT hContext;
	std::vector< NFCReader* > readers;
};
