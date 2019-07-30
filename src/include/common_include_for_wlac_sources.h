
// common_include_for_wlac_sources.h

#ifndef __common_include_for_wlac_sources_h__
#define __common_include_for_wlac_sources_h__


#ifndef __USING_GEM_SOURCES__
#ifndef ___GEM_LIB_CREATION___
#define ___GEM_LIB_CREATION___
#endif
#endif

#ifdef __USING_GEM_SOURCES__
//#define GEM_API_FAR
//#define GEM_VAR_FAR		extern 
#define GEM_API			
#define GEM_VAR			
#else
//#define GEM_API_FAR		EXPORT_TO_LIB_API
//#define GEM_VAR_FAR		extern EXPORT_TO_LIB_VAR
#define GEM_API			EXPORT_TO_LIB_API
#define GEM_VAR			EXPORT_TO_LIB_VAR
#endif  // #ifdef __USING_NWLDAP_SOURCES__

//#define u_long unsigned __int64  // To do
#include "first_includes/common_include_for_headers.h"


#endif // #ifndef __common_include_for_headers_h__
