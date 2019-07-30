

// pthread_private_for_source.h
// 2017 Dec 27

#ifndef pthread_private_for_source_h__
#define pthread_private_for_source_h__

#include "first_includes/common_include_for_headers.h"
#include <windows.h>
#include <stddef.h>

__BEGIN_C_DECLS

struct pthread_s
{
	HANDLE			thrd;
	char*			threadName;
	char*			resourse;
	void*			reserved;
	start_routine_t	func;
	void*			arg;
	DWORD			resourseSize;
	DWORD			thrdID;
};


struct pthread_attr_s
{
	size_t m_attrib;
};

extern DWORD g_tlsPthreadDataKey;

pthread_t CreateNewThreadData(int isCurrentThread);
void ProperlyRemoveTlsInCurrentThread(void* a_pRet);
int SetThreadNameForDebugger(pthread_t a_target_thread,const char *a_name);

__END_C_DECLS



#endif  // #ifndef pthread_private_for_source_h__
