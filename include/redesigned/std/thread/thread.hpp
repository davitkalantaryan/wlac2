// 
// file:			thread.hpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//

#ifndef WIN_STD_THREAD_HPP
#define WIN_STD_THREAD_HPP

#ifndef _WIN32
#error Use this class from windows
#endif

#pragma include_alias( <thread>, <thread> )
#pragma include_alias( "thread", "thread" )
#include <mutex>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdint.h>
#include <first_includes/common_definations_wul.h>

#include <chrono>

#if (_MSVC_LANG>=201101L)
#include <thread>
#ifdef thread
#undef thread
#endif
typedef std::thread::id wlac_thread_id;
#else
typedef uint64_t wlac_thread_id;
#endif
#define thread	wlac_thread
#ifdef this_thread
#undef this_thread
#endif
//#define this_thread	wlac_this_thread


namespace std {

class ThreadModifier;

class GEM_API_FAR wlac_thread
{
	friend class ThreadModifier;
public:
	typedef HANDLE native_handle_type;
	typedef wlac_thread_id id;
	wlac_thread() noexcept;
	wlac_thread(const wlac_thread&) = delete;
	wlac_thread(wlac_thread&& other) noexcept;
	template< class Function, class... Args >
	explicit wlac_thread(Function&& f, Args&&... args);
	~wlac_thread();
	native_handle_type native_handle();
	void join();
	bool joinable() const noexcept;
	id get_id() const noexcept;
	void detach();
	wlac_thread& operator=(wlac_thread&& other) noexcept;

protected:
	native_handle_type	m_handle;
	union{
		struct {
			uint64_t	shouldRun : 1;
			uint64_t	started : 1;
			uint64_t	finished : 1;
			uint64_t	threadId : 32;
		}st;
		uint64_t	allBits;
	}m_state;
};


//namespace wlac_this_thread{
//std::wlac_thread::id get_id() noexcept;
//template< class Rep, class Period >
//void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration);
//} // namespace this_thread{


}  // namespace doocs {


#ifndef WIN_STD_THREAD_IMPL_HPP
#include "thread.impl.hpp"
#endif

#endif   // #ifndef DOOCS_THREAD_HPP
