// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

#ifndef __win_sys_ioctl_h__
#define __win_sys_ioctl_h__

#include <first_includes/common_include_for_headers.h>
#include <bits/ioctls.h>

__BEGIN_C_DECLS

GEM_API_FAR int ioctl(int d, int request, ...);

__END_C_DECLS

#endif  /* #ifndef __win_sys_ioctl_h__ */
