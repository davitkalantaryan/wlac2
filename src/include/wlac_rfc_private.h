//
// file:		wlac_rfc_private.h
// created on:	2018 Dec 17
//

#ifndef __wlac_wlac_rfc_private_h__
#define __wlac_wlac_rfc_private_h__

#include <first_includes/common_definations_wul.h>
#include "rfc/wlac_rfc.h"
#include <stddef.h>
#include <stdint.h>

#ifndef RFC_API
#ifdef ___RFC_LIB_CREATION___
#define RFC_API			EXPORT_TO_LIB_API
#define RFC_VAR			extern EXPORT_TO_LIB_VAR
#elif defined(__USING_GEM_SOURCES__)  // #ifdef ___GEM_LIB_CREATION___
#define RFC_API
#define RFC_VAR			extern
#else   // #ifdef ___GEM_LIB_CREATION___
#define RFC_API			IMPORT_FROM_LIB_API
#define RFC_VAR			IMPORT_FROM_LIB_VAR
#endif  // #ifdef ___GEM_LIB_CREATION___
#endif  // #ifndef GEM_API

#ifndef AllocMemOnRemoteProcess
#define		AllocMemOnRemoteProcess(__hProcess, __size) \
					VirtualAllocEx((__hProcess),0,(__size),MEM_COMMIT,PAGE_READWRITE)
#endif // #ifndef AllocMemOnRemoteProcess
#ifndef DeleteMemOnRemoteProcess
#define		DeleteMemOnRemoteProcess(__hProcess,__mem) \
					VirtualFreeEx((__hProcess),(LPVOID)((size_t)(__mem)), 0, MEM_RELEASE)
#endif  // #ifndef DeleteMemOnRemoteProcess

__BEGIN_C_DECLS

struct SInjectedType {
	uint64_t	dllNameLen;		// rounded to 8
	//uint16_t	funcionNameLen;	// rounded to 8
	//uint64_t	dllName;		// should be casted to char*
	//uint64_t	functionName;	// should be casted to char*
	uint64_t	memorySize;		// is a same as provided by user, but allocation done more
	//uint64_t	memoryAddress;
};

#define ROUND_FOR_PADDING(_value)							(   (  ((_value)>>3)+1  )<<3   )
#define DLL_NAME_FROM_INJECT(_injectData)					(  ( (char*)(_injectData) ) + sizeof(struct SInjectedType)   )
#define MEMORY_FROM_INJECT(_injectData, _injectLocal)		(  DLL_NAME_FROM_INJECT(_injectData) + \
																((size_t)(_injectLocal).dllNameLen)  )
#define FUNCTION_NAME_FROM_INJECT(_injectData,_injectLocal)	(  MEMORY_FROM_INJECT(_injectData,_injectLocal) + \
																( (size_t)ROUND_FOR_PADDING( (_injectLocal).memorySize ) )  )


__END_C_DECLS


#endif  // #ifndef __wlac_wlac_rfc_private_h__
