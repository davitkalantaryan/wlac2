// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <redesigned/sys/stat.h> For WINDOWS MFC
 *
 *	Created on: Aug 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_redesigned_sys_stat_h__
#define __win_redesigned_sys_stat_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <sys/stat.h>, <sys/stat.h> )
#pragma include_alias( "sys/stat.h", "sys/stat.h" )
#include <sys/stat.h>
#include <direct.h>

#ifdef mkdir
#undef mkdir
#endif
#ifdef mkdir_windows_style_needed
#define mkdir		_mkdir
#else
#define mkdir(_a_path_,_a_permission_) _mkdir((_a_path_))
#endif

__BEGIN_C_DECLS

__END_C_DECLS


#endif  // ifndef __win_redesigned_sys_stat_h__
