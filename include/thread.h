// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <thread.h> For WINDOWS MFC
 *  This file is not standard file, but is needed for rpc_bind service
 *
 *	Created on: Aug 20, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 */

#ifndef __win_thread_h___
#define __win_thread_h___

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>


__BEGIN_C_DECLS

GEM_API_FAR int thr_main(void);

__END_C_DECLS

#endif  // #ifndef __win_thread_h___
