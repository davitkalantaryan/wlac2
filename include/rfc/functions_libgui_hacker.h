// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: functions_libgui_hacker.h
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

#ifndef __functions_libgui_hacker_h__
#define __functions_libgui_hacker_h__

#include "functions_libgui_hacker_base.h"

#ifndef GUIHACKER_LINKAGE
#define GUIHACKER_LINKAGE
#endif


#ifdef __cplusplus
extern "C"
{
#endif


GUIHACKER_LINKAGE HWND SearchWindowInProcess(int pid, compare_type function, const void* data);
GUIHACKER_LINKAGE HWND SearchWindowInProcessByName(int pid, const char* windowText);
GUIHACKER_LINKAGE HWND SearchWindowInProcessByClassName(int pid, const char* className);
GUIHACKER_LINKAGE HWND SearchWindowInProcessByRelativeCoordinate(int pid, const POINT* clntPoint);
GUIHACKER_LINKAGE HWND SearchWindowInProcessByAbsoluteCoordinate(int pid, const POINT* absPoint);
GUIHACKER_LINKAGE BOOL ListProcessThreads2(int ownerPID, const void* data, int(*iterFunc)(struct tagTHREADENTRY32*, const void*));


#ifdef __cplusplus
}
#endif



#endif  // #ifndef __functions_libgui_hacker_h__
