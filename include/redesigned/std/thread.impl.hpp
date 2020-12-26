// 
// file:			doocs/thread.hpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//

#ifndef WIN_STD_THREAD_IMPL_HPP
#define WIN_STD_THREAD_IMPL_HPP

#ifdef DONT_USE_STD_THREAD

#ifndef WIN_STD_THREAD_HPP
#if defined(__INTELLISENSE__) || defined(QTCREATOR_USED)
#include "thread.hpp"
#else
#error do not include this header directly
#endif
#endif

#include <functional>
#include <stdlib.h>

//typedef std::function<void(sssproto::ErrorCode, const std::string)>	TypeErrorReportFunc;

namespace std{ namespace threadN{

//static void TestFunctionForType(int){}
//static auto s_fncVoid = std::bind(TestFunctionForType,1);
//typedef decltype(s_fncVoid)	TypeFunc;
//typedef std::function<void(void)>	TypeFunc;
//typedef decltype(std::bind([](int){},1))	TypeFunc;

template< class TypeFunction>
static DWORD WINAPI ThreadStartRoutine2(LPVOID a_lpThreadParameter);

template< class TypeFunction>
struct SThreadArgStruct {
	TypeFunction				func;
	std::wlac_thread::Core*	thisObjCore;
	void (*functionCleaner)(void*);
	SThreadArgStruct(TypeFunction&& a_func):func(a_func){}
	SThreadArgStruct(const TypeFunction& a_func):func(a_func){}
};

}}  // namespace std{namespace threadN{


template< class Function, class... Args >
std::wlac_thread::wlac_thread(Function&& a_f, Args&&... a_args)
{
	m_pCore = (decltype(m_pCore))malloc(sizeof(*m_pCore));
	if (!m_pCore) { throw std::bad_alloc(); }

	//auto bndRes = std::bind(a_f, a_args...);
	//typedef decltype(bndRes) TypeBndRes;
	//threadN::SThreadArgStruct<decltype(bndRes)>* pThreadArg = static_cast<threadN::SThreadArgStruct<decltype(bndRes)>*>(malloc(sizeof(threadN::SThreadArgStruct<decltype(bndRes)>)));
	threadN::SThreadArgStruct<decltype(std::bind(a_f, a_args...))>* pThreadArg = 
		new threadN::SThreadArgStruct<decltype(std::bind(a_f, a_args...))>(std::bind(a_f, a_args...) );
	
	m_pCore->state.allBits = 0;
	if(pThreadArg){
		DWORD threadId;
		//pThreadArg->func = bndRes;
		pThreadArg->thisObjCore = this->m_pCore;
		//pThreadArg->functionCleaner = [](void*a_fncPointer){delete static_cast<TypeBndRes*>(a_fncPointer);}  ;
		pThreadArg->functionCleaner = [](void* ) {};
		m_pCore->state.st.shouldRun = 1;
		m_pCore->handle=CreateThread(nullptr, 0, &threadN::ThreadStartRoutine2<decltype(pThreadArg->func)>,pThreadArg,0,&threadId);
		m_pCore->state.st.threadId = static_cast<uint64_t>(threadId);
	}
	else{
		// function is noexcept, so no exception
		m_pCore->handle = nullptr;
	}
}


namespace std{ namespace threadN{

class ThreadModifier{
public:
	ThreadModifier(std::wlac_thread::Core* a_pCore):m_pCore(a_pCore){ m_pCore->state.st.started = 1;}
	~ThreadModifier(){ m_pCore->state.st.finished = 1;}
	uint64_t shouldRun()const {return m_pCore->state.st.shouldRun;}
private:
	std::wlac_thread::Core* m_pCore;
};

template< class TypeFunction>
static DWORD WINAPI ThreadStartRoutine2(LPVOID a_lpThreadParameter)
{
	SThreadArgStruct<TypeFunction>* pThreadArg = static_cast<SThreadArgStruct<TypeFunction>*>(a_lpThreadParameter);
	ThreadModifier aModifier(pThreadArg->thisObjCore);

	if(aModifier.shouldRun()){
		try{
			//(*pThreadArg->pFunc)();
			std::invoke(pThreadArg->func);
		}
		catch(...){
			//(*pThreadArg->functionCleaner)(pThreadArg->pFunc);
			//free(pThreadArg);
			delete pThreadArg;
			pThreadArg = nullptr;
			throw;
		}
	}

	//(*pThreadArg->functionCleaner)(pThreadArg->pFunc);
	//free(pThreadArg);
	delete pThreadArg;
	pThreadArg = nullptr;
	return 0;
}

}}  // namespace std{ namespace threadN{


//namespace wlac_this_thread{
//#undef this_thread
//template< class Rep, class Period >
//void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
//{
//	this_thread::sleep_for(sleep_duration);
//}
//#define this_thread	wlac_this_thread
//} // namespace this_thread{

#endif  // #ifdef DONT_USE_STD_THREAD

#endif   // #ifndef WIN_STD_THREAD_IMPL_HPP
