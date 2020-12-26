/*
 *	File: unix_sync_objects.cpp
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include "pthread.h"
#include "sys/time.h"
#include "sys/ipc.h"
#include "unix_sync_objects_private.h"

__BEGIN_C_DECLS

static _inline int atomic_mutex_creation(_VOLATILE_ pthread_mutex_t *mutex);
static _inline int atomic_cond_t_creation(_VOLATILE_ pthread_cond_t * cond);


GEM_API int pthread_mutex_init(pthread_mutex_t *RESTRICT a_mutex,
	const pthread_mutexattr_t *RESTRICT a_attr)
{
	const struct _SECURITY_ATTRIBUTES* pSecurAttribs = NULL;
	const char* cpcMutexName = NULL; // Not shared
	BOOL bInitialOwner  = FALSE;

	if (a_attr && (*a_attr))
	{
		switch ((*a_attr)->unix_attrib_flag)
		{
		case _MUTEX_NO_ATTRIB_:
			pSecurAttribs = NULL;
			cpcMutexName = NULL;
			bInitialOwner = FALSE;
			break;
		default:
			pSecurAttribs = &(*a_attr)->win_attributes;
			cpcMutexName = NULL;
			bInitialOwner = FALSE;
			break;
		}
	}

	*a_mutex = (pthread_mutex_t)malloc(sizeof(struct pthread_mutex_s));
	if (!*a_mutex) {return ENOMEM;}
	(*a_mutex)->mutex = CreateMutexA((struct _SECURITY_ATTRIBUTES*)pSecurAttribs, bInitialOwner, cpcMutexName);

	if ((*a_mutex)->mutex == NULL)
	{
		free(*a_mutex);
		int nError = (int)GetLastError();
		return nError > 0 ? -nError : nError;
	}

	return 0;
}


GEM_API int pthread_mutex_destroy(pthread_mutex_t *a_mutex)
{
	int nReturn = 0;

	if(*a_mutex){
		if (!CloseHandle((*a_mutex)->mutex)){nReturn = (int)GetLastError();}
		free(*a_mutex);
	}
	return nReturn;
}


GEM_API int pthread_mutexattr_init(pthread_mutexattr_t *a_attr)
{
	if (a_attr == NULL)return -1;
	*a_attr = (pthread_mutexattr_t)malloc(sizeof(pthread_mutexattr_s));
	if(!*a_attr){return ENOMEM;}
	memset(*a_attr, 0, sizeof(struct pthread_mutexattr_s));
	return 0;
}


GEM_API int pthread_mutexattr_destroy(pthread_mutexattr_t *a_attr)
{
	if(*a_attr){
		free(*a_attr);
	}
	return -(a_attr == NULL);
}


GEM_API int pthread_mutexattr_gettype(const pthread_mutexattr_t *RESTRICT a_attr,
	int *RESTRICT a_type)
{
	if (a_attr == NULL || a_type == NULL || (*a_attr) == NULL) return -2;
	*a_type = (int)(*a_attr)->unix_attrib_flag;
	return 0;
}


GEM_API int pthread_mutexattr_settype(pthread_mutexattr_t *a_attr, int a_type)
{
	if (a_attr == NULL || (*a_attr) == NULL) return -1;
	(*a_attr)->unix_attrib_flag = (__int64)a_type;
	return 0;
}


GEM_API int pthread_mutex_unlock(pthread_mutex_t *a_mutex)
{
	return ReleaseMutex(HANDLE_FROM_MUTEX(*a_mutex)) ? 0 : EPERM;
}


GEM_API int pthread_mutex_timedlock(pthread_mutex_t *RESTRICT a_mutex,
	const struct timespec *RESTRICT a_abs_timeout)
{
	DWORD dwWaitTimeMs;
	int nReturn;

	if (a_abs_timeout == NULL){ dwWaitTimeMs = INFINITE; }
	else
	{
		struct timeval aNow;
		gettimeofday(&aNow, NULL);
		dwWaitTimeMs = ((DWORD)(a_abs_timeout->tv_sec - aNow.tv_sec)) * 1000 +
			((DWORD)(a_abs_timeout->tv_nsec - aNow.tv_usec * 1000)) / 1000000;
	}


	if (!(*a_mutex)){ if(atomic_mutex_creation(a_mutex)){return -1;} }
	nReturn = WaitForSingleObject(HANDLE_FROM_MUTEX(*a_mutex),dwWaitTimeMs);
	if (nReturn == WAIT_FAILED){
		nReturn = GetLastError();
		nReturn = nReturn > 0 ? -nReturn : nReturn;
	}
	else if (nReturn == WAIT_OBJECT_0){
		nReturn = 0;
	}
	
	return nReturn;
}


GEM_API int pthread_mutex_lock(pthread_mutex_t *a_mutex)
{
	return pthread_mutex_timedlock(a_mutex,NULL);
}


GEM_API int pthread_mutex_trylock(pthread_mutex_t *a_mutex)
{
	const struct timespec aWaitTime = {0,0};
	return pthread_mutex_timedlock(a_mutex, &aWaitTime);
}


GEM_API int pthread_condattr_destroy(pthread_condattr_t *a_attr)
{
	if(*a_attr){
		// other deininitialization code
		free(*a_attr);
	}
	
	return 0;
}


GEM_API int pthread_condattr_init(pthread_condattr_t *a_attr)
{
	*a_attr = (pthread_condattr_t)malloc(sizeof(union pthread_condattr_u));
	if (!(*a_attr)) {return ENOMEM;}
	memset(*a_attr, 0, sizeof(union pthread_condattr_u));
	return 0;
}


GEM_API int pthread_cond_destroy(pthread_cond_t *a_cond)
{
	int nReturn = 0;
	if (*a_cond){
		DeleteCriticalSection(&((*a_cond)->cs));
		// delete condition variable
		free(*a_cond);
	}
	return nReturn;
}


GEM_API int pthread_cond_init(pthread_cond_t *RESTRICT a_cond, const pthread_condattr_t *RESTRICT a_attr)
{
	*a_cond = (pthread_cond_t)malloc(sizeof(struct pthread_cond_s));
	if (!*a_cond) { return ENOMEM; }
	InitializeConditionVariable(&((*a_cond)->cond));
	InitializeCriticalSection(&((*a_cond)->cs));
	
	return 0;
}



GEM_API int pthread_cond_broadcast(pthread_cond_t *a_cond)
{
	if(!(*a_cond)){atomic_cond_t_creation(a_cond); }
	EnterCriticalSection(&((*a_cond)->cs));
	WakeAllConditionVariable(&((*a_cond)->cond));
	LeaveCriticalSection(&((*a_cond)->cs));
	return 0;
}


GEM_API int pthread_cond_signal(pthread_cond_t *a_cond)
{
	if(!(*a_cond)){atomic_cond_t_creation(a_cond); }
	EnterCriticalSection(&((*a_cond)->cs));
	WakeConditionVariable(&((*a_cond)->cond));
	LeaveCriticalSection(&((*a_cond)->cs));
	return 0;
}


GEM_API int pthread_cond_timedwait(pthread_cond_t *RESTRICT a_cond, pthread_mutex_t *RESTRICT a_mutex,
	const struct timespec *RESTRICT a_abstime)
{
	struct timeval aTV;
	long lnWaitTime;
	DWORD dwWaitTime;
	int nReturn = 0;
	BOOL bReturnFromSleep;

	if(!(*a_cond)){atomic_cond_t_creation(a_cond); }
	EnterCriticalSection(&((*a_cond)->cs));
	if (!ReleaseMutex(HANDLE_FROM_MUTEX(*a_mutex))) { return GetLastError(); }

	if (a_abstime){
		gettimeofday(&aTV, NULL);
		lnWaitTime = ((long)(a_abstime->tv_sec - aTV.tv_sec)) * 1000 +
			(a_abstime->tv_nsec / 1000000 + aTV.tv_usec / 1000);
		dwWaitTime = lnWaitTime >0 ? lnWaitTime : 0;
	}else{dwWaitTime = INFINITE;}

	
	bReturnFromSleep = SleepConditionVariableCS(&((*a_cond)->cond), &((*a_cond)->cs),dwWaitTime);

	if (!(bReturnFromSleep)) {
		nReturn = GetLastError();
		if (nReturn == ERROR_TIMEOUT) { nReturn = ETIMEDOUT; }
	}

	LeaveCriticalSection(&((*a_cond)->cs));
	WaitForSingleObject(HANDLE_FROM_MUTEX(*a_mutex), INFINITE);

	return nReturn;

}


GEM_API int pthread_cond_wait(pthread_cond_t *RESTRICT a_cond, pthread_mutex_t *RESTRICT a_mutex)
{
	return pthread_cond_timedwait(a_cond,a_mutex,NULL);
}


GEM_API int semget(key_t a_key, int a_nsems, int a_semflg)
{
	int nSem;
	char* pcSemName = (char*)alloca(IPC_STRING_DEF_LEN + 1);

	PREPARE_STRING(pcSemName, a_key, a_nsems, a_semflg);
	nSem = (int)((size_t)CreateSemaphoreA(0, (LONG)a_nsems, (LONG)MAX_SEMAPHORE_COUNT, pcSemName));
	return nSem > 0 ? nSem : -1;
}


static _inline int atomic_mutex_creation(_VOLATILE_ pthread_mutex_t *a_mx)
{
	pthread_mutex_t aMx = (pthread_mutex_t)malloc(sizeof(struct pthread_mutex_s));
	
	if (!aMx) { return ENOMEM; }
	aMx->mutex = CreateMutex(NULL,FALSE,NULL);
	
	if(!aMx->mutex){return GetLastError();}

	if (InterlockedCompareExchangePointer((volatile PVOID*)a_mx,aMx,(pthread_mutex_t)0)) {
		CloseHandle(aMx->mutex);
		free(aMx);
	}
	return 0;
}


static _inline int atomic_cond_t_creation(_VOLATILE_ pthread_cond_t *a_mx)
{
	pthread_cond_t aMx = (pthread_cond_t)0;
	int nRet = pthread_cond_init(&aMx,NULL);

	if(nRet){return nRet;}

	if (InterlockedCompareExchangePointer((volatile PVOID*)a_mx, aMx,(pthread_cond_t)0)) {
		pthread_cond_destroy(&aMx);
	}
	
	return 0;
}

__END_C_DECLS
