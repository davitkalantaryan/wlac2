// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <common_definations_wul.h> 
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __common_definations_wul_h__
#define __common_definations_wul_h__

#ifndef WLAC_USED
#define WLAC_USED
#endif

#define LITTLE_ENDIAN	1
#define BIG_ENDIAN		2

#if defined(_M_ARM)  // some other platforms for windows with little endian?
#define BYTE_ORDER		BIG_ENDIAN
#else
#define BYTE_ORDER		LITTLE_ENDIAN
#endif

#ifndef __TARGET_COMPILER__
#ifndef __GNUC__
#define	__GNUC__		1
#define __GLIBC__		2
#define	__GLIBC_MINOR__	15
#endif  // #ifndef __GNUC__
#define __TARGET_COMPILER__ "GNUC"
#endif  // #ifndef TARGET_COMPILER

#ifndef __TARGET_OS__
#ifndef LINUX
#define LINUX
#endif
#define __TARGET_OS__	"LINUX"
#endif

#if (defined(_WIN64) | defined(_WIN32) ) & !defined(WIN32)
#define WIN32
#endif

#ifdef _MSC_VER
#define IMPORT_FROM_LIB_API		_declspec(dllimport)
#define IMPORT_FROM_LIB_VAR		extern _declspec(dllimport)
#define EXPORT_TO_LIB_API		_declspec(dllexport)
#define EXPORT_TO_LIB_VAR		_declspec(dllexport)
#define	NOT_IMPLEMENTED_API
#elif defined(__GNUC__)  // #ifdef _MSC_VER
#else  // #ifdef _MSC_VER
#endif // #ifdef _MSC_VER

#ifdef _WIN32
#define PATH_CHAR	'\\'
#else
#define PATH_CHAR	'/'
#endif

#ifndef WUL_EXTERN_C
#ifdef __cplusplus
#define WUL_EXTERN_C	extern "C"
#else
#define WUL_EXTERN_C
#endif
#endif

#ifndef __BEGIN_C_DECLS
#ifdef __cplusplus
#define __BEGIN_C_DECLS	extern "C" {
#else  /* #ifdef __cplusplus */
#define __BEGIN_C_DECLS
#endif	/* #ifdef __cplusplus */
#endif  /* #ifndef __BEGIN_C_DECLS */

#ifndef __END_C_DECLS
#ifdef __cplusplus
#define	__END_C_DECLS		}
#else  /* #ifdef __cplusplus */
#define	__END_C_DECLS
#endif	/* #ifdef __cplusplus */
#endif  /* #ifndef __END_C_DECLS */

#ifdef _MSC_VER
#define _DEPRICATED_TEXT_(_text_)	__declspec(deprecated(_text_))
#define _DEPRICATED_	_DEPRICATED_TEXT_("")
#else  // #ifdef _MSC_VER
#define	_DEPRICATED_	__attribute__((deprecated))
#endif  // #ifdef _MSC_VER

#ifdef _MSC_VER
#define SET_WLAC_ALIAS(_file_)		__pragma (include_alias(<##_file_>, <redesigned/##_file_>)) \
									__pragma (include_alias("##_file_", "redesigned/##_file_"))
#define RESET_WLAC_ALIAS(_file_)	__pragma (include_alias("##_file_", "##_file_")); \
									__pragma (include_alias(<##_file_>, <##_file_>))
#endif // #ifdef _MSC_VER

#ifndef GEM_API_FAR
#ifdef ___GEM_LIB_CREATION___
#define GEM_API_FAR		EXPORT_TO_LIB_API
#define GEM_VAR_FAR		extern EXPORT_TO_LIB_VAR
#elif defined(__USING_GEM_SOURCES__)  // #ifdef ___GEM_LIB_CREATION___
#define GEM_API_FAR
#define GEM_VAR_FAR		extern
#else   // #ifdef ___GEM_LIB_CREATION___
#define GEM_API_FAR		IMPORT_FROM_LIB_API
#define GEM_VAR_FAR		IMPORT_FROM_LIB_VAR
#endif  // #ifdef ___GEM_LIB_CREATION___
#endif  // #ifndef GEM_API_FAR

 /// Not implemented
#define NON_IMPLEMENTED_API(__api__)
#define IMPLEMENTED_API(__api__)	__api__

#ifndef __THROW
#define __THROW
#endif

#ifndef RESTRICT
#if(_MSC_VER >= 1400)
#define RESTRICT	__restrict
#else
#define RESTRICT
#endif
#endif  // #ifndef RESTRICT

#ifndef InterlockedCompareExchangePointerNew
#if(_MSC_VER >= 1400)
#define InterlockedCompareExchangePointerNew InterlockedCompareExchangePointer
#else
#define InterlockedCompareExchangePointerNew InterlockedCompareExchange
#endif
#endif

#ifndef LIKELY
#define LIKELY(_cond)	(_cond)
#endif

#ifdef _WIN64
#define _VOLATILE_	volatile
#else
#define _VOLATILE_
#endif

#ifndef UNDEFINED_API_NOT_NEEDED
#endif  // #ifndef UNDEFINED_API_NOT_NEEDED

#define __USE_POSIX199309

#ifdef _TEST_PRINTF_USED_
#endif  // #ifdef _TEST_PRINTF_USED_

#endif  /* #ifndef __common_definations_wul_h__ */
