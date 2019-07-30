// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <netinet/in.h> For WINDOWS MFC
 *
 *	Created on: Dec 10, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_netinet_in_h__
#define __win_netinet_in_h__

#include <first_includes/common_include_for_headers.h>
#include <win_socket_common.h>
#include <memory.h>


#ifndef bool_t
#define	bool_t	int
#endif
#ifndef enum_t
#define	enum_t	int
#endif
#ifndef FALSE
#define	FALSE	(0)
#endif
#ifndef TRUE
#define	TRUE	(1)
#endif
#ifndef __dontcare__
#define __dontcare__	-1
#endif
#ifndef NULL
#	define NULL 0
#endif

#include <stdlib.h>		/* For malloc decl.  */
#ifndef mem_alloc
#define mem_alloc(bsize)	malloc(bsize)
#endif
#ifndef mem_free
#define mem_free(ptr, bsize)	do{free(ptr);ptr=NULL;}while(0)
#endif

#ifndef makedev /* ie, we haven't already included it */
#include <sys/types.h>
#endif
#include <sys/time.h>
#include <sys/param.h>

//#include <netinet/in.h>

#if !defined(INADDR_LOOPBACK) & !defined(_WS2DEF_)
#define       INADDR_LOOPBACK         (u_long)0x7F000001
#endif
#ifndef MAXHOSTNAMELEN
#define        MAXHOSTNAMELEN  64
#endif

__BEGIN_C_DECLS

GEM_API_FAR int bindresvport(int sockfd, struct sockaddr_in *sin);
GEM_API_FAR BOOL initialize_windows_socket_library(void);

__END_C_DECLS

#endif  /* #ifndef __win_netinet_in_h__ */
