//
// file:			wlac2_internal_header.h
// path:			include/wlac2_internal_header.h
// created on:		2021 Dec 28
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef WLAC2_INCLUDE_WLAC2_INTERNAL_HEADER_H
#define WLAC2_INCLUDE_WLAC2_INTERNAL_HEADER_H


#ifdef WLAC2_CPPUTILS_DEFS_NEEDED
#include <cpputils_internal_header.h>
#endif

#if defined(__EMSCRIPTEN__) || defined(EMSCRIPTEN) || defined(WLAC2_WASM)
#define WLAC2_EMSCRIPTEN_IS_USED
#endif

#ifdef WLAC2_SYSTEM_EXE_START_IS_POSSIBLE
#define WLAC2_EXE_START_IS_POSSIBLE
#endif

#if !defined(WLAC2_EMSCRIPTEN_IS_USED) // todo: continue list
#define WLAC2_DESKTOP_IS_USED
#endif

#if !defined(WLAC2_EMSCRIPTEN_IS_USED) || defined(USE_DLOPEN_FROM_WASM)
#define WLAC2_DLOPEN_IS_POSSIBLE
#endif

#ifdef _MSC_VER
#pragma warning (disable:4355)
#define WLAC2_BEFORE_CPP_17_FALL_THR
#if defined(_MSVC_LANG) && (_MSVC_LANG>=201100L)
#define WLAC2_CPP_11_DEFINED		1
#endif
#if defined(_MSVC_LANG) && (_MSVC_LANG>=201400L)
#define WLAC2_CPP_14_DEFINED		1
#endif
#if defined(_MSVC_LANG) && (_MSVC_LANG>=201700L)
#define WLAC2_CPP_17_DEFINED		1
#endif
#if defined(_MSVC_LANG) && (_MSVC_LANG>=202000L)
#define WLAC2_CPP_20_DEFINED		1
#endif
#define WLAC2_THISCALL	__thiscall
#define WLAC2_DLL_PUBLIC		__declspec(dllexport)
#define WLAC2_DLL_PRIVATE
#define WLAC2_IMPORT_FROM_DLL	__declspec(dllimport)
#if !defined(_WIN64) && !defined(_M_ARM64)
#define WLAC2_32_BIT
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define WLAC2_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
#if defined(__cplusplus) && (__cplusplus>=201100L)
#define WLAC2_CPP_11_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201400L)
#define WLAC2_CPP_14_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201700L)
#define WLAC2_CPP_17_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=202000L)
#define WLAC2_CPP_20_DEFINED		1
#endif
#define WLAC2_THISCALL
//#define WLAC2_DLL_PUBLIC		__attribute__((visibility("default")))
#define WLAC2_DLL_PUBLIC
#define WLAC2_DLL_PRIVATE		__attribute__((visibility("hidden")))
#define WLAC2_IMPORT_FROM_DLL
#elif defined(__CYGWIN__)
#define WLAC2_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
#if defined(__cplusplus) && (__cplusplus>=201100L)
#define WLAC2_CPP_11_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201400L)
#define WLAC2_CPP_14_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201700L)
#define WLAC2_CPP_17_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=202000L)
#define WLAC2_CPP_20_DEFINED		1
#endif
#define WLAC2_THISCALL
#define WLAC2_DLL_PUBLIC		__attribute__((dllexport))
#define WLAC2_DLL_PRIVATE
#define WLAC2_IMPORT_FROM_DLL	__attribute__((dllimport))
#elif defined(__MINGW64__) || defined(__MINGW32__)
#define WLAC2_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
#if defined(__cplusplus) && (__cplusplus>=201100L)
#define WLAC2_CPP_11_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201400L)
#define WLAC2_CPP_14_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201700L)
#define WLAC2_CPP_17_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=202000L)
#define WLAC2_CPP_20_DEFINED		1
#endif
#define WLAC2_THISCALL
#define WLAC2_DLL_PUBLIC		_declspec(dllexport)
#define WLAC2_DLL_PRIVATE
#define WLAC2_IMPORT_FROM_DLL	_declspec(dllimport)
#elif defined(__SUNPRO_CC)
	// #define WLAC2_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ; // ???
#define WLAC2_BEFORE_CPP_17_FALL_THR
#if defined(__cplusplus) && (__cplusplus>=201100L)
#define WLAC2_CPP_11_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201400L)
#define WLAC2_CPP_14_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=201700L)
#define WLAC2_CPP_17_DEFINED		1
#endif
#if defined(__cplusplus) && (__cplusplus>=202000L)
#define WLAC2_CPP_20_DEFINED		1
#endif
#define WLAC2_THISCALL
#define WLAC2_DLL_PUBLIC
#define WLAC2_DLL_PRIVATE		__hidden
#define WLAC2_IMPORT_FROM_DLL
#endif  // #ifdef _MSC_VER

#if defined(WLAC2_COMPILING_SHARED_LIB)
#define WLAC2_EXPORT WLAC2_DLL_PUBLIC
#elif defined(WLAC2_USING_STATIC_LIB_OR_OBJECTS)
#define WLAC2_EXPORT
#else
#define WLAC2_EXPORT WLAC2_IMPORT_FROM_DLL
#endif

#if defined(WLAC2_HOOK_COMPILING_SHARED_LIB)
#define WLAC2_HOOK_EXPORT WLAC2_DLL_PUBLIC
#elif defined(WLAC2_HOOK_USING_STATIC_LIB_OR_OBJECTS)
#define WLAC2_HOOK_EXPORT
#else
#define WLAC2_HOOK_EXPORT WLAC2_IMPORT_FROM_DLL
#endif

#ifdef __cplusplus
#define WLAC2_BEGIN_C   extern "C" {
#define WLAC2_END_C     }
#define WLAC2_EXTERN_C  extern "C"
#else
#define WLAC2_BEGIN_C
#define WLAC2_END_C
#define WLAC2_EXTERN_C
#endif


#ifndef WLAC2_FALLTHROUGH
#ifdef WLAC2_CPP_17_DEFINED
#define WLAC2_FALLTHROUGH	[[fallthrough]] ;
#else
#define WLAC2_FALLTHROUGH	WLAC2_BEFORE_CPP_17_FALL_THR
//#define WLAC2_FALLTHROUGH
#endif
#endif

#ifndef WLAC2_NULL
#ifdef WLAC2_CPP_11_DEFINED
#define WLAC2_NULL	nullptr
#else
#define WLAC2_NULL	NULL
#endif
#endif

#ifndef WLAC2_REGISTER
#ifdef __cplusplus
#define WLAC2_REGISTER
#else
#define WLAC2_REGISTER	register
#endif
#endif

//#define WLAC2_IS_LITTLE_ENDIAN (((union { unsigned x; unsigned char c; }){1}).c)

#if defined(_DEBUG) || defined(CPPUTILS_DEBUG) || defined(WLAC2_DEBUG)
// we have debug compilation
#else
// we have release
#ifndef NDEBUG
// let's define NDEBUG (No DEBUG)
#define NDEBUG
#endif
#endif

#ifdef NDEBUG
//#define MAKE_VOID(_val)                 static_cast<void>(_val)
#define DO_DEBUG_EXP(_exp)
#define WLAC2_SAFE_CAST(_type,_val)	static_cast<_type>(_val)
#else
//#define MAKE_VOID(_var)                 do{}while(0)
//#define MAKE_VOID(_var)
#define DO_DEBUG_EXP(_exp)              _exp ;
#define WLAC2_SAFE_CAST(_type,_val)	dynamic_cast<_type>(_val)
#endif


#ifdef __cplusplus
#define WLAC2_STATIC_CAST(_type,_val)    static_cast<_type>(_val)
#else
#define WLAC2_STATIC_CAST(_type,_val)    ( (_type)(_val) )
#endif


#ifdef WLAC2_CPP_20_DEFINED
#define WLAC2_NODISCARD	[[nodiscard]]
#elif defined(WLAC2_CPP_11_DEFINED)
#define WLAC2_NODISCARD	noexcept
#else
#define WLAC2_NODISCARD	throw()
#endif

#ifdef WLAC2_CPP_11_DEFINED
#define WLAC2_NOEXCEPT	noexcept
#define WLAC2_DELETE(_func)		_func = delete ;
#else
#define WLAC2_NOEXCEPT	throw()
#define WLAC2_DELETE(_func)
#endif

#ifdef WLAC2_DO_NOT_USE_AT_ALL
#ifndef WLAC2_DO_NOT_USE_NEW_DEL
#define WLAC2_DO_NOT_USE_NEW_DEL
#endif
#ifndef WLAC2_DO_NOT_USE_MAL_FREE
#define WLAC2_DO_NOT_USE_MAL_FREE
#endif
#endif

#define WLAC2_STRINGIFY(_num)		WLAC2_STRINGIFY_RAW(_num)
#define WLAC2_STRINGIFY_RAW(_num)	#_num


#endif  // #ifndef INCLUDE_MICOS_CONTROLLER_MICOS_CONTROLLER_INTERNAL_HEADER_H
