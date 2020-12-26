/*
 *	File: local_signal_functions.cpp
 *
 *	Created on: Oct 19, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  Implements: Functions that is called in behalf of remote process
 *              implemented here
 *
 *
 */

#include <stdio.h>
#include <signal.h>
#include "remote_function_caller_base.h"
#include "windows_signal.h"
#include "remote_function_caller_private.h"


#ifdef __cplusplus
extern "C"
{
#endif

HANDLE					g_SignalSemaphore;
int						g_nNumberOfSignalWaiters = 0;
FifoSig*				g_pFifoForSignal = NULL;
signal_handler_struct	g_vSignalHandlers[_NSIG];
LSIS					g_bLocalSignalsInitState = LSIS_NOT_INITED;

REMOTE_FNC_LINKAGE BOOL initialize_local_signals(void);
REMOTE_FNC_LINKAGE void clean_local_signals(void);
REMOTE_FNC_LINKAGE __int64 raise_on_remote(_uint64_ttt a_pArg);
static void default_sa_restorer(void){}
static void default_signal_handler(int a_sig, siginfo_t* a_info, void* a_data);
static void DefaultSystemSignalHandler(int a_nSignal);


REMOTE_FNC_LINKAGE int wait_for_signal_synchron(siginfo_t* a_pSignal,int a_nWaitMS)
{
	struct SWinSignal aSig;

	if(g_bLocalSignalsInitState == LSIS_INITED){
		++g_nNumberOfSignalWaiters;
		DWORD dwWait = WaitForSingleObject(g_SignalSemaphore, a_nWaitMS);
		if(dwWait== WAIT_OBJECT_0){
			if(g_pFifoForSignal->GetFirst(&aSig)){
				if(a_pSignal){*a_pSignal = aSig.sig_info;}
				return aSig.m_sig_num;
			}
		}
	}

	return 0;
}


REMOTE_FNC_LINKAGE BOOL modify_signal_table(int a_sig, const signal_handler_struct* a_handler, signal_handler_struct* a_handler_old)
{
	int nSigNumNorm = a_sig % _NSIG;

	if (!initialize_local_signals()) { return FALSE; }

	if (a_handler_old){ memcpy(a_handler_old, &g_vSignalHandlers[nSigNumNorm], sizeof(signal_handler_struct)); }

	if (a_handler)
	{
		if ( a_handler->win_func_ptr == ((void*)SIG_DFL) )
		{
			g_vSignalHandlers[nSigNumNorm].is_active = 1;
			g_vSignalHandlers[nSigNumNorm].sa_flags = SA_SIGINFO;
			g_vSignalHandlers[nSigNumNorm].win_sa_sigaction = default_signal_handler;
		}
		else if (a_handler->win_func_ptr == ((void*)SIG_IGN))
		{
			g_vSignalHandlers[nSigNumNorm].is_active = 0;
			g_vSignalHandlers[nSigNumNorm].sa_flags = SA_SIGINFO;
			g_vSignalHandlers[nSigNumNorm].win_sa_sigaction = default_signal_handler;
		}
		else if (	(a_handler->win_func_ptr == ((void*)SIG_GET)) || 
					(a_handler->win_func_ptr == ((void*)SIG_SGE)) ||
					(a_handler->win_func_ptr == ((void*)SIG_ACK)))
		{
		}
		else
		{
			g_vSignalHandlers[nSigNumNorm].is_active = a_handler->is_active;
			if (a_handler->is_active)
			{
				g_vSignalHandlers[nSigNumNorm].sa_flags = a_handler->sa_flags;
				g_vSignalHandlers[nSigNumNorm].win_func_ptr = a_handler->win_func_ptr;
			}
		}

		if (a_handler->sa_restorer){ g_vSignalHandlers[nSigNumNorm].sa_restorer = a_handler->sa_restorer; }
	}

	signal(a_sig, &DefaultSystemSignalHandler);
	
	return TRUE;
}


REMOTE_FNC_LINKAGE BOOL initialize_local_signals(void)
{
	int i = 0;

	if (g_bLocalSignalsInitState == LSIS_INITED) return TRUE;
	else if((g_bLocalSignalsInitState== LSIS_CLEANING)||(g_bLocalSignalsInitState== LSIS_INITING)) return FALSE;

	g_bLocalSignalsInitState = LSIS_INITING;

	g_nNumberOfSignalWaiters = 0;
	g_SignalSemaphore = CreateSemaphore(0, 0, MAX_QUEUE_SIZE, 0);
	if (!g_SignalSemaphore) { g_bLocalSignalsInitState = LSIS_INITING_STOPPED; clean_local_signals(); return FALSE; }
	g_pFifoForSignal = new FifoSig;
	if (!g_pFifoForSignal) { g_bLocalSignalsInitState = LSIS_INITING_STOPPED; clean_local_signals(); return FALSE; }

	for (; i < _NSIG; ++i)
	{
		g_vSignalHandlers[i].sa_flags = SA_SIGINFO;
		g_vSignalHandlers[i].win_sa_sigaction = default_signal_handler;
		g_vSignalHandlers[i].sa_restorer = default_sa_restorer;//is_active
		g_vSignalHandlers[i].is_active = 1;
		signal(i, &DefaultSystemSignalHandler);
	}

	g_bLocalSignalsInitState = LSIS_INITED;

	return TRUE;
}


REMOTE_FNC_LINKAGE void clean_local_signals(void)
{
	if (g_bLocalSignalsInitState == LSIS_NOT_INITED) return;
	

	while ((g_bLocalSignalsInitState == LSIS_CLEANING) || (g_bLocalSignalsInitState == LSIS_INITING)){
		Sleep(1);
	}

	g_bLocalSignalsInitState = LSIS_CLEANING;


	if (g_SignalSemaphore) {
		CloseHandle(g_SignalSemaphore);
	}
	if(g_pFifoForSignal){
		SWinSignal aSig;

		while(g_nNumberOfSignalWaiters>0){
			--g_nNumberOfSignalWaiters;
			ReleaseSemaphore(g_SignalSemaphore, 1, 0);
		}

		while(g_pFifoForSignal->GetFirst(&aSig)){
		}
		delete g_pFifoForSignal;
		g_pFifoForSignal = NULL;
	} // if(g_pFifoForSignal){

	g_bLocalSignalsInitState = LSIS_NOT_INITED;
}


static void default_signal_handler(int a_sig, siginfo_t* a_info, void* a_data)
{

	if (g_vSignalHandlers[a_sig].is_active)
	{
		switch (a_sig)
		{
		case 0: /*echoing the app*/break;
		case SIGINT:  case SIGILL:   case SIGFPE: case SIGSEGV:
		case SIGTERM: case SIGBREAK: case SIGABRT:case SIGABRT_COMPAT:
			break;
		default:
			exit(3);
			break;
		}
	} // if (g_vSignalHandlers[a_sig].is_active)
}


static void DefaultSystemSignalHandler(int a_nSignal)
{
	SWinSignal aSignal;

	aSignal.m_isFromSystem = 1;
	aSignal.m_sig_num = a_nSignal;
	raise_on_remote((_uint64_ttt)&aSignal);
	signal(a_nSignal, &DefaultSystemSignalHandler);
}


#ifdef __cplusplus
}
#endif


FifoSig::FifoSig()
	:
	m_first(NULL),
	m_last(NULL)
{
	m_mutex = CreateMutex(NULL, FALSE, NULL);
}


FifoSig::~FifoSig()
{
	CloseHandle(m_mutex);
}


void FifoSig::Add(const SWinSignal& a_sig)
{
	SList* pNew = new SList(a_sig );

	WaitForSingleObject(m_mutex, INFINITE);
	
	if(m_last){m_last->next = pNew;}
	pNew->prev = m_last;
	m_last = pNew;
	if(!m_first){m_first=pNew;}

	ReleaseMutex(m_mutex);
}



bool FifoSig::GetFirst(SWinSignal* a_pSig)
{
	SList* pNew = NULL;
	bool bRet(false);

	WaitForSingleObject(m_mutex, INFINITE);

	if(m_first){
		pNew = m_first;
		m_first = m_first->next;
		if(m_first){
			m_first->prev = NULL;
		}
		bRet = true;
	}

	ReleaseMutex(m_mutex);

	if(bRet){
		*a_pSig = pNew->sig;
		delete pNew;
	}
	return bRet;
}
