// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <netdb.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_bits_sigaction_h__
#define __win_bits_sigaction_h__

#include <first_includes/common_include_for_headers.h>

#ifndef __win_signal_new_h__
# error "Never include <bits/sigaction.h> directly; use <signal.h> instead."
#endif


#include <bits/siginfo.h>

/* Bits in `sa_flags'.  */
#define	SA_NOCLDSTOP  1		 /* Don't send SIGCHLD when children stop.  */
#define SA_NOCLDWAIT  2		 /* Don't create zombie on child death.  */
#define SA_SIGINFO    4		 /* Invoke signal-catching function with
three arguments instead of one.  */

# define SA_RESTART   0x10000000 /* Restart syscall on signal return.  */
# define SA_NODEFER   0x40000000 /* Don't automatically block the signal when
 its handler is being executed.  */
# define SA_RESETHAND 0x80000000 /* Reset to SIG_DFL on entry to handler.  */

/* Structure describing the action to be taken when a signal arrives.  */
struct sigaction
{
	/* Signal handler.  */
#ifdef __USE_POSIX199309
	union
	{
		/* Used if SA_SIGINFO is not set.  */
		__sighandler_t sa_handler;
		/* Used if SA_SIGINFO is set.  */
		void(*sa_sigaction) (int, siginfo_t *, void *);
	}
	__sigaction_handler;
# define sa_handler	__sigaction_handler.sa_handler
# define sa_sigaction	__sigaction_handler.sa_sigaction
#else
	__sighandler_t sa_handler;
#endif

	/* Additional set of signals to be blocked.  */
	__sigset_t sa_mask;

	/* Special flags.  */
	int sa_flags;

	/* Restore handler.  */
	void(*sa_restorer) (void);
};


__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_bits_sigaction_h__ */
