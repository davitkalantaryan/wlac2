/*
 *	File: functions_on_remote.cpp
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
#include <exception>


#ifdef __cplusplus
extern "C"
{
#endif

extern LSIS					g_bLocalSignalsInitState;

REMOTE_FNC_LINKAGE __int64 freopen_std_new_on_remote(_uint64_ttt a_pArg)
{
	STDFREOPENDATA* pFreData = (STDFREOPENDATA*)((void*)((size_t)a_pArg));
	return (_uint64_ttt)((size_t)freopen(pFreData->file_name, pFreData->mode, s_cfpOutputs[pFreData->std_fd]));
}


REMOTE_FNC_LINKAGE __int64 raise_on_remote(_uint64_ttt a_pArg)
{
	win_signal_handler_type		pInitialHandler;
	struct SWinSignal* pSignal = (struct SWinSignal*)((void*)((size_t)a_pArg));
	int nSigNumNorm = pSignal->m_sig_num % _NSIG;

	switch (pSignal->m_sig_num)
	{
	case 0: return 0;
	//case SIGKILL: ExitProcess(3);
	case SIGKILL: 
		::std::terminate();
	default:break;
	}

	pSignal->sig_info.si_signo = pSignal->m_sig_num;

	if (g_bLocalSignalsInitState==LSIS_INITED){
		if (g_vSignalHandlers[nSigNumNorm].sa_flags == SA_SIGINFO)
		{
			(*g_vSignalHandlers[nSigNumNorm].win_sa_sigaction)(pSignal->m_sig_num, &(pSignal->sig_info), NULL);
		}
		else
		{
			(*g_vSignalHandlers[nSigNumNorm].win_sa_handler)(pSignal->m_sig_num);
		}

		if(g_nNumberOfSignalWaiters>0){
			--g_nNumberOfSignalWaiters;
			g_pFifoForSignal->Add(*pSignal);
			ReleaseSemaphore(g_SignalSemaphore, 1, 0);
		}

		pSignal->m_return2 = 0;
	}  // if (g_bLocalSignalsInitState==LSIS_INITED)
	else{
		switch (pSignal->m_sig_num)
		{
		case SIGINT:  case SIGILL:   case SIGFPE: case SIGSEGV:
		case SIGTERM: case SIGBREAK: case SIGABRT:case SIGABRT_COMPAT:
			//if (!(g_vSignalHandlers[nSigNumNorm].sa_flags&SA_RESETHAND))
			{
				//pSignal->m_return = raise(pSignal->m_sig_num);
			}
			//else
			{
				pInitialHandler = signal(pSignal->m_sig_num, SIG_GET);
				pSignal->m_return2 = raise(pSignal->m_sig_num);
				signal(pSignal->m_sig_num, pInitialHandler);
			}
			break;
		default:
			//ExitProcess(3);
			::std::terminate();
			break;
		}
	}  // else{ of if (g_bLocalSignalsInitState==LSIS_INITED)

	
	return 0;
}


REMOTE_FNC_LINKAGE __int64 printf_on_remote(_uint64_ttt a_pArg)
{
	char* pcStringToPrint = (char*)((void*)((size_t)a_pArg));
	return (__int64)printf("%s", pcStringToPrint);
}


REMOTE_FNC_LINKAGE __int64 print_error_on_remote(_uint64_ttt a_pArg)
{
	char* pcStringToPrint = (char*)((void*)((size_t)a_pArg));
	return (__int64)fprintf(stderr, "%s", pcStringToPrint);
}


REMOTE_FNC_LINKAGE __int64 perror_on_remote(_uint64_ttt a_pArg)
{
	char* pcStringToPrint = (char*)((void*)((size_t)a_pArg));
	perror(pcStringToPrint);
	return 0;
}


#ifdef __cplusplus
}
#endif
