// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: functions_libgui_hacker_base.h
 *
 *	Created on: Oct 24, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions for hacking the GUIs
 *		1) ...
 *		2) ...
 *		...
 *
 *
 *
 */

#ifndef __functions_libgui_hacker_base_h__
#define __functions_libgui_hacker_base_h__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>


#ifndef GUIHACKER_LINKAGE
#define GUIHACKER_LINKAGE
#endif


#ifdef __cplusplus
extern "C"
{
#endif

typedef BOOL(*compare_type)(HWND,const void*);


#ifdef __cplusplus
}
#endif



#endif  // #ifndef __functions_libgui_hacker_base_h__
