
// wlac_wait_functions.cpp
// 2017 Dec 29

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <windows.h>
#include <stddef.h>

__BEGIN_C_DECLS

GEM_VAR_FAR void* g_pProcessExitCallCode;
GEM_VAR_FAR int   g_nLibraryCleanupStarted;
extern DWORD g_nLoaderThreadTID;

#if 0
#define ALLOWED_WAITING(...)	(	(!g_nLibraryCleanupStarted) ||						\
									(  (g_pProcessExitCallCode==NULL) &&				\
									   (g_nLoaderThreadTID==GetCurrentThreadId())  )  )
#endif

#define ALLOWED_WAITING(...)	(g_pProcessExitCallCode==NULL)


#ifdef WaitForSingleObject
#undef WaitForSingleObject
#endif
GEM_API DWORD wlac_WaitForSingleObject(HANDLE a_hHandle,DWORD a_dwMilliseconds)
{
	DWORD dwRet = WAIT_OBJECT_0;

	if(ALLOWED_WAITING()){
		dwRet = WaitForSingleObject(a_hHandle, a_dwMilliseconds);
	}

	return dwRet;
}


#ifdef WaitForSingleObjectEx
#undef WaitForSingleObjectEx
#endif
GEM_API DWORD wlac_WaitForSingleObjectEx(HANDLE a_hHandle,DWORD a_dwMilliseconds,BOOL a_bAlertable)
{
	DWORD dwRet = WAIT_OBJECT_0;

	if(ALLOWED_WAITING()){
		dwRet = WaitForSingleObjectEx(a_hHandle, a_dwMilliseconds,a_bAlertable);
	}

	return dwRet;
}


#ifdef WaitForMultipleObjects
#undef WaitForMultipleObjects
#endif
GEM_API DWORD wlac_WaitForMultipleObjects(
	DWORD  a_nCount,
	const HANDLE *a_lpHandles,
	BOOL   a_bWaitAll,
	DWORD  a_dwMilliseconds)
{
	DWORD dwRet = WAIT_OBJECT_0;

	if(ALLOWED_WAITING()){
		dwRet = WaitForMultipleObjects(
			a_nCount,a_lpHandles,
			a_bWaitAll,a_dwMilliseconds);
	}

	return dwRet;
}


#ifdef WaitForMultipleObjectsEx
#undef WaitForMultipleObjectsEx
#endif
GEM_API DWORD wlac_WaitForMultipleObjectsEx(
	DWORD  a_nCount,
	const HANDLE *a_lpHandles,
	BOOL   a_bWaitAll,
	DWORD  a_dwMilliseconds,
	BOOL a_bAlertable)
{
	DWORD dwRet = WAIT_OBJECT_0;

	if (ALLOWED_WAITING()) {
		dwRet = WaitForMultipleObjectsEx(
			a_nCount, a_lpHandles,
			a_bWaitAll, a_dwMilliseconds, a_bAlertable);
	}

	return dwRet;
}


#ifdef EnterCriticalSection
#undef EnterCriticalSection
#endif
GEM_API void wlac_EnterCriticalSection(LPCRITICAL_SECTION a_lpCriticalSection)
{
	if (ALLOWED_WAITING()) {
		EnterCriticalSection(a_lpCriticalSection);
	}
}


#ifdef SleepConditionVariableCS
#undef SleepConditionVariableCS
#endif
GEM_API BOOL wlac_SleepConditionVariableCS(
	PCONDITION_VARIABLE a_ConditionVariable,
	PCRITICAL_SECTION   a_CriticalSection,
	DWORD               a_dwMilliseconds)
{
	BOOL bRet = TRUE;

	if (ALLOWED_WAITING()) {
		bRet = SleepConditionVariableCS(a_ConditionVariable, a_CriticalSection,a_dwMilliseconds);
	}
	return bRet;
}


#ifdef ReleaseMutex
#undef ReleaseMutex
#endif
GEM_API BOOL wlac_ReleaseMutex(HANDLE a_hMutex)
{
	BOOL bRet = TRUE;

	if (ALLOWED_WAITING()) {
		bRet = ReleaseMutex(a_hMutex);
	}
	return bRet;
}


#ifdef ReleaseSemaphore
#undef ReleaseSemaphore
#endif
GEM_API BOOL wlac_ReleaseSemaphore(HANDLE a_hSema,LONG a_lRelease,LPLONG a_lpPrevious)
{
	BOOL bRet = TRUE;

	if (ALLOWED_WAITING()) {
		bRet = ReleaseSemaphore(a_hSema, a_lRelease, a_lpPrevious);
	}
	return bRet;
}


#ifdef LeaveCriticalSection
#undef LeaveCriticalSection
#endif
GEM_API void wlac_LeaveCriticalSection(LPCRITICAL_SECTION a_lpCriticalSection)
{
	if (ALLOWED_WAITING()) {
		LeaveCriticalSection(a_lpCriticalSection);
	}
}


#ifdef WakeConditionVariable
#undef WakeConditionVariable
#endif
GEM_API void wlac_WakeConditionVariable(PCONDITION_VARIABLE a_ConditionVariable)
{
	if (ALLOWED_WAITING()) {
		WakeConditionVariable(a_ConditionVariable);
	}
}


#ifdef WakeAllConditionVariable
#undef WakeAllConditionVariable
#endif
GEM_API void wlac_WakeAllConditionVariable(PCONDITION_VARIABLE a_ConditionVariable)
{
	if (ALLOWED_WAITING()) {
		WakeAllConditionVariable(a_ConditionVariable);
	}
}

__END_C_DECLS
