
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SUNRPC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SUNRPC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SUNRPC_EXPORTS
#define SUNRPC_API __declspec(dllexport)
#else
#define SUNRPC_API __declspec(dllimport)
#endif
#include <windows.h>

#if !defined(__win_sys_time_h__) && !defined(timezone__defined)
/* Structure crudely representing a timezone.
This is obsolete and should never be used.  */
struct timezone
{
	int tz_minuteswest;		/* Minutes west of GMT.  */
	int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};
#define timezone__defined
#endif


/*
// This class is exported from the sunrpc.dll
class SUNRPC_API CSunrpc {
public:
	CSunrpc(void);
	// TODO: add your methods here.
};

extern SUNRPC_API int nSunrpc;

SUNRPC_API int fnSunrpc(void);
*/
