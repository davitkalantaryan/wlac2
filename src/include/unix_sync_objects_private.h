
// unix_sync_objects_private.h
// 2017 Dec 27

#ifndef __unix_sync_objects_private_h__
#define __unix_sync_objects_private_h__


#include "first_includes/common_include_for_headers.h"
#include <windows.h>

__BEGIN_C_DECLS

struct pthread_mutex_s
{
	HANDLE mutex;
};

struct pthread_rwlock_s
{
	HANDLE mutex;
};


struct pthread_cond_s
{
	CONDITION_VARIABLE	cond;
	CRITICAL_SECTION	cs;
};


#define HANDLE_FROM_MUTEX(__mutex__)	((__mutex__)->mutex)


struct pthread_mutexattr_s
{
	__int64						unix_attrib_flag;
	char						unix_attributes[UNIX_ATTRIBS_LEN];
	struct _SECURITY_ATTRIBUTES	win_attributes;
};



union pthread_condattr_u
{
	char __size[__SIZEOF_PTHREAD_CONDATTR_T];
	int __align;
};

__END_C_DECLS



#endif  // #ifndef __unix_sync_objects_private_h__
