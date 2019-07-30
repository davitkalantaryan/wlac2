// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// wlac/io.h
// 2017 Dec 07

#ifndef __wlac__io_h__
#define __wlac__io_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <io.h>, <io.h> )
#pragma include_alias( "io.h", "io.h" )
#include <io.h>
#if defined(_MSC_VER) && (_MSC_VER == 1900)
//#include "corecrt_io.h"
#endif

__BEGIN_C_DECLS

__END_C_DECLS


#if defined(_MSC_VER) & (_MSC_VER >= 1400)
#pragma warning(disable : 4996)  // for open, should be other 
#endif

#endif  // #ifndef __wlac__io_h__
