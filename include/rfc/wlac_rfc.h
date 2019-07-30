//
// file:		rfc/wlac_rfc.h
// created on:	2018 Dec 17
//

#ifndef __wlac_rfc_wlac_rfc_h__
#define __wlac_rfc_wlac_rfc_h__

#include <first_includes/common_definations_wul.h>
#ifndef CINTERFACE
#define CINTERFACE
#endif // !CINTERFACE
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stddef.h>

//SET_WLAC_ALIAS(stdio.h)

#ifndef RFC_API_FAR
#ifdef ___RFC_LIB_CREATION___
#define RFC_API_FAR		EXPORT_TO_LIB_API
#define RFC_VAR_FAR		extern EXPORT_TO_LIB_VAR
#elif defined(__USING_RFC_SOURCES__)  // #ifdef ___GEM_LIB_CREATION___
#define RFC_API_FAR
#define RFC_VAR_FAR		extern
#else   // #ifdef ___GEM_LIB_CREATION___
#define RFC_API_FAR		IMPORT_FROM_LIB_API
#define RFC_VAR_FAR		IMPORT_FROM_LIB_VAR
#endif  // #ifdef ___GEM_LIB_CREATION___
#endif  // #ifndef GEM_API_FAR


__BEGIN_C_DECLS

typedef void* (*TypeFunctionIsCalledOnRemote)(void*,int);

RFC_API_FAR int CallFunctionOnRemoteProcess(
	HANDLE hProcess, const char* libName,const char* fncName,void* data, size_t memorySize,void** pReturn);
RFC_API_FAR int CallFunctionOnRemoteProcessByPid(
	int pid, const char* libName,const char* fncName,void* data, size_t memorySize,void** pReturn);

RFC_VAR_FAR	int	g_nRfcLibraryLoggingLevel;

__END_C_DECLS


#endif  // #ifndef __wlac_rfc_wlac_rfc_h__
