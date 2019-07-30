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
#ifndef __win_bits_semaphore_h__
#define __win_bits_semaphore_h__

#include <first_includes/common_include_for_headers.h>
#include <Windows.h>

#ifndef __win_semaphore_h__
# error "Never use <bits/semaphore.h> directly; include <semaphore.h> instead."
#endif

#include <bits/wordsize.h>

#if __WORDSIZE == 64
# define __SIZEOF_SEM_T	32
#else
# define __SIZEOF_SEM_T	16
#endif


/* Value returned if `sem_open' failed.  */
#define SEM_FAILED      ((sem_t *) 0)


typedef union
{
	char __size[__SIZEOF_SEM_T];
	long int __align;
	HANDLE	sem;
} sem_t;


#endif  /* #ifndef __win_bits_semaphore_h__ */
