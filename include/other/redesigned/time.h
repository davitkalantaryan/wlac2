// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <time_new.h> For WINDOWS MFC
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_time_new_h__
#define __win_time_new_h__

// if we are here then include below is included
//#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <time.h>, <time.h> )
#pragma include_alias( "time.h", "time.h" )
#include <time.h>

//#ifdef __cplusplus
//GEM_API_FAR struct tm* wlac_localtime(const long* timep);
//#if ( defined(_WIN64) || defined(_M_ARM64) ) && !defined(_USE_32BIT_TIME_T)
//GEM_API_FAR struct tm* wlac_localtime(const time_t* a_timep);
//#endif
//#endif

#ifndef wlac_localtime_not_needed
#ifdef localtime
#undef localtime
#endif
#if !defined(_USE_32BIT_TIME_T) // sizeof(time_t) != sizeof(long) , in some unix codes this isassumption, so please fix
#ifdef __cplusplus
// in the case of c++ let tis be in parrallel to usual defination
#define localtime	localtime_cpp
GEM_API_FAR struct tm* localtime_cpp(const long* a_timep);
GEM_API_FAR struct tm* localtime_cpp(const time_t* a_timep);
#else
#define localtime	wlac_localtime
#endif
#endif
#endif


#ifndef wlac_localtime_r_not_needed
#ifdef localtime_r
#undef localtime_r
#endif
#if !defined(_USE_32BIT_TIME_T) // sizeof(time_t) != sizeof(long) , in some unix codes this isassumption, so please fix
#ifdef __cplusplus
// in the case of c++ let tis be in parrallel to usual defination
#define localtime_r		localtime_r_cpp
GEM_API_FAR struct tm* localtime_r_cpp(const long* a_timep, struct tm* result);
GEM_API_FAR struct tm* localtime_r_cpp(const time_t* a_timep, struct tm* result);
#else
#ifdef localtime_r_not_change_type
#define	localtime_r(_timep_,_result_)	( localtime_s((_result_),(_timep_)) ? (_result_) : NULL )
#else
#define localtime_r		wlac_localtime_r
#endif
#endif
#else // #if !defined(_USE_32BIT_TIME_T)
//#define	localtime_r(_timep_,_result_)	(localtime(_timep_) ? ((struct tm*)memcpy((_result_),localtime(_timep_),sizeof(struct tm))) : NULL)
#define	localtime_r(_timep_,_result_)	( localtime_s((_result_),(_timep_)) ? (_result_) : NULL )
#endif
#endif


#ifdef strftime
#undef strftime
#endif
#define strftime wlac_strftime


#if defined(_MSC_VER) && (_MSC_VER>=1900) && defined(_INC_TIME)
#define TIMESPEC_NOT_NEEDED
#endif

#if !defined(__timespec_defined) && !defined(timespec_defined) && !defined(TIMESPEC_NOT_NEEDED)
struct timespec {
	time_t   tv_sec;        /* seconds */
	long     tv_nsec;       /* nanoseconds */
};
#ifndef _CRT_NO_TIME_T
#define _CRT_NO_TIME_T
#endif
#define __timespec_defined
#define timespec_defined
#endif  // #if !defined(__timespec_defined) && !defined(timespec_defined)

//char *ctime_r(const time_t *timep, char *buf);
#if !defined(ctime_r) & !defined(ctime_r_defined)
#define ctime_r(_a_timep, _a_buf)	(ctime((_a_timep)) ? strcpy((_a_buf),ctime((_a_timep))) : NULL)
#endif  // #if !defined(ctime_r) & !defined(ctime_r_defined)

__BEGIN_C_DECLS

GEM_API_FAR int nanosleep(const struct timespec *a_req, struct timespec *a_rem);
GEM_API_FAR char *strptime(const char *s, const char *format, struct tm *tm);
GEM_API_FAR size_t wlac_strftime(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr);
GEM_API_FAR struct tm* wlac_localtime(const long* timep);
GEM_API_FAR struct tm* wlac_localtime_r(const long* a_timep, struct tm* result);

__END_C_DECLS


#endif  /* #ifndef __win_time_new_h__ */
