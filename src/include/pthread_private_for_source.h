

// pthread_private_for_source.h
// 2017 Dec 27

#ifndef pthread_private_for_source_h__
#define pthread_private_for_source_h__

#include "first_includes/wlac_compiler_internal.h"
#include "first_includes/common_include_for_headers.h"
#include <Windows.h>
#include <stddef.h>
#include <stdint.h>

__BEGIN_C_DECLS

struct pthread_s_new
{
	HANDLE			thrd;
	char*			threadName;
	char*			resourse;
	void*			reserved;
	start_routine_t	func;
	void*			arg;
	DWORD			resourseSize;
	DWORD			thrdID;
	uint64_t		existOnThreadLocalStorage : 1;
	uint64_t		isAlive : 1;
	uint64_t		reserved64bit : 62;
};


struct pthread_attr_s{
	struct _SECURITY_ATTRIBUTES secAttr;
	SIZE_T                  dwStackSize;
	DWORD                   dwCreationFlags;
	DWORD					reserved4Byte;
};


//int SetThreadNameForDebugger(pthread_t a_target_thread,const char *a_name);
HIDE_SYMBOL2 struct pthread_s_new* GetCurrentThreadDataPointer(void);
HIDE_SYMBOL2 struct pthread_s_new* GetAnyThreadDataPointer(pthread_t a_anyThread);
HIDE_SYMBOL2 int SetThreadNameForDebugger(DWORD a_target_thread_id, const char* a_name);

__END_C_DECLS



#endif  // #ifndef pthread_private_for_source_h__
