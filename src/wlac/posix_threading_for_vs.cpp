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
#include <first_includes/wlac_compiler_internal.h>
#include "common_include_for_wlac_sources.h"
#include "pthread.h"
#include "redesigned/process.h"
#include <malloc.h>
#include "pthread_private_for_source.h"
#include <common/hashtbl.hpp>
#include <Windows.h>


#ifdef __cplusplus
#define WLAC_INITIALIZER(f) \
        static void f(void); \
        struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
        static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
#if defined(_WIN64) || defined(_M_ARM)
#define WLAC_INITIALIZER(f) INITIALIZER2_(f,"")
#else
#define WLAC_INITIALIZER(f) INITIALIZER2_(f,"_")
#endif
#else
#define WLAC_INITIALIZER(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif

// todo: delete below
//#define		_USE_BEGIN_THREAD_EX
//#ifdef _USE_BEGIN_THREAD_EX
//#define	THREAD_RET_TYPE		unsigned
//#define	THREAD_CALL_CONV	__stdcall
//#define	THREAD_INP_TYPE		void*
//#else  // #ifdef _USE_BEGIN_THREAD_EX
//#define	THREAD_RET_TYPE		DWORD
//#define	THREAD_CALL_CONV	WINAPI
//#define	THREAD_INP_TYPE		LPVOID
//#endif  // #ifdef _USE_BEGIN_THREAD_EX

static common::HashTbl<pthread_s_new*>  s_hashByHandles2;  // key is pthread_t => HANDLE
static common::HashTbl<pthread_s_new*>  s_hashByIds2;     // key is DWORD
static struct pthread_s_new*			s_pFirstThreadData;
static HANDLE	s_mutexForThreadContainers = NULL;
static DWORD	s_tlsPthreadDataKey = 0;
static int		s_nRunDebuggerThread = 0;
static HANDLE	s_handleDebggerHandlerThread = NEWNULLPTR2;

__BEGIN_C_DECLS


static DWORD WINAPI Thread_Start_Routine_Static(LPVOID arg);
static DWORD WINAPI ThreadDebuggerHandling(LPVOID arg);
static _inline int IterFuncForThreadNumber(THREADENTRY32* a_pThrItem, void* a_pUser);
static void FreeThreadDataAndRemoveFromList(struct pthread_s_new* threadData);
static void InitThreadDataAndAddToList(struct pthread_s_new* threadData, pthread_t threadHandle, DWORD a_id);
static void InitThreadDataOnlyPointersAndAddToList(struct pthread_s_new* a_threadData);
static int SetThreadNameForDebugger(struct pthread_s_new* a_threadData);

#ifdef WaitForSingleObject
#ifndef __INTELLISENSE__
#undef WaitForSingleObject
#endif
#endif

static struct pthread_s_new* GetThreadPointerFromId(DWORD a_id)
{
	BOOL bAddedHere = FALSE;
	pthread_s_new* pThreadData = NULL;
	pthread_t threadHandle;

	WaitForSingleObject(s_mutexForThreadContainers,INFINITE);
	if(!s_hashByIds2.FindEntry(&a_id,sizeof(DWORD),&pThreadData)){
		threadHandle=OpenThread(THREAD_ALL_ACCESS,FALSE,a_id);
		if(threadHandle){
			// we have thread currently not in the containers
			pThreadData = STATIC_CAST2(struct pthread_s_new*,calloc(1, sizeof(struct pthread_s_new)));
			if(LIKELY2(pThreadData)){
				InitThreadDataAndAddToList(pThreadData,threadHandle,a_id);
				s_hashByIds2.AddEntry(&a_id, sizeof(DWORD), pThreadData);
				s_hashByHandles2.AddEntry(&threadHandle, sizeof(pthread_t), pThreadData);
				bAddedHere = TRUE;
			}
		}
	}
	ReleaseMutex(s_mutexForThreadContainers);

	if(bAddedHere && (a_id==GetCurrentThreadId())){
		pThreadData->existOnThreadLocalStorage = 1;
		TlsSetValue(s_tlsPthreadDataKey, pThreadData);
	}

	return pThreadData;
}


GEM_API int pthread_create(pthread_t *a_thread, const pthread_attr_t *a_attr,void *(*a_start_routine) (void *), void *a_arg)
{
	DWORD dwCreationFlags = a_attr && (*a_attr) ? (*a_attr)->dwCreationFlags : 0;
	LPSECURITY_ATTRIBUTES   lpThreadAttributes = a_attr&& (*a_attr) ? CONST_CAST2(LPSECURITY_ATTRIBUTES,&(*a_attr)->secAttr) : NEWNULLPTR2;
	SIZE_T dwStackSize = a_attr&& (*a_attr) ? (*a_attr)->dwStackSize : 0;
	struct pthread_s_new* pThreadData = STATIC_CAST2(struct pthread_s_new*,calloc(1,sizeof(struct pthread_s_new)));

	if(!pThreadData){
		SetLastError(ENOMEM);
		return -1;
	}

	pThreadData->func = a_start_routine;
	pThreadData->arg = a_arg;

	*a_thread=pThreadData->thrd = CreateThread(lpThreadAttributes,dwStackSize,&Thread_Start_Routine_Static,pThreadData,dwCreationFlags,&pThreadData->thrdID);

	if(!pThreadData->thrd){
		free(pThreadData);
		return -1;
	}

	pThreadData->isAlive = 1;
	InitThreadDataOnlyPointersAndAddToList(pThreadData);

	WaitForSingleObject(s_mutexForThreadContainers, INFINITE);
	s_hashByIds2.AddEntry(&pThreadData->thrdID, sizeof(DWORD), pThreadData);
	s_hashByHandles2.AddEntry(&pThreadData->thrd, sizeof(pthread_t), pThreadData);
	ReleaseMutex(s_mutexForThreadContainers);

	return 0;
}


GEM_API int pthread_join(pthread_t a_thread, void **a_retval)
{
	struct pthread_s_new* pThreadData = NEWNULLPTR2;
	DWORD dwExitCode;

	if(gh_bLibraryCleanupStarted){
		GetExitCodeThread(a_thread, &dwExitCode);
		if (dwExitCode == STILL_ACTIVE){
			// lock and unlock is not needed
			if(gh_bIsAllowedToWaitForSignal){WaitForSingleObject(s_mutexForThreadContainers, INFINITE);}
			if(s_hashByHandles2.FindEntry(&a_thread, sizeof(pthread_t), &pThreadData)){
				s_hashByHandles2.RemoveEntry(&a_thread, sizeof(pthread_t));
				s_hashByIds2.RemoveEntry(&pThreadData->thrdID, sizeof(DWORD));
			}
			if(gh_bIsAllowedToWaitForSignal){ReleaseMutex(s_mutexForThreadContainers);}
			if(pThreadData){
				// Target thread will not exit untill DllMain for the thread is not called
				// spin untill thread alive
				while (pThreadData->isAlive) { Sleep(1); }
			}
			
		}

		dwExitCode = pThreadData ? (DWORD)((size_t)pThreadData->reserved) : STILL_ACTIVE;
	}
	else{
		WaitForSingleObject(s_mutexForThreadContainers, INFINITE);
		if (s_hashByHandles2.FindEntry(&a_thread, sizeof(pthread_t), &pThreadData)) {
			s_hashByHandles2.RemoveEntry(&a_thread, sizeof(pthread_t));
			s_hashByIds2.RemoveEntry(&pThreadData->thrdID, sizeof(DWORD));
		}
		ReleaseMutex(s_mutexForThreadContainers);
		WaitForSingleObject(a_thread, INFINITE);
		GetExitCodeThread(a_thread, &dwExitCode);
	}

	if(pThreadData){FreeThreadDataAndRemoveFromList(pThreadData);}
	if(a_retval){*a_retval=(void*)((size_t)dwExitCode);}
	
	return 0;
}


GEM_API int pthread_attr_init(pthread_attr_t *a_attr)
{
	*a_attr = STATIC_CAST2(struct pthread_attr_s*, calloc(1, sizeof(struct pthread_attr_s)));
	return 0;
}


GEM_API int pthread_attr_destroy(pthread_attr_t *a_attr)
{
	free(*a_attr);
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
	return GetCurrentThread();
	//return GetPthreadDataPointer();
}


GEM_API int pthread_getname_np(pthread_t a_thread, char* a_buffer, size_t a_len)
{
	struct pthread_s_new* pThreadData = GetAnyThreadDataPointer(a_thread);

	if(LIKELY2(pThreadData && pThreadData->threadName)){
		strncpy(a_buffer, pThreadData->threadName, a_len);
		return 0;
	}

	return -1;
}


GEM_API int pthread_setname_np(pthread_t a_thread, __const char* a_name)
{
	struct pthread_s_new* pThreadData = GetAnyThreadDataPointer(a_thread);

	if(LIKELY2(pThreadData)){
		BOOL bDebugger;
		size_t unStrLenPlus1 = strlen(a_name) + 1;
		char* newName = STATIC_CAST2(char*,realloc(pThreadData->threadName, unStrLenPlus1));
		if (!newName) { return ENOMEM; }
		memcpy(newName, a_name, unStrLenPlus1);
		pThreadData->threadName = newName;
		pThreadData->isNameSet = 1;

		//CheckRemoteDebuggerPresent(GetCurrentProcess(), &bDebugger);
		bDebugger=IsDebuggerPresent();

		if (bDebugger) {
			SetThreadNameForDebugger(pThreadData);
		}

		return 0;
	}

	return -1;
}


GEM_API BOOL ListProcessThreads(DWORD a_dwOwnerPID, void* a_pUser, int(*a_IterFunc)(struct tagTHREADENTRY32*, void*))
{

	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,a_dwOwnerPID); // if a_dwOwnerPID==0, then all processes of current desktop listed
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32)){
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == a_dwOwnerPID) // this check is not necessary
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


GEM_API void FinalizingCalculateThreadsNumbers(void)
{
	int nNumberOfThreads = GetNumberOfProcessThreads(GetCurrentProcessId());
	if(nNumberOfThreads>1){
		gh_bIsAllowedToWaitForSignal = TRUE;
		//gh_bLibraryCleanupStarted = FALSE;
		gh_bLibraryCleanupStarted = TRUE;
	}
	else{
		gh_bIsAllowedToWaitForSignal = FALSE;
		gh_bLibraryCleanupStarted = TRUE;
	}
}


static void FreeThreadDataAndRemoveFromList(struct pthread_s_new* a_threadData)
{
#ifdef __INTELLISENSE__
	struct pthread_s_new
	{
		pthread_s_new* prev, * next;
		HANDLE			thrd;
		char* threadName;
		char* resourse;
		void* reserved;
		start_routine_t	func;
		void* arg;
		DWORD			resourseSize;
		DWORD			thrdID;
		uint64_t		existOnThreadLocalStorage : 1;
		uint64_t		isAlive : 1;
		uint64_t		isNameSet : 1;
		uint64_t		isDetached : 1;
		uint64_t		reserved64bit : 60;
	};
#endif

	if(a_threadData->prev){a_threadData->prev->next=a_threadData->next;}
	if(a_threadData->next){a_threadData->next->prev=a_threadData->prev;}
	if(a_threadData==s_pFirstThreadData){s_pFirstThreadData=a_threadData->next;}

	free(a_threadData->resourse);
	free(a_threadData->threadName);
	if(a_threadData->thrd){CloseHandle(a_threadData->thrd);}
	free(a_threadData);
}


//void ProperlyRemoveTlsInCurrentThread(void* a_pRet)
//{
//	if(g_tlsPthreadDataKey){
//		pthread_t pThrData = (pthread_t)TlsGetValue(g_tlsPthreadDataKey);
//		TlsSetValue(g_tlsPthreadDataKey, NULL);
//		if(pThrData){FreeThreadDataOnlyResource2(pThrData,a_pRet);}
//	}
//}


static DWORD WINAPI Thread_Start_Routine_Static(LPVOID a_pArg)
{
	struct pthread_s_new* pThreadData = STATIC_CAST2(struct pthread_s_new* ,a_pArg);
	DWORD unReturn;
	
	gh_bIsAllowedToWaitForSignal = TRUE;
	pThreadData->existOnThreadLocalStorage = 1;
	TlsSetValue(s_tlsPthreadDataKey, pThreadData);

	try {
		unReturn = STATIC_CAST2(DWORD,REINTERPRET_CAST2(size_t,(*pThreadData->func)(pThreadData->arg)));
	}
	catch(...){
	}

	pThreadData->isAlive = 0;
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


static void InitThreadDataOnlyPointersAndAddToList(struct pthread_s_new* a_threadData)
{
	char* pNewResource;
	a_threadData->threadName = _strdup("wlac_thread");
	pNewResource = STATIC_CAST2(char*, realloc(a_threadData->resourse, 1024));
	if (pNewResource) {
		a_threadData->resourse = pNewResource;
		a_threadData->resourseSize = 1024;
	}

	a_threadData->prev = NEWNULLPTR2;
	if(s_pFirstThreadData){s_pFirstThreadData->prev=a_threadData;}
	a_threadData->next = s_pFirstThreadData;
	s_pFirstThreadData = a_threadData;
}


static void InitThreadDataAndAddToList(struct pthread_s_new* a_threadData, pthread_t a_threadHandle, DWORD a_id)
{
#ifdef __INTELLISENSE__
#endif
	//struct pthread_s_new* pReturn = (struct pthread_s_new*)calloc(1, sizeof(struct pthread_s_new));
	//if(UNLIKELY2(!pReturn)){return NEWNULLPTR2;}
	a_threadData->thrd = a_threadHandle;
	a_threadData->thrdID = a_id;
	InitThreadDataOnlyPointersAndAddToList(a_threadData);

	//if (a_isTheCurrentThread) {
	//	pReturn->existOnThreadLocalStorage =1;
	//	TlsSetValue(s_tlsPthreadDataKey, pReturn);
	//}  // if (s_tlsPthreadDataKey) {

}


HIDE_SYMBOL2 struct pthread_s_new* GetCurrentThreadDataPointer(void)
{
	pthread_t aThread;
	DWORD dwThrId;
	struct pthread_s_new* pThreadData = STATIC_CAST2(struct pthread_s_new*,TlsGetValue(s_tlsPthreadDataKey));
	if (pThreadData) { return pThreadData; }

	dwThrId = GetCurrentThreadId();
	WaitForSingleObject(s_mutexForThreadContainers, INFINITE);
	if (!s_hashByIds2.FindEntry(&dwThrId, sizeof(DWORD), &pThreadData)) {
		aThread = pthread_self();
		pThreadData = STATIC_CAST2(struct pthread_s_new*, calloc(1, sizeof(struct pthread_s_new)));
		if(LIKELY2(pThreadData)){
			InitThreadDataAndAddToList(pThreadData,aThread,dwThrId);
			s_hashByIds2.AddEntry(&dwThrId, sizeof(DWORD),pThreadData);
			s_hashByHandles2.AddEntry(&aThread, sizeof(pthread_t), pThreadData);
		}
	}
	ReleaseMutex(s_mutexForThreadContainers);

	if(LIKELY2(pThreadData)){
		pThreadData->existOnThreadLocalStorage = 1;
		TlsSetValue(s_tlsPthreadDataKey,pThreadData);
	}

	return pThreadData;
}



HIDE_SYMBOL2 struct pthread_s_new* GetAnyThreadDataPointer(pthread_t a_anyThread)
{
	struct pthread_s_new* pThreadData = NEWNULLPTR2;
	pthread_t aCurrentThread = pthread_self();

	if(aCurrentThread==a_anyThread){
		return GetCurrentThreadDataPointer();
	}

	WaitForSingleObject(s_mutexForThreadContainers, INFINITE);
	if (!s_hashByHandles2.FindEntry(&a_anyThread, sizeof(pthread_t), &pThreadData)) {
		DWORD dwThrId = GetThreadId(a_anyThread);

		if (s_hashByIds2.FindEntry(&dwThrId, sizeof(DWORD), &pThreadData)) {
			s_hashByHandles2.AddEntry(&a_anyThread, sizeof(pthread_t), pThreadData);
		}
		else {
			pThreadData = STATIC_CAST2(struct pthread_s_new*, calloc(1, sizeof(struct pthread_s_new)));
			if (LIKELY2(pThreadData)) {
				InitThreadDataAndAddToList(pThreadData, a_anyThread, dwThrId);
				s_hashByIds2.AddEntry(&dwThrId, sizeof(DWORD), pThreadData);
				s_hashByHandles2.AddEntry(&a_anyThread, sizeof(pthread_t), pThreadData);
			}
		}
	}
	ReleaseMutex(s_mutexForThreadContainers);

	return pThreadData;
}


/*//////////////////////////////////////////////*/
#define MS_VC_EXCEPTION 0x406d1388
#ifdef _WIN64
#define LAST_ARG_TYPE	ULONG_PTR
#else
#define LAST_ARG_TYPE	DWORD
#endif
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;        // must be 0x1000
	LPCSTR szName;       // pointer to name (in same addr space)
	DWORD dwThreadID;    // thread ID (-1 caller thread)
	DWORD dwFlags;       // reserved for future use, most be zero
} THREADNAME_INFO;

static inline int SetThreadNameForDebuggerRaw(DWORD a_threadId, const char* a_threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = a_threadName;
	//info.dwThreadID = GetThreadId(a_target_thread->thrd);
	info.dwThreadID = a_threadId;
	info.dwFlags = 0;

	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (LAST_ARG_TYPE*)&info);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION) {
	}
	return 0;
}


static int SetThreadNameForDebugger(struct pthread_s_new* a_threadData)
{
	if(a_threadData->isNameSet){
		SetThreadNameForDebuggerRaw(a_threadData->thrdID, a_threadData->threadName);
	}
	return 0;
}


static DWORD WINAPI ThreadDebuggerHandling(LPVOID)
{
	struct pthread_s_new* pThreadData;
	DWORD thisThreadId = GetCurrentThreadId();
	BOOL bDebuggerPresent,bDebuggerPresentOld = IsDebuggerPresent();

	//while (s_nRunDebuggerThread&&(!g_nLibraryCleanupStarted)){
	while (s_nRunDebuggerThread&&(!gh_bLibraryCleanupStarted)){
		SleepEx(2000, TRUE);
		bDebuggerPresent = IsDebuggerPresent();
		if(bDebuggerPresent && (!bDebuggerPresentOld)){
			SetThreadNameForDebuggerRaw(thisThreadId, "DebuggerHandlingThread");
			WaitForSingleObject(s_mutexForThreadContainers, INFINITE);
			pThreadData = s_pFirstThreadData;
			while(pThreadData){
				SetThreadNameForDebugger(pThreadData);
				pThreadData = pThreadData->next;
			}
			ReleaseMutex(s_mutexForThreadContainers);
		}
		bDebuggerPresentOld = bDebuggerPresent;
	}

	return 0;
}


__END_C_DECLS

static VOID NTAPI DebuggerThreadInterrupter(_In_ ULONG_PTR) {}


static void ThreadFunctionsCleanup(void)
{
	if(s_mutexForThreadContainers){
		CloseHandle(s_mutexForThreadContainers);
		s_mutexForThreadContainers = NULL;
	}

	if (s_tlsPthreadDataKey) {
		TlsFree(s_tlsPthreadDataKey);
		s_tlsPthreadDataKey = 0;
	}

	if(s_handleDebggerHandlerThread){
		s_nRunDebuggerThread = 0;
		QueueUserAPC(&DebuggerThreadInterrupter,s_handleDebggerHandlerThread,0);
		if(!gh_bLibraryCleanupStarted){WaitForSingleObject(s_handleDebggerHandlerThread,INFINITE);}
		else { SleepEx(1,FALSE); }
		CloseHandle(s_handleDebggerHandlerThread);
		s_handleDebggerHandlerThread = NEWNULLPTR2;
	}
}


WLAC_INITIALIZER(ThreadFunctionsInit) 
{
	if(UNLIKELY2(s_mutexForThreadContainers)){return;}

	s_mutexForThreadContainers = CreateMutex(NULL, FALSE, NULL);
	s_tlsPthreadDataKey = TlsAlloc();

	s_nRunDebuggerThread = 1;
	s_handleDebggerHandlerThread = CreateThread(NEWNULLPTR2,0,&ThreadDebuggerHandling,NEWNULLPTR2,0,NEWNULLPTR2);

	atexit(&ThreadFunctionsCleanup); 
}

