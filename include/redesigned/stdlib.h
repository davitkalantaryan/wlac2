// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// wlac/redesigned/stdlib.h
// 2017 Dec 07

#ifndef __wlac_redesigned_stdlib_h__
#define __wlac_redesigned_stdlib_h__

#include <first_includes/common_include_for_headers.h>
#include <.privatei/header_for_resolving_errno.h>


__BEGIN_C_DECLS

GEM_API_FAR char* wlac_getenv(const char* a_name);
GEM_API_FAR int* wlac_errno(void);

__END_C_DECLS

#define getenv wlac_getenv

#endif  // #ifndef __wlac_redesigned_stdlib_h__
