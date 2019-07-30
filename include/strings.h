// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <strings.h> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#include <first_includes/common_include_for_headers.h>
#include <sys/cdefs.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>

__BEGIN_C_DECLS

#ifndef bcopy
#define bcopy(__a_src__,__a_dst__,__a_n__)	memmove((__a_dst__),(__a_src__),(__a_n__))
#define	bzero(__a_s__,__a_size__)			memset((__a_s__),0,(__a_size__))
#define bcmp	memcmp
#endif
#ifndef strtok_r
#define	strtok_r	strtok_s
#endif

/* Return the position of the first bit set in I, or 0 if none are set.
The least-significant bit is position 1, the most-significant 32.  */
//extern int ffs(int __i) __THROW __attribute__((__const__));
GEM_API_FAR int ffs(int __i) __THROW;

GEM_API_FAR int strncasecmp(const char *s1, const char *s2, size_t n);
GEM_API_FAR int strcasecmp(const char *s1, const char *s2);

__END_C_DECLS
