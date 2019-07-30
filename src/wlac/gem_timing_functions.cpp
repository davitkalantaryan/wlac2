/*
 *	File: gem_timing_functions.cpp
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) gettimeofday
 *		2) usleep
 *		3) nanosleep
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <Windows.h>
#include <sys/time.h>
#include <io.h>
#include <stdlib.h>
#include <redesigned/errno.h>
#include <sdef_gem_windows.h>
#include <sys/types.h>

__BEGIN_C_DECLS


GEM_API int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	static void* spPointer = NULL;
	FILETIME ft;
	unsigned __int64 tmpres = 0;
#if(_MSC_VER >= 1400)
	long lnTzTemp;
	int nldTemp;
#endif

	if (tv)
	{
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		tmpres /= 10;  /*convert into microseconds*/
		/*converting file time to unix epoch*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
	}

	if (tz)
	{
		// (setting timezone environmental variable) once
		if (InterlockedCompareExchangePointerNew(&spPointer, (void*)1, NULL) == NULL)
		{
			_tzset();
		}
#if(_MSC_VER >= 1400)
		_get_timezone(&lnTzTemp);
		tz->tz_minuteswest = lnTzTemp / 60;
		_get_daylight(&nldTemp);
		tz->tz_dsttime = nldTemp;
#else
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
#endif
	}

	return 0;
}


/// Should be cheched
GEM_API int usleep(int64_t a_usec)
{
#if 0
	struct timeval tv;
	struct timeval* pTv = NULL;
	int maxsd, fh1;
	fd_set rfds;

	fh1 = (int)socket(AF_INET, SOCK_STREAM, 0);

	//fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET((unsigned int)fh1, &rfds);
	maxsd = fh1 + 1;

	if (a_usec > 0)
	{
		tv.tv_sec = a_usec / 1000000L;
		tv.tv_usec = a_usec % 1000000L;
		pTv = &tv;
	}

	return (long)select(maxsd, &rfds, 0, 0, pTv);
#else

	DWORD msSleep = (DWORD)(a_usec / (int64_t)1000);
	Sleep(msSleep);
	return 0;

#endif
}


GEM_API int nanosleep(const struct timespec *a_req, struct timespec *a_rem)
{
	int64_t timeToWait = a_req ? (a_req->tv_sec * 1000000 + a_req->tv_nsec / 1000) : 0;
	return usleep(timeToWait);
}


#if defined(_MSC_VER) & (_MSC_VER >= 1400)
#else  // #if defined(_MSC_VER) & (_MSC_VER >= 1400)
GEM_API struct tm *gmtime_r(const time_t *a_timep, struct tm *a_result)
{
	struct tm * result = gmtime(a_timep);

	if (!result) return NULL;
	memcpy(a_result, result, sizeof(struct tm));
	return a_result;
}
#endif // #if defined(_MSC_VER) & (_MSC_VER >= 1400)


__END_C_DECLS



