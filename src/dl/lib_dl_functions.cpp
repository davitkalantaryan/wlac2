//
//
//

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include "dlfcn.h"
#include <stdio.h>

__BEGIN_C_DECLS

GEM_API char *dlerror(void)
{
	static char vcErrorBuffer[DL_ERROR_BUFFER_LENGTH];
	int nLastError = GetLastError();

	if(!nLastError){return NULL;}

	if(!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,NULL,0,0,vcErrorBuffer,DL_ERROR_BUFFER_LENGTH,NULL))
	{
		snprintf(vcErrorBuffer, DL_ERROR_BUFFER_LENGTH,"Errorcode is 0x%x\n", nLastError);
	}

	return vcErrorBuffer;

}


GEM_API void* dlopen(const char* a_filename, int)
{
	HMODULE* returnModulePtr = (HMODULE*)malloc(sizeof(HMODULE));
	if (!returnModulePtr) { return NULL; }

	*returnModulePtr = LoadLibraryA(a_filename);
	if (!(*returnModulePtr)) {
		free(returnModulePtr);
		return NULL;
	}

	return (void*)returnModulePtr;
}


GEM_API int dlclose(void* a_handle)
{
	if (a_handle) {
		HMODULE* returnModulePtr = (HMODULE*)a_handle;
		BOOL bRet = FreeLibrary(*returnModulePtr);
		free(returnModulePtr);
		return bRet ? 0 : -1;
	}

	return -1;
}


GEM_API void* dlsym(void* a_handle, const char* a_symbol)
{
	if (a_handle) {
		HMODULE* returnModulePtr = (HMODULE*)a_handle;
		return GetProcAddress(*returnModulePtr, a_symbol);
	}

	return NULL;
}


__END_C_DECLS
