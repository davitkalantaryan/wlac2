/*
 *	File: unix_emul_source_half_implement_c.c
 *
 *	Created on: Dec 18, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <net/if.h>
#include <sys/time.h>
#include <io.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/uio.h>
#include <stdio.h>  
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <redesigned/signal.h>

#ifdef _MSC_VER
#if(_MSC_VER >= 1400)
#pragma warning(disable : 4996)
#endif
#endif

#define CONS_POS_SIZE_MIN1	511
static const int s_nCONS_POS_SIZE=CONS_POS_SIZE_MIN1+1;

__BEGIN_C_DECLS

#ifdef getsockopt
#undef getsockopt
#endif
GEM_API int wlac_getsockopt(int sockfd, int level, int optname, void *optval,socklen_t *optlen)
{
	return getsockopt((SOCKET)sockfd,level, optname,(char*)optval, optlen);
}


#ifdef setsockopt
#undef setsockopt
#endif
GEM_API int wlac_setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen)
{
	return setsockopt((SOCKET)sockfd, level, optname, (const char*)optval, optlen);
}


GEM_API int pread(int a_fd, void *a_buf, size_t a_count, int a_offset)
{
	if (_lseek(a_fd, a_offset, SEEK_SET) != a_offset) {
		return -1;
	}
	return _read(a_fd, a_buf, (unsigned int)a_count);
}


GEM_API int pwrite(int a_fd, const void* a_buf, size_t a_count, int a_offset)
{
	if (_lseek(a_fd, a_offset, SEEK_SET) != a_offset) {
		return -1;
	}
	return _write(a_fd, a_buf, (unsigned int)a_count);
}


GEM_API int sigaddset(sigset_t *__set, int __signo) __THROW
{
	if (__set && (__signo<_SIGSET_NWORDS)){
		__set->__val[__signo] = 1;
	}
	return 0;
}

GEM_API int sigtimedwait(const sigset_t * a_set, siginfo_t * a_info, const struct timespec * a_timeout)
{
	int nWait = a_timeout?( ((int)(a_timeout->tv_sec * 1000)) + ((int)(a_timeout->tv_nsec / 1000000)) ) : INFINITE;
	int nSignal=wait_for_signal_synchron(a_info, nWait);
	if(nSignal){
		if((nSignal<_SIGSET_NWORDS)&&a_set->__val[nSignal]){
			return nSignal;
		}
	}

	return -1;
}


GEM_API int sigwait(const sigset_t * a_set, int *a_sig)
{
	int nSignal = sigtimedwait(a_set, NULL, NULL);
	if (a_sig) {*a_sig=nSignal;}
	if(nSignal>=0){
		return 0;
	}
	return -1; // to be done 
}

GEM_VAR int optind = 1;
GEM_VAR int opterr, optopt;
GEM_VAR	char* optarg;


GEM_API int getopt(int a_argc, char * const a_argv2[], const char *a_optstring)
{
	int strLenArgV;
	int nOptionStringLength = (int)strlen(a_optstring);
	const char* cpcOptionString;
	char* pcArgV;

	for (; optind < a_argc; ++optind)
	{
		pcArgV = a_argv2[optind];
		strLenArgV = (int)strlen(pcArgV);
		if ((strLenArgV > 1) && (pcArgV[0] == '-') && (pcArgV[1] != '-') && (pcArgV[1] != ' '))
		{
			cpcOptionString = strchr(a_optstring, pcArgV[1]);
			if (cpcOptionString != NULL)
			{
				if ((((int)(cpcOptionString - a_optstring)) < nOptionStringLength) && (*(cpcOptionString + 1) == ':'))
				{
					if (strLenArgV > 2)
					{
						optarg = a_argv2[optind] + 2;
					}
					else if ( (optind + 1)<a_argc )
					{
						optarg = a_argv2[++optind];
					}
					else
					{
						return -1;
					}
				} // if ((((int)(cpcOptionString - a_optstring)) < nStrLenOptionString) && (*(cpcOptionString + 1) == ':'))
				++optind;
				return pcArgV[1];
			} // if (cpcOptionString != NULL)
		} // if ((strLenArgV > 1) && (a_argv[0] == '-') && (a_argv[1] != '-') && (a_argv[1] != ' '))
	} // for (; optind < a_argc; ++optind)

	return -1;
}

GEM_API in_addr_t inet_netof(struct in_addr a_in) __THROW
{
	const char* cpcIP = inet_ntoa(a_in);
	return inet_addr(cpcIP);
}


/* Return the position of the first bit set in I, or 0 if none are set.
The least-significant bit is position 1, the most-significant 32.  */
//extern int ffs(int __i) __THROW __attribute__((__const__));
GEM_API int ffs(int a_i) __THROW
{
	int nReturn = 1;
	unsigned int u_i = (unsigned int)a_i;

	for (; u_i; u_i >>= 1, ++nReturn)
	{
		if (u_i & 1)
		{
			return nReturn;
		}
	}

	return 0;
}


GEM_API key_t ftok(const char *pathname, int proj_id)
{
	key_t tReturn=0;
	if (!pathname) return 0;
	while (pathname)
	{
		tReturn += ((key_t)((size_t)(pathname++)));
	}

	return tReturn + proj_id;
}


GEM_API ssize_t writev(int a_fd, __const struct iovec *a_iovec, int a_count)
{
	typedef void* type_void_ptr;
	int nRet,i=0,nReturnAll = 0,nSize=(int)a_iovec->iov_len;
	type_void_ptr* pBuffers = (type_void_ptr*)a_iovec->iov_base;

	for (; i < nSize; ++i)
	{
		nRet = write(a_fd, pBuffers[i], a_count);
		if (nRet < 0){return nRet;}
		nReturnAll += nRet;
	}

	return nReturnAll;
}

__END_C_DECLS
