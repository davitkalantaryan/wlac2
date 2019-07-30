// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// windows.h
// 2017 Dec 24


#ifndef __wlac_windows_h__
#define __wlac_windows_h__


#include <.privatei/windows_and_socket_headers.h>
#ifndef __BEGIN_C_DECLS
#include <first_includes/common_definations_wul.h>
#endif


__BEGIN_C_DECLS

GEM_API_FAR DWORD wlac_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
GEM_API_FAR DWORD wlac_WaitForSingleObjectEx(
	HANDLE hHandle,DWORD dwMilliseconds,BOOL bAlertable);
GEM_API_FAR DWORD wlac_WaitForMultipleObjects(
	DWORD  nCount,
	const HANDLE *lpHandles,
	BOOL   bWaitAll,
	DWORD  dwMilliseconds);
GEM_API_FAR DWORD wlac_WaitForMultipleObjectsEx(
	DWORD  a_nCount,
	const HANDLE *a_lpHandles,
	BOOL   a_bWaitAll,
	DWORD  a_dwMilliseconds,
	BOOL a_bAlertable);
GEM_API_FAR void wlac_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
GEM_API_FAR BOOL wlac_SleepConditionVariableCS(
	PCONDITION_VARIABLE ConditionVariable,
	PCRITICAL_SECTION   CriticalSection,
	DWORD               dwMilliseconds);
GEM_API_FAR BOOL wlac_ReleaseMutex(HANDLE hMutex);
GEM_API_FAR BOOL wlac_ReleaseSemaphore(HANDLE sema,LONG release, LPLONG pPrev);
GEM_API_FAR void wlac_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
GEM_API_FAR void wlac_WakeConditionVariable(PCONDITION_VARIABLE a_ConditionVariable);
GEM_API_FAR void wlac_WakeAllConditionVariable(PCONDITION_VARIABLE a_ConditionVariable);

__END_C_DECLS

#define WaitForSingleObject			wlac_WaitForSingleObject
#define WaitForSingleObjectEx		wlac_WaitForSingleObjectEx
#define WaitForMultipleObjects		wlac_WaitForMultipleObjects
#define WaitForMultipleObjectsEx	wlac_WaitForMultipleObjectsEx
#define EnterCriticalSection		wlac_EnterCriticalSection
#define	SleepConditionVariableCS	wlac_SleepConditionVariableCS
#if 1
#define ReleaseMutex				wlac_ReleaseMutex
#define ReleaseSemaphore			wlac_ReleaseSemaphore
#define	LeaveCriticalSection		wlac_LeaveCriticalSection
#define	WakeConditionVariable		wlac_WakeConditionVariable
#define	WakeAllConditionVariable	wlac_WakeAllConditionVariable
#endif

#endif  // #ifndef __wlac_windows_h__
