// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// common_include_for_headers.h
// 2017 Dec 06
// created by D. Kalantaryan

#ifndef __common_include_for_headers_h__
#define __common_include_for_headers_h__

#ifdef _MSC_VER
//#define INCLUDE_NEXT	#include
#endif

#if !defined(_WIN64) && !defined(_M_ARM64)
// In the UNIX like systems sizeof(time_t)==sizeof(void*)==sizeof(long)
#if !defined(_USE_32BIT_TIME_T)
#define _USE_32BIT_TIME_T
#endif  // #if !defined(_USE_32BIT_TIME_T)
#endif


#if defined(_MSC_VER)& (_MSC_VER >= 1900) & defined(WARNINGS_DISABLE)
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#pragma warning(disable : 4477)
#pragma warning(disable : 4018)		//'<' : signed/unsigned mismatch
#pragma warning(disable : 4244)		// Possible loss of data
#pragma warning(disable : 4267)		// Warning for integer type mismatch. In gcc ...
#pragma warning(disable : 4800)		// forcing value to bool ...
#pragma warning(disable : 4305)		//  truncation from 'double' to 'float'
#pragma warning(disable : 4805)		//  '==' : unsafe mix of type 'int' and type 'bool' in operation
#endif  // #if defined(_MSC_VER)& (_MSC_VER >= 1900) & defined(WARNINGS_DISABLE)

#include <first_includes/common_definations_wul.h>

#pragma include_alias( <sys/stat.h>, <other/redesigned/sys/stat.h> )
#pragma include_alias( "sys/stat.h", "other/redesigned/sys/stat.h" )

#pragma include_alias( <sys/types.h>, <other/redesigned/sys/types.h> )
#pragma include_alias( "sys/types.h", "other/redesigned/sys/types.h" )

#pragma include_alias( <errno.h>, <other/redesigned/errno.h> )
#pragma include_alias( "errno.h", "other/redesigned/errno.h" )

#pragma include_alias( <fcntl.h>, <other/redesigned/fcntl.h> )
#pragma include_alias( "fcntl.h", "other/redesigned/fcntl.h" )

#pragma include_alias( <io.h>, <other/redesigned/io.h> )
#pragma include_alias( "io.h", "other/redesigned/io.h" )

#pragma include_alias( <process.h>, <other/redesigned/process.h> )
#pragma include_alias( "process.h", "other/redesigned/process.h" )

#pragma include_alias( <signal.h>, <other/redesigned/signal.h> )
#pragma include_alias( "signal.h", "other/redesigned/signal.h" )

#pragma include_alias( <stdio.h>, <other/redesigned/stdio.h> )
#pragma include_alias( "stdio.h", "other/redesigned/stdio.h" )

#pragma include_alias( <stdlib.h>, <other/redesigned/stdlib.h> )
#pragma include_alias( "stdlib.h", "other/redesigned/stdlib.h" )

#pragma include_alias( <time.h>, <other/redesigned/time.h> )
#pragma include_alias( "time.h", "other/redesigned/time.h" )

#pragma include_alias( <wchar.h>, <other/redesigned/wchar.h> )
#pragma include_alias( "wchar.h", "other/redesigned/wchar.h" )

#pragma include_alias( <direct.h>, <other/redesigned/direct.h> )
#pragma include_alias( "direct.h", "other/redesigned/direct.h" )

#pragma include_alias( <rpc.h>, <other/redesigned/rpc.h> )
#pragma include_alias( "rpc.h", "other/redesigned/rpc.h" )

#pragma include_alias( <fstream>, <other/redesigned/fstream> )
#pragma include_alias( "fstream", "other/redesigned/fstream" )

#pragma include_alias( <windows.h>, <other/redesigned/windows.h> )
#pragma include_alias( "windows.h", "other/redesigned/windows.h" )

#pragma include_alias( <WinSock2.h>, <other/redesigned/WinSock2.h> )
#pragma include_alias( "WinSock2.h", "other/redesigned/WinSock2.h" )


#pragma include_alias( <WS2tcpip.h>, <other/redesigned/WS2tcpip.h> )
#pragma include_alias( "WS2tcpip.h", "other/redesigned/WS2tcpip.h" )

#pragma include_alias( <stddef.h>, <other/redesigned/stddef.h> )
#pragma include_alias( "stddef.h", "other/redesigned/stddef.h" )

#pragma include_alias( <string.h>, <other/redesigned/string.h> )
#pragma include_alias( "string.h", "other/redesigned/string.h" )

#ifdef std_wlac_thread_used
#pragma include_alias( <thread>, <other/redesigned/std/thread/wlac_thread> )
#pragma include_alias( "thread", "other/redesigned/std/thread/wlac_thread" )
#endif

// alloca
//#include <malloc.h>
#ifndef alloca
#define alloca _alloca
#endif

__BEGIN_C_DECLS


__END_C_DECLS

#ifdef _MSC_VER

#if !defined(quad_t) && !defined(quad_t_defined)
#define quad_t_defined
typedef __int64 quad_t;
#endif  // #if !defined(quad_t) && !defined(quad_t_defined)

#if !defined(u_quad_t) && !defined(u_quad_t_defined)
#define u_quad_t_defined
typedef unsigned __int64 u_quad_t;
#endif  // #if !defined(quad_t) && !defined(quad_t_defined)

#endif// #ifdef _MSC_VER

#ifndef WLAC_TMP_API
#define WLAC_TMP_API
#endif


#endif // #ifndef __common_include_for_headers_h__
