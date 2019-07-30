// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <semaphore.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_semaphore_h__
#define __win_semaphore_h__

#include <first_includes/common_include_for_headers.h>
#include <features.h>
#include <sys/types.h>
#ifdef __USE_XOPEN2K
# define __need_timespec
# include <time.h>
#endif

/* Get the definition for sem_t.  */
#include <bits/semaphore.h>


__BEGIN_C_DECLS

/* Initialize semaphore object SEM to VALUE.  If PSHARED then share it
with other processes.  */
GEM_API_FAR int sem_init(sem_t *__sem, int __pshared, unsigned int __value1)__THROW;

/* Free resources associated with semaphore object SEM.  */
GEM_API_FAR int sem_destroy(sem_t *__sem) __THROW;

/* Open a named semaphore NAME with open flags OFLAG.  */
GEM_API_FAR sem_t *sem_open(__const char *__name, int __oflag, ...) __THROW;

/* Close descriptor for named semaphore SEM.  */
GEM_API_FAR int sem_close(sem_t *__sem) __THROW;

/* Remove named semaphore NAME.  */
GEM_API_FAR int sem_unlink(__const char *__name) __THROW;

/* Wait for SEM being posted.

This function is a cancellation point and therefore not marked with
__THROW.  */
GEM_API_FAR int sem_wait(sem_t *__sem);

#ifdef __USE_XOPEN2K
/* Similar to `sem_wait' but wait only until ABSTIME.

This function is a cancellation point and therefore not marked with
__THROW.  */
GEM_API_FAR int sem_timedwait(sem_t *__restrict __sem,
	__const struct timespec *__restrict __abstime);
#endif

/* Test whether SEM is posted.  */
GEM_API_FAR int sem_trywait(sem_t *__sem) __THROW;

/* Post SEM.  */
GEM_API_FAR int sem_post(sem_t *__sem) __THROW;

/* Get current value of SEM and store it in *SVAL.  */
GEM_API_FAR int sem_getvalue(sem_t *__restrict __sem, int *__restrict __sval)
__THROW;


__END_C_DECLS


#endif  /* #ifndef __win_semaphore_h__ */
