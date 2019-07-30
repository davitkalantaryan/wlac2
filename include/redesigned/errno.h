// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <gem_errno.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __wlac_redesigned_errno_h__
#define __wlac_redesigned_errno_h__


#include <first_includes/common_include_for_headers.h>
#include <.privatei/header_for_resolving_errno.h>

//#pragma include_alias( <errno.h>, <errno.h> )
//#include <errno.h>
//#pragma include_alias( <errno.h>, <redesigned/errno.h> )


#define	API_NOT_IMPLEMENTED	-1

#ifndef ENOMEM
#define	ENOMEM		12	/* Out of memory */
#endif
#ifndef E_SELECT
#define E_SELECT		-28	/* error by select */
#endif
#ifndef E_FATAL
#define E_FATAL			-1	/* fatal error */
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT       138
#endif

__BEGIN_C_DECLS

GEM_API_FAR int* wlac_errno(void);

__END_C_DECLS


#endif  /* #ifndef __wlac_redesigned_errno_h__ */
