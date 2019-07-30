/*
 *	File: <strings.h> For WINDOWS MFC
 *
 *	Created on: Sep 19, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#include <stddef.h>
#include <memory.h>
#include <string.h>


#ifndef bcopy
#define bcopy(__a_src__,__a_dst__,__a_n__)	memmove((__a_dst__),(__a_src__),(__a_n__))
#define	bzero(__a_s__,__a_size__)			memset((__a_s__),0,(__a_size__))
#define bcmp	memcmp
#endif
