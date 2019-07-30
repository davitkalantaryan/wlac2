// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/file.h> For WINDOWS MFC
 *
 *	Created on: Aug 17, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_sys_file_h___
#define __win_sys_file_h___

#include <first_includes/common_include_for_headers.h>
#include <fcntl.h>

__BEGIN_C_DECLS

/* Alternate names for values for the WHENCE argument to `lseek'.
These are the same as SEEK_SET, SEEK_CUR, and SEEK_END, respectively.  */
#ifndef L_SET
# define L_SET	0	/* Seek from beginning of file.  */
# define L_INCR	1	/* Seek from current position.  */
# define L_XTND	2	/* Seek from end of file.  */
#endif


/* Operations for the `flock' call.  */
#define	LOCK_SH	1	/* Shared lock.  */
#define	LOCK_EX	2 	/* Exclusive lock.  */
#define	LOCK_UN	8	/* Unlock.  */

/* Can be OR'd in to one of the above.  */
#define	LOCK_NB	4	/* Don't block when locking.  */


/* Apply or remove an advisory lock, according to OPERATION,
on the file FD refers to.  */
GEM_API_FAR int flock(int __fd, int __operation) __THROW;


__END_C_DECLS



#endif  // #ifndef __win_sys_file_h___
