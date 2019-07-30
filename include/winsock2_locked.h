// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// winsock2.h
// 2017 Dec 26

#ifndef __winsock2_h__
#define __winsock2_h__

#include <stddef.h>

#ifdef u_long
#define u_long_defined_before_winsock2
#pragma push_macro("u_long")
#undef u_long
#endif

#define u_long unsigned long

#ifdef _MSC_VER

#if _MSC_VER == 1900
#include <redesigned/.original/msc1900/winsock2.h>
#else
#include <redesigned/.original/other/winsock2.h>
#endif

#else   // #ifdef _MSC_VER
#include <redesigned/.original/other/winsock2.h>
#endif  // #ifdef _MSC_VER

#ifdef u_long_defined_before_winsock2
#pragma pop_macro("u_long")
#else
#undef u_long
#endif


#endif  // #ifndef __winsock2_h__
