// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/un.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * 
 */
#ifndef __win_sys_un_h__
#define __win_sys_un_h__

#include "first_includes/common_include_for_headers.h"
#include <sdef_gem_windows.h>

/* POSIX.1g specifies this type name for the `sa_family' member.  */
#ifndef sa_family_t_defined
typedef unsigned short int sa_family_t;
#define sa_family_t_defined
#endif

#ifndef __SOCKADDR_COMMON
#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family
#endif

__BEGIN_C_DECLS

/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
#ifndef sockaddr_un_defined
struct sockaddr_un
{
	__SOCKADDR_COMMON(sun_);
	char sun_path[108];		/* Path name.  */
};
#define sockaddr_un_defined
#endif


__END_C_DECLS

#endif  /* #ifndef __win_sys_un_h__ */
