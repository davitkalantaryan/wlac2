// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/cdef.h> For WINDOWS MFC
 *
 *	Created on: Dec 17, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_sys_cdef_h___
#define __win_sys_cdef_h___

#include <first_includes/common_include_for_headers.h>
#include <redesigned/sys/types.h>

#ifndef bool_t
#define	bool_t	int
#endif
#ifndef enum_t
#define	enum_t	int
#endif
#ifndef FALSE
#define	FALSE	(0)
#endif
#ifndef TRUE
#define	TRUE	(1)
#endif
#ifndef __dontcare__
#define __dontcare__	-1
#endif
#ifndef NULL
#	define NULL 0
#endif
#ifndef u_int
//#define u_int unsigned int
#endif

#endif  /* #ifndef __win_sys_cdef_h___ */
