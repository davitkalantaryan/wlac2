// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <bits/semaphore.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_bits_wordsize_h__
#define __win_bits_wordsize_h__

#include <first_includes/common_include_for_headers.h>

#ifdef _WIN64
#define __x86_64__
#endif

#if defined __x86_64__
# define __WORDSIZE	64
# define __WORDSIZE_COMPAT32	1
#else
# define __WORDSIZE	32
#endif

__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_bits_wordsize_h__ */
