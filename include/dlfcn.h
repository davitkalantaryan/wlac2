// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

//
// file:		dlfcn.h
// created on:	2018 Dec 17
//
#ifndef __wlac_dlfcn_h__
#define __wlac_dlfcn_h__

#include <first_includes/common_include_for_headers.h>


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>


// https://linux.die.net/man/3/dlopen
// https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-loadlibraryexa

#define	DL_ERROR_BUFFER_LENGTH	1024
#define	RTLD_LAZY				(LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE|LOAD_LIBRARY_AS_IMAGE_RESOURCE)

#define dlopen(_fileName,_flag)			( (void*)LoadLibraryExA((_fileName),NULL,(_flag))	)
#define dlclose(_handle)				FreeLibrary((HMODULE)(_handle))
#define	dlsym(_handle, _symbolName)		( (void*)GetProcAddress((HMODULE)(_handle),(_symbolName)) )

__BEGIN_C_DECLS


GEM_API_FAR char *dlerror(void);

__END_C_DECLS

#endif  // #ifndef __wlac_dlfcn_h__
