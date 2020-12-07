// sunrpc.cpp : Defines the entry point for the DLL application.
//

//#include <intrin.h>
#include <windows.h>
#include "sunrpc.h"
//#include <winnt.h>

#ifdef __cplusplus
extern "C"
{
#endif

	//#include <winsock2.h>
//#include <windows.h>

#ifdef _WIN64
#define InterlockedCompareExchangePointerMc	InterlockedCompareExchangePointer
	typedef void* TypeInterlocked;
#else
#define InterlockedCompareExchangePointerMc	_InterlockedCompareExchange
	typedef LONG TypeInterlocked;
#endif

	//#define VOID void
	//#define LPTSTR char*

	//WSADATA WSAData;

	//static int init = 0;
		extern int sun_rpc_init();
		extern int sun_rpc_exit();
	BOOL APIENTRY DllMain(HANDLE hModule,
		DWORD  ul_reason_for_call,
		LPVOID lpReserved
		)
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
			sun_rpc_init();
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			//printf("!!!!%s: DLL_THREAD_DETACH\n", __FUNCTION__);
			break;
		case DLL_PROCESS_DETACH:
			//printf("!!!!%s: DLL_PROCESS_DETACH\n", __FUNCTION__);
			sun_rpc_exit();
			break;
		}
		return TRUE;
	}


#ifdef __cplusplus
}
#endif


/*
// This is an example of an exported variable
SUNRPC_API int nSunrpc=0;

// This is an example of an exported function.
SUNRPC_API int fnSunrpc(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see sunrpc.h for the class definition
CSunrpc::CSunrpc()
{ 
	return; 
}

*/