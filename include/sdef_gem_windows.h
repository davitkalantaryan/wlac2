// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sdef_gem_windows.h> 
 *
 *  This is the file that is included from all GEM related includes
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_sdef_gem_windows_h__
#define __win_sdef_gem_windows_h__

#include <first_includes/common_include_for_headers.h>
#include <first_includes/gem_undefined_apis.h>
#include <windows.h>


#ifndef RESTRICT
#define RESTRICT  // may be restrict?
#endif  // #ifndef RESTRICT

#define _(__a_str__)	(__a_str__)

#ifdef _WIN64
#ifndef __amd64
#define __amd64
#endif  // #ifndef __amd64
#else  // #ifdef _WIN64
#ifndef __i386
#define __i386
#endif  // #ifndef __i386
#endif  // #ifdef _WIN64

#ifndef __KEY_TYPE_TLS__
//#define __KEY_TYPE_TLS__	DWORD
#define __KEY_TYPE_TLS__	LONG
#endif

__BEGIN_C_DECLS

GEM_VAR_FAR void* g_pWlacLoadCode;
GEM_VAR_FAR void* g_pProcessExitCallCode;
GEM_VAR_FAR int   g_nLibraryCleanupStarted;

#if !defined(lib_cons_dest_t_defined) && !defined(__lib_cons_dest_t_defined)
typedef void(*lib_cons_dest_t)(void);
#define lib_cons_dest_t_defined
#define __lib_cons_dest_t_defined
#endif  // #if !defined(lib_cons_dest_t_defined) && !defined(__lib_cons_dest_t_defined)

#define	__GET_FUNC_NAME__(_a_func_) #_a_func_

#ifndef WLAC_LIBRARY_NOT_INCLUDE
#endif // #ifndef WLAC_LIBRARY_NOT_INCLUDE

#ifndef NETLIB_CONSTRUCT_DESTRUCT_BASE
#define NETLIB_CONSTRUCT_DESTRUCT_BASE(_constructor_function_,_destructor_function_) \
static void Destructor_##_destructor_function_(void)\
{\
	static int snLibraryCleanupForCurrentSource = 0; \
	if(snLibraryCleanupForCurrentSource==0) \
	{\
		snLibraryCleanupForCurrentSource = 1; \
		g_nLibraryCleanupStarted = 1; \
		if(!g_pWlacLoadCode){(_destructor_function_)();} \
		/*(_destructor_function_)();*/ \
		WSACleanup(); \
	}\
}\
static int Construncor_##_constructor_function_(void)\
{\
	static int snLibraryInitializationForCurrentSource = 0; \
	if(snLibraryInitializationForCurrentSource==0) \
	{\
		snLibraryInitializationForCurrentSource = 1;\
		if(!initialize_windows_socket_library()){ \
			snLibraryInitializationForCurrentSource = 0;return FALSE; \
		}  \
		(_constructor_function_)(); \
	}\
	return TRUE; \
}
#endif  // #ifndef NETLIB_CONSTRUCT_DESTRUCT_BASE


#ifndef NETLIB_CONSTRUCT_DESTRUCT
#if defined(_USRDLL)
#define NETLIB_CONSTRUCT_DESTRUCT(_constructor_function_,_destructor_function_,_extra_) \
NETLIB_CONSTRUCT_DESTRUCT_BASE(_constructor_function_,_destructor_function_) \
EXPORT_TO_LIB_API BOOL WINAPI DllMain##_extra_(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) \
{\
	switch (fdwReason) \
	{\
	case DLL_PROCESS_ATTACH: /*DisableThreadLibraryCalls(hinstDLL);*/\
		return Construncor_##_constructor_function_(); \
	case DLL_PROCESS_DETACH: \
		g_pProcessExitCallCode = lpvReserved; \
		Destructor_##_destructor_function_();\
	default: break; \
	}\
	return TRUE;\
} \
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){return DllMain##_extra_(hinstDLL,fdwReason,lpvReserved);}
#else  // #if defined(_USRDLL)
#define NETLIB_CONSTRUCT_DESTRUCT(_constructor_function_,_destructor_function_,_extra_) \
NETLIB_CONSTRUCT_DESTRUCT_BASE(_constructor_function_,_destructor_function_) \
class netlib_cons_destr##_extra_ \
{ \
public: \
	netlib_cons_destr##_extra_(void) \
		{Construncor_##_constructor_function_();} \
	~netlib_cons_destr##_extra_(){Destructor_##_destructor_function_();} \
}static volatile s_netlib_cons_destr_; 
//static volatile netlib_cons_destr##extra  s_netlib_cons_destr_;
#endif // #if defined(_USRDLL)
#endif // #ifndef NETLIB_CONSTRUCT_DESTRUCT


#define	_MAX_BUFFER_LEN_				65536 // ~65 MB
#define _NEW_SOCKET_DEFINE_FOR_64_BIT

__END_C_DECLS


#endif  // #ifndef __win_sdef_gem_windows_h__
