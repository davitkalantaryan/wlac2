/*
 *	File: dllmain_remote_function_call_false_without_lib.cpp
 *
 *	Created on: Sep 24, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions ...
 *		1) ...
 *		2) ...
 *		...
 *
 *
 */

#include <stdio.h>
#include "namedpipeclient.h"
#include "remote_function_caller_base.h"
#include "remote_function_caller_private.h"


#ifdef __cplusplus
extern "C"
{
#endif

static BOOL CallLibraryFunction(void);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	static int snFunctionCalled = 0;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (snFunctionCalled) return FALSE;
		if (!CallLibraryFunction()) {snFunctionCalled = 0; return TRUE; }
		snFunctionCalled = 1;
		return FALSE;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		//return FALSE;
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}


static BOOL CallLibraryFunction(void)
{
	bool bMoreData;
	_uint64_ttt				ullnReturn;
	FILE**					ppflFileToReturn;
	HMODULE					hLibrary = NULL;
	remote_function_type2	fpFunc;
	INJDATA*				pInjectData;
	NamedPipeClient			aPipeClient;
	char					vcPipeName[1024];
	unsigned __int64		ullnAddress;
	int						nIndex;
	int						nPid = (int)GetCurrentProcessId();

	_snprintf(vcPipeName, 1023, "\\\\.\\pipe\\%d_%d", nPid, nPid);

	if (aPipeClient.ConnectToNamedPipeC(vcPipeName, 10) != 0){
		//Some application wants to have this library permanently
		return FALSE;
	}

	nIndex = aPipeClient.Read(&ullnAddress, sizeof(_uint64_ttt), bMoreData);
	aPipeClient.ClosePipe();
	if(nIndex<0){
		pInjectData = NULL;
		goto returnPoint;
	}
	pInjectData = (INJDATA*)((size_t)ullnAddress);

	switch (pInjectData->access_type)
	{
	case ACCESS_FREOPEN:
		freopen(pInjectData->file_name, pInjectData->function_name, s_cfpOutputs[pInjectData->data_in]);
		break;
	case GET_FILE_DESCRIPTOR:
		ppflFileToReturn = (FILE**)to_pointer(pInjectData->data_in);
		*ppflFileToReturn = fopen(pInjectData->file_name, pInjectData->function_name);
		break;
	case GET_STANDARD_FILE_DESCRIPTOR:
		nIndex = *((int*)to_pointer(pInjectData->data_in));
		ppflFileToReturn = (FILE**)to_pointer(pInjectData->data_in);
		*ppflFileToReturn = s_cfpOutputs[nIndex];
		break;
	case ACCESS_FNC_CALL:
		hLibrary = GetModuleHandleA(pInjectData->file_name);
		if (!hLibrary){goto returnPoint;}
		fpFunc = (remote_function_type2)GetProcAddress(hLibrary, pInjectData->function_name);
		ullnReturn = (*fpFunc)(pInjectData->data_in);
		break;
	case NEW_THREAD_INFO:
		hLibrary = GetModuleHandleA(pInjectData->file_name);
		if (!hLibrary){goto returnPoint;}
		ullnReturn = (_uint64_ttt)GetProcAddress(hLibrary, pInjectData->function_name);
		break;
	case FREEING_LIBRARY:
		hLibrary = GetModuleHandleA(pInjectData->file_name);
		if (!hLibrary){ goto returnPoint; }
		ullnReturn = (_uint64_ttt)GetProcAddress(hLibrary, pInjectData->function_name);
		break;
	default:
		break;
	}

returnPoint:

	if (hLibrary&&(pInjectData->in_freelib_out_return)){FreeLibrary(hLibrary);}
	if(pInjectData){pInjectData->in_freelib_out_return = ullnReturn;}
	return TRUE;
}

#ifdef __cplusplus
}
#endif
