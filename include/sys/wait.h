// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/wait.h> For WINDOWS MFC
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_wait_h__
#define __win_sys_wait_h__

#include <first_includes/common_include_for_headers.h>
#include <sys/cdef.h>
#include <sdef_gem_windows.h>
#include <linux/limits.h>
#include <bits/resource.h>

#ifndef WIFEXITED
#define WIFEXITED(...)  (1)
#endif

#ifndef WEXITSTATUS
#define WEXITSTATUS(...)  (0)
#endif

/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* Bits in the fourth argument to `waitid'.  */
#define WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
#define WEXITED		4	/* Report dead child.  */
#define WCONTINUED	8	/* Report continued child.  */
#define WNOWAIT		0x01000000 /* Don't reap, just poll status.  */

#define __WNOTHREAD     0x20000000 /* Don't wait on children of other threads
in this group */
#define __WALL		0x40000000 /* Wait for any child.  */
#define __WCLONE	0x80000000 /* Wait for cloned process.  */

__BEGIN_C_DECLS

pid_t wait3(int *status, int options,struct rusage *rusage);

__END_C_DECLS

#endif  /* #ifndef __win_sys_wait_h__ */
