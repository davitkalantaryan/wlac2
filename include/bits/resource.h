// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <bits/resource.h> For WINDOWS MFC
 *
 *	Created on: Aug 17, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_bits_resource_h___
#define __win_bits_resource_h___

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

#if !defined(__rlim_t_defined) & !defined(rlim_t_defined)
typedef unsigned __int64 rlim_t;
#define __rlim_t_defined
#define rlim_t_defined
#endif  // #if !defined(__rlim_t_defined) & !defined(rlim_t_defined)

struct rlimit
{
	/* The current (soft) limit.  */
	rlim_t rlim_cur;
	/* The hard limit.  */
	rlim_t rlim_max;
};

__BEGIN_C_DECLS


__END_C_DECLS

/* Kinds of resource limit.  */
enum __rlimit_resource
{
	/* Per-process CPU limit, in seconds.  */
	RLIMIT_CPU = 0,
#define RLIMIT_CPU RLIMIT_CPU

	/* Largest file that can be created, in bytes.  */
	RLIMIT_FSIZE = 1,
#define	RLIMIT_FSIZE RLIMIT_FSIZE

	/* Maximum size of data segment, in bytes.  */
	RLIMIT_DATA = 2,
#define	RLIMIT_DATA RLIMIT_DATA

	/* Maximum size of stack segment, in bytes.  */
	RLIMIT_STACK = 3,
#define	RLIMIT_STACK RLIMIT_STACK

	/* Largest core file that can be created, in bytes.  */
	RLIMIT_CORE = 4,
#define	RLIMIT_CORE RLIMIT_CORE

	/* Largest resident set size, in bytes.
	This affects swapping; processes that are exceeding their
	resident set size will be more likely to have physical memory
	taken from them.  */
	__RLIMIT_RSS = 5,
#define	RLIMIT_RSS __RLIMIT_RSS

	/* Number of open files.  */
	RLIMIT_NOFILE = 7,
	__RLIMIT_OFILE = RLIMIT_NOFILE, /* BSD name for same.  */
#define RLIMIT_NOFILE RLIMIT_NOFILE
#define RLIMIT_OFILE __RLIMIT_OFILE

	/* Address space limit.  */
	RLIMIT_AS = 9,
#define RLIMIT_AS RLIMIT_AS

	/* Number of processes.  */
	__RLIMIT_NPROC = 6,
#define RLIMIT_NPROC __RLIMIT_NPROC

	/* Locked-in-memory address space.  */
	__RLIMIT_MEMLOCK = 8,
#define RLIMIT_MEMLOCK __RLIMIT_MEMLOCK

	/* Maximum number of file locks.  */
	__RLIMIT_LOCKS = 10,
#define RLIMIT_LOCKS __RLIMIT_LOCKS

	/* Maximum number of pending signals.  */
	__RLIMIT_SIGPENDING = 11,
#define RLIMIT_SIGPENDING __RLIMIT_SIGPENDING

	/* Maximum bytes in POSIX message queues.  */
	__RLIMIT_MSGQUEUE = 12,
#define RLIMIT_MSGQUEUE __RLIMIT_MSGQUEUE

	/* Maximum nice priority allowed to raise to.
	Nice levels 19 .. -20 correspond to 0 .. 39
	values of this resource limit.  */
	__RLIMIT_NICE = 13,
#define RLIMIT_NICE __RLIMIT_NICE

	/* Maximum realtime priority allowed for non-priviledged
	processes.  */
	__RLIMIT_RTPRIO = 14,
#define RLIMIT_RTPRIO __RLIMIT_RTPRIO

	__RLIMIT_NLIMITS = 15,
	__RLIM_NLIMITS = __RLIMIT_NLIMITS
#define RLIMIT_NLIMITS __RLIMIT_NLIMITS
#define RLIM_NLIMITS __RLIM_NLIMITS
};

/* Structure which says how much of each resource has been used.  */
struct rusage
{
	/* Total amount of user time used.  */
	struct timeval ru_utime;
	/* Total amount of system time used.  */
	struct timeval ru_stime;
	/* Maximum resident set size (in kilobytes).  */
	long int ru_maxrss;
	/* Amount of sharing of text segment memory
	with other processes (kilobyte-seconds).  */
	long int ru_ixrss;
	/* Amount of data segment memory used (kilobyte-seconds).  */
	long int ru_idrss;
	/* Amount of stack memory used (kilobyte-seconds).  */
	long int ru_isrss;
	/* Number of soft page faults (i.e. those serviced by reclaiming
	a page from the list of pages awaiting reallocation.  */
	long int ru_minflt;
	/* Number of hard page faults (i.e. those that required I/O).  */
	long int ru_majflt;
	/* Number of times a process was swapped out of physical memory.  */
	long int ru_nswap;
	/* Number of input operations via the file system.  Note: This
	and `ru_oublock' do not include operations with the cache.  */
	long int ru_inblock;
	/* Number of output operations via the file system.  */
	long int ru_oublock;
	/* Number of IPC messages sent.  */
	long int ru_msgsnd;
	/* Number of IPC messages received.  */
	long int ru_msgrcv;
	/* Number of signals delivered.  */
	long int ru_nsignals;
	/* Number of voluntary context switches, i.e. because the process
	gave up the process before it had to (usually to wait for some
	resource to be available).  */
	long int ru_nvcsw;
	/* Number of involuntary context switches, i.e. a higher priority process
	became runnable or the current process used up its time slice.  */
	long int ru_nivcsw;
};



#endif  // #ifndef __win_bits_resource_h___
