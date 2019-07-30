// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <bits/ioctls.h> For WINDOWS MFC
 *
 *	Created on: Dec 12, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_bits_ioctls_h__
#define __win_bits_ioctls_h__

#include <first_includes/common_include_for_headers.h>

#ifndef __win_sys_ioctl_h__
#error "Never use <bits/ioctls.h> directly; include <sys/ioctl.h> instead."
#endif

#define		SIOCGIFCONF		0x8912		/* get iface list		*/
#define		SIOCGIFFLAGS	0x8913		/* get flags			*/

__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_bits_ioctls_h__ */
