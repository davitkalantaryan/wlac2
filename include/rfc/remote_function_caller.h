// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: remote_function_caller.h
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
#ifndef __remote_function_caller_h__
#define __remote_function_caller_h__

#ifndef REMOTE_FNC_LINKAGE
#define REMOTE_FNC_LINKAGE
#endif

#include "windows_signal.h"
#include "remote_function_caller_base.h"
#include <windows.h>

#define	process_fromPID(__pid)			OpenProcess(PROCESS_ALL_ACCESS,FALSE,(__pid)),(__pid)
#define	process_fromHandle(__hProcess)	(__hProcess),GetProcessId((__hProcess))

#ifdef __cplusplus
extern "C"
{
#endif

REMOTE_FNC_LINKAGE BOOL initialize_local_signals(void);
REMOTE_FNC_LINKAGE void clean_local_signals(void);
REMOTE_FNC_LINKAGE BOOL modify_signal_table(int sig, const signal_handler_struct_t* handler_cp, signal_handler_struct_t* handler_old_p);
REMOTE_FNC_LINKAGE BOOL wait_for_signal_synchron(siginfo_t* signal, int waitMS);

/// Fast versions of remote functions, when handle and pid are available
REMOTE_FNC_LINKAGE BOOL RemoteLoadLibrary_fast(HANDLE hProcess, const char* library_name);
REMOTE_FNC_LINKAGE void RemoteFreeLibrary_from_resource_fast(HANDLE hProcess, int pid, SRemoteData* pResource);
REMOTE_FNC_LINKAGE void RemoteFreeLibrary_fast(HANDLE hProcess, int pid, const char* libraryNameOnLocal);
REMOTE_FNC_LINKAGE void Clearresourse_fast(HANDLE hProcess, SRemoteData* resource);
REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_fast(HANDLE hProcess, int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data, SRemoteData* resource, int free_lib_or_not);
#define RunFunctionOnRemoteProcess_fast(_hProcess,_pid,_library_name, _func_name, _data,_resource) \
	RunProcedureOnRemoteProcess_fast((_hProcess),(_pid),ACCESS_FNC_CALL,(_library_name),(_func_name),(_data),(_resource),0)
REMOTE_FNC_LINKAGE __int64 AccessRemoteProcess_fast(HANDLE hProcess, int pid, __int64 access_type,
	const char* file_name, const char* func_name, __int64 data);
REMOTE_FNC_LINKAGE FILE* fopen_remote_fast(HANDLE hProcess, int pid, const char* fileName, const char* mode);
REMOTE_FNC_LINKAGE FILE* stdhandle_remote_fast(HANDLE hProcess, int pid, int fd); // fd: 0->stdin, 1->stdout, 2->stderr
REMOTE_FNC_LINKAGE int freopen_std_new_fast(HANDLE hProcess, int pid, const char *path, const char *mode, int std_fd);
#define CallFunctionOnRemoteProcess_fast(_hProcess,_pid,_library_name, _func_name, _data) \
	AccessRemoteProcess_fast((_hProcess),(_pid),ACCESS_FNC_CALL,(_library_name),(_func_name),(_data))
REMOTE_FNC_LINKAGE int sigqueue_ex_fast(HANDLE hProcess, int pid, int sig, const siginfo_t* sig_info);
REMOTE_FNC_LINKAGE int remote_printf_fast(HANDLE hProcess, int pid, const char* format, ...);
REMOTE_FNC_LINKAGE int remote_print_error_fast(HANDLE hProcess, int pid, const char* format, ...);
REMOTE_FNC_LINKAGE int remote_perror_fast(HANDLE hProcess, int pid, const char* error_string);
REMOTE_FNC_LINKAGE int sigqueue_fast(HANDLE hProcess, int pid, int sig, const union sigval value);


/// To call having only process HANDLE
#define RemoteLoadLibrary_fromHandle	RemoteLoadLibrary_fast
#define RemoteFreeLibrary_from_resource_fromHandle(__hProcess, ____resource) \
	RemoteFreeLibrary_from_resource_fast(process_fromHandle(__hProcess),(____resource))
#define RemoteFreeLibrary_fromHandle(__hProcess, __library_name) \
	RemoteFreeLibrary_fast(process_fromHandle(__hProcess),(__library_name))
#define	Clearresourse_fromHandle		Clearresourse_fast
#define RunProcedureOnRemoteProcess_fromHandle(__hProcess, __access_type,__fl_name, __fnc_name, __data, __resource,__free_lib_or_not) \
	RunProcedureOnRemoteProcess_fast(process_fromHandle(__hProcess),(__access_type),(__fl_name),(__fnc_name),(__data),(__resource),(__free_lib_or_not))
#define RunFunctionOnRemoteProcess_fromHandle(_hProcess,_library_name, _func_name, _data,_resource) \
	RunProcedureOnRemoteProcess_fromHandle((_hProcess),ACCESS_FNC_CALL,(_library_name),(_func_name),(_data),(_resource),0)
#define AccessRemoteProcess_fromHandle(__hProcess, __access_type,__fl_name, __fnc_name, __data) \
	AccessRemoteProcess_fast(process_fromHandle(__hProcess),(__access_type),(__fl_name),(__fnc_name),(__data))
#define fopen_remote_fromHandle(__hProcess,__fl_name,__mode)	\
	fopen_remote_fast(process_fromHandle(__hProcess),(__fl_name),(__mode))
#define stdhandle_remote_fromHandle(__hProcess,__fd) \
	stdhandle_remote_fast(process_fromHandle(__hProcess),(__fd))
#define freopen_std_new_fromHandle(__hProcess,__path,__mode,__std_fd) \
	freopen_std_new_fast(process_fromHandle(__hProcess),(__path),(__mode),(__std_fd))
#define CallFunctionOnRemoteProcess_fromHandle(__hProcess,__libr_name, __func_name, __data) \
	CallFunctionOnRemoteProcess_fast(process_fromHandle(__hProcess),(__libr_name),(__func_name),(__data))
#define sigqueue_ex_fromHandle(__hProcess,__sig, __sig_info) \
	sigqueue_ex_fast(process_fromHandle(__hProcess),(__sig),(__sig_info))
REMOTE_FNC_LINKAGE int remote_printf_fromHandle(HANDLE hProcess, const char* format, ...);
REMOTE_FNC_LINKAGE int remote_print_error_fromHandle(HANDLE hProcess, const char* format, ...);
#define sigqueue_fromHandle(__hProcess,__sig, __sig_value) \
	sigqueue_fast(process_fromHandle(__hProcess),(__sig),(__sig_value))


/// To call having only pid
REMOTE_FNC_LINKAGE BOOL RemoteLoadLibrary_fromPID(int pid, const char* __libr_name);
REMOTE_FNC_LINKAGE void RemoteFreeLibrary_from_resource_fromPID(int pid, SRemoteData* pResource);
REMOTE_FNC_LINKAGE void RemoteFreeLibrary_fromPID(int pid, const char* libraryNameOnLocal);
REMOTE_FNC_LINKAGE void Clearresourse_fromPID(int pid, SRemoteData* pResource);
REMOTE_FNC_LINKAGE int RunProcedureOnRemoteProcess_fromPID(int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data, SRemoteData* resource, int free_lib_or_not, HANDLE* phProcess);
#define RunFunctionOnRemoteProcess_fromPID(_pid,_library_name, _func_name, _data,_resource,_phProcess) \
	RunProcedureOnRemoteProcess_fromPID((_pid),ACCESS_FNC_CALL,(_library_name),(_func_name),(_data),(_resource),0,(_phProcess))
REMOTE_FNC_LINKAGE int freopen_std_new_fromPID(int pid, const char *path, const char *mode, int stdFd);
REMOTE_FNC_LINKAGE FILE* fopen_remote_fromPID(int pid, const char* fileName, const char* mode);
REMOTE_FNC_LINKAGE FILE* stdhandle_remote_fromPID(int pid, int nFD);
REMOTE_FNC_LINKAGE int AccessRemoteProcess_fromPID(int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data);
#define CallFunctionOnRemoteProcess_fromPID(_pid,_library_name, _func_name, _data) \
	AccessRemoteProcess_fromPID((_pid),ACCESS_FNC_CALL,(_library_name),(_func_name),(_data))
REMOTE_FNC_LINKAGE int sigqueue_ex_fromPID(int pid, int sig, const siginfo_t* sig_info);
//int kill(pid_t pid, int sig);
#define kill(__pid,__sig)	sigqueue_ex_fromPID((__pid),(__sig),NULL)
REMOTE_FNC_LINKAGE int remote_printf_fromPID(int pid, const char* format, ...);
REMOTE_FNC_LINKAGE int remote_print_error_fromPID(int pid, const char* format, ...);
REMOTE_FNC_LINKAGE int remote_perror_fromPID(int pid, const char* error_string);
REMOTE_FNC_LINKAGE int sigqueue_fromPID(int pid, int sig, const union sigval value);


#ifdef __cplusplus
}
#endif


#endif  // #ifndef __remote_function_caller_h__
