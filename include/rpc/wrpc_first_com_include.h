/*
 *	File: <wrpc_first_com_include.h> For WINDOWS MFC
 *
 *	Created on: Dec 29, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This is the include file that is included by all gem related headers
 *
 */
#ifndef __wrpc_first_com_include1_h__
#define __wrpc_first_com_include1_h__

#if defined(_MSC_VER) & (_MSC_VER>1400)
// 'function': incompatible types - from 'pmap *' to 'caddr_t' [CLNT_CALL(client, PMAPPROC_UNSET, xdr_pmap, &parms, xdr_bool, &rslt,tottimeout); ]
#pragma warning(disable:4133) 
#endif  // #if defined(_MSC_VER) & (_MSC_VER>1400)

#include <first_includes/common_include_for_headers.h>

#ifndef WSAerrno
#define WSAerrno (WSAGetLastError())
#endif

#ifndef WRPC_API_FAR
#ifdef __WRPC_LIB_CREATION__
#define WRPC_API_FAR		EXPORT_TO_LIB_API
#define WRPC_VAR_FAR		extern EXPORT_TO_LIB_VAR
#elif defined(__USING_WRPC_SOURCES__)  // #ifdef __WRPC_LIB_CREATION__
#define WRPC_API_FAR
#define WRPC_VAR_FAR
#else   // #ifdef __WRPC_LIB_CREATION__
#define WRPC_API_FAR		IMPORT_FROM_LIB_API
#define WRPC_VAR_FAR		IMPORT_FROM_LIB_VAR
#endif  // #ifdef __WRPC_LIB_CREATION__
#endif  // #ifndef WRPC_API_FAR

 // these 2 definations will be deleted
#ifndef LINKAGE_HDR
 //#define LINKAGE_HDR	WRPC_API_FAR
#define LINKAGE_HDR
#endif
#ifndef LINKAGE_SRC
#define LINKAGE_SRC	WRPC_API_FAR
#endif

#ifndef DllExport
#define DllExport WRPC_API_FAR
#endif

// Include these headers when everything is already included
#if !defined(__cplusplus)
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/svc.h>
#include <rpc/clnt.h>
#include <process.h>
#include <rpc/pmap_prot.h>
#include <rpc/auth_unix.h>
#endif

#endif  // #ifndef __wrpc_first_com_include_h__
