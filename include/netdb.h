// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <netdb.h> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_netdb_h__
#define __win_netdb_h__

#include <first_includes/common_include_for_headers.h>
#include <win_socket_common.h>
#include <sys/cdefs.h>
#include <malloc.h>
#include <process.h>
#include <unistd.h>

#ifndef alloca
#define alloca	_alloca
#endif

#ifndef getpid
#define getpid _getpid
#endif

//#if defined __USE_MISC || defined __USE_GNU
# define NETDB_INTERNAL	-1	/* See errno.  */
# define NETDB_SUCCESS	0	/* No problem.  */
//# define NO_ADDRESS	NO_DATA	/* No address, look for MX record.  */
//#endif

__BEGIN_C_DECLS

struct rpcent {
	char    *r_name;        /* name of server for this rpc program */
	char    **r_aliases;    /* alias list */
	int     r_number;       /* rpc program number */
};
struct rpcent *getrpcbyname(), *getrpcbynumber(), *getrpcent();

GEM_API_FAR int gethostbyname_r(const char *name,
				struct hostent *ret, char *buf, size_t buflen,
				struct hostent **result, int *h_errnop);
GEM_API_FAR int getprotobyname_r(const char *name,
				struct protoent *result_buf, char *buf,
					size_t buflen, struct protoent **result);

__END_C_DECLS

#endif  /* #ifndef __win_netdb_h__ */
