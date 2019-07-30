// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

//
// File:		wlac  -> wlac_temporary.h
// Created:		2018 March 25
//


#ifndef __wlac_tmp_wlac_temporary_h__
#define __wlac_tmp_wlac_temporary_h__

#include <first_includes/common_include_for_headers.h>
#include <stddef.h>
#include <stdio.h>
#include <io.h>

#ifndef MS_SYNC		// https://linux.die.net/man/2/msync
#define MS_SYNC 0	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366563(v=vs.85).aspx
#endif

#ifndef F_GETFL
#define F_GETFL	0
#endif

#ifndef F_SETFL
#define F_SETFL	1
#endif

#ifndef O_NONBLOCK
#define O_NONBLOCK FIONBIO
#endif

#if !defined(S_ISLNK) & !defined(S_ISLNK_defined)
#define S_ISLNK_defined
#define S_ISLNK(...)  (0)
#endif  // #if !defined(S_ISLNK) & !defined(S_ISLNK_defined)

#ifndef ssize_t_defined
#define ssize_t_defined
typedef size_t	ssize_t;
#endif

#ifndef fread_unlocked_defined
#define fread_unlocked_defined
#ifndef fread_unlocked
#define fread_unlocked	fread
#endif
#endif

#ifndef ftruncate_defined
#define ftruncate_defined
#define ftruncate(_fd,_size)	_chsize((_fd),(_size))
#endif

#ifndef fdatasync_defined
#define fdatasync_defined
#ifndef fdatasync
#define fdatasync(_fd)		(FlushFileBuffers((HANDLE)_get_osfhandle((_fd)))?0:-1)
#endif
#endif

#ifndef msync_defined
#define msync_defined
#define msync(_baseAddress,_numOfBytes,_flag)  (FlushViewOfFile((_baseAddress),(_numOfBytes))?0:-1)
#endif

#ifndef EPOLL_CTL_MOD
#define EPOLL_CTL_MOD	1
#endif

#ifndef EPOLL_CTL_ADD
#define EPOLL_CTL_ADD	2
#endif

#ifndef EPOLL_CTL_DEL
#define EPOLL_CTL_DEL	3
#endif


__BEGIN_C_DECLS

GEM_API_FAR pid_t fork(void);
GEM_API_FAR pid_t setsid(void);

GEM_API_FAR int epoll_create(int size);
GEM_API_FAR int epoll_create1(int flags);
GEM_API_FAR int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

GEM_API_FAR int epoll_wait(int epfd, struct epoll_event *events,
	int maxevents, int timeout);

__END_C_DECLS

#endif  // #ifndef __wlac_tmp_wlac_temporary_h__
