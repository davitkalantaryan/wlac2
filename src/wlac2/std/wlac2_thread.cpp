// 
// file:			wlac_thread.cpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//


#ifndef WLAC_THREAD_IS_NOT_NEEDED

#ifndef DONT_USE_STD_THREAD
#define DONT_USE_STD_THREAD
#endif

#include <redesigned/std/thread.hpp>
#include <new>
#include <stdlib.h>
#include <string.h>

//#ifdef DONT_USE_STD_THREAD

// making XCZ will force to call this function last during initialization
// and first during cleanup
#pragma section(".CRT$XCZ",read)
#define WLAC_INITIALIZER_C_RAW(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCZ")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
#if defined(_WIN64) || defined(_M_ARM)
#define WLAC_INITIALIZER_C(f) WLAC_INITIALIZER_C_RAW(f,"")
#else
#define WLAC_INITIALIZER_C(f) WLAC_INITIALIZER_C_RAW(f,"_")
#endif

static int s_nCleanupStarted = 0;
static int s_doocsThreadInitialized = 0;

extern "C"{
WLAC_INITIALIZER_C(WlacThreadFunctionsInit)
{
	if(s_doocsThreadInitialized){
		return;
	}
	s_nCleanupStarted = 0;
	s_doocsThreadInitialized = 1;
	atexit([]() {
		s_doocsThreadInitialized = 0;
		s_nCleanupStarted = 1;
	});
}
}

//class StdWlacThreadInitializeCleanup{
//public:
//	StdWlacThreadInitializeCleanup(){}
//	~StdWlacThreadInitializeCleanup(){
//		s_nCleanupStarted = 1;
//	}
//}static s_StdWlacThreadInitializeCleanup;

std::wlac_thread::wlac_thread() noexcept
{
	m_pCore = nullptr;
}


std::wlac_thread::wlac_thread(wlac_thread&& a_other) noexcept
{
	this->m_pCore = a_other.m_pCore;
	a_other.m_pCore = nullptr;
}


std::wlac_thread::~wlac_thread()
{
	free(m_pCore);
}


std::wlac_thread& std::wlac_thread::operator=(std::wlac_thread&& a_other) noexcept
{
	decltype(m_pCore) thisStatPtr = this->m_pCore;

	this->m_pCore = a_other.m_pCore;
	a_other.m_pCore = thisStatPtr;
	return *this;
}


std::wlac_thread::native_handle_type std::wlac_thread::native_handle()
{
	return m_pCore? m_pCore->handle:nullptr;
}


std::wlac_thread::id std::wlac_thread::get_id() const noexcept
{
#if (_MSVC_LANG>=201101L)
	std::wlac_thread::id return_id;
	*((_Thrd_id_t*)&return_id) = m_pCore ?(_Thrd_id_t)m_pCore->state.st.threadId:(_Thrd_id_t)0;
	return return_id;
#else
	return m_state.st.threadId;
#endif
}


void std::wlac_thread::detach()
{
	if(m_pCore){
		CloseHandle(m_pCore->handle);
		m_pCore->handle = nullptr;
		m_pCore->state.allBits = 0;
	}
}


bool std::wlac_thread::joinable() const noexcept
{
	return (m_pCore&& m_pCore->handle) ? true : false;
}


void std::wlac_thread::join()
{
	if(m_pCore){
		if (!m_pCore->handle) { m_pCore->state.allBits = 0; return; }
		if ((!m_pCore->state.st.started) && ((!m_pCore->state.st.shouldRun) || s_nCleanupStarted)) {
			TerminateThread(m_pCore->handle, 0);
			CloseHandle(m_pCore->handle);
			m_pCore->handle = nullptr;
			m_pCore->state.allBits = 0;
			return;
		}
		if (s_nCleanupStarted) {
			while (!m_pCore->state.st.finished) {
				Sleep(1);
			}
			CloseHandle(m_pCore->handle);
			m_pCore->handle = nullptr;
			m_pCore->state.allBits = 0;
			return;
		}

		WaitForSingleObject(m_pCore->handle, INFINITE);
	}
}


/*///////////// globals  ////////////////////////////////////////*/

/*///////////// this_thread  ////////////////////////////////////////*/

//namespace wlac_this_thread {
//
//std::wlac_thread::id get_id() noexcept
//{
//	return GetCurrentThreadId();
//}
//
//} // namespace this_thread{

#endif   // #ifdef DONT_USE_STD_THREAD
