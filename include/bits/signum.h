// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

#ifndef __win_bits_signum_h__
#define __win_bits_signum_h__

#include <first_includes/common_include_for_headers.h>

#ifdef __win_signal_new_h__

#define	SIGHUP		1	/* Hangup (POSIX).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
#define	SIGBUS		7	/* BUS error (4.2 BSD).  */
#define	SIGUSR1		10
#define	SIGUSR2		12
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
#define	SIGCHLD		17	/* Child status has changed (POSIX).  */
#define	SIGURG		23	/* Urgent condition on socket (4.2 BSD).  */

#endif  // #ifdef __win_signal_new_h__

__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_bits_signum_h__ */
