

#ifndef __wlac_stdio2_h__
#define __wlac_stdio2_h__

#pragma include_alias( <stdio.h>, <stdio.h> )
#pragma include_alias( "stdio.h", "stdio.h" )
#include <stdio.h>
#include <share.h>

#ifdef __cplusplus
extern "C" {
#endif

//char* wlac_getenv(const char* a_name);
FILE* wlac_fopen(const char* a_fileName, const char* a_mode);

#ifdef __cplusplus
}
#endif

//#define fopen(_fileName_,_mode_) _fsopen((_fileName_),(_mode_),_SH_DENYNO)
#define fopen wlac_fopen


#endif
