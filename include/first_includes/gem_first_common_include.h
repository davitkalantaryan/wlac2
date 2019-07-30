// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <gem_first_common_include.h> For WINDOWS MFC
 *
 *	Created on: Dec 26, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This is the include file that is included by all gem related headers
 *
 */
#ifndef __gem_first_common_include_h__
#define __gem_first_common_include_h__

// for backward compatibility

#include <sys/timeb.h>
#ifndef timeb
//#define timeb _timeb
#endif

#include <stdio.h>
#include <io.h>
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

/// Code can be written to link this library automatically to application
#ifndef _SOURCE_FILE_
#define _SOURCE_FILE_	\
	(  strrchr(__FILE__,'/') ? (strrchr(__FILE__,'/')+1) : \
		(strrchr(__FILE__,'\\') ? (strrchr(__FILE__,'\\')+1) : __FILE__) )
#endif


#if ( !defined(__cplusplus)||defined(exit_redefination_needed) ) && !defined(exit_redefination_not_needed)
#ifdef change_exit
#define exit(__code)	\
	do{ \
		printf("fl:\"%s\", ln:%d, fnc:%s\n",_SOURCE_FILE_,__LINE__,__FUNCTION__); \
		ExitProcess((__code)); \
	}while(0)
#endif  //#ifdef change_exit
#endif  // #if ( !defined(__cplusplus)||defined(exit_redefination_needed) ) && !defined(exit_redefination_not_needed)

#endif  /* #ifndef __gem_first_common_include_h__ */
