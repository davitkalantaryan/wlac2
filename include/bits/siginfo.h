// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <bits/siginfo.h> For WINDOWS MFC
 *
 *	Created on: Aug 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_bits_siginfo_h___
#define __win_bits_siginfo_h___

#include <first_includes/common_include_for_headers.h>
#include <time.h>

#ifdef _WIN64
#ifdef __WORDSIZE
#undef __WORDSIZE
#endif
#define __WORDSIZE 64
#endif  // #ifdef _WIN64

# define __SI_MAX_SIZE     128
# if __WORDSIZE == 64
#ifndef __SI_PAD_SIZE
#  define __SI_PAD_SIZE     ((__SI_MAX_SIZE / sizeof (int)) - 4)
#endif
# else
#ifndef __SI_PAD_SIZE
#  define __SI_PAD_SIZE     ((__SI_MAX_SIZE / sizeof (int)) - 3)
#endif
# endif

#if !defined(__have_sigval_t) && !defined(sigval_defined) && !defined(sigval_t_defined)
# define __have_sigval_t	1
#define sigval_defined
#define sigval_t_defined
/* Type for data associated with a signal.  */
typedef union sigval
{
	int sival_int;
	void *sival_ptr;
} sigval_t;
#endif  //  #if !defined(__have_sigval_t) && !defined(sigval_defined) && !defined(sigval_t_defined)

#if !defined(siginfo_defined) && !defined(siginfo_t_defined)
#define siginfo_defined
#define siginfo_t_defined

#if !defined(pid_t_defined)
#define pid_t_defined
typedef unsigned int pid_t;
#endif

#if !defined(uid_t_defined)
#define uid_t_defined
typedef unsigned int uid_t;
#endif


typedef struct siginfo
{
	int si_signo;		/* Signal number.  */
	int si_errno;		/* If non-zero, an errno value associated with
						this signal, as defined in <errno.h>.  */
	int si_code;		/* Signal code.  */

	union
	{
		int _pad[__SI_PAD_SIZE];

		/* kill().  */
		struct
		{
			pid_t si_pid;	/* Sending process ID.  */
			uid_t si_uid;	/* Real user ID of sending process.  */
		} _kill;

		/* POSIX.1b timers.  */
		struct
		{
			int si_tid;		/* Timer ID.  */
			int si_overrun;	/* Overrun count.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _timer;

		/* POSIX.1b signals.  */
		struct
		{
			pid_t si_pid;	/* Sending process ID.  */
			uid_t si_uid;	/* Real user ID of sending process.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _rt;

		/* SIGCHLD.  */
		struct
		{
			pid_t si_pid;	/* Which child.  */
			uid_t si_uid;	/* Real user ID of sending process.  */
			int si_status;	/* Exit value or signal.  */
			clock_t si_utime;
			clock_t si_stime;
		} _sigchld;

		/* SIGILL, SIGFPE, SIGSEGV, SIGBUS.  */
		struct
		{
			void *si_addr;	/* Faulting insn/memory ref.  */
		} _sigfault;

		/* SIGPOLL.  */
		struct
		{
			long int si_band;	/* Band event for SIGPOLL.  */
			int si_fd;
		} _sigpoll;
	} _sifields;
} siginfo_t;

#endif  // #if !defined(siginfo_defined) && !defined(siginfo_t_defined)


/* X/Open requires some more fields with fixed names.  */
# define si_pid		_sifields._kill.si_pid
# define si_uid		_sifields._kill.si_uid
# define si_timerid	_sifields._timer.si_tid
# define si_overrun	_sifields._timer.si_overrun
# define si_status	_sifields._sigchld.si_status
# define si_utime	_sifields._sigchld.si_utime
# define si_stime	_sifields._sigchld.si_stime
# define si_value	_sifields._rt.si_sigval
# define si_int		_sifields._rt.si_sigval.sival_int
# define si_ptr		_sifields._rt.si_sigval.sival_ptr
# define si_addr	_sifields._sigfault.si_addr
# define si_band	_sifields._sigpoll.si_band
# define si_fd		_sifields._sigpoll.si_fd


__BEGIN_C_DECLS

__END_C_DECLS


#endif  // #ifndef __win_bits_siginfo_h___
