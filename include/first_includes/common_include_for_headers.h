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

#ifndef _WIN64
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

#pragma include_alias( <sys/stat.h>, <redesigned/sys/stat.h> )
#pragma include_alias( "sys/stat.h", "redesigned/sys/stat.h" )

#pragma include_alias( <sys/types.h>, <redesigned/sys/types.h> )
#pragma include_alias( "sys/types.h", "redesigned/sys/types.h" )

#pragma include_alias( <errno.h>, <redesigned/errno.h> )
#pragma include_alias( "errno.h", "redesigned/errno.h" )

#pragma include_alias( <fcntl.h>, <redesigned/fcntl.h> )
#pragma include_alias( "fcntl.h", "redesigned/fcntl.h" )

#pragma include_alias( <io.h>, <redesigned/io.h> )
#pragma include_alias( "io.h", "redesigned/io.h" )

#pragma include_alias( <process.h>, <redesigned/process.h> )
#pragma include_alias( "process.h", "redesigned/process.h" )

#pragma include_alias( <signal.h>, <redesigned/signal.h> )
#pragma include_alias( "signal.h", "redesigned/signal.h" )

#pragma include_alias( <stdio.h>, <redesigned/stdio.h> )
#pragma include_alias( "stdio.h", "redesigned/stdio.h" )

#pragma include_alias( <stdlib.h>, <redesigned/stdlib.h> )
#pragma include_alias( "stdlib.h", "redesigned/stdlib.h" )

#pragma include_alias( <time.h>, <redesigned/time.h> )
#pragma include_alias( "time.h", "redesigned/time.h" )

#pragma include_alias( <wchar.h>, <redesigned/wchar.h> )
#pragma include_alias( "wchar.h", "redesigned/wchar.h" )

#pragma include_alias( <direct.h>, <redesigned/direct.h> )
#pragma include_alias( "direct.h", "redesigned/direct.h" )

#pragma include_alias( <rpc.h>, <redesigned/rpc.h> )
#pragma include_alias( "rpc.h", "redesigned/rpc.h" )

#pragma include_alias( <fstream>, <redesigned/fstream> )
#pragma include_alias( "fstream", "redesigned/fstream" )

#pragma include_alias( <windows.h>, <redesigned/windows.h> )
#pragma include_alias( "windows.h", "redesigned/windows.h" )

#pragma include_alias( <WinSock2.h>, <redesigned/WinSock2.h> )
#pragma include_alias( "WinSock2.h", "redesigned/WinSock2.h" )


#pragma include_alias( <WS2tcpip.h>, <redesigned/WS2tcpip.h> )
#pragma include_alias( "WS2tcpip.h", "redesigned/WS2tcpip.h" )

#pragma include_alias( <stddef.h>, <redesigned/stddef.h> )
#pragma include_alias( "stddef.h", "redesigned/stddef.h" )

#pragma include_alias( <string.h>, <redesigned/string.h> )
#pragma include_alias( "string.h", "redesigned/string.h" )

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
