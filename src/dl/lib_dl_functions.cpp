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


__END_C_DECLS
