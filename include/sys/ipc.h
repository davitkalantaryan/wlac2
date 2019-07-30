// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/ipc.h> For WINDOWS MFC
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_ipc_h__
#define __win_sys_ipc_h__

#include <first_includes/common_include_for_headers.h>
#include <Windows.h>
#include <sdef_gem_windows.h>
#include <linux/limits.h>
#include <rfc/windows_signal.h>
#include <rfc/remote_function_caller.h>

#ifndef  SHM_RDONLY
#define	 SHM_RDONLY	FILE_MAP_READ
#endif

#define MAX_SEMAPHORE_COUNT	1024

// key_t a_key, int a_nsems, int a_semflg
#define PREPARE_STRING_PRIVATE(_string_,_str_len_,_key_,_size_,_flag_) \
	(snprintf((_string_), (_str_len_), "01234567898765432____key=%d__size=%d__flag=%d", \
		(int)(_key_), (int)(_size_), (int)(_flag_)))

#define	IPC_STRING_DEF_LEN	127
#define PREPARE_STRING(_string_,_key_,_size_,_flag_) \
	PREPARE_STRING_PRIVATE(_string_,IPC_STRING_DEF_LEN,_key_,_size_,_flag_)

/* Mode bits for `msgget', `semget', and `shmget'.  */
#ifndef IPC_CREAT
#define IPC_CREAT	01000		/* Create key if key does not exist. */
#define IPC_EXCL	02000		/* Fail if key exists.  */
#define IPC_NOWAIT	04000		/* Return error on wait.  */
#endif  // #ifndef IPC_CREAT

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

GEM_API_FAR key_t ftok(const char *pathname, int proj_id);
GEM_API_FAR segment_t shmget(key_t key, size_t size, int shmflg);
GEM_API_FAR int shmctl(segment_t a_shmid, int cmd, struct shmid_ds *buf);

__END_C_DECLS

#endif  /* #ifndef __win_sys_ipc_h__ */
