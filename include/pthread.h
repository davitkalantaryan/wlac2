// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <pthred.h> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *            : Anushavan Azatyan (Email: anushavan.azatyan@desy.de)
 *
 * //
 *
 *
 */
#ifndef __win_pthread_h__
#define __win_pthread_h__

#include <first_includes/common_include_for_headers.h>
#include <win_socket_common.h>
#include <tlhelp32.h>
#include <sys/cdefs.h>
#include <stdlib.h>
#include <strings.h>
#include <redesigned/errno.h>
#include <redesigned/time.h>

__BEGIN_C_DECLS

typedef void*(*start_routine_t)(void*);
typedef struct pthread_s* pthread_t;

#if !defined(__pthread_key_t_defined) & !defined(pthread_key_t_defined) & !defined(pthread_key_t)
typedef LONG pthread_key_t;
#define __pthread_key_t_defined
#define pthread_key_t_defined
#endif  // #if !defined(__pthread_key_t_defined) & !defined(pthread_key_t_defined) & !defined(pthread_key_t)

///* Once-only execution */
#if !defined(__pthread_once_t_defined) & !defined(pthread_once_t_defined)
//typedef LONG pthread_once_t;
typedef void* pthread_once_t;
#define __pthread_once_t_defined
#define pthread_once_t_defined
#endif  // #if !defined(__pthread_once_t_defined) & !defined(pthread_once_t_defined)

#ifndef PTHREAD_ONCE_INIT
#define PTHREAD_ONCE_INIT		(pthread_once_t)0
#endif

#ifndef PTHREAD_ONCE_KEY_NP
#define	PTHREAD_ONCE_KEY_NP		PTHREAD_ONCE_INIT
#endif

#ifndef pthread_exit
#define pthread_exit(_exit_code)	ExitThread( ((DWORD)(_exit_code)) )
#endif

#define	UNIX_ATTRIBS_LEN	64
#define	_MUTEX_NO_ATTRIB_	0

# define __SIZEOF_PTHREAD_CONDATTR_T 4

#ifndef PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP
#define	PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP	1
#endif

typedef struct pthread_mutex_s* pthread_mutex_t;
//typedef struct pthread_rwlock_s* pthread_rwlock_t;
#define pthread_rwlock_t  pthread_mutex_t
typedef struct pthread_mutexattr_s* pthread_mutexattr_t;
typedef struct pthread_cond_s* pthread_cond_t;
typedef struct pthread_attr_s* pthread_attr_t;
#define pthread_rwlockattr_t		pthread_mutexattr_t
typedef union pthread_condattr_u* pthread_condattr_t;

#define	PTHREAD_MUTEX_INITIALIZER	((pthread_mutex_t)0)
#define	PTHREAD_RWLOCK_INITIALIZER	((pthread_rwlock_t)0)
#define PTHREAD_COND_INITIALIZER	((pthread_cond_t)0)

#define pthread_rwlock_init			pthread_mutex_init
#define pthread_rwlock_destroy		pthread_mutex_destroy
#define pthread_rwlock_wrlock		pthread_mutex_lock
#define pthread_rwlock_rdlock		pthread_mutex_lock
#define pthread_rwlock_unlock		pthread_mutex_unlock

#define pthread_rwlockattr_init		pthread_mutexattr_init
#define pthread_rwlockattr_setkind_np(...)


/* Scope handling.  */
enum
{
	PTHREAD_SCOPE_SYSTEM,
#define PTHREAD_SCOPE_SYSTEM    PTHREAD_SCOPE_SYSTEM
	PTHREAD_SCOPE_PROCESS
#define PTHREAD_SCOPE_PROCESS   PTHREAD_SCOPE_PROCESS
};

/* Process shared or private flag.  */
enum
{
	PTHREAD_PROCESS_PRIVATE,
#define PTHREAD_PROCESS_PRIVATE PTHREAD_PROCESS_PRIVATE
	PTHREAD_PROCESS_SHARED
#define PTHREAD_PROCESS_SHARED  PTHREAD_PROCESS_SHARED
};

/* Detach state.  */
enum
{
	PTHREAD_CREATE_JOINABLE,
#define PTHREAD_CREATE_JOINABLE	PTHREAD_CREATE_JOINABLE
	PTHREAD_CREATE_DETACHED
#define PTHREAD_CREATE_DETACHED	PTHREAD_CREATE_DETACHED
};

GEM_VAR_FAR int kb_main_total;
GEM_VAR_FAR int kb_swap_free;

GEM_API_FAR int GetNumberOfProcessThreads(int pid);
GEM_API_FAR pthread_t GetPthreadDataPointer(void);
GEM_API_FAR int pthread_mutex_init(pthread_mutex_t *RESTRICT mutex,
	const pthread_mutexattr_t *RESTRICT attr);
GEM_API_FAR int pthread_mutex_destroy(pthread_mutex_t *mutex);
GEM_API_FAR int pthread_mutexattr_init(pthread_mutexattr_t *attr);
GEM_API_FAR int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
GEM_API_FAR int pthread_mutexattr_gettype(const pthread_mutexattr_t *RESTRICT attr,
	int *RESTRICT type);
GEM_API_FAR int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
GEM_API_FAR int pthread_mutex_lock(pthread_mutex_t *mutex);
GEM_API_FAR int pthread_mutex_trylock(pthread_mutex_t *mutex);
GEM_API_FAR int pthread_mutex_unlock(pthread_mutex_t *mutex);
GEM_API_FAR int pthread_mutex_timedlock(pthread_mutex_t *RESTRICT a_mutex,
	const struct timespec *RESTRICT a_abs_timeout);
GEM_API_FAR int pthread_create(pthread_t *a_thread, const pthread_attr_t *a_attr,
	void *(*a_start_routine) (void *), void *a_arg);
GEM_API_FAR int pthread_join(pthread_t thread, void **retval);
GEM_API_FAR int pthread_attr_init(pthread_attr_t *a_attr);
GEM_API_FAR int pthread_attr_destroy(pthread_attr_t *a_attr);
GEM_API_FAR int pthread_attr_setscope(pthread_attr_t *attr, int scope);
GEM_API_FAR int pthread_attr_getscope(pthread_attr_t *attr, int *scope);
GEM_API_FAR int pthread_attr_setdetachstate(pthread_attr_t *attrint, int detachstate);
GEM_API_FAR int pthread_attr_getdetachstate(pthread_attr_t *attrint, int *detachstate);
GEM_API_FAR int pthread_condattr_destroy(pthread_condattr_t *attr);
GEM_API_FAR int pthread_condattr_init(pthread_condattr_t *attr);
GEM_API_FAR int pthread_cond_destroy(pthread_cond_t *cond);
GEM_API_FAR int pthread_cond_init(pthread_cond_t *RESTRICT cond, const pthread_condattr_t *RESTRICT attr);
GEM_API_FAR int pthread_cond_broadcast(pthread_cond_t *cond);
GEM_API_FAR int pthread_cond_signal(pthread_cond_t *cond);
GEM_API_FAR int pthread_cond_timedwait(pthread_cond_t *RESTRICT cond, pthread_mutex_t *RESTRICT mutex,
	const struct timespec *RESTRICT abstime);
GEM_API_FAR int pthread_cond_wait(pthread_cond_t *RESTRICT cond, pthread_mutex_t *RESTRICT mutex);
GEM_API_FAR int pthread_setname_np(pthread_t __target_thread, __const char *__name);
GEM_API_FAR int pthread_getname_np(pthread_t thread,char *name, size_t len);
GEM_API_FAR pthread_t pthread_self(void);
GEM_API_FAR int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,int pshared);
GEM_API_FAR int pthread_condattr_setpshared(pthread_condattr_t *attr,int pshared);
GEM_API_FAR int pthread_setschedparam(pthread_t thread, int policy,const struct sched_param *param);
GEM_API_FAR int pthread_getschedparam(pthread_t thread, int *policy,struct sched_param *param);
GEM_API_FAR int pthread_once(pthread_once_t *once_control, void(*init_routine)(void));
GEM_API_FAR int pthread_once_void_ptr(pthread_once_t *once_control, void(*init_routine)(void*), void* a_arg);
GEM_API_FAR int pthread_key_create(pthread_key_t *key, void(*destructor)(void*));

__END_C_DECLS

#endif  /* #ifndef __win_pthread_h__ */
