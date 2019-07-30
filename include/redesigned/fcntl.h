// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <redisagned/fcntl.h> For WINDOWS MFC
 *
 *	Created on: Dec 15, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 *
 */
#ifndef __win_fcntl_h__
#define __win_fcntl_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <fcntl.h>, <fcntl.h> )
#pragma include_alias( "fcntl.h", "fcntl.h" )
#include <fcntl.h>


#ifndef F_RDLCK
/* For posix fcntl() and `l_type' field of a `struct flock' for lockf().  */
# define F_RDLCK		0	/* Read lock.  */
# define F_WRLCK		1	/* Write lock.	*/
# define F_UNLCK		2	/* Remove lock.	 */
#endif

#ifndef F_GETLK64
# define F_GETLK64	12	/* Get record locking info.  */
# define F_SETLK64	13	/* Set record locking info (non-blocking).  */
# define F_SETLKW64	14	/* Set record locking info (blocking).	*/
#endif

#ifndef F_GETLK
# ifndef __USE_FILE_OFFSET64
#  define F_GETLK	5	/* Get record locking info.  */
#  define F_SETLK	6	/* Set record locking info (non-blocking).  */
#  define F_SETLKW	7	/* Set record locking info (blocking).	*/
# else
#  define F_GETLK	F_GETLK64  /* Get record locking info.	*/
#  define F_SETLK	F_SETLK64  /* Set record locking info (non-blocking).*/
#  define F_SETLKW	F_SETLKW64 /* Set record locking info (blocking).  */
# endif
#endif // #ifndef F_GETLK

#define F_SETFD		1 // https://linux.die.net/man/3/fcntl
#define F_GETFD		2
#define	FD_CLOEXEC	3

__BEGIN_C_DECLS

struct flock
{
	short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
	short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
#ifndef __USE_FILE_OFFSET64
	__int64 l_start;	/* Offset where the lock begins.  */
	__int64 l_len;	/* Size of the locked area; zero means until EOF.  */
#else
	__int64 l_start;	/* Offset where the lock begins.  */
	__int64 l_len;	/* Size of the locked area; zero means until EOF.  */
#endif
	int l_pid;	/* Process holding the lock.  */
};

GEM_API_FAR int fcntl(int fd, int cmd, ... /* arg */);

__END_C_DECLS

#endif  /* #ifndef __win_fcntl_h__ */
