// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// 2017 Dec 07

#ifndef __wlac_stdio_h__
#define __wlac_stdio_h__

#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <stdio.h>, <stdio.h> )
#pragma include_alias( "stdio.h", "stdio.h" )
#include <stdio.h>
#include <share.h>
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#include <corecrt_io.h>
#endif

__BEGIN_C_DECLS


GEM_API_FAR FILE* wlac_fopen(const char* fileName, const char* mode);
GEM_API_FAR int wlac_rename(const char *oldname, const char *newname);
#ifdef _WIN64
GEM_API_FAR int wlac_snprintf(char* buffer, size_t count, const char* format, ...);
GEM_API_FAR int wlac_sprintf(char* buffer, const char* format, ...);
GEM_API_FAR int wlac_sscanf(const char* buffer, const char* format,...);
#endif   // #ifdef _WIN64

__END_C_DECLS

#ifndef NOT_USE_WLAC_REDESIGNED
#ifdef fopen
#undef fopen
#endif
#define fopen wlac_fopen

#ifdef rename
#undef rename
#endif
#define rename wlac_rename

#ifdef _WIN64

#ifdef snprintf
#undef snprintf
#endif
#define snprintf wlac_snprintf

#ifdef sprintf
#undef sprintf
#endif
#define sprintf wlac_sprintf

#ifdef sscanf
#undef sscanf
#endif
#define sscanf wlac_sscanf

#else  // #ifdef _WIN64

#endif   //#ifndef NOT_USE_WLAC_REDESIGNED

#if defined(_MSC_VER) & (_MSC_VER>1400)
#pragma warning (disable:4996)
#endif

#endif  // #ifdef _WIN64


#endif  // #ifndef __wlac_stdio_h__
