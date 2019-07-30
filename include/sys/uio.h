// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/uio.h> For WINDOWS MFC
 *
 *	Created on: Jun 1, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef _SYS_UIO_H
#define _SYS_UIO_H	1

#include <first_includes/common_include_for_headers.h>
#include <features.h>
#include <sys/types.h>

__BEGIN_C_DECLS

/* This file defines `struct iovec'.  */
#include <bits/uio.h>

#if !defined(IOV_MAX) & !defined(IOV_MAX_defined)
#define IOV_MAX	1024
#define IOV_MAX_defined
#endif  // #if !defined(IOV_MAX) & !defined(IOV_MAX_defined)

#ifndef ssize_t_defined
typedef int ssize_t;
#define ssize_t_defined
#endif


/* Read data from file descriptor FD, and put the result in the
buffers described by IOVEC, which is a vector of COUNT 'struct iovec's.
The buffers are filled in the order specified.
Operates just like 'read' (see <unistd.h>) except that data are
put in IOVEC instead of a contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
///extern ssize_t readv(int __fd, __const struct iovec *__iovec, int __count);

/* Write data pointed by the buffers described by IOVEC, which
is a vector of COUNT 'struct iovec's, to file descriptor FD.
The data is written in the order specified.
Operates just like 'write' (see <unistd.h>) except that the data
are taken from IOVEC instead of a contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
GEM_API_FAR extern ssize_t writev(int __fd, __const struct iovec *__iovec, int __count);


#ifdef __USE_BSD
# ifndef __USE_FILE_OFFSET64
/* Read data from file descriptor FD at the given position OFFSET
without change the file pointer, and put the result in the buffers
described by IOVEC, which is a vector of COUNT 'struct iovec's.
The buffers are filled in the order specified.  Operates just like
'pread' (see <unistd.h>) except that data are put in IOVEC instead
of a contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
extern ssize_t preadv(int __fd, __const struct iovec *__iovec, int __count,
	__off_t __offset) __wur;

/* Write data pointed by the buffers described by IOVEC, which is a
vector of COUNT 'struct iovec's, to file descriptor FD at the given
position OFFSET without change the file pointer.  The data is
written in the order specified.  Operates just like 'pwrite' (see
<unistd.h>) except that the data are taken from IOVEC instead of a
contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
extern ssize_t pwritev(int __fd, __const struct iovec *__iovec, int __count,
	__off_t __offset) __wur;
# else
#  ifdef __REDIRECT
extern ssize_t __REDIRECT(preadv, (int __fd, __const struct iovec *__iovec,
	int __count, __off64_t __offset),
	preadv64) __wur;
extern ssize_t __REDIRECT(pwritev, (int __fd, __const struct iovec *__iovec,
	int __count, __off64_t __offset),
	pwritev64) __wur;
#  else
#   define preadv preadv64
#   define pwritev pwritev64
#  endif
# endif

# ifdef __USE_LARGEFILE64
/* Read data from file descriptor FD at the given position OFFSET
without change the file pointer, and put the result in the buffers
described by IOVEC, which is a vector of COUNT 'struct iovec's.
The buffers are filled in the order specified.  Operates just like
'pread' (see <unistd.h>) except that data are put in IOVEC instead
of a contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
extern ssize_t preadv64(int __fd, __const struct iovec *__iovec, int __count,
	__off64_t __offset) __wur;

/* Write data pointed by the buffers described by IOVEC, which is a
vector of COUNT 'struct iovec's, to file descriptor FD at the given
position OFFSET without change the file pointer.  The data is
written in the order specified.  Operates just like 'pwrite' (see
<unistd.h>) except that the data are taken from IOVEC instead of a
contiguous buffer.

This function is a cancellation point and therefore not marked with
__THROW.  */
extern ssize_t pwritev64(int __fd, __const struct iovec *__iovec, int __count,
	__off64_t __offset) __wur;
# endif
#endif	/* Use BSD */

__END_C_DECLS

#endif /* sys/uio.h */
