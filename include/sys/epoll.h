// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// file:		wlac sys/epoll.h
// created on:	2018 March 23
// to include:	#include <sys/epoll.h>

#ifndef __wlac_sys_epoll_h__
#define __wlac_sys_epoll_h__

#include <first_includes/common_include_for_headers.h>
#include <poll.h>
#include <stdint.h>

#define EPOLLIN		POLLIN
#define EPOLLOUT	POLLOUT
#define EPOLLERR	POLLERR
#define EPOLLHUP	POLLHUP

typedef union epoll_data {
	void    *ptr;
	int      fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event {
	uint32_t     events;    /* Epoll events */
	epoll_data_t data;      /* User data variable */
};


__BEGIN_C_DECLS


__END_C_DECLS


#endif  // #ifndef __wlac_sys_epoll_h__
