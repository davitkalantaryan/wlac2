/*
 *	File: remote_function_caller_private.h
 *
 *	Created on: Oct 19, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  Implements: 
 *
 *
 */
#ifndef __remote_function_caller_private_h__
#define __remote_function_caller_private_h__

#include <stdio.h>
#include <windows_signal.h>

typedef FILE* FILE_P;

//#ifdef __cplusplus
static const FILE_P		s_cfpOutputs[] = { stdin, stdout, stderr };
//#else
//static const FILE_P		s_cfpOutputs[] = { (FILE_P)0, (FILE_P)1, (FILE_P)2 };
//#endif

#if 1
#if _MSC_VER > 1400
#pragma warning(disable : 4996)
#endif
#endif


#define MAX_QUEUE_SIZE	128


#ifndef REMOTE_FNC_LINKAGE
#define REMOTE_FNC_LINKAGE
#endif


#ifdef __cplusplus
extern "C"
{
#endif

// local signal init state
enum LSIS{ LSIS_NOT_INITED,LSIS_INITING,LSIS_INITED,LSIS_CLEANING,LSIS_INITING_STOPPED};

extern HANDLE					g_SignalSemaphore;
extern int						g_nNumberOfSignalWaiters;
extern struct FifoSig*			g_pFifoForSignal;
extern signal_handler_struct	g_vSignalHandlers[];
extern char						g_vcThisLibrary_Path[];

int printf_on_remote_fast_private(HANDLE hProcess, int pid, const char* function_name, const char* format, const va_list* list);

typedef struct SWinSignal
{
	siginfo_t	sig_info;
	int			m_sig_num;
	short int	m_return2;
	short int	m_isFromSystem;
}SWinSignal;

#ifdef __cplusplus
}
#endif

struct SList {
	struct SWinSignal sig; SList *prev, *next;
#ifdef __cplusplus
	SList(const SWinSignal& a_sig) :sig(a_sig), next(NULL){}
#endif
};
struct FifoSig
{
#ifdef __cplusplus
	FifoSig();
	~FifoSig();
	void Add(const SWinSignal& si);
	bool GetFirst(SWinSignal* siPtr);
#endif
	HANDLE		m_mutex;
	SList*		m_first;
	SList*		m_last;
};


#endif  // #ifndef __remote_function_caller_private_h__
