/*
 *	File: dllmain_remote_function_call.cpp
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
#include "remote_function_caller_base.h"
#include "windows_signal.h"
#include <tchar.h>

#define		LIBRARY_NAME_TO_LOAD_ASCII	LIBRARY_NAME_TO_LOAD_BASE "." WLAC_LIBRARY_VERSION ".dll"

#if !defined(_USRDLL) & !defined(__cplusplus)
#error No method to initialize the library
#endif

#ifdef __cplusplus
extern "C"
{
#endif

REMOTE_FNC_LINKAGE BOOL initialize_local_signals(void);
REMOTE_FNC_LINKAGE void clean_local_signals(void);
char	g_vcThisLibrary_Path[MAX_PATH];
char	g_vcFalseLibPath[MAX_PATH];

static void init_library(HINSTANCE hinstDLL,LPVOID lpvReserved);
static void clean_library(void);

static int s_nLibraryInited = 0;

#ifdef _USRDLL
BOOL WINAPI DllMain(HINSTANCE a_hinstDLL, DWORD a_fdwReason, LPVOID a_lpvReserved)
{
	switch (a_fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		init_library(a_hinstDLL,a_lpvReserved);
		break;
	case DLL_PROCESS_DETACH:
		clean_library();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}
#else   // #ifdef _USRDLL

class CLibIniter
{
public:
	CLibIniter()
	{
		HMODULE hinstDLL = GetModuleHandle(_T(LIBRARY_NAME_TO_LOAD_NEW));
		init_library(hinstDLL);
	}
	~CLibIniter()
	{
		clean_library();
	}
};

static volatile CLibIniter s_Initer;

#endif  // #ifdef _USRDLL


static void clean_library(void)
{
	if (s_nLibraryInited==0) { return; }
	clean_local_signals();
	s_nLibraryInited = 0;
}


static void init_library(HINSTANCE a_hinstDLL,LPVOID a_lpvReserved)
{
	char* pcDir;
	size_t unRem,unDirNameLen, unCopyLen = strlen(LIBRARY_NAME_TO_LOAD_ASCII)+1;

	if (a_lpvReserved) {
		initialize_local_signals();
	}

	if(s_nLibraryInited){return;}

	GetModuleFileNameA(a_hinstDLL, g_vcThisLibrary_Path, MAX_PATH - 1);
	GetModuleFileNameA(a_hinstDLL, g_vcFalseLibPath, MAX_PATH - 1);

	pcDir = strrchr(g_vcFalseLibPath, '\\');
	if(!pcDir){return;}

	unDirNameLen = ((size_t)pcDir)-((size_t)g_vcFalseLibPath);
	unRem = MAX_PATH - unDirNameLen-1;
	unCopyLen = unCopyLen > unRem ? unRem : unCopyLen;
	memcpy(pcDir+1, LIBRARY_NAME_TO_LOAD_ASCII, unCopyLen);
	
	s_nLibraryInited = 1;
}


#ifdef __cplusplus
}
#endif
