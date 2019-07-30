// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/errno.h> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_errno_h__
#define __win_sys_errno_h__

#include <first_includes/common_include_for_headers.h>
#include <errno.h>
#include <process.h>

#ifndef EPFNOSUPPORT
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#endif
#ifndef EADDRINUSE
#define EADDRINUSE      100
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT    102
#endif
#ifndef ECONNRESET
#define ECONNRESET      108
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK     140
#endif

#ifndef getpid
#define getpid	_getpid
#endif

__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_sys_errno_h__ */
