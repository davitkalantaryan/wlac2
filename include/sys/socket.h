// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/socket.h> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_socket_h__
#define __win_sys_socket_h__

#include <first_includes/common_include_for_headers.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <win_socket_common.h>
#include <redesigned/signal.h>

__BEGIN_C_DECLS 

GEM_API_FAR struct in_addr inet_makeaddr(unsigned long net,
	unsigned long lna);
GEM_API_FAR int wlac_getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
#define getsockopt	wlac_getsockopt
GEM_API_FAR int wlac_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
#define setsockopt	wlac_setsockopt

__END_C_DECLS

#endif  /* #ifndef __win_sys_socket_h__ */
