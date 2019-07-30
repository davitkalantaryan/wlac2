// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/types_new.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * 
 */
#ifndef __win_sys_types_new_h__
#define __win_sys_types_new_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <sys/types.h>, <sys/types.h> )
#pragma include_alias( "sys/types.h", "sys/types.h" )
#include <bits/mman.h>
#include <win_socket_common.h>
#include <sys/types.h>

#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif

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

#ifndef __S32_TYPE
#define	__S32_TYPE	int
#endif
#ifndef __KEY_T_TYPE
#define __KEY_T_TYPE		__S32_TYPE
#endif
#ifndef __STD_TYPE
# define __STD_TYPE		typedef
#endif

#if !defined(____key_t_defined) && !defined(__key_t_defined)
__STD_TYPE __KEY_T_TYPE __key_t;	/* Type of an IPC key.  */
#define ____key_t_defined
//#define __key_t_defined // Will be defined next
#endif  // #if !defined(____key_t_defined) && !defined(__key_t_defined)

#if /*(defined __USE_SVID || defined __USE_XOPEN) && */ !defined __key_t_defined && !defined key_t_defined
typedef __key_t key_t;
# define __key_t_defined
#define key_t_defined
#endif

#if !defined(__mode_t_defined) & !defined(mode_t_defined)
typedef unsigned int mode_t;
#define __mode_t_defined
#define mode_t_defined
#endif  // #if !defined(__mode_t_defined) & !defined(mode_t_defined)

#ifndef ____uint32_t_defined
typedef unsigned int __uint32_t;
#define ____uint32_t_defined
#endif

#ifndef __u_short_defined
typedef unsigned short int u_short;
#define __u_short_defined
#endif

#ifndef __u_char_defined
typedef unsigned char u_char;
#define __u_char_defined
#endif


#ifndef ____int32_t_defined
typedef int __int32_t;
#define ____int32_t_defined
#endif

#if !defined(__int64_t_defined) &&  !defined(int64_t_defined)
typedef __int64 int64_t;
#define __int64_t_defined
#define int64_t_defined
#endif

#if !defined(__uint64_t_defined) &&  !defined(uint64_t_defined)
typedef unsigned __int64 uint64_t;
#define __uint64_t_defined
#define uint64_t_defined
#endif

#ifndef __u_int32_t_defined
typedef unsigned int u_int32_t;
#define __u_int32_t_defined
#endif

#ifndef __uint_defined
typedef u_int32_t uint;
#define __uint_defined
#endif

#if !defined(__int32_t_defined) && !defined(int32_t_defined)
typedef int int32_t;
#define __int32_t_defined
#define int32_t_defined
#endif  // #if !defined(__int32_t_defined) && !defined(int32_t_defined)


#if !defined(__uid_t_defined) && !defined(uid_t_defined)
typedef unsigned int uid_t;
typedef uid_t __uid_t;
#define __uid_t_defined
#endif  // #if !defined(__uid_t_defined) && !defined(uid_t_defined)

#if !defined(pid_t_defined)
#define pid_t_defined
typedef unsigned int pid_t;
#endif


#ifndef __gid_t_defined
typedef __uint32_t gid_t;
typedef gid_t __gid_t;
#define __gid_t_defined
#endif

#ifndef __P
#define __P(protos)	protos
#endif

#ifndef LINKAGE_HDR
#ifdef __WRPC_LIB_CREATION__
#define LINKAGE_HDR __declspec(dllexport)
#else
#define LINKAGE_HDR __declspec(dllimport)
#endif
#endif

#ifndef __const
#define __const const
#endif

#ifndef __uint32_t_defined
typedef __uint32_t uint32_t;
#define __uint32_t_defined
#endif

#ifndef __in_addr_t_defined
typedef uint32_t in_addr_t;
#define __in_addr_t_defined
#endif

#ifndef ____caddr_t_defined
typedef char *__caddr_t;
#define ____caddr_t_defined
#endif

#ifndef __caddr_t_defined
typedef char* caddr_t;
//typedef void* caddr_t;
#define __caddr_t_defined
#endif

#if !defined(__int64_t_defined) && !defined(int64_t_defined)
typedef __int64 int64_t;
#define __int64_t_defined
#define int64_t_defined
#endif

#if !defined(__segment_t_defined) && !defined(segment_t_defined)
typedef unsigned __int64 segment_t;
#define __segment_t_defined
#define segment_t_defined
#endif


#if !defined(__shmid_ds_defined) && !defined(shmid_ds_defined)
struct shmid_ds
{
	int m;
};
#define __shmid_ds_defined
#define shmid_ds_defined
#endif

__BEGIN_C_DECLS

GEM_API_FAR key_t ftok(const char *pathname, int proj_id);
GEM_API_FAR void *shmat(segment_t shmid, const void *shmaddr, int shmflg);
GEM_API_FAR int shmdt(const void *shmaddr);

__END_C_DECLS

#endif  /* #ifndef __win_sys_types_new_h__ */
