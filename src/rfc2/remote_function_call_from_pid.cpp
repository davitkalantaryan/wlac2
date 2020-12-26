/*
 *	File: remote_function_call_from_pid.cpp
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

#include <stdio.h>
#include <process.h>
#include "remote_function_caller_base.h"
#include "windows_signal.h"
#include "remote_function_caller_private.h"


#ifdef __cplusplus
extern "C"
{
#endif

REMOTE_FNC_LINKAGE BOOL RemoteLoadLibrary_fast(HANDLE hProcess, const char* libraryName);
REMOTE_FNC_LINKAGE void Clearresourse_fast(HANDLE hProcess, SRemoteData* pResource);
REMOTE_FNC_LINKAGE int RunProcedureOnRemoteProcess_fast(HANDLE hProcess, int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data, SRemoteData* resource,int free_lib_or_not);
REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_private_fast(HANDLE hProcess, int pid, SRemoteData* pResource);
REMOTE_FNC_LINKAGE int freopen_std_new_fast(HANDLE hProcess, int pid, const char *path, const char *mode, int stdFd);
REMOTE_FNC_LINKAGE FILE* fopen_remote_fast(HANDLE hProcess, int pid, const char* fileName, const char* mode);
REMOTE_FNC_LINKAGE FILE* stdhandle_remote_fast(HANDLE hProcess, int pid, int nFD);
REMOTE_FNC_LINKAGE __int64 AccessRemoteProcess_fast(HANDLE hProcess, int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data);
REMOTE_FNC_LINKAGE int sigqueue_ex_fast(HANDLE hProcess, int pid, int sig, const siginfo_t* value);
REMOTE_FNC_LINKAGE int sigqueue_fast(HANDLE hProcess, int pid, int sig, const union sigval value);

REMOTE_FNC_LINKAGE BOOL RemoteLoadLibrary_fromPID(int a_nPid, const char* __libr_name)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		BOOL bRet = RemoteLoadLibrary_fast(hProcess, (__libr_name));
		CloseHandle(hProcess);
		return bRet;
	}
	return FALSE;
}


REMOTE_FNC_LINKAGE void RemoteFreeLibrary_from_resource_fromPID(int a_nPid, SRemoteData* a_pResource)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));
	SIZE_T	swWritten;
	__int64 access_type = FREEING_LIBRARY;

	if (hProcess)
	{
		WriteProcessMemory(hProcess, &(a_pResource->pInjectDataRemote->access_type), &access_type, sizeof(__int64), &swWritten);
		RunProcedureOnRemoteProcess_private_fast(hProcess, a_nPid, a_pResource);
		CloseHandle(hProcess);
	}
}


REMOTE_FNC_LINKAGE void RemoteFreeLibrary_fromPID(int a_nPid, const char* a_cpcLibraryNameOnLocal)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));
	
	if (hProcess)
	{ 
		AccessRemoteProcess_fast(hProcess, a_nPid, FREEING_LIBRARY, a_cpcLibraryNameOnLocal, NULL, 0);
		CloseHandle(hProcess);
	}
}

REMOTE_FNC_LINKAGE void Clearresourse_fromPID(int a_nPid, SRemoteData* a_pResource)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		Clearresourse_fast(hProcess, a_pResource);
		CloseHandle(hProcess);
	}
}


REMOTE_FNC_LINKAGE int RunProcedureOnRemoteProcess_fromPID(int a_nPid, __int64 a_ullnAccessType,
	const char* a_cpcFileName, const char* a_function_name, __int64 a_data, SRemoteData* a_pResource,int a_nFreeLibOrNot,HANDLE* a_phProcess)
{
	*a_phProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (*a_phProcess)
	{
		return RunProcedureOnRemoteProcess_fast(*a_phProcess, a_nPid, a_ullnAccessType, a_cpcFileName, a_function_name, a_data, a_pResource, a_nFreeLibOrNot);
	}
	return -60;
}


REMOTE_FNC_LINKAGE int freopen_std_new_fromPID(int a_nPid, const char *a_path, const char *a_mode, int a_nStdFd)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		int nRet = freopen_std_new_fast(hProcess, a_nPid,a_path,a_mode,a_nStdFd);
		CloseHandle(hProcess);
		return nRet;
	}
	return -40;
}


REMOTE_FNC_LINKAGE FILE* fopen_remote_fromPID(int a_nPid, const char* a_cpcFileName, const char* a_mode)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		FILE* pflRet = fopen_remote_fast(hProcess, a_nPid, a_cpcFileName, a_mode);
		CloseHandle(hProcess);
		return pflRet;
	}
	return NULL;
}


REMOTE_FNC_LINKAGE FILE* stdhandle_remote_fromPID(int a_nPid, int a_nFD)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		FILE* pflRet = stdhandle_remote_fast(hProcess, a_nPid,a_nFD);
		CloseHandle(hProcess);
		return pflRet;
	}
	return NULL;
}


REMOTE_FNC_LINKAGE __int64 AccessRemoteProcess_fromPID(int a_nPid, __int64 a_ullnAccessType,
	const char* a_cpcFileName, const char* a_function_name, __int64 a_data)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		__int64 nRet = AccessRemoteProcess_fast(hProcess, a_nPid, a_ullnAccessType,
			a_cpcFileName, a_function_name,a_data);
		CloseHandle(hProcess);
		return nRet;
	}
	return -40;
}


REMOTE_FNC_LINKAGE int sigqueue_ex_fromPID(int a_nPid, int a_sig, const siginfo_t* a_sig_info)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		int nRet = sigqueue_ex_fast(hProcess, a_nPid, a_sig, a_sig_info);
		CloseHandle(hProcess);
		return nRet;
	}
	return -50;
}


REMOTE_FNC_LINKAGE int sigqueue_fromPID(int a_nPid, int a_sig, const union sigval a_value)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (hProcess)
	{
		int nRet = sigqueue_fast(hProcess, a_nPid, a_sig, a_value);
		CloseHandle(hProcess);
		return nRet;
	}
	return -50;
}


REMOTE_FNC_LINKAGE int remote_printf_fromPID(int a_nPid, const char* a_format, ...)
{
	int		nReturn = 0;
	va_list	arg_list;
	HANDLE	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (!hProcess)
	{
		return -71;
	}
	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(hProcess, a_nPid, "printf_on_remote",a_format, &arg_list);
	va_end(arg_list);
	CloseHandle(hProcess);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_print_error_fromPID(int a_nPid, const char* a_format, ...)
{
	int		nReturn = 0;
	va_list	arg_list;
	HANDLE	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (!hProcess)
	{
		return -71;
	}
	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(hProcess, a_nPid, "print_error_on_remote", a_format, &arg_list);
	va_end(arg_list);
	CloseHandle(hProcess);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_perror_fromPID(int a_nPid, const char* a_error_string)
{
	int		nReturn = 0;
	HANDLE	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (a_nPid));

	if (!hProcess)
	{
		return -71;
	}
	nReturn = printf_on_remote_fast_private(hProcess, a_nPid, "perror_on_remote", a_error_string, NULL);
	CloseHandle(hProcess);
	return nReturn;

}

///////////////////////////////////////////////////////////////////////////////////////////////

REMOTE_FNC_LINKAGE int remote_printf_fromHandle(HANDLE a_hProcess, const char* a_format, ...)
{
	int		nReturn = 0;
	va_list	arg_list;
	int	nPid = GetProcessId(a_hProcess);

	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(a_hProcess, nPid, "printf_on_remote",a_format, &arg_list);
	va_end(arg_list);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_print_error_fromHandle(HANDLE a_hProcess, const char* a_format, ...)
{
	int		nReturn = 0;
	va_list	arg_list;
	int	nPid = GetProcessId(a_hProcess);

	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(a_hProcess, nPid, "print_error_on_remote",a_format, &arg_list);
	va_end(arg_list);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_perror_fromHandle(HANDLE a_hProcess, const char* a_error_string)
{
	int		nReturn = 0;
	int		nPid = GetProcessId(a_hProcess);

	nReturn = printf_on_remote_fast_private(a_hProcess, nPid, "perror_on_remote", a_error_string, NULL);
	return nReturn;
}



#ifdef __cplusplus
}
#endif
