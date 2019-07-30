// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <poll.h> For WINDOWS MFC
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_poll_h__
#define __win_poll_h__

#include <first_includes/common_include_for_headers.h>
#include <win_socket_common.h>
#include <sys/ioctl.h>

/* Type used for the number of file descriptors.  */
typedef unsigned long int nfds_t;

__BEGIN_C_DECLS

GEM_API_FAR int poll_sockets(struct pollfd *fds, nfds_t nfds, int timeout);
GEM_API_FAR int wlac_poll(struct pollfd *fds, nfds_t nfds, int timeout);

__END_C_DECLS

#if !defined(_IGNORE_WLAC_POLL) && !defined(poll)
#define  poll wlac_poll
#endif  // #if !defined(_IGNORE_WLAC_POLL) && !defined(poll)

#endif  /* #ifndef __win_poll_h__ */
