// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <strings.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_signal_new_h__
#define __win_signal_new_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <signal.h>, <signal.h> )
#pragma include_alias( "signal.h", "signal.h" )
#include <signal.h>

#include <rfc/windows_signal.h>
#include <pthread.h>


#ifndef SIGIO
#define	SIGIO		29	/* I/O now possible (4.2 BSD).  */
#endif

/* Values for the HOW argument to `sigprocmask'.  */
#ifndef SIG_BLOCK
#define	SIG_BLOCK     0		 /* Block signals.  */
#endif
#ifndef SIG_UNBLOCK
#define	SIG_UNBLOCK   1		 /* Unblock signals.  */
#endif
#ifndef SIG_SETMASK
#define	SIG_SETMASK   2		 /* Set the set of blocked signals.  */
#endif

/* Type of a signal handler.  */
#if !defined(__sighandler_t_defined) && !defined(____sighandler_t_defined)
typedef void(*__sighandler_t) (int);
#define __sighandler_t_defined
#define ____sighandler_t_defined
#endif  // #if !defined(__sighandler_t_defined) && defined(____sighandler_t_defined)

/* A `sigset_t' has a bit for each signal.  */
# define _SIGSET_NWORDS	(1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
	unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;


//#if defined __need_sigset_t || (defined _SIGNAL_H && defined __USE_POSIX)
#if !defined(__sigset_t_defined) && !defined(sigset_t_defined)
typedef __sigset_t sigset_t;
#  define __sigset_t_defined
#define sigset_t_defined
#endif  // #if !defined(__sigset_t_defined) && !defined(sigset_t_defined)

#if !defined(__pid_t_defined) && !defined(pid_t_defined)
typedef unsigned int pid_t;
#define __pid_t_defined
#define pid_t_defined
#endif


__BEGIN_C_DECLS

/* Clear all signals from SET.  */
GEM_API_FAR int sigemptyset(sigset_t *__set) __THROW;
/* Add SIGNO to SET.  */
GEM_API_FAR int sigaddset(sigset_t *__set, int __signo) __THROW;
/* Get and/or set the action for signal SIG.  */
GEM_API_FAR int sigaction(int sig, const struct sigaction *RESTRICT action, struct sigaction *RESTRICT oldAction) __THROW;
GEM_API_FAR signal_handler_simple_type wlac_signal(int a_sig, signal_handler_simple_type a_handler);
GEM_API_FAR int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
GEM_API_FAR int pthread_kill(pthread_t thread, int sig);
GEM_API_FAR int sigtimedwait(const sigset_t * a_set, siginfo_t * a_info, const struct timespec * a_timeout);
GEM_API_FAR int sigwait(const sigset_t *set, int *sig);
GEM_API_FAR int sigfillset(sigset_t *set);

#define sigwaitinfo(_set,_info)	sigtimedwait((_set),(_info),NULL)
#define signal	wlac_signal

__END_C_DECLS

// should be defined after everything is known
#include <bits/sigaction.h>
#include <bits/signum.h>
#include <bits/siginfo.h>
#include <sdef_gem_windows.h>
#include <redesigned/sys/types.h>
#include <windows.h>
// from remote_function_call project
#include <rfc/windows_signal.h> 
#include <rfc/remote_function_caller.h>


#endif  /* #ifndef __win_signal_new_h__ */
