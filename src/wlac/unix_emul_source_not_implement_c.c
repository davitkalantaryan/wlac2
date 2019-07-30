/*
 *	File: unix_emulator_source_not_implement_c.c
 *
 *	Created on: Dec 10, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <sys/time.h>
#include <io.h>
#include <malloc.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/uio.h>
#include <err.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include "pthread_private_for_source.h"
#include <grp.h>

__BEGIN_C_DECLS

GEM_VAR int kb_main_total;
GEM_VAR int kb_main_free;
GEM_VAR int kb_main_cached;
GEM_VAR int kb_swap_total;
GEM_VAR int kb_swap_free;
GEM_VAR int g_numProcessors;

GEM_API int statvfs64(const char * __file, struct statvfs64 * __buf)
{
	return 0;
}
GEM_API int statvfs(const char *path, struct statvfs *buf)
{
	return 0;
}

GEM_API int epoll_wait(int epfd, struct epoll_event *events,
	int maxevents, int timeout)
{
	return 0;
}

GEM_API int epoll_ctl(int epfd, int op, int fd, struct epoll_event *a_event)
{
	return 0;
}

GEM_API int epoll_create(int size)
{
	return 0;
}

GEM_API int epoll_create1(int flags)
{
	return 0;
}

GEM_API pid_t fork(void)
{
	return 0;
}

GEM_API pid_t setsid(void)
{
	return 0;
}

GEM_API void uptime(double* uptime_secs, double* idle_secs)
{
}

GEM_API void meminfo(void) 
{
}


GEM_API struct in_addr inet_makeaddr(unsigned long net,
		unsigned long lna)
{
	struct in_addr sRet;
	sRet.S_un.S_addr = ((unsigned short int)(net << 16)) & ((unsigned short int)lna);
	return sRet;
}


GEM_API int getdtablesize(void)
{
	return  1024; ///?
}


GEM_API long sysconf(int name)
{
	return (long)name;
}


GEM_API int semctl(int semid, int semnum, int cmd, ...)
{
	return 0;
}

GEM_API int semop(int semid, struct sembuf *sops, unsigned nsops)
{
	return 0;
}


GEM_API int sem_init(sem_t * a_sem, int __pshared, unsigned int a_value1)__THROW
{
	a_sem->sem = CreateSemaphoreA(NULL, a_value1, 1024, NULL);
	return 0;
}


GEM_API int sem_destroy(sem_t *a_sem) __THROW
{
	//LONG i,lPrevCount;
	//ReleaseSemaphore(a_sem->sem,1,&lPrevCount);
	//++lPrevCount;
	//for(i=0;i<lPrevCount;++i){WaitForSingleObject(a_sem->sem, 0);}
	CloseHandle(a_sem->sem);
	return 0;
}


GEM_API int sem_wait(sem_t *a_sem)
{
	return ((WaitForSingleObject(a_sem->sem, INFINITE) == WAIT_OBJECT_0) ? 0 : -1);
}


GEM_API int sem_post(sem_t *a_sem) __THROW
{
	ReleaseSemaphore(a_sem->sem, 1, 0);
	return 0;
}


GEM_API char *strptime(const char *s, const char *format, struct tm *tm)
{
	return (char*)s;
}


GEM_API int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
	return 0;
}


GEM_API int pthread_getname_np(pthread_t a_thread, char *a_buffer, size_t a_len)
{
	if((!a_thread)||(!a_thread->threadName)){return -1;}
	strncpy(a_buffer,a_thread->threadName,a_len);
	return 0;
}


GEM_API int pthread_setname_np(pthread_t a_target_thread, __const char *a_name)
{
	int nReturn = -1;
	BOOL bDebugger = TRUE;

	if (a_target_thread){
		size_t unStrLenPlus1 = strlen(a_name)+1;
		char* newName = (char*)realloc(a_target_thread->threadName,unStrLenPlus1);
		if(!newName){return ENOMEM;}
		memcpy(newName, a_name,unStrLenPlus1);
		a_target_thread->threadName = newName;
		nReturn = 0;
	}

	CheckRemoteDebuggerPresent(GetCurrentProcess(), &bDebugger);

	if (bDebugger) {
		nReturn= SetThreadNameForDebugger(a_target_thread,a_name);
	}
	return nReturn;
}


GEM_API int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
	return 0;
}


GEM_API int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
	return 0;
}


GEM_API int pthread_kill(pthread_t thread, int sig)
{
	return 0;
}


GEM_API int getpwnam_r(const char *name, struct passwd *pwd,
	char *buf, size_t buflen, struct passwd **result)
{
	return 0;
}


GEM_API struct passwd *getpwnam(const char *name)
{
	static struct passwd aPasswd;
	return &aPasswd;
}



GEM_API int getpwuid_r(uid_t a_uid, struct passwd *a_pwd, char *buf, size_t a_buflen, struct passwd **a_result)
{
	*a_result = a_pwd;
	return 0;
}


GEM_API struct passwd *getpwuid(uid_t a_uid)
{
	static struct passwd aPasswd;
	struct passwd* pReturn;
	getpwuid_r(a_uid,&aPasswd,(char*)&aPasswd,sizeof(struct passwd),&pReturn);
	return pReturn;
}


GEM_API int fsync(int fildes)
{
	Sleep(1);
	return 0;
}


GEM_API int sched_get_priority_max(int policy)
{
	return 10;
}


GEM_API int sched_get_priority_min(int policy)
{
	return 1;
}


GEM_API_FAR int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
	return 0;
}


GEM_API_FAR int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param)
{
	return 0;
}


GEM_API int flock(int __fd, int __operation) __THROW
{
	return 0;
}


GEM_API void warn(__const char *__format, ...)
{
	va_list args;
	va_start(args, __format);
	vfprintf(stderr, __format,args);
	va_end(args);
}


static int s_rlimit_soft = 1024;
static int s_rlimit_hard = 1024;

GEM_API int getrlimit(int resource, struct rlimit *rlim)
{
	rlim->rlim_cur = s_rlimit_soft;
	rlim->rlim_max = s_rlimit_hard;
	return 0;
}


GEM_API int setrlimit(int resource, const struct rlimit *rlim)
{
	s_rlimit_soft = (int)rlim->rlim_cur;
	s_rlimit_hard = (int)rlim->rlim_max;
	return 0;
}


GEM_API int daemon(int nochdir, int noclose)
{
	return 0;
}


GEM_API int setgid(gid_t gid)
{
	return 0;
}


//GEM_API int getgroups(int size, gid_t list[])
//{
//	return 0;
//}


GEM_API int setuid(uid_t uid)
{
	return 0;
}


GEM_API int setgroups(size_t size, const gid_t *list)
{
	return 0;
}


GEM_API int getgrgid_r(gid_t gid, struct group *grp, char *buf, size_t buflen, struct group **result)
{
	grp->gr_gid = gid;
	*result = grp;
	return 0;
}



typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;        // must be 0x1000
	LPCSTR szName;       // pointer to name (in same addr space)
	DWORD dwThreadID;    // thread ID (-1 caller thread)
	DWORD dwFlags;       // reserved for future use, most be zero
} THREADNAME_INFO;

#define MS_VC_EXCEPTION 0x406d1388
#if(_MSC_VER >= 1400)
#pragma warning(disable: 6312)
#pragma warning(disable: 6322)
#else
#ifndef _In_
//#define _In_    _SAL2_Source_(_In_, (), _Pre1_impl_(__notnull_impl_notref) _Pre_valid_impl_ _Deref_pre1_impl_(__readaccess_impl_notref))
#define _In_
#endif
//WINBASEAPI
//DWORD
//WINAPI
//GetThreadId(
//    _In_ HANDLE Thread
//    );
#ifndef GetThreadId
#define GetThreadId(__thread__) GetCurrentThreadId()
#endif
#endif
#ifdef _WIN64
#define LAST_ARG_TYPE	ULONG_PTR
#else
#define LAST_ARG_TYPE	DWORD
#endif


int SetThreadNameForDebugger(pthread_t a_target_thread,const char *a_name)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = a_name;
	//info.dwThreadID = GetThreadId(a_target_thread->thrd);
	info.dwThreadID = a_target_thread->thrdID;
	info.dwFlags = 0;

	__try{
		RaiseException(MS_VC_EXCEPTION,0, sizeof(info)/sizeof(DWORD),(LAST_ARG_TYPE*)&info);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION){
	}
	return 0;
}

__END_C_DECLS
