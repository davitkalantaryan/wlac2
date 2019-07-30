// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <arpa/inet.h> For WINDOWS MFC
 *
 *	Created on: Dec 16, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_arpa_inet_h__
#define __win_arpa_inet_h__

#include <first_includes/common_include_for_headers.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <win_socket_common.h>

__BEGIN_C_DECLS

/* Return network number part of the Internet address IN.  */
GEM_API_FAR in_addr_t inet_netof(struct in_addr __in) __THROW;

__END_C_DECLS

#endif  /* #ifndef __win_arpa_inet_h__ */
