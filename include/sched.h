// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sched.h> For WINDOWS MFC
 *
 *	Created on: Aug 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_sched_h___
#define __win_sched_h___

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

/* Scheduling algorithms.  */
#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#ifdef __USE_GNU
# define SCHED_BATCH		3
# define SCHED_IDLE		5

# define SCHED_RESET_ON_FORK	0x40000000
#endif

#define sched_priority	__sched_priority

/* The official definition.  */
struct sched_param
{
	int __sched_priority;
};

__BEGIN_C_DECLS

GEM_API_FAR int sched_get_priority_max(int policy);
GEM_API_FAR int sched_get_priority_min(int policy);

__END_C_DECLS

#endif  // #ifndef __win_sched_h___
