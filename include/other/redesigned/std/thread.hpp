// 
// file:			doocs_thread.hpp
// created on:		2020 Mar 30
// created by:		D. Kalantaryan
//

#ifndef WIN_STD_THREAD_HPP
#define WIN_STD_THREAD_HPP

#ifdef __cplusplus

#ifdef DONT_USE_STD_THREAD

#ifndef _WIN32
#error Use this class from windows only
#endif

#pragma include_alias( <thread>, <thread> )
#pragma include_alias( "thread", "thread" )
#include <mutex>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdint.h>
#include "first_includes/common_definations_wul.h"


//#if (_MSVC_LANG>=201101L)
#include <thread>
#ifdef thread
#undef thread
#endif


namespace std {


typedef std::thread::id doocs_thread_id;
class ThreadModifier;
#define thread	doocs_thread

class GEM_API_FAR wlac_thread
{
	friend class ThreadModifier;
public:
	typedef HANDLE native_handle_type;
	typedef doocs_thread_id id;
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

public:
	struct Core{
		native_handle_type	handle;
		union {
			struct {
				uint64_t	shouldRun : 1;
				uint64_t	started : 1;
				uint64_t	finished : 1;
				uint64_t	threadId : 32;
			}st;
			uint64_t	allBits;
		} state;
	};

private:
	Core*	m_pCore;

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

#endif  // #ifdef DONT_USE_STD_THREAD

#endif // #ifdef __cplusplus

#endif   // #ifndef DOOCS_THREAD_HPP
