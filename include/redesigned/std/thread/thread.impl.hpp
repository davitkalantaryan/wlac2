// 
// file:			doocs/thread.hpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//

#ifndef WIN_STD_THREAD_IMPL_HPP
#define WIN_STD_THREAD_IMPL_HPP

#ifndef WIN_STD_THREAD_HPP
#if defined(__INTELLISENSE__) || defined(QTCREATOR_USED)
#include "thread.hpp"
#else
#error do not include this header directly
#endif
#endif

#include <functional>
#include <stdlib.h>

namespace std{namespace threadN{
static void TestFunctionForType(){}
static auto s_fncVoid = std::bind(TestFunctionForType);
typedef decltype(s_fncVoid)	TypeFunc;
GEM_API_FAR DWORD WINAPI ThreadStartRoutine(LPVOID a_lpThreadParameter);
struct SThreadArgStruct {
	TypeFunc*			pFunc;
	std::wlac_thread*	thisObj;
	void (*functionCleaner)(void*);
};
}}  // namespace doocs{namespace threadN{


template< class Function, class... Args >
std::wlac_thread::wlac_thread(Function&& a_f, Args&&... a_args)
{
	threadN::SThreadArgStruct* pThreadArg = static_cast<threadN::SThreadArgStruct*>(malloc(sizeof(threadN::SThreadArgStruct)));
	m_state.allBits = 0;
	if(pThreadArg){
		DWORD threadId;
		auto bndRes = ::std::bind(a_f, a_args...);
		typedef decltype(bndRes) TypeBndRes;
		pThreadArg->pFunc = reinterpret_cast<threadN::TypeFunc*>(new TypeBndRes(bndRes) );
		pThreadArg->thisObj = this;
		pThreadArg->functionCleaner = [](void*a_fncPointer){delete static_cast<TypeBndRes*>(a_fncPointer);}  ;
		m_state.st.shouldRun = 1;
		m_handle=CreateThread(nullptr, 0, &threadN::ThreadStartRoutine,pThreadArg,0,&threadId);
		m_state.st.threadId = static_cast<uint64_t>(threadId);
	}
	else{
		// function is noexcept, so no exception
		m_handle = nullptr;
	}
}


//namespace wlac_this_thread{
//#undef this_thread
//template< class Rep, class Period >
//void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
//{
//	this_thread::sleep_for(sleep_duration);
//}
//#define this_thread	wlac_this_thread
//} // namespace this_thread{

#endif   // #ifndef WIN_STD_THREAD_IMPL_HPP
