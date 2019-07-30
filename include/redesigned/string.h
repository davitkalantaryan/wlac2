// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// wlac/redesigned/stdlib.h
// 2017 Dec 07

#ifndef __wlac_redesigned_string_h__
#define __wlac_redesigned_string_h__

// if we are here then include below is included
//#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <string.h>, <string.h> )
#include <string.h>

__BEGIN_C_DECLS

GEM_API_FAR char* wlac_strerror(int errnum);

__END_C_DECLS

#define strerror wlac_strerror

#endif  // #ifndef __wlac_redesigned_string_h__
