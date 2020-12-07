// 
// file:			thread.cpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//

#include <redesigned/std/thread/thread.hpp>

static int s_nCleanupStarted = 0;

class StdWlacThreadInitializeCleanup{
public:
	StdWlacThreadInitializeCleanup(){}
	~StdWlacThreadInitializeCleanup(){
		s_nCleanupStarted = 1;
	}
}static s_StdWlacThreadInitializeCleanup;

std::wlac_thread::wlac_thread() noexcept
	:
	m_handle(nullptr)
{
	m_state.allBits = 0;
}


std::wlac_thread::wlac_thread(wlac_thread&& a_other) noexcept
	:
	m_handle(a_other.m_handle)
{
	m_state.allBits = a_other.m_state.allBits;
	a_other.m_handle = nullptr;
	a_other.m_state.allBits = 0;
}


std::wlac_thread::~wlac_thread()
{
}


std::wlac_thread& std::wlac_thread::operator=(std::wlac_thread&& a_other) noexcept
{
	m_handle = a_other.m_handle;
	m_state.allBits = a_other.m_state.allBits;
	a_other.m_handle = nullptr;
	a_other.m_state.allBits = 0;
	return *this;
}


std::wlac_thread::native_handle_type std::wlac_thread::native_handle()
{
	return m_handle;
}


std::wlac_thread::id std::wlac_thread::get_id() const noexcept
{
#if (_MSVC_LANG>=201101L)
	std::wlac_thread::id return_id;
	*((_Thrd_id_t*)&return_id) = (_Thrd_id_t)m_state.st.threadId;
	return return_id;
#else
	return m_state.st.threadId;
#endif
}


void std::wlac_thread::detach()
{
	CloseHandle(m_handle);
	m_handle = nullptr;
	m_state.allBits = 0;
}


bool std::wlac_thread::joinable() const noexcept
{
	return m_handle ? true : false;
}


void std::wlac_thread::join()
{
	if(!m_handle){m_state.allBits = 0;return;}
	if((!m_state.st.started)&&((!m_state.st.shouldRun)||s_nCleanupStarted)){
		TerminateThread(m_handle,0);
		CloseHandle(m_handle);
		m_handle = nullptr;
		m_state.allBits = 0;
		return;
	}
	if(s_nCleanupStarted){
		while(!m_state.st.finished){
			Sleep(1);
		}
		CloseHandle(m_handle);
		m_handle = nullptr;
		m_state.allBits = 0;
		return;
	}

	WaitForSingleObject(m_handle,INFINITE);
}


/*///////////// globals  ////////////////////////////////////////*/
namespace std{

class ThreadModifier{
public:
	ThreadModifier(std::wlac_thread* a_pThread):m_pThread(a_pThread){m_pThread->m_state.st.started = 1;}
	~ThreadModifier(){m_pThread->m_state.st.finished = 1;}
	uint64_t shouldRun()const {return m_pThread->m_state.st.shouldRun;}
private:
	std::wlac_thread* m_pThread;
};

namespace threadN{

GEM_API_FAR DWORD WINAPI ThreadStartRoutine(LPVOID a_lpThreadParameter)
{
	SThreadArgStruct* pThreadArg = static_cast<SThreadArgStruct*>(a_lpThreadParameter);
	::std::ThreadModifier aModifier(pThreadArg->thisObj);

	if(aModifier.shouldRun()){
		try{
			(*pThreadArg->pFunc)();
		}
		catch(...){
			(*pThreadArg->functionCleaner)(pThreadArg->pFunc);
			free(pThreadArg);
			pThreadArg = nullptr;
			throw;
		}
	}

	(*pThreadArg->functionCleaner)(pThreadArg->pFunc);
	free(pThreadArg);
	return 0;
}

}}  // namespace doocs{namespace threadN{


/*///////////// this_thread  ////////////////////////////////////////*/

//namespace wlac_this_thread {
//
//std::wlac_thread::id get_id() noexcept
//{
//	return GetCurrentThreadId();
//}
//
//} // namespace this_thread{
