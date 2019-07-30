// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: remote_function_caller_base.h
 *
 *	Created on: Sep 24, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions ...
 *		1) ...
 *		2) ...
 *		...
 *
 *
 */
#ifndef __remote_function_caller_base_h__
#define __remote_function_caller_base_h__

#ifndef REMOTE_FNC_LINKAGE
#ifdef _USRDLL
#define REMOTE_FNC_LINKAGE	_declspec(dllexport)
#else
#define REMOTE_FNC_LINKAGE
#endif
#endif

#ifndef _uint64_ttt_defined
#define _uint64_ttt_defined
typedef unsigned __int64	_uint64_ttt;
#endif  // #ifndef _uint64_ttt_defined

#define	ACCESS_FREOPEN					1
#define	GET_FILE_DESCRIPTOR				2
#define	GET_STANDARD_FILE_DESCRIPTOR	3
#define	ACCESS_FNC_CALL					4
#define	NEW_THREAD_INFO					5
#define	FREEING_LIBRARY					6

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stddef.h>

#define to_pointer(_value1)	((void*)((size_t)(_value1)))
#define to_value(_pointer1)	((_uint64_ttt)((size_t)(_pointer1)))

#ifndef AllocMemOnRemoteProcess
#define		AllocMemOnRemoteProcess(__hProcess, __size) \
					(_uint64_ttt)VirtualAllocEx((__hProcess),0,(__size),MEM_COMMIT,PAGE_READWRITE)
#endif // #ifndef AllocMemOnRemoteProcess
#ifndef DeleteMemOnRemoteProcess
#define		DeleteMemOnRemoteProcess(__hProcess,__mem) \
					VirtualFreeEx((__hProcess),(LPVOID)((size_t)(__mem)), 0, MEM_RELEASE)
#endif  // #ifndef DeleteMemOnRemoteProcess

#else  // #ifdef WIN32

////

#endif  // #ifdef WIN32

#ifdef __cplusplus
extern "C"
{
#endif

typedef __int64(*remote_function_type2)(_uint64_ttt a_arg);


#ifndef INJDATA_defined
typedef struct {
	__int64				access_type;
	char*				file_name;
	char*				function_name;
	_uint64_ttt			data_in;
	__int64				in_freelib_out_return;  // <0 error
} INJDATA, *PINJDATA;
#define INJDATA_defined
#endif  // #ifndef INJDATA_defined

#ifndef STDFREOPENDATA_defined
typedef struct {
	char*				file_name;
	char*				mode;
	unsigned __int64	std_fd; // 0->stdin, 1->stdout, 2->stderr
} STDFREOPENDATA, *PSTDFREOPENDATA;
#define STDFREOPENDATA_defined
#endif  // #ifndef STDFREOPENDATA_defined

#ifndef FREOPENDATA_defined
typedef struct {
	char*				file_name;
	char*				mode;
	FILE*				stream; 
} FREOPENDATA, *PFREOPENDATA;
#define FREOPENDATA_defined
#endif  // #ifndef FREOPENDATA_defined


typedef struct SRemoteData
{
	INJDATA*			pInjectDataRemote;
	char*				pcRemoteNameOfRemoteCallLibName;
	char*				pcRemoteNameOfFile;
	char*				pcRemoteNameOfFunction;
	HANDLE				hThread;
}SRemoteData;

#ifdef __cplusplus
}
#endif


#endif  // #ifndef __remote_function_caller_base_h__
