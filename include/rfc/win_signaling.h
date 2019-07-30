//
// file:		win_signaling.h
// created on:	2018 Dec 18
//

#include <rfc/wlac_rfc.h>

#ifndef WIN_SIG_API_FAR
#ifdef ___WIN_SIG_LIB_CREATION___
#define WIN_SIG_API_FAR		EXPORT_TO_LIB_API
#define WIN_SIG_VAR_FAR		extern EXPORT_TO_LIB_VAR
#elif defined(__USING_WIN_SIG_SOURCES__)  // #ifdef ___GEM_LIB_CREATION___
#define WIN_SIG_API_FAR
#define WIN_SIG_VAR_FAR		extern
#else   // #ifdef ___GEM_LIB_CREATION___
#define WIN_SIG_API_FAR		IMPORT_FROM_LIB_API
#define WIN_SIG_VAR_FAR		IMPORT_FROM_LIB_VAR
#endif  // #ifdef ___GEM_LIB_CREATION___
#endif  // #ifndef GEM_API_FAR

__BEGIN_C_DECLS

WIN_SIG_API_FAR int  WinSigRemotePrintf(int pid, const char* a_format, ...);
WIN_SIG_API_FAR void InitializeWinSigLibrary(void* instance);  // do not use this, this will do library
WIN_SIG_API_FAR void CleanupWinSigLibrary(void);

__END_C_DECLS
