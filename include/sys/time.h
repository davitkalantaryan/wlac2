// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/time.h> For WINDOWS MFC
 *
 *	Created on: Dec 10, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * 
 */
#ifndef __win_sys_time_h__
#define __win_sys_time_h__

#include <first_includes/common_include_for_headers.h>
#include <time.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

/* Structure crudely representing a timezone.
This is obsolete and should never be used.  */
#if !defined(timezone_defined) && !defined(timezone_not_needed)
#define timezone_defined
#ifdef timezone
#undef timezone
#endif
struct timezone
{
	int tz_minuteswest;		/* Minutes west of GMT.  */
	int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};
#endif


__BEGIN_C_DECLS

#ifndef GET_TIME_OF_ZONE_DEFINED
GEM_API_FAR int gettimeofday(struct timeval *tv, struct timezone *tz);
#define GET_TIME_OF_ZONE_DEFINED
#endif

__END_C_DECLS

#endif  /* #ifndef __win_sys_time_h__ */
