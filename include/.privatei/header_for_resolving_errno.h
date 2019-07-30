// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// header_for_resolving_errno.h
// 2017 Dec 29

#ifndef __wlac_dot_privatei_header_for_resolving_errno_h__
#define __wlac_dot_privatei_header_for_resolving_errno_h__

//#include <first_includes/common_include_for_headers.h>

#pragma include_alias( <errno.h>, <errno.h> )
#pragma include_alias( <stddef.h>, <stddef.h> )
#pragma include_alias( <stdlib.h>, <stdlib.h> )

#pragma include_alias( "errno.h", "errno.h" )
#pragma include_alias( "stddef.h", "stddef.h" )
#pragma include_alias( "stdlib.h", "stdlib.h" )

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
//#pragma include_alias( <stdlib.h>, <redesigned/stdlib.h> )
//#pragma include_alias( <stddef.h>, <redesigned/stddef.h> )
//#pragma include_alias( <errno.h>, <redesigned/errno.h> )

__BEGIN_C_DECLS

GEM_API_FAR int* wlac_errno(void);
GEM_API_FAR void wlac_exit(int status, int signal);

__END_C_DECLS

#ifdef errno
#undef errno
#endif

#define errno (*wlac_errno())

#ifndef __FILE_FROM_PATH__
#define __FILE_FROM_PATH__(_path_)	( strrchr((_path_),'\\') ? \
										(strrchr((_path_),'\\')+1): \
										(strrchr((_path_),'/') ? (strrchr((_path_),'\\')+1):(_path_))  )
#endif

//#define exit wlac_exit

#if ( !defined(__cplusplus)||defined(exit_redefination_needed) ) && !defined(exit_redefination_not_needed)

#ifdef SIGNAL_FOR_EXIT
#define exit(_status_) \
	do{ \
		printf("Fl:%s,ln:%d: ExitCall(%d)\n",__FILE_FROM_PATH__(__FILE__),__LINE__,(_status_)); \
		wlac_exit((_status_),SIGNAL_FOR_EXIT);\
	}while(0)
#else
#define exit _exit
#endif

#endif   // #if ( !defined(__cplusplus)||defined(exit_redefination_needed) ) && !defined(exit_redefination_not_needed)


#endif  // #ifndef __wlac_dot_privatei_header_for_resolving_errno_h__
