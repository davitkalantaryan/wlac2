// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/resource.h> For WINDOWS MFC
 *
 *	Created on: May 6, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __resource_h__
#define __resource_h__

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>
#include <bits/resource.h>

__BEGIN_C_DECLS

GEM_API_FAR int getrlimit(int resource, struct rlimit *rlim);
GEM_API_FAR int setrlimit(int resource, const struct rlimit *rlim);

__END_C_DECLS


#endif  // #ifndef __resource_h__
