/*
 *	File: posix_threadin_forvs.cpp
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) pthread_create
 *		2) pthread_attr_init
 *		3) pthread_attr_destroy
 *
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include "pthread.h"
#include "redesigned/process.h"
#include <malloc.h>
#include "pthread_private_for_source.h"

//#define		_USE_BEGIN_THREAD_EX
#ifdef _USE_BEGIN_THREAD_EX
#define	THREAD_RET_TYPE		unsigned
#define	THREAD_CALL_CONV	__stdcall
#define	THREAD_INP_TYPE		void*
#else  // #ifdef _USE_BEGIN_THREAD_EX
#define	THREAD_RET_TYPE		DWORD
#define	THREAD_CALL_CONV	WINAPI
#define	THREAD_INP_TYPE		LPVOID
#endif  // #ifdef _USE_BEGIN_THREAD_EX

__BEGIN_C_DECLS

static pthread_t	s_threadsTable[4096];

static THREAD_RET_TYPE THREAD_CALL_CONV Thread_Start_Routine_Static(void* arg);
static _inline int IterFuncForThreadNumber(THREADENTRY32* a_pThrItem, void* a_pUser);
static void FreeThreadDataOnlyResource2(pthread_t a_data, void* a_pRet);
static void FreeThreadData3(pthread_t a_thread);

extern DWORD g_tlsPthreadDataKey;

GEM_API pthread_t GetThreadHandleFromId(DWORD a_id) 
{
	return s_threadsTable[a_id%4096];
}

GEM_API int pthread_create(pthread_t *a_thread, const pthread_attr_t *a_attr,
								void *(*a_start_routine) (void *), void *a_arg)
{
	*a_thread = CreateNewThreadData(0);
	if (!(*a_thread)) {return -ENOMEM;}

	(*a_thread)->func = a_start_routine;
	(*a_thread)->arg = a_arg;
	(*a_thread)->thrd = CreateThread(
		NULL,0,&Thread_Start_Routine_Static, 
		(*a_thread),0,&(*a_thread)->thrdID);

	if (!(*a_thread)->thrd){
		FreeThreadData3(*a_thread);
		*a_thread = (pthread_t)0;
		return errno > 0 ? -errno : errno;
	}

	s_threadsTable[(*a_thread)->thrdID % 4096] = *a_thread;

	return 0;
}

GEM_VAR_FAR int   g_nLibraryCleanupStarted;


GEM_API int pthread_join(pthread_t a_thread, void **a_retval)
{
	DWORD dwExitCode;

	if(g_nLibraryCleanupStarted){
		GetExitCodeThread(a_thread->thrd, &dwExitCode);
		if (a_thread->resourse && (dwExitCode == STILL_ACTIVE)){
			// Target thread will not exit untill DllMain for the thread is not called
			// spin untill thread alive
			s_threadsTable[a_thread->thrdID % 4096] = NULL;
			while(a_thread->resourse){Sleep(1);}
			dwExitCode = (DWORD)((size_t)a_thread->reserved);
			// Mark the thread to delete in DllMain // not necessary
		}
		else if (dwExitCode == STILL_ACTIVE) {
			dwExitCode = (DWORD)((size_t)a_thread->reserved);
		}
	}
	else{
		WaitForSingleObject(a_thread->thrd, INFINITE);
		GetExitCodeThread(a_thread->thrd, &dwExitCode);
	}

	FreeThreadData3(a_thread);
	if(a_retval){*a_retval=(void*)((size_t)dwExitCode);}
	
	return 0;
}


GEM_API int pthread_attr_init(pthread_attr_t *a_attr)
{
	return 0;
}


GEM_API int pthread_attr_destroy(pthread_attr_t *a_attr)
{
	return 0;
}


GEM_API int pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
	return 0;
}


GEM_API int pthread_attr_getscope(pthread_attr_t *attr, int *scope)
{
	return 0;
}


GEM_API int pthread_attr_setdetachstate(pthread_attr_t *attrint, int detachstate)
{
	return 0;
}


GEM_API int pthread_attr_getdetachstate(pthread_attr_t *attrint, int *detachstate)
{
	return 0;
}


GEM_API pthread_t pthread_self(void)
{
	//return GetCurrentThread();
	return GetPthreadDataPointer();
}


GEM_API pthread_t GetPthreadDataPointer(void)
{
	if (g_tlsPthreadDataKey) {
		pthread_s* pThrData = (pthread_s*)TlsGetValue(g_tlsPthreadDataKey);
		if (pThrData) {return pThrData;}
		else {
			pThrData = CreateNewThreadData(1);
			return TlsSetValue(g_tlsPthreadDataKey, pThrData) ? pThrData : NULL;
		}
	}  // if (s_tlsPthreadDataKey) {

	return (pthread_t)0;
}


GEM_API BOOL ListProcessThreads(DWORD a_dwOwnerPID, void* a_pUser, int(*a_IterFunc)(struct tagTHREADENTRY32*, void*))
{

	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == a_dwOwnerPID)
		{
			if ((*a_IterFunc)(&te32, a_pUser))
			{
				goto returnPoint;
			}
		}
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
returnPoint:
	CloseHandle(hThreadSnap);
	return(TRUE);

}



GEM_API int GetNumberOfProcessThreads(int a_nPid)
{
	int nThreadsNumber = 0;
	ListProcessThreads(a_nPid,(void*)&nThreadsNumber,&IterFuncForThreadNumber);
	return nThreadsNumber;
}

//
pthread_t CreateNewThreadData(int a_isCurrentThread)
{
	pthread_t pThread = (pthread_t)malloc(sizeof(struct pthread_s));

	if(LIKELY(pThread)){
		memset(pThread, 0, sizeof(struct pthread_s));
		if(a_isCurrentThread){
			pThread->thrd = GetCurrentThread();
			pThread->thrdID = GetCurrentThreadId();
		}
		pThread->resourse = (char*)malloc(128);
		if (pThread->resourse) {
			*(pThread->resourse) = 0;
			pThread->resourseSize = 128;
		}
		else {
			pThread->resourseSize = 0;
		}
		pThread->threadName = (char*)malloc(1);
		if(pThread->threadName){*(pThread->threadName) = 0;}
	}

	return pThread;
}


static void FreeThreadData3(pthread_t a_thread)
{
	FreeThreadDataOnlyResource2(a_thread,NULL);
	free(a_thread->threadName);
	if(a_thread->thrd){CloseHandle(a_thread->thrd);}
	free(a_thread);
}


void ProperlyRemoveTlsInCurrentThread(void* a_pRet)
{
	if(g_tlsPthreadDataKey){
		pthread_t pThrData = (pthread_t)TlsGetValue(g_tlsPthreadDataKey);
		TlsSetValue(g_tlsPthreadDataKey, NULL);
		if(pThrData){FreeThreadDataOnlyResource2(pThrData,a_pRet);}
	}
}


static void FreeThreadDataOnlyResource2(pthread_t a_data,void* a_pRet)
{
	a_data->resourseSize = 0;
	free(a_data->resourse);
	a_data->resourse = NULL;
	//a_data->thrdID = 0;
	a_data->reserved = a_pRet;
}


static THREAD_RET_TYPE THREAD_CALL_CONV Thread_Start_Routine_Static(void* a_pArg)
{
	pthread_t pThread = (pthread_t)a_pArg;
	THREAD_RET_TYPE unReturn;
	
	if (g_tlsPthreadDataKey) { TlsSetValue(g_tlsPthreadDataKey,pThread); }
	unReturn = (THREAD_RET_TYPE)((size_t)(*pThread->func)(pThread->arg));
	ProperlyRemoveTlsInCurrentThread((void*)((size_t)unReturn));
	ExitThread(unReturn);
	return unReturn;
}


static _inline int IterFuncForThreadNumber(THREADENTRY32* a_pThrItem, void* a_pUser)
{
	if (!a_pThrItem) return 0;
	int* pNum = (int*)a_pUser;
	++(*pNum);
	return 0;
}

__END_C_DECLS
