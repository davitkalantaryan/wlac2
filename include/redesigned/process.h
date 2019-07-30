// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// process.h
// 2017 Dec 07
// created by D. Kalantaryan

#ifndef __wlac_redesigend_process_h__
#define __wlac_redesigend_process_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <process.h>, <process.h> )
#pragma include_alias( "process.h", "process.h" )

#ifdef exit
#define exit_defined_before_process_h
#pragma push_macro("exit")
#undef exit
#endif

#include <process.h>

#ifdef exit_defined_before_process_h
#pragma pop_macro("exit")
#endif

#include <.privatei/header_for_resolving_errno.h>

#include <stdio.h>
#include <string.h>

#define getpid _getpid

__BEGIN_C_DECLS

GEM_API_FAR void wlac_exit(int status, int signal);

__END_C_DECLS

#endif  // #ifndef __wlac_redesigend_process_h__
