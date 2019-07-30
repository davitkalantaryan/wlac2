// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/sem.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_sem_h__
#define __win_sys_sem_h__

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

/* Commands for `semctl'.  */
#ifndef GETPID
#define GETPID		11		/* get sempid */
#define GETVAL		12		/* get semval */
#define GETALL		13		/* get all semval's */
#define GETNCNT		14		/* get semncnt */
#define GETZCNT		15		/* get semzcnt */
#define SETVAL		16		/* set semval */
#define SETALL		17		/* set all semval's */
#endif  // #ifndef GETPID

/* Flags for `semop'.  */
#ifndef SEM_UNDO
#define SEM_UNDO	0x1000		/* undo the operation on exit */
#endif

/* Structure used for argument to `semop' to describe operations.  */
struct sembuf
{
	unsigned short int sem_num;	/* semaphore number */
	short int sem_op;		/* semaphore operation */
	short int sem_flg;		/* operation flag */
};

__BEGIN_C_DECLS

GEM_API_FAR int semget(key_t key, int nsems, int semflg);
GEM_API_FAR int semctl(int semid, int semnum, int cmd, ...);
GEM_API_FAR int semop(int semid, struct sembuf *sops, unsigned nsops);

__END_C_DECLS

#endif  /* #ifndef __win_sys_sem_h__ */
