
// wlac/redesigned/stdlib.h
// 2017 Dec 07

#ifndef __stdlib_h__
#define __stdlib_h__

#pragma include_alias( <stdlib.h>, <stdlib.h> )
#include <stdlib.h>
#pragma include_alias( <stdlib.h>, <redesigned/stdlib.h> )

#ifdef __cplusplus
extern "C" {
#endif

char* wlac_getenv(const char* a_name);

#ifdef __cplusplus
}
#endif

#define getenv wlac_getenv

#endif  // #ifndef __stdlib_h__
