// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/shm.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_shm_h__
#define __win_sys_shm_h__

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>
#include <linux/limits.h>

#ifndef  SHM_RDONLY
#define	 SHM_RDONLY	FILE_MAP_READ
#endif


/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#ifndef IPC_RMID
#define IPC_RMID	0		/* Remove identifier.  */
#define IPC_SET		1		/* Set `ipc_perm' options.  */
#define IPC_STAT	2		/* Get `ipc_perm' options.  */
#ifdef __USE_GNU
# define IPC_INFO	3		/* See ipcs.  */
#endif
#endif // #ifndef IPC_RMID

__BEGIN_C_DECLS

GEM_API_FAR segment_t shmget(key_t key, size_t size, int shmflg);
GEM_API_FAR int shmctl(segment_t a_shmid, int cmd, struct shmid_ds *buf);
GEM_API_FAR void *shmat(segment_t shmid, const void *shmaddr, int shmflg);
GEM_API_FAR int shmdt(const void *shmaddr);

__END_C_DECLS

#endif  /* #ifndef __win_sys_shm_h__ */
