// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: windows_signal.h For WINDOWS
 *
 *	Created on: Oct 17, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 *
 */
#ifndef __wlac_windows_signal_h__
#define __wlac_windows_signal_h__

#pragma include_alias( <signal.h>, <signal.h> )
#pragma include_alias( "signal.h", "signal.h" )

#include <signal.h>

#ifndef UNIX_SIGNALS_DEFINED
#define UNIX_SIGNALS_DEFINED

# define si_value	_sifields._rt.si_sigval

/* Bits in `sa_flags'.  */
#define	SA_NOCLDSTOP	1			/* Don't send SIGCHLD when children stop.  */
#define SA_NOCLDWAIT	2			/* Don't create zombie on child death.  */
#define SA_SIGINFO		4			/* Invoke signal-catching function with three arguments instead of one.  */
#define SA_RESETHAND	0x80000000	/* Reset to SIG_DFL on entry to handler.  */

/* Signals.  */
#define	SIGHUP		1	/* Hangup (POSIX).  */
//#define	SIGINT		2	/* Interrupt (ANSI).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
//#define	SIGILL		4	/* Illegal instruction (ANSI).  */
#define	SIGTRAP		5	/* Trace trap (POSIX).  */
//#define	SIGABRT		6	/* Abort (ANSI).  */
#define	SIGIOT		6	/* IOT trap (4.2 BSD).  */
#define	SIGBUS		7	/* BUS error (4.2 BSD).  */
//#define	SIGFPE		8	/* Floating-point exception (ANSI).  */
#define	SIGKILL		9	/* Kill, unblockable (POSIX).  */
#define	SIGUSR1		10	/* User-defined signal 1 (POSIX).  */
//#define	SIGSEGV		11	/* Segmentation violation (ANSI).  */
#define	SIGUSR2		12	/* User-defined signal 2 (POSIX).  */
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
//#define	SIGTERM		15	/* Termination (ANSI).  */
#define	SIGSTKFLT	16	/* Stack fault.  */
#define	SIGCLD		SIGCHLD	/* Same as SIGCHLD (System V).  */
#define	SIGCHLD		17	/* Child status has changed (POSIX).  */
#define	SIGCONT		18	/* Continue (POSIX).  */
#define	SIGSTOP		19	/* Stop, unblockable (POSIX).  */
#define	SIGTSTP		20	/* Keyboard stop (POSIX).  */
///#define	SIGTTIN		21	/* Background read from tty (POSIX).  */  /// This is used by windows
///#define	SIGTTOU		22	/* Background write to tty (POSIX).  */   /// This is used by windows
#define	SIGURG		23	/* Urgent condition on socket (4.2 BSD).  */
#define	SIGXCPU		24	/* CPU limit exceeded (4.2 BSD).  */
#define	SIGXFSZ		25	/* File size limit exceeded (4.2 BSD).  */
#define	SIGVTALRM	26	/* Virtual alarm clock (4.2 BSD).  */
#define	SIGPROF		27	/* Profiling alarm clock (4.2 BSD).  */
#define	SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */
#define	SIGPOLL		SIGIO	/* Pollable event occurred (System V).  */
#define	SIGIO		29	/* I/O now possible (4.2 BSD).  */
#define	SIGPWR		30	/* Power failure restart (System V).  */
#define SIGSYS		31	/* Bad system call.  */
#define SIGUNUSED	31

#ifndef SIGABRT_COMPAT
#define SIGABRT_COMPAT	6
#endif

#define	_NSIG		65	/* Biggest signal number + 1(including real-time signals).  */

#endif  // #ifndef UNIX_SIGNALS_DEFINED

#ifdef __cplusplus
extern "C"
{
#endif

#include <bits/siginfo.h>

typedef void (__cdecl * win_signal_handler_type)(int);
typedef void(*signal_handler_simple_type)(int);
typedef void(*signal_handler_siginfo_type)(int, siginfo_t*, void*);
typedef struct signal_handler_struct
{
	union
	{
		void*						win_func_ptr;
		signal_handler_simple_type	win_sa_handler;
		/* Used if SA_SIGINFO is set.  */
		signal_handler_siginfo_type	win_sa_sigaction;
	};
	int sa_flags;
	/* Restore handler.  */
	void(*sa_restorer) (void);
	int	is_active;
}signal_handler_struct_t;

#ifdef __cplusplus
}
#endif



#endif  // #ifndef __windows_signal_h__
