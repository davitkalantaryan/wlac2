
// 2017 Dec 15

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rfc/wlac_rfc.h"
#include "wlac_rfc_private.h"
#include <psapi.h>
#include <malloc.h>

__BEGIN_C_DECLS

RFC_VAR	int	g_nRfcLibraryLoggingLevel = 0;

#define DEBUG_TROJAN(_logLevel,...)  do{if((_logLevel)<=g_nRfcLibraryLoggingLevel){printf(__VA_ARGS__);}}while(0)

static DWORD WINAPI PTHREAD_START_ROUTINE_on_remote_app(LPVOID a_lpThreadParameter);

static volatile int s_nLibraryInited=0;
static char			s_vcThisDllPath[_MAX_PATH];
static int			s_nThisDllNameLenPlus1	=	0;

static  int64_t s_unOffsetToFunction = 0;


BOOL WINAPI DllMain(_In_ HINSTANCE a_hinstDLL,_In_ DWORD a_fdwReason,_In_ LPVOID a_lpvReserved)
{
	DEBUG_TROJAN(1,"hInstance=%p, reserved=%p, reason=%d(",a_hinstDLL,a_lpvReserved, (int)a_fdwReason);

	switch (a_fdwReason){
	case DLL_PROCESS_ATTACH:
		if (!s_nLibraryInited) {
			LPTHREAD_START_ROUTINE fpRemoteFunction = &PTHREAD_START_ROUTINE_on_remote_app;
			DWORD dwBytes = GetModuleFileNameA(a_hinstDLL, s_vcThisDllPath, MAX_PATH);
			if (!dwBytes) { return FALSE; }
			s_nThisDllNameLenPlus1 = ((int)dwBytes) + 1;
			s_unOffsetToFunction = (char*)(*((void**)&fpRemoteFunction)) - (char*)a_hinstDLL;
			DEBUG_TROJAN(2,"DLL_PROCESS_ATTACH, g_unOffsetToFunction=%d, fp=%p", (int)s_unOffsetToFunction, fpRemoteFunction);
			s_nLibraryInited = 1;
		}
		break;
	case DLL_THREAD_ATTACH:
		DEBUG_TROJAN(2,"DLL_THREAD_ATTACH");
		break;
	case DLL_PROCESS_DETACH:
		DEBUG_TROJAN(2,"DLL_PROCESS_DETACH");
		break;
	case DLL_THREAD_DETACH:
		DEBUG_TROJAN(2,"DLL_THREAD_DETACH");
		break;
	default:
		DEBUG_TROJAN(2,"default");
		break;
	}

	DEBUG_TROJAN(1,")\n");

	return TRUE;
}

RFC_API_FAR int CallFunctionOnRemoteProcessByPid(
	int a_pid, const char* a_libName,
	const char* a_fncName, void* a_data, size_t a_memorySize, void** a_pReturn)
{
	int nReturn;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, a_pid);
	if(!hProcess){return -1;}
	nReturn = CallFunctionOnRemoteProcess(hProcess, a_libName,a_fncName,a_data, a_memorySize,a_pReturn);
	CloseHandle(hProcess);
	return nReturn;
}


//#define _TEST_FIRST_LOCAL

#ifdef _TEST_FIRST_LOCAL
#define WriteProcessMemoryNew(_hProcess,_Dst,_Src,_size,_sizePtr)	memcpy((_Dst),(_Src),(_size))
#else
#define WriteProcessMemoryNew(_hProcess,_Dst,_Src,_size,_sizePtr)	WriteProcessMemory((_hProcess),(_Dst),(_Src),(_size),(_sizePtr))
#endif

RFC_API 
int CallFunctionOnRemoteProcess(
	HANDLE a_hProcess, const char* a_libName, 
	const char* a_fncName, void* a_data, size_t a_memorySize, void** a_pReturn)
{
#ifdef _WIN64
	int						i, nNumOfModules;
	DWORD					cbNeeded;
	DWORD					dwHmoduleOnremoteLow;
	HMODULE*				pMods;
#endif
	HMODULE					hModuleThisOnRemote;
	char*					pcRemoteMemory = NULL;
	struct SInjectedType	localInject;
	HANDLE					hRemoteThread = (HANDLE)0;
	SIZE_T					szWritten;
	DWORD					dwThreadId;
	DWORD					dwHinstanseOfThisOnRemoteLow, dwRemThreadExit;
	int						nReturn = -1;
	
	int64_t					dllNameLenPlus1 = ((int64_t)strlen(a_libName))+1;
	int64_t					funcionNameLenPlus1 = ((int64_t)strlen(a_fncName)) + 1;
	
	int64_t					targetDllNameRounded = ROUND_FOR_PADDING(dllNameLenPlus1 );
	int64_t					memorySizeRounded = (int64_t)ROUND_FOR_PADDING(a_memorySize);
	int64_t					funcionNameLenRounded = ROUND_FOR_PADDING(funcionNameLenPlus1);
	int64_t					thisDllNameLenRounded = (int64_t)ROUND_FOR_PADDING(s_nThisDllNameLenPlus1);

	int64_t					nAllocSizeForSecondThread = sizeof(struct SInjectedType) + targetDllNameRounded + memorySizeRounded + funcionNameLenRounded;
	int64_t					nSizeToAlloc = thisDllNameLenRounded>nAllocSizeForSecondThread?thisDllNameLenRounded:nAllocSizeForSecondThread;
	int64_t					llunRemoteReturn;

#ifdef _TEST_FIRST_LOCAL
	char*					pcRealRemoteMemory = NULL;

	pcRealRemoteMemory = (char*)AllocMemOnRemoteProcess(a_hProcess, (size_t)nSizeToAlloc);
	if (!pcRealRemoteMemory) { goto returnPoint; }

	pcRemoteMemory = (char*)malloc((size_t)nSizeToAlloc);
	if (!pcRealRemoteMemory) { goto returnPoint; }
#else
#define pcRealRemoteMemory pcRemoteMemory
	pcRemoteMemory = (char*)AllocMemOnRemoteProcess(a_hProcess, (size_t)nSizeToAlloc);
	if (!pcRemoteMemory) { goto returnPoint; }
#endif


	// load this library
	WriteProcessMemory(a_hProcess, pcRealRemoteMemory,s_vcThisDllPath,s_nThisDllNameLenPlus1,&szWritten);
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL,0,(LPTHREAD_START_ROUTINE)LoadLibraryA, pcRealRemoteMemory,0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwHinstanseOfThisOnRemoteLow);
#ifdef _WIN64
	if (!EnumProcessModules(a_hProcess, &hModuleThisOnRemote, sizeof(hModuleThisOnRemote), &cbNeeded)){goto returnPoint;}
	pMods = (HMODULE*)_alloca(cbNeeded);
	if (!EnumProcessModules(a_hProcess, pMods, cbNeeded, &cbNeeded)){goto returnPoint;}
	nNumOfModules = cbNeeded / sizeof(HMODULE);
	hModuleThisOnRemote = (HMODULE)0;
	for (i = 0; i < nNumOfModules; i++) {
		dwHmoduleOnremoteLow = (DWORD)((size_t)pMods[i]);
		if(dwHmoduleOnremoteLow == dwHinstanseOfThisOnRemoteLow){
			//if (GetModuleFileNameEx(hProcess, hMods[i], szModName,  
			// we can also check module name, but for now I'll skip this

			hModuleThisOnRemote = pMods[i];
			break;
		}
	}
#else
	hModuleThisOnRemote = (HMODULE)dwHinstanseOfThisOnRemoteLow;
#endif
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;
	if(!hModuleThisOnRemote){goto returnPoint;}

	// now finally call function
	localInject.memorySize = (uint64_t)a_memorySize;
	localInject.dllNameLen = (uint64_t)targetDllNameRounded;
	WriteProcessMemoryNew(a_hProcess, pcRemoteMemory, &localInject,sizeof(struct SInjectedType), &szWritten);
	WriteProcessMemoryNew(a_hProcess, DLL_NAME_FROM_INJECT(pcRemoteMemory),a_libName,(SIZE_T)dllNameLenPlus1, &szWritten);
	WriteProcessMemoryNew(a_hProcess, MEMORY_FROM_INJECT(pcRemoteMemory, localInject),a_data, a_memorySize, &szWritten);
	WriteProcessMemoryNew(a_hProcess, FUNCTION_NAME_FROM_INJECT(pcRemoteMemory,localInject),a_fncName,(SIZE_T)funcionNameLenPlus1, &szWritten);
#ifdef _TEST_FIRST_LOCAL
	WriteProcessMemory(a_hProcess, pcRealRemoteMemory, pcRemoteMemory, (SIZE_T)nSizeToAlloc, &szWritten);
#endif
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)(s_unOffsetToFunction + ((int64_t)((size_t)hModuleThisOnRemote))    ),
		pcRealRemoteMemory, 0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwRemThreadExit);
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;

	if(a_pReturn){
		ReadProcessMemory(a_hProcess,pcRemoteMemory,&llunRemoteReturn,8,&szWritten);
		*a_pReturn = (void*)((size_t)llunRemoteReturn);
	}

	// now let us unload this library from target application
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL,0,
		(LPTHREAD_START_ROUTINE)FreeLibrary,hModuleThisOnRemote,0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwRemThreadExit);
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;

	nReturn = 0;
returnPoint:
	if(hRemoteThread){CloseHandle(hRemoteThread);}
	if (pcRealRemoteMemory) { DeleteMemOnRemoteProcess(a_hProcess, pcRealRemoteMemory); }
#ifdef _TEST_FIRST_LOCAL
	if (pcRemoteMemory) { free(pcRemoteMemory); }
#endif
	return nReturn;
}


#if 0
RFC_API 
int CallFunctionOnRemoteProcess(
	HANDLE a_hProcess, const char* a_libName, 
	const char* a_fncName, void* a_data, size_t a_memorySize, void** a_pReturn)
{
	char*					pcRemoteMemory = NULL;
	struct SInjectedType	localInject;
	HANDLE					hRemoteThread = (HANDLE)0;
	SIZE_T					szWritten;
	DWORD					dwThreadId;
	DWORD					dwHinstanseOfThisOnRemote, dwRemThreadExit;
	int						nReturn = -1;
	
	int64_t					dllNameLenPlus1 = ((int64_t)strlen(a_libName))+1;
	int64_t					funcionNameLenPlus1 = ((int64_t)strlen(a_fncName)) + 1;
	
	int64_t					targetDllNameRounded = ROUND_FOR_PADDING(dllNameLenPlus1 );
	int64_t					memorySizeRounded = (int64_t)ROUND_FOR_PADDING(a_memorySize);
	int64_t					funcionNameLenRounded = ROUND_FOR_PADDING(funcionNameLenPlus1);
	int64_t					thisDllNameLenRounded = (int64_t)ROUND_FOR_PADDING(s_nThisDllNameLenPlus1);

	int64_t					nAllocSizeForSecondThread = sizeof(struct SInjectedType) + targetDllNameRounded + memorySizeRounded + funcionNameLenRounded;
	int64_t					nSizeToAlloc = thisDllNameLenRounded>nAllocSizeForSecondThread?thisDllNameLenRounded:nAllocSizeForSecondThread;
	int64_t					llunRemoteReturn;

	pcRemoteMemory = (char*)AllocMemOnRemoteProcess(a_hProcess, (size_t)nSizeToAlloc);
	if(!pcRemoteMemory){goto returnPoint;}

	// load this library
	WriteProcessMemory(a_hProcess,pcRemoteMemory,s_vcThisDllPath,s_nThisDllNameLenPlus1,&szWritten);
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL,0,(LPTHREAD_START_ROUTINE)LoadLibraryA, pcRemoteMemory,0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwHinstanseOfThisOnRemote);
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;
	if(!dwHinstanseOfThisOnRemote){goto returnPoint;}

	// now finally call function
	localInject.memorySize = (uint64_t)a_memorySize;
	localInject.dllNameLen = (uint64_t)targetDllNameRounded;
	WriteProcessMemory(a_hProcess, pcRemoteMemory, &localInject,sizeof(struct SInjectedType), &szWritten);
	WriteProcessMemory(a_hProcess, DLL_NAME_FROM_INJECT(pcRemoteMemory),a_libName,(SIZE_T)dllNameLenPlus1, &szWritten);
	WriteProcessMemory(a_hProcess, MEMORY_FROM_INJECT(pcRemoteMemory, localInject),a_data, a_memorySize, &szWritten);
	WriteProcessMemory(a_hProcess, FUNCTION_NAME_FROM_INJECT(pcRemoteMemory,localInject),a_fncName,(SIZE_T)funcionNameLenPlus1, &szWritten);
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)(g_unOffsetToFunction + ((int64_t)dwHinstanseOfThisOnRemote)),
		pcRemoteMemory, 0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwRemThreadExit);
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;

	if(a_pReturn){
		ReadProcessMemory(a_hProcess,pcRemoteMemory,&llunRemoteReturn,8,&szWritten);
		*a_pReturn = (void*)((size_t)llunRemoteReturn);
	}

	// now let us unload this library from target application
	hRemoteThread = CreateRemoteThread(a_hProcess, NULL,0,
		(LPTHREAD_START_ROUTINE)FreeLibrary,(void*)((size_t)dwHinstanseOfThisOnRemote),0, &dwThreadId);
	if(!hRemoteThread){goto returnPoint;}
	WaitForSingleObject(hRemoteThread, INFINITE);
	GetExitCodeThread(hRemoteThread, &dwRemThreadExit);
	CloseHandle(hRemoteThread);
	hRemoteThread = (HANDLE)0;

	nReturn = 0;
returnPoint:
	if(hRemoteThread){CloseHandle(hRemoteThread);}
	if (pcRemoteMemory) {DeleteMemOnRemoteProcess(a_hProcess, pcRemoteMemory);}
	return nReturn;
}
#endif


static DWORD WINAPI PTHREAD_START_ROUTINE_on_remote_app(LPVOID a_lpThreadParameter)
{
	void* pFncReturn = (void*)-1;
	int nReturn = -1;
	TypeFunctionIsCalledOnRemote fpFunction;
	struct SInjectedType* pInjectData ;
	HMODULE hLibrary;

	pInjectData = (struct SInjectedType*)a_lpThreadParameter;
	hLibrary = LoadLibraryA(DLL_NAME_FROM_INJECT(pInjectData));

	if(!hLibrary){ goto returnPoint;}

	DEBUG_TROJAN(1,"line:%d, fn:%s(%p)\n", __LINE__,__FUNCTION__, a_lpThreadParameter);

	fpFunction = (TypeFunctionIsCalledOnRemote)GetProcAddress(hLibrary, FUNCTION_NAME_FROM_INJECT(pInjectData,*pInjectData));
	if(!fpFunction){goto returnPoint;}

	pFncReturn = (*fpFunction)(MEMORY_FROM_INJECT(pInjectData,*pInjectData),(int)pInjectData->memorySize);

	nReturn = 0;
returnPoint:

	if(hLibrary){FreeLibrary(hLibrary);}
	pInjectData->dllNameLen = (uint64_t)pFncReturn;

	return nReturn;
}


__END_C_DECLS
