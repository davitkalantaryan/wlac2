//gem_networking.cpp
/*
 *	File: gem_networking.cpp
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include "win_socket_common.h"
#include <sys/time.h>
#include <io.h>
#include <malloc.h>
#include <errno.h>
#include <sys/errno.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <poll.h>

#ifdef _MSC_VER
#if(_MSC_VER >= 1400)
#pragma warning(disable : 4996)
#endif
#endif

__BEGIN_C_DECLS


/// Testing TINE implementation
#if 1

GEM_API int poll_sockets(struct pollfd *fds, nfds_t nfds, int timeout)
{
	fd_set rdfds, wrfds, erfds;
	fd_set *prdfds=NULL, *pwrfds=NULL, *perfds=NULL;
	struct timeval to;
	nfds_t i, nready, fdmax = 0;

	FD_ZERO(&rdfds); FD_ZERO(&wrfds); FD_ZERO(&erfds);

	for (i=0; i<nfds; i++)
	{
		if (((nfds_t)fds[i].fd) > fdmax) fdmax = (nfds_t)fds[i].fd;
		if (fds[i].events & POLLIN) { FD_SET(fds[i].fd,&rdfds); prdfds = &rdfds; }
		if (fds[i].events & POLLOUT) { FD_SET(fds[i].fd,&wrfds); pwrfds = &wrfds; }
		FD_SET(fds[i].fd,&erfds); 
		perfds = &erfds;
		fds[i].revents = 0;
	}
	to.tv_sec = timeout/1000; to.tv_usec = (timeout%1000)*1000;
	nready = select(fdmax+1,prdfds,pwrfds,perfds,&to);
	if (nready > 0)
	{
		for (i=0; i<nfds; i++)
		{
			if (prdfds != NULL && fds[i].events & POLLIN && FD_ISSET(fds[i].fd,prdfds))
			{
				fds[i].revents |= POLLIN;
			}
			if (pwrfds != NULL && fds[i].events & POLLOUT && FD_ISSET(fds[i].fd,pwrfds))
			{
				fds[i].revents |= POLLOUT;
			}
			if (perfds != NULL && FD_ISSET(fds[i].fd,perfds))
			{
				fds[i].revents |= POLLERR;
			}
		}
	}
	return (int)nready;
}

#else  // #if 1

GEM_API int poll_sockets(struct pollfd *a_fds, nfds_t a_nfds, int a_timeout)
{
#ifdef WSA_POLL_IMPLEMENTED
	
	int nReturn = WSAPoll(a_fds, a_nfds, a_timeout);
	if (nReturn  == SOCKET_ERROR)
	{
		return -1;
	}

	return nReturn;

#else  // #ifdef WSA_POLL_IMPLEMENTED

	//pollfds.events = POLLIN | POLLOUT | POLLERR;
	fd_set rfdsIn, rfdsOut, rfdsErr;
	int maxsd = 0;
	int rtn = 0;
	nfds_t i = 1;
	struct timeval		aTimeout;
	struct timeval*		pTimeout;
	int nReturn = 0;
	int nDoPlus;

	if (!a_nfds) return 0;

	maxsd = a_fds[0].fd;
	FD_ZERO(&rfdsIn);
	FD_ZERO(&rfdsOut);
	FD_ZERO(&rfdsErr);
	FD_SET((unsigned int)a_fds[0].fd, &rfdsIn);
	FD_SET((unsigned int)a_fds[0].fd, &rfdsOut);
	FD_SET((unsigned int)a_fds[0].fd, &rfdsErr);
	a_fds[0].revents = 0;
	for (; i < a_nfds; ++i)
	{
		FD_SET((unsigned int)a_fds[i].fd, &rfdsIn);
		FD_SET((unsigned int)a_fds[i].fd, &rfdsOut);
		FD_SET((unsigned int)a_fds[i].fd, &rfdsErr);
		a_fds[i].revents = 0;
		if (a_fds[i].fd > maxsd){ maxsd = a_fds[i].fd; }
	}

	if (a_timeout >= 0)
	{
		aTimeout.tv_sec = a_timeout / 1000L;
		aTimeout.tv_usec = (a_timeout % 1000L) * 1000L;
		pTimeout = &aTimeout;
	}
	else
	{
		pTimeout = NULL;
	}

	rtn = select(++maxsd, &rfdsIn, &rfdsOut, &rfdsErr, pTimeout);

	switch (rtn)
	{
	case 0:	/* time out */
		return 0;
#ifdef	WIN32
	case SOCKET_ERROR:
#else
	case -1:
#endif
		if (errno == EINTR)
		{
			/* interrupted by signal */
			return 2;
		}

		return(E_SELECT);
	default:
		break;
		//return(E_FATAL);
	}

	for (i=0; i < a_nfds; ++i)
	{
		nDoPlus = 0;
		//pollfds.events = POLLIN | POLLOUT | POLLERR;
		if ((a_fds[i].events & POLLIN) && FD_ISSET(a_fds[i].fd, &rfdsIn))
		{
			a_fds[i].revents |= POLLIN;
			nDoPlus = 1;
		}
		if ((a_fds[i].events & POLLOUT) && FD_ISSET(a_fds[i].fd, &rfdsOut))
		{
			a_fds[i].revents |= POLLOUT;
			nDoPlus = 1;
		}
		if ((a_fds[i].events & POLLERR) && FD_ISSET(a_fds[i].fd, &rfdsErr))
		{
			a_fds[i].revents |= POLLERR;
			nDoPlus = 1;
		}
		if (nDoPlus){ nReturn++; }
	}

	return nReturn;

#endif  // #ifdef WSA_POLL_IMPLEMENTED
}

#endif  // #if 1


GEM_API int gethostbyname_r(const char *name, struct hostent *ret, char *buf,
	unsigned int buflen, struct hostent **result, int *h_errnop)
{
	int                     hsave;
	struct hostent         *ph;
	static pthread_mutex_t __mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&__mutex); /* begin critical area */
	hsave = h_errno;

	ph = gethostbyname(name);
	//getaddrinfo()

	*h_errnop = h_errno; /* copy h_errno to *h_herrnop */

	if (ph == NULL) *result = NULL;
	else {

		char    **p;
		char    **q;
		char    *pbuf;
		int     nbytes = 0;
		int     naddr = 0;
		int     naliases = 0;

		/* determine if we have enough space in buf */

		/* count how many addresses */

		for (p = ph->h_addr_list; *p != 0; p++) {

			nbytes += ph->h_length; /* addresses */
			nbytes += sizeof(*p);  /* pointers */
			naddr++;
		}
		nbytes += sizeof(*p); /* one more for the terminating NULL */

		/* count how many aliases, and total length of strings */

		for (p = ph->h_aliases; *p != 0; p++) {

			nbytes += (int)(strlen(*p) + 1); /* aliases */
			nbytes += sizeof(*p);       /* pointers */
			naliases++;
		}
		nbytes += sizeof(*p); /* one more for the terminating NULL */

		/* here nbytes is the number of bytes required in buffer */
		/* as a terminator must be there, the minimum value is ph->h_length */

		if ((unsigned int)nbytes > buflen) {

			*result = NULL;
			pthread_mutex_unlock(&__mutex); /* end critical area */
			return ERANGE; /* not enough space in buf!! */
		}

		/* There is enough space. Now we need to do a deep copy! */

		/* Allocation in buffer:
		from [0] to [(naddr - 1) * sizeof (*p)]:
		pointers to addresses
		at [naddr * sizeof (*p)]:
		NULL
		from [(naddr+1) * sizeof(*p)] to [(naddr+naliases) * sizeof(*p)] :
		pointers to aliases
		at [(naddr+naliases+1) * sizeof(*p)]:
		NULL
		then naddr addresses (fixed length), and naliases aliases (asciiz).
		*/

		*ret = *ph;   /* copy whole structure (not its address!) */

		/* copy addresses */

		q = (char **)buf; /* pointer to pointers area (type: char **) */
		ret->h_addr_list = q; /* update pointer to address list */
		pbuf = buf + ((naddr + naliases + 2) * sizeof(*p)); /* skip that area */

		for (p = ph->h_addr_list; *p != 0; p++) {

			memcpy(pbuf, *p, ph->h_length); /* copy address bytes */
			*q++ = pbuf;                     /* the pointer is the one inside buf... */
			pbuf += ph->h_length;            /* advance pbuf */
		}
		*q++ = NULL; /* address list terminator */

		/* copy aliases */

		ret->h_aliases = q; /* update pointer to aliases list */
		for (p = ph->h_aliases; *p != 0; p++) {

			strcpy(pbuf, *p);   /* copy alias strings */
			*q++ = pbuf;         /* the pointer is the one inside buf... */
			pbuf += strlen(*p); /* advance pbuf */
			*pbuf++ = 0;         /* string terminator */
		}
		*q++ = NULL; /* terminator */

		strcpy(pbuf, ph->h_name);  /* copy alias strings */
		ret->h_name = pbuf;
		pbuf += strlen(ph->h_name); /* advance pbuf */
		*pbuf++ = 0;                /* string terminator */

		*result = ret;  /* and let *result point to structure */

	}
	h_errno = hsave;  /* restore h_errno */

	pthread_mutex_unlock(&__mutex); /* end critical area */
	return *result == NULL ? -1 : 0;
}



GEM_API int getprotobyname_r(const char *name, struct protoent * ret,
	char * buf, unsigned int __buflen, struct protoent ** result)
{
	//int                 hsave;///???
	struct protoent     *ph;

	static pthread_mutex_t __mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&__mutex); /* begin critical area */

	ph = getprotobyname(name);
	if (ph == NULL)
	{
		*result = NULL;
	}
	else{

		char    **p;
		char    **q;
		char    *pbuf;
		int     nbytes = 0;
		int     naddr = 0;
		int     naliases = 0;

		/* determine if we have enough space in buf */

		/* count how many addresses */

		////////////////////
		/* count how many aliases, and total length of strings */

		for (p = ph->p_aliases; *p != 0; p++) {

			nbytes += (int)(strlen(*p) + 1); /* aliases */
			nbytes += sizeof(*p);       /* pointers */
			naliases++;
		}
		//        nbytes += sizeof (*p); /* one more for the terminating NULL */

		/* here nbytes is the number of bytes required in buffer */
		/* as a terminator must be there, the minimum value is ph->h_length */

		if ((unsigned)nbytes > __buflen) {

			*result = NULL;
			pthread_mutex_unlock(&__mutex); /* end critical area */
			return ERANGE; /* not enough space in buf!! */
		}
		*ret = *ph;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		q = (char **)buf; /* pointer to pointers area (type: char **) */
		// ret->h_addr_list = q; /* update pointer to address list */
		pbuf = buf + ((naddr + naliases + 2) * sizeof(*p)); /* skip that area */

		//       for (p = ph->h_addr_list; *p != 0; p++) {

		//            memcpy (pbuf, *p, ph->h_length); /* copy address bytes */
		//           *q++ = pbuf;                     /* the pointer is the one inside buf... */
		//            pbuf += ph->h_length;            /* advance pbuf */
		//       }
		*q++ = NULL; /* address list terminator */

		/* copy aliases */

		ret->p_aliases = q; /* update pointer to aliases list */
		for (p = ph->p_aliases; *p != 0; p++) {

			strcpy(pbuf, *p);   /* copy alias strings */
			*q++ = pbuf;         /* the pointer is the one inside buf... */
			pbuf += strlen(*p); /* advance pbuf */
			*pbuf++ = 0;         /* string terminator */
		}

		*q++ = NULL; /* terminator */

		strcpy(pbuf, ph->p_name);  /* copy alias strings */
		ret->p_name = pbuf;
		pbuf += strlen(ph->p_name); /* advance pbuf */
		*pbuf++ = 0;                /* string terminator */

		*result = ret;  /* and let *result point to structure */

	}
	//h_errno = hsave;  /* restore h_errno */

	pthread_mutex_unlock(&__mutex); /* end critical area */
	return *result == NULL;
}


GEM_API BOOL initialize_windows_socket_library(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return FALSE;
	}

	/* Confirm that the WinSock DLL supports 2.2.		*/
	/* Note that if the DLL supports versions greater	*/
	/* than 2.2 in addition to 2.2, it will still return*/
	/* 2.2 in wVersion since that is the version we		*/
	/* requested.										*/

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return FALSE;
	}

	return TRUE; 
}



/*
* Bind a socket to a privileged IP port
*/
GEM_API
int
bindresvport(int sd, struct sockaddr_in * sin)
{
#ifdef USE_DAVITS_IMPLEMENTATION
	int res;
	static short port;
	struct sockaddr_in myaddr;
#ifndef errno
	extern int errno;
#endif
	int i;

#define STARTPORT 600
#define ENDPORT (IPPORT_RESERVED - 1)
#define NPORTS	(ENDPORT - STARTPORT + 1)

	if (sin == (struct sockaddr_in *)0) {
		sin = &myaddr;
		bzero(sin, sizeof(*sin));
		sin->sin_family = AF_INET;
	}
	else if (sin->sin_family != AF_INET) {
		errno = EPFNOSUPPORT;
		return (-1);
	}
	if (port == 0) {
		port = (getpid() % NPORTS) + STARTPORT;
	}
	res = -1;
	errno = EADDRINUSE;
	for (i = 0; i < NPORTS && res < 0 && errno == EADDRINUSE; i++) {
		sin->sin_port = htons(port++);
		if (port > ENDPORT) {
			port = STARTPORT;
		}
		res = bind(sd, (struct sockaddr *)sin, sizeof(struct sockaddr_in));
	}
	return (res);
#else  // #ifdef USE_DAVITS_IMPLEMENTATION

	int res;
	static short port;
	struct sockaddr_in myaddr;
	//int my_errno;
	int i;

#define STARTPORT 600
#define ENDPORT (IPPORT_RESERVED - 1)
#define NPORTS	(ENDPORT - STARTPORT + 1)

	if (sin == (struct sockaddr_in *)0) {
		sin = &myaddr;
		bzero(sin, sizeof (*sin));
		sin->sin_family = AF_INET;
	} else if (sin->sin_family != AF_INET) {
		errno = WSAEPFNOSUPPORT;
		return (-1);
	}
	if (port == 0) {
		port = (getpid() % NPORTS) + STARTPORT;
	}
	res = -1;
	errno = WSAEADDRINUSE;
	for (i = 0; i < NPORTS && res < 0 && errno == WSAEADDRINUSE; i++) {
		sin->sin_port = htons(port++);
		if (port > ENDPORT) {
			port = STARTPORT;
		}
		res = bind(sd, (struct sockaddr*)sin, sizeof(struct sockaddr_in));
		errno = WSAGetLastError();
	}
	return (res);

#endif // #ifdef USE_DAVITS_IMPLEMENTATION
}


__END_C_DECLS
