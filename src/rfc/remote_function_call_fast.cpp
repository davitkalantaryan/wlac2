/*
 *	File: remote_function_call_fast.cpp
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
#include <signal.h>
#include <stdarg.h>
#include "namedpipeserver.h"
#include "remote_function_caller_base.h"
#include "windows_signal.h"
#include "remote_function_caller_private.h"


#ifdef __cplusplus
extern "C"
{
#endif


REMOTE_FNC_LINKAGE __int64 AccessRemoteProcess_fast(HANDLE hProcess, int pid, __int64 accessType,
	const char* fileName, const char* function_name, __int64 data);
REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_private_fast(HANDLE hProcess, int pid, SRemoteData* pResource);

extern char	g_vcFalseLibPath[];

REMOTE_FNC_LINKAGE BOOL RemoteLoadLibrary_fast(HANDLE a_hProcess, const char* a_cpcLibraryName)
{
	BOOL		hModReturn = NULL;
	int			error_code;
	wchar_t*	pwcErrorBuffer = NULL;
	//char*	pwcErrorBuffer = NULL;
	//char		pwcErrorBuffer[1024];
	DWORD		dwThreadId = 0;
	DWORD		dwReturned;
	HANDLE		hThread = NULL;
	SIZE_T		szWritten;
	char*		pcRemoteNameOfLibrary = NULL;
	int			nStrLenPlus1 = ((int)strlen(a_cpcLibraryName)) + 1;

	pcRemoteNameOfLibrary = (char*)AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1);
	if (pcRemoteNameOfLibrary == NULL){ goto returnPoint; }
	WriteProcessMemory(a_hProcess, pcRemoteNameOfLibrary, a_cpcLibraryName, nStrLenPlus1, &szWritten);

	hThread = CreateRemoteThread(a_hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryA,
		pcRemoteNameOfLibrary, 0, &dwThreadId);
	if (hThread == NULL)
	{
		error_code = GetLastError();
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&pwcErrorBuffer, 0, NULL);
		if (pwcErrorBuffer)
		{
			wprintf(L"error=%s\n", pwcErrorBuffer);
			LocalFree(pwcErrorBuffer);
		}
		goto returnPoint;
	}

returnPoint:
	if (hThread != NULL)
	{
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwReturned);
		hModReturn = (BOOL)dwReturned;
		CloseHandle(hThread);
	}

	if (pcRemoteNameOfLibrary != NULL){ DeleteMemOnRemoteProcess(a_hProcess, pcRemoteNameOfLibrary); }
	return hModReturn;
}


REMOTE_FNC_LINKAGE void RemoteFreeLibrary_from_resource_fast(HANDLE a_hProcess, int a_nPid, SRemoteData* a_pResource)
{
	SIZE_T	swWritten;
	__int64 access_type = FREEING_LIBRARY;

	WriteProcessMemory(a_hProcess, &(a_pResource->pInjectDataRemote->access_type), &access_type, sizeof(__int64), &swWritten);
	RunProcedureOnRemoteProcess_private_fast(a_hProcess, a_nPid, a_pResource);
}


REMOTE_FNC_LINKAGE void RemoteFreeLibrary_fast(HANDLE a_hProcess, int a_nPid,const char* a_cpcLibraryNameOnLocal)
{
	AccessRemoteProcess_fast(a_hProcess, a_nPid, FREEING_LIBRARY, a_cpcLibraryNameOnLocal, NULL, 0);
}


REMOTE_FNC_LINKAGE int freopen_std_new_fast(HANDLE a_hProcess, int a_nPid, const char *a_path, const char *a_mode, int a_nStdFd)
{
	__int64			nReturn			= 0;
	int				nStrLenPlus1;
	SIZE_T			szWritten;
	STDFREOPENDATA	aFreDataLocal;
	/// Remote data
	char*			pcPathRemote	= NULL;
	char*			pcModeRemote	= NULL;
	STDFREOPENDATA* pFreDataRemote = (STDFREOPENDATA*)to_pointer(AllocMemOnRemoteProcess(a_hProcess, sizeof(STDFREOPENDATA)));

	if (!pFreDataRemote)
	{
		nReturn = -21;
		goto returnPoint;
	}

	nStrLenPlus1 = 1 + ((int)strlen(a_path));
	pcPathRemote = (char*)to_pointer(AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1));
	if (!pcPathRemote)
	{
		nReturn = -22;
		goto returnPoint;
	}
	WriteProcessMemory(a_hProcess, pcPathRemote, a_path, nStrLenPlus1, &szWritten);

	nStrLenPlus1 = 1 + ((int)strlen(a_mode));
	pcModeRemote = (char*)to_pointer(AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1));
	if (!pcModeRemote)
	{
		nReturn = -23;
		goto returnPoint;
	}
	WriteProcessMemory(a_hProcess, pcModeRemote, a_mode, nStrLenPlus1, &szWritten);

	aFreDataLocal.file_name	= pcPathRemote;
	aFreDataLocal.mode		= pcModeRemote;
	aFreDataLocal.std_fd	= a_nStdFd;
	WriteProcessMemory(a_hProcess, pFreDataRemote, &aFreDataLocal, sizeof(STDFREOPENDATA), &szWritten);
	
	nReturn = AccessRemoteProcess_fast(a_hProcess, a_nPid, ACCESS_FNC_CALL, 
		g_vcThisLibrary_Path, "freopen_std_new_on_remote", to_value(pFreDataRemote));

returnPoint:
	if (pcModeRemote){DeleteMemOnRemoteProcess(a_hProcess, pcModeRemote);}
	if (pcPathRemote){DeleteMemOnRemoteProcess(a_hProcess, pcPathRemote);}
	if (pFreDataRemote){DeleteMemOnRemoteProcess(a_hProcess, pFreDataRemote);}
	return (int)nReturn;
}


REMOTE_FNC_LINKAGE FILE* fopen_remote_fast(HANDLE a_hProcess, int a_nPid, const char* a_cpcFileName, const char* a_mode)
{
	SIZE_T	szReaded;
	FILE*	pflReturn = NULL;
	FILE**	ppflFileOnRemote = (FILE**)to_pointer(AllocMemOnRemoteProcess(a_hProcess, sizeof(FILE*)));

	if (!ppflFileOnRemote) return NULL;
	AccessRemoteProcess_fast(a_hProcess, a_nPid, GET_FILE_DESCRIPTOR, a_cpcFileName, a_mode, to_value(ppflFileOnRemote));
	ReadProcessMemory(a_hProcess, ppflFileOnRemote, &pflReturn, sizeof(FILE*), &szReaded);
	DeleteMemOnRemoteProcess(a_hProcess, ppflFileOnRemote);
	return pflReturn;
}


REMOTE_FNC_LINKAGE FILE* stdhandle_remote_fast(HANDLE a_hProcess, int a_nPid, int a_nFD)
{
	SIZE_T	szReaded;
	FILE*	pflReturn = NULL;
	FILE**	ppflFileOnRemote = (FILE**)to_pointer(AllocMemOnRemoteProcess(a_hProcess, sizeof(FILE*)));

	if (!ppflFileOnRemote) return NULL;
	AccessRemoteProcess_fast(a_hProcess, a_nPid, GET_STANDARD_FILE_DESCRIPTOR, NULL, NULL, to_value(ppflFileOnRemote));
	ReadProcessMemory(a_hProcess, ppflFileOnRemote, &pflReturn, sizeof(FILE*), &szReaded);
	DeleteMemOnRemoteProcess(a_hProcess, ppflFileOnRemote);
	return pflReturn;
}


REMOTE_FNC_LINKAGE void Clearresourse_fast(HANDLE a_hProcess, SRemoteData* a_pResource)
{
	DWORD dwReturned;

	if (a_pResource->hThread != NULL)
	{
		WaitForSingleObject(a_pResource->hThread, INFINITE);
		GetExitCodeThread(a_pResource->hThread, &dwReturned);
		CloseHandle(a_pResource->hThread);
		a_pResource->hThread = NULL;
	}

#define		DeleteMemOnRemoteProcess_n(__hProcess,__mem) \
				do{DeleteMemOnRemoteProcess((__hProcess),(__mem));(__mem)=NULL;}while(0)

	if (a_pResource->pInjectDataRemote != NULL){ DeleteMemOnRemoteProcess_n(a_hProcess, a_pResource->pInjectDataRemote); }
	if (a_pResource->pcRemoteNameOfFunction != NULL){ DeleteMemOnRemoteProcess_n(a_hProcess, a_pResource->pcRemoteNameOfFunction); }
	if (a_pResource->pcRemoteNameOfFile != NULL){ DeleteMemOnRemoteProcess_n(a_hProcess, a_pResource->pcRemoteNameOfFile); }
	if (a_pResource->pcRemoteNameOfRemoteCallLibName != NULL){ DeleteMemOnRemoteProcess_n(a_hProcess, a_pResource->pcRemoteNameOfRemoteCallLibName); }
}


REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_private_fast(HANDLE a_hProcess, int a_nPid,SRemoteData* a_pResource)
{
	__int64				nReturn = 0;
	int					nPipeReturned;
	int					nPipeWrite;
	_uint64_ttt			ullnAddress;
	NamedPipeServer		aPipeServer;
	DWORD				dwThreadId = 0;
	char				vcPipeName[1024];
	char				vcBufferForPipeResponce[8];
	bool				bMore;

	_snprintf(vcPipeName, 1023, "\\\\.\\pipe\\%d_%d", a_nPid, a_nPid);
	nPipeReturned = aPipeServer.CreateServer(vcPipeName);
	if (nPipeReturned)
	{
		nReturn = -16;
		goto returnPoint;
	}

	// Start execution of remote ThreadFunc
	a_pResource->hThread = CreateRemoteThread(a_hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryA,
		a_pResource->pcRemoteNameOfRemoteCallLibName, 0, &dwThreadId);
	if (a_pResource->hThread == NULL)
	{
		nReturn = -17;
		goto returnPoint;
	}

	if (aPipeServer.WaitForConnection())
	{
		nReturn = -18; // not sure that this is error, may be kill is done before
		goto returnPoint;
	}
	ullnAddress = (_uint64_ttt)((size_t)a_pResource->pInjectDataRemote);
	nPipeWrite=aPipeServer.Write(&ullnAddress, sizeof(_uint64_ttt));
	if(nPipeWrite>0){
		aPipeServer.Read(vcBufferForPipeResponce, 1, bMore);  // waiting untill process is done
#if 0
		if(vcBufferForPipeResponce[0]!='o'){
			nReturn = -19;
			goto returnPoint;
		}
#endif
	}

returnPoint:

	//if (nReturn<0){ Clearresourse_fast(a_hProcess, a_pResource); }
	return nReturn;
}



REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_fast_private(
	HANDLE a_hProcess, int a_nPid, __int64 a_ullnAccessType,
	const char* a_cpcFileName, const char* a_function_name, __int64 a_data, 
	SRemoteData* a_pResource, int a_nFreeLib,
	const char* a_cpcFalseLibraryFullPath)
{
	__int64				nReturn = 0;
	INJDATA				aInjectDataLocal;
	int					nStrLenPlus1;
	DWORD				dwThreadId = 0;
	SIZE_T				szWritten;
	//const char*			cpcRemoteCallLibName = FULL_LIBRARY_NAME_TO_LOAD2;
	/// Remote data  /// All of this is modified by a_pResource
	//INJDATA*			pInjectDataRemote = NULL;
	//char*				pcRemoteNameOfRemoteCallLibName = NULL;
	//char*				pcRemoteNameOfFile = NULL;
	//char*				pcRemoteNameOfFunction = NULL;
	
	a_pResource->pInjectDataRemote = NULL;
	a_pResource->pcRemoteNameOfRemoteCallLibName = NULL;
	a_pResource->pcRemoteNameOfFile = NULL;
	a_pResource->pcRemoteNameOfFunction = NULL;
	a_pResource->hThread = NULL;

	if ((a_ullnAccessType == ACCESS_FNC_CALL) && a_cpcFileName)
	{
		if (!RemoteLoadLibrary_fast(a_hProcess, a_cpcFileName))
		{
			//report("\n%s\n", a_cpcLibFileName);
			nReturn = -11;
			goto returnPoint;
		}
	}

	nStrLenPlus1 = 1 + (int)strlen(a_cpcFalseLibraryFullPath);
	a_pResource->pcRemoteNameOfRemoteCallLibName = (char*)AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1);
	if (a_pResource->pcRemoteNameOfRemoteCallLibName == NULL)
	{
		nReturn = -12;
		goto returnPoint;
	}
	WriteProcessMemory(a_hProcess, a_pResource->pcRemoteNameOfRemoteCallLibName, a_cpcFalseLibraryFullPath, nStrLenPlus1, &szWritten);

	if (a_cpcFileName)
	{
		nStrLenPlus1 = 1 + (int)strlen(a_cpcFileName);
		a_pResource->pcRemoteNameOfFile = (char*)AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1);
		if (a_pResource->pcRemoteNameOfFile == NULL)
		{
			nReturn = -13;
			goto returnPoint;
		}
		WriteProcessMemory(a_hProcess, a_pResource->pcRemoteNameOfFile, a_cpcFileName, nStrLenPlus1, &szWritten);
	}

	if (a_function_name)
	{
		nStrLenPlus1 = 1 + (int)strlen(a_function_name);
		a_pResource->pcRemoteNameOfFunction = (char*)AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1);
		if (a_pResource->pcRemoteNameOfFunction == NULL)
		{
			nReturn = -14;
			goto returnPoint;
		}
		WriteProcessMemory(a_hProcess, a_pResource->pcRemoteNameOfFunction, a_function_name, nStrLenPlus1, &szWritten);
	}

	aInjectDataLocal.access_type = a_ullnAccessType;
	aInjectDataLocal.file_name = a_pResource->pcRemoteNameOfFile;
	aInjectDataLocal.function_name = a_pResource->pcRemoteNameOfFunction;
	aInjectDataLocal.data_in = a_data;
	aInjectDataLocal.in_freelib_out_return = (__int64)a_nFreeLib;

	a_pResource->pInjectDataRemote = (INJDATA*)AllocMemOnRemoteProcess(a_hProcess, sizeof(INJDATA));
	if (a_pResource->pInjectDataRemote == NULL)
	{
		nReturn = -15;
		goto returnPoint;
	}
	WriteProcessMemory(a_hProcess, a_pResource->pInjectDataRemote, &aInjectDataLocal, sizeof(INJDATA), &szWritten);

returnPoint:

	if (nReturn<0){ Clearresourse_fast(a_hProcess, a_pResource); }
	else { nReturn = RunProcedureOnRemoteProcess_private_fast(a_hProcess, a_nPid, a_pResource); }
	return nReturn;
}


REMOTE_FNC_LINKAGE __int64 RunProcedureOnRemoteProcess_fast(
	HANDLE a_hProcess, int a_nPid, __int64 a_ullnAccessType,
	const char* a_cpcFileName, const char* a_function_name, __int64 a_data, 
	SRemoteData* a_pResource, int a_nFreeLib)
{
	return RunProcedureOnRemoteProcess_fast_private(a_hProcess, a_nPid, a_ullnAccessType,
		a_cpcFileName, a_function_name, a_data, a_pResource, a_nFreeLib, g_vcFalseLibPath);

}


REMOTE_FNC_LINKAGE __int64 AccessRemoteProcess_fast(HANDLE a_hProcess, int a_nPid, __int64 a_ullnAccessType,
	const char* a_cpcFileName, const char* a_function_name, __int64 a_data)
{
	DWORD				dwExitCode;
	SIZE_T				szReaded;
	SRemoteData			aRemData;
	__int64				nReturn = RunProcedureOnRemoteProcess_fast(a_hProcess, a_nPid, a_ullnAccessType, a_cpcFileName, a_function_name, a_data, &aRemData,1);
	
	if (nReturn < 0){ goto returnPoint; }
	if (aRemData.hThread)
	{
		WaitForSingleObject(aRemData.hThread, -1);
		GetExitCodeThread(aRemData.hThread, &dwExitCode);
		CloseHandle(aRemData.hThread);
		aRemData.hThread = NULL;
	}
	ReadProcessMemory(a_hProcess, &(aRemData.pInjectDataRemote->in_freelib_out_return), &nReturn, sizeof(__int64), &szReaded);

returnPoint:
	Clearresourse_fast(a_hProcess, &aRemData);
	return nReturn;
}


REMOTE_FNC_LINKAGE int sigqueue_ex_fast(HANDLE a_hProcess, int a_nPid, int a_sig, const siginfo_t* a_sig_info)
{
	__int64			nReturn = 0;

	SIZE_T			szRW;
	SWinSignal		aSigDataLocal;
	/// Remote data
	SWinSignal*		pSigDataRemote = (SWinSignal*)to_pointer(AllocMemOnRemoteProcess(a_hProcess, sizeof(SWinSignal)));

	if (!pSigDataRemote)
	{
		nReturn = -31;
		goto returnPoint;
	}

	if (a_sig_info){ memcpy(&(aSigDataLocal.sig_info), a_sig_info, sizeof(siginfo_t)); }
	aSigDataLocal.m_sig_num = a_sig;
	aSigDataLocal.m_return2 = 0;

	WriteProcessMemory(a_hProcess, pSigDataRemote, &aSigDataLocal, sizeof(SWinSignal), &szRW);

	nReturn = AccessRemoteProcess_fast(a_hProcess, a_nPid, ACCESS_FNC_CALL,
		g_vcThisLibrary_Path, "raise_on_remote", to_value(pSigDataRemote));

	if (nReturn){goto returnPoint;}
	ReadProcessMemory(a_hProcess, pSigDataRemote, &aSigDataLocal, sizeof(SWinSignal), &szRW);
	nReturn = aSigDataLocal.m_return2;

returnPoint:
	if (pSigDataRemote){ DeleteMemOnRemoteProcess(a_hProcess, pSigDataRemote); }
	return (int)nReturn;
}


REMOTE_FNC_LINKAGE int sigqueue_fast(HANDLE a_hProcess, int a_nPid, int a_sig, const union sigval a_value)
{
	siginfo_t	sig_info;
	sig_info._sifields._rt.si_sigval = a_value;
	return sigqueue_ex_fast(a_hProcess, a_nPid, a_sig, &sig_info);
}


REMOTE_FNC_LINKAGE int remote_printf_fast(HANDLE a_hProcess, int a_nPid, const char* a_format, ...)
{
	int				nReturn = 0;
	va_list			arg_list;

	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(a_hProcess, a_nPid,"printf_on_remote", a_format, &arg_list);
	va_end(arg_list);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_print_error_fast(HANDLE a_hProcess, int a_nPid, const char* a_format, ...)
{
	int				nReturn = 0;
	va_list			arg_list;

	va_start(arg_list, a_format);     /* Initialize variable arguments. */
	nReturn = printf_on_remote_fast_private(a_hProcess, a_nPid, "print_error_on_remote", a_format, &arg_list);
	va_end(arg_list);
	return nReturn;
}


REMOTE_FNC_LINKAGE int remote_perror_fast(HANDLE a_hProcess, int a_nPid, const char* a_error_string)
{
	int	nReturn = 0;
	nReturn = printf_on_remote_fast_private(a_hProcess, a_nPid, "perror_on_remote", a_error_string,NULL);
	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
int printf_on_remote_fast_private(HANDLE a_hProcess, int a_nPid, const char* a_function_name, const char* a_format, const va_list* a_list)
{
#define	SIZE_OF_INT		4
#define REALLOC_SIZE	1024

	__int64			nReturn = 0;
	int				nCurSize = 0;
	int				nStrLenPlus1;
	SIZE_T			szRW;
	char*			pcStringLocal;
	char*			pcStringLocalTemp;
	/// Remote data
	void*			pDataRemote = NULL;

	nCurSize += REALLOC_SIZE;
	pcStringLocal = (char*)malloc(nCurSize);
	if (!pcStringLocal){ nReturn = -61; goto returnPoint; }
	if (a_list){ nStrLenPlus1 = _vsnprintf(pcStringLocal, nCurSize - 1, a_format, *a_list); }
	else{ nStrLenPlus1 = _snprintf(pcStringLocal, nCurSize - 1, a_format); }

	while (nStrLenPlus1 >= (nCurSize - 1))
	{
		nCurSize += REALLOC_SIZE;
		pcStringLocalTemp = (char*)realloc(pcStringLocal, nCurSize);
		if (!pcStringLocalTemp){ nReturn = -62; goto returnPoint; }
		pcStringLocal = pcStringLocalTemp;
		if (a_list){ nStrLenPlus1 = _vsnprintf(pcStringLocal, nCurSize - 1, a_format, *a_list); }
		else { nStrLenPlus1 = _snprintf(pcStringLocal, nCurSize - 1, a_format); }
	}

	nStrLenPlus1 = ((int)strlen(pcStringLocal)) + 1;
	nStrLenPlus1 = nStrLenPlus1<SIZE_OF_INT ? SIZE_OF_INT : nStrLenPlus1;
	pDataRemote = (char*)to_pointer(AllocMemOnRemoteProcess(a_hProcess, nStrLenPlus1));

	if (!pDataRemote)
	{
		nReturn = -31;
		goto returnPoint;
	}

	WriteProcessMemory(a_hProcess, pDataRemote, pcStringLocal, nStrLenPlus1, &szRW);

	nReturn = AccessRemoteProcess_fast(a_hProcess, a_nPid, ACCESS_FNC_CALL,
		g_vcThisLibrary_Path, a_function_name, to_value(pDataRemote));

	//if (nReturn){ goto returnPoint; }
	//ReadProcessMemory(a_hProcess, pDataRemote, &nReturn, 4, &szRW);

returnPoint:
	if (pDataRemote){ DeleteMemOnRemoteProcess(a_hProcess, pDataRemote); }
	if (pcStringLocal){ free(pcStringLocal); }
	return (int)nReturn;
}


#ifdef __cplusplus
}
#endif
