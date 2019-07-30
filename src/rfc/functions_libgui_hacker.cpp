/*
 *	File: functions_libgui_hacker.cpp
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

#include "functions_libgui_hacker_base.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <malloc.h>
#include <stdio.h>
#include <process.h>

#ifdef _MSC_VER
#if(_MSC_VER >= 1400)
//#define		_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif
#endif


#ifdef __cplusplus
extern "C"
{
#endif


static BOOL compare_function_for_wnd_name(HWND hWnd, const void* arg);
static BOOL compare_function_for_class_name(HWND hWnd, const void* arg);
static BOOL compare_function_for_client_point(HWND hWnd, const void* arg);
static BOOL compare_function_for_abs_point(HWND hWnd, const void* arg);
static int IterFunc(struct tagTHREADENTRY32* threadInfo, const void* data);
static BOOL CALLBACK WNDENUMPROC_new(HWND a_hWnd, LPARAM a_pUser);
GUIHACKER_LINKAGE BOOL ListProcessThreads2(int ownerPID, const void* data, int(*iterFunc)(struct tagTHREADENTRY32*, const void*));
GUIHACKER_LINKAGE HWND SearchWindowInProcess(int pid, compare_type a_function, const void* a_data);


typedef struct SCallBackStruct
{
	const void*		data;
	compare_type	compare_function;
	HWND			founded_window;
	BOOL			founded;
}SCallBackStruct;

static FILE* s_fpFileToLog = NULL;

static BOOL compare_function_prvt(HWND a_hWnd, const void* a_pFilter)
{
	int nRet, i, nSize;
	char vcText[1024];
	RECT aWindowRect;
	HWND pParent,pParent0 = GetParent(a_hWnd);
	POINT leftTop, rightBottom;
	const char* cpcFilter = (const char*)a_pFilter;
	ULONG_PTR pResult;

	pParent = pParent0 ? pParent0 : a_hWnd;

	GetClassNameA(a_hWnd, vcText, 1023);
	/*if (cpcFilter && (strncmp(cpcFilter, vcText, strlen(cpcFilter)) != 0))
	{
	return FALSE;
	}*/
	nRet = fprintf(s_fpFileToLog, "(pr:%d)class_name=\"%s\";", pParent0 ? 1:0,vcText);
	nSize = 50 - nRet; nSize = nSize > 0 ? nSize : 0;
	for (i = 0; i < nSize; ++i){ fprintf(s_fpFileToLog," "); }

	//SendMessageA(a_hWnd, WM_GETTEXT, 1023, (LPARAM)vcText);
	//GetWindowTextA(a_hWnd, vcText,1023);
	vcText[0] = 0;
	SendMessageTimeoutA(a_hWnd, WM_GETTEXT, 1023, (LPARAM)vcText, SMTO_NORMAL, 10, &pResult);
	nRet = fprintf(s_fpFileToLog, "wnd_text=\"%s\";", vcText);
	nSize = 30 - nRet; nSize = nSize > 0 ? nSize : 0;
	for (i = 0; i < nSize; ++i){ fprintf(s_fpFileToLog, " "); }

	GetWindowRect(a_hWnd, &aWindowRect);
	leftTop.x = aWindowRect.left;
	leftTop.y = aWindowRect.top;
	rightBottom.x = aWindowRect.right;
	rightBottom.y = aWindowRect.bottom;
	ScreenToClient(pParent, &leftTop);
	ScreenToClient(pParent, &rightBottom);
	nRet = fprintf(s_fpFileToLog, "wnd_rect={l=%d,t=%d,r=%d,b=%d};",
		(int)leftTop.x, (int)leftTop.y, (int)rightBottom.x, (int)rightBottom.y);
	//nSize = 50 - nRet; nSize = nSize > 0 ? nSize : 0;
	//for (i = 0; i < nSize; ++i){ fprintf(s_fpFileToLog," "); }

	fprintf(s_fpFileToLog, "\n");

	return FALSE;
}



GUIHACKER_LINKAGE void SearchWindowOnRemote(void*)
{
	int nPid = _getpid();
	
	s_fpFileToLog = fopen("//afs/ifh.de/group/pitz/doocs/develop/kalantar/public2/aaa.txt", "w");
	if (!s_fpFileToLog){return;}

	SearchWindowInProcess(nPid, compare_function_prvt, NULL);

	fclose(s_fpFileToLog);
	s_fpFileToLog = NULL;
}


static HHOOK s_hook;
static HINSTANCE s_hInst;

BOOL APIENTRY DllMain(HINSTANCE hModule,
	DWORD  dwReason,
	LPVOID lpReserved)
{
	s_hInst = hModule;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		s_hInst = hModule;
		break;
	default:
		break;
	}
	return TRUE;
}


static LRESULT CALLBACK HOOKPROC_static(int a_code, WPARAM a_wParam, LPARAM a_lParam)
{
	typedef unsigned __int64 _uint64_t_;
	fprintf(s_fpFileToLog,"code=%.3d, wParam=%.5I64u, lParam=%.5I64u\n",a_code,(_uint64_t_)a_wParam,(_uint64_t_)a_lParam);
	fflush(s_fpFileToLog);
	return CallNextHookEx(s_hook, a_code, a_wParam, a_lParam);
}

static volatile int s_nWork;

#ifndef _uint64_t__defined
typedef unsigned __int64 _uint64_t_;
#define _uint64_t__defined
#endif

static LRESULT CALLBACK msghook(UINT nCode, WPARAM wParam, LPARAM lParam)
{
	//if (s_fpFileToLog){ fprintf(s_fpFileToLog, "code=%.3d, wParam=%.5I64u, lParam=%.5I64u\n", (int)nCode, (_uint64_t_)wParam, (_uint64_t_)lParam); }
	if (nCode < 0)
	{ /* pass it on */
		CallNextHookEx(s_hook, nCode, wParam, lParam);
		return 0;
	} /* pass it on */
	LPMSG msg = (LPMSG)lParam;
	if (s_fpFileToLog){ fprintf(s_fpFileToLog, "code=%.3d, wParam=%.5I64u, lParam=%.5I64u\n", (int)msg->message, (_uint64_t_)wParam, (_uint64_t_)lParam); }
	if (msg->message == WM_MOUSEMOVE ||
		msg->message == WM_NCMOUSEMOVE)
		//PostMessage(hWndServer, UWM_MOUSEMOVE, 0, 0);
	return CallNextHookEx(s_hook, nCode, wParam, lParam);
} // msghook

__declspec(dllexport) BOOL setMyHook2(void)
{
	if (!s_fpFileToLog){ s_fpFileToLog = fopen("//afs/ifh.de/group/pitz/doocs/develop/kalantar/public2/hook.txt", "w"); }
	s_hook = SetWindowsHookEx(WH_GETMESSAGE,
		(HOOKPROC)msghook,
		s_hInst,
		0);
	if (s_hook != NULL)
	{ /* success */
		//hWndServer = hWnd;
		return TRUE;
	} /* success */
	return FALSE; // failed to set hook
} // setMyHook

static void __cdecl _StartAddress_function(void *)
{
	MSG message;

	if (s_fpFileToLog){ return; }
	s_fpFileToLog = fopen("//afs/ifh.de/group/pitz/doocs/develop/kalantar/public2/hook.txt", "w");
	if (!s_fpFileToLog){ return; }
	
	fprintf(s_fpFileToLog, "thread_tid=%d\n", GetCurrentThreadId()); fflush(s_fpFileToLog);
	
	s_hook = SetWindowsHookExA(WH_CALLWNDPROC, HOOKPROC_static, NULL, GetCurrentThreadId());//WH_GETMESSAGE
	//s_hook = SetWindowsHookExA(WH_GETMESSAGE, HOOKPROC_static, NULL, 0);//WH_GETMESSAGE
	if (!s_hook)
	{
		fprintf(s_fpFileToLog, "Can not hook2!\n");
		goto returnPoint;
	}

	s_nWork = 2;
	while ((s_nWork == 2) && GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

returnPoint:
	if (s_hook){UnhookWindowsHookEx(s_hook); s_hook = NULL;}
	if (s_fpFileToLog){fclose(s_fpFileToLog); s_fpFileToLog = NULL;}
	s_nWork = 0;
}

GUIHACKER_LINKAGE void InstallCallbackHook(void*)
{

#if 0
	MSG message;

	if (s_fpFileToLog){ return; }
	s_fpFileToLog = fopen("//afs/ifh.de/group/pitz/doocs/develop/kalantar/public2/hook.txt", "w");
	if (!s_fpFileToLog){ return; }
	s_hook = SetWindowsHookExA(WH_CALLWNDPROC, HOOKPROC_static, NULL, GetCurrentThreadId());//WH_GETMESSAGE
	//s_hook = SetWindowsHookExA(WH_GETMESSAGE, HOOKPROC_static, NULL, 0);//WH_GETMESSAGE
	if (!s_hook)
	{
		fprintf(s_fpFileToLog,"Can not hook2!\n");
		fclose(s_fpFileToLog);
		s_fpFileToLog = NULL;
		return;
	}

	s_nWork = 2;

	while ((s_nWork==2) && GetMessage(&message,NULL,0,0))
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}

	UnhookWindowsHookEx(s_hook);
	fclose(s_fpFileToLog);
	s_fpFileToLog = NULL;
	s_nWork = 0;

#else  // #if 1
	
	_beginthread(_StartAddress_function, 0, NULL);

#endif  // #if 1
}


GUIHACKER_LINKAGE void UninstallCallbackHook(void*)
{
	if (s_nWork == 2){ s_nWork = 1; }
}


GUIHACKER_LINKAGE HWND SearchWindowInProcess(int a_nPid, compare_type a_function, const void* a_data)
{
	SCallBackStruct aStr;

	aStr.data = a_data;
	aStr.compare_function = a_function;
	aStr.founded_window = NULL;
	aStr.founded = FALSE;
	ListProcessThreads2(a_nPid, &aStr, IterFunc);
	return aStr.founded_window;
}


GUIHACKER_LINKAGE HWND SearchWindowInProcessByName(int a_nPid, const char* a_cpcWindowText)
{
	return SearchWindowInProcess(a_nPid, compare_function_for_wnd_name, a_cpcWindowText);
}


GUIHACKER_LINKAGE HWND SearchWindowInProcessByClassName(int a_nPid, const char* a_cpcClassName)
{
	return SearchWindowInProcess(a_nPid, compare_function_for_class_name, a_cpcClassName);
}


GUIHACKER_LINKAGE HWND SearchWindowInProcessByRelativeCoordinate(int a_nPid, const POINT* a_pClntPoint )
{
	return SearchWindowInProcess(a_nPid, compare_function_for_client_point, a_pClntPoint);
}


GUIHACKER_LINKAGE HWND SearchWindowInProcessByAbsoluteCoordinate(int a_nPid, const POINT* a_pAbsPoint)
{
	return SearchWindowInProcess(a_nPid, compare_function_for_abs_point, a_pAbsPoint);
}


GUIHACKER_LINKAGE BOOL ListProcessThreads2(int a_dwOwnerPID, const void* a_pData, int(*a_IterFunc)(struct tagTHREADENTRY32*, const void*))
{

	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if ((a_dwOwnerPID<0) || (((int)te32.th32OwnerProcessID) == a_dwOwnerPID))
		{
			if ((*a_IterFunc)(&te32, a_pData))
			{
				goto returnPoint;
			}
		}
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
returnPoint:
	CloseHandle(hThreadSnap);
	return(TRUE);

}


static int IterFunc(struct tagTHREADENTRY32* a_pThreadInfo, const void* a_pData)
{
	EnumThreadWindows(a_pThreadInfo->th32ThreadID, WNDENUMPROC_new, (LPARAM)a_pData);
	return 0;
}


static BOOL CALLBACK WNDENUMPROC_new(HWND a_hWnd, LPARAM a_pUser)
{
	SCallBackStruct*	pClbkStruct = (SCallBackStruct*)a_pUser;
	compare_type		compare_function = pClbkStruct->compare_function;
	const void*			pData = pClbkStruct->data;

	if (pClbkStruct->founded){ return FALSE; }
	if ((*compare_function)(a_hWnd, pData))
	{
		pClbkStruct->founded = TRUE;
		pClbkStruct->founded_window = a_hWnd;
		return FALSE;
	}

	EnumChildWindows(a_hWnd, WNDENUMPROC_new, a_pUser);
	return TRUE;
}


static BOOL compare_function_for_wnd_name(HWND a_hWnd, const void* a_pArg)
{
	const char* cpcWndNameToSearch = (const char*)a_pArg;
	const size_t unStrLenPlus1 = strlen(cpcWndNameToSearch) + 1;
	char* pcBufferForWndText = (char*)_alloca(unStrLenPlus1 + 1);

	SendMessageA(a_hWnd, WM_GETTEXT, unStrLenPlus1, (LPARAM)pcBufferForWndText);
	return strncmp(pcBufferForWndText, cpcWndNameToSearch, unStrLenPlus1) == 0;
}


static BOOL compare_function_for_class_name(HWND a_hWnd, const void* a_pArg)
{
	const char* cpcWndClassNameToSearch = (const char*)a_pArg;
	const size_t unStrLenPlus1 = strlen(cpcWndClassNameToSearch) + 1;
	char* pcBufferForClassNameText = (char*)_alloca(unStrLenPlus1 + 1);

	GetClassNameA(a_hWnd, pcBufferForClassNameText, (int)unStrLenPlus1);
	return strncmp(pcBufferForClassNameText, cpcWndClassNameToSearch, unStrLenPlus1) == 0;
}


static BOOL compare_function_for_client_point(HWND a_hWnd, const void* a_pArg)
{
	const POINT*	cppClientCoordinate = (const POINT*)a_pArg;
	POINT			aPoint				= *cppClientCoordinate;
	RECT			aWndRect;
	HWND			pParent = GetParent(a_hWnd);

	pParent = pParent ? pParent : a_hWnd;
	GetWindowRect(a_hWnd, &aWndRect);
	ClientToScreen(pParent, &aPoint);

	return PtInRect(&aWndRect, aPoint);
}


static BOOL compare_function_for_abs_point(HWND a_hWnd, const void* a_pArg)
{
	const POINT*	cppAbsCoordinate = (const POINT*)a_pArg;
	RECT			aWndRect;
	HWND			pParent = GetParent(a_hWnd);

	pParent = pParent ? pParent : a_hWnd;
	GetWindowRect(a_hWnd, &aWndRect);

	return PtInRect(&aWndRect, *cppAbsCoordinate);
}


#ifdef __cplusplus
}
#endif
