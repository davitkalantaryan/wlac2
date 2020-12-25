//
// file:		dllmain_win_signaling.cpp
// created on:	2018 Dec 18
//

#include "rfc/win_signaling.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>


#define	PRINTF_MAX_BUF_SIZE						1024
#define WIN_SIG_HANDLE_MEM_DEF(_pointer,...)	do{if(!(_pointer)){exit(1);}}while(0)

__BEGIN_C_DECLS

enum remCallReqType {remCallReqPrint};

struct SharedStruct
{
	int32_t type;
	int32_t	reserved;
	//int64_t	dataPtr;
};

#define BUFFER_FOR_PRINT(_sharedStr)	(  ((char*)(_sharedStr))+sizeof(struct SharedStruct)  )

static int		s_nLibraryInited = 0;
static char		s_vcThisDllPath[_MAX_PATH];

WIN_SIG_API_FAR void* WinSigFunctionIsCalledOnRemote(void* a_pData, int a_nDataLen)
{
	struct SharedStruct* pData = (struct SharedStruct*)a_pData;
	char* pcBufferWillBePrinted = BUFFER_FOR_PRINT(pData);
	
	//printf("data:%p, len:%d\n", a_pData, a_nDataLen);
	switch(pData->type){
	case remCallReqPrint:
		printf("%s", pcBufferWillBePrinted);
		break;
	default:
		break;
	}

	return NULL;
}


BOOL WINAPI DllMain(_In_ HINSTANCE a_hinstDLL,_In_ DWORD a_fdwReason,_In_ LPVOID a_lpvReserved)
{
	switch (a_fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		InitializeWinSigLibrary((void*)a_hinstDLL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		CleanupWinSigLibrary();
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}


WIN_SIG_API_FAR int WinSigRemotePrintf(int a_nPid, const char* a_format, ...)
{
	void* pRemCallRet;
	int nReturnByPrint;
	int nReturnByRemCall;
	va_list printArgs;
	size_t unMemorySize = sizeof(struct SharedStruct) + PRINTF_MAX_BUF_SIZE;
	struct SharedStruct* pData = (struct SharedStruct*)malloc(unMemorySize);
	char* pcBufferWillBePrinted = BUFFER_FOR_PRINT(pData);

	WIN_SIG_HANDLE_MEM_DEF(pData);

	va_start(printArgs,a_format);
	nReturnByPrint = vsnprintf(pcBufferWillBePrinted, PRINTF_MAX_BUF_SIZE, a_format,printArgs);
	va_end(printArgs);

	pData->type = remCallReqPrint;
	nReturnByRemCall=CallFunctionOnRemoteProcessByPid(a_nPid,s_vcThisDllPath,"WinSigFunctionIsCalledOnRemote",pData,unMemorySize,&pRemCallRet);
	if(nReturnByRemCall<0){nReturnByPrint=nReturnByRemCall;}

	return nReturnByPrint;
}


WIN_SIG_API_FAR void InitializeWinSigLibrary(void* a_pInstance)
{
	HINSTANCE hInstance = (HINSTANCE)a_pInstance;
	if(s_nLibraryInited){return;}
	if (!a_pInstance) {/*get instance here for the future*/return;}
	GetModuleFileNameA(hInstance, s_vcThisDllPath, MAX_PATH);
	s_nLibraryInited = 1;
}


WIN_SIG_API_FAR void CleanupWinSigLibrary(void)
{
	if (!s_nLibraryInited) { return; }
	s_nLibraryInited = 0;
}


__END_C_DECLS
