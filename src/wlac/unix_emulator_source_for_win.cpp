/*
*	File: unix_emulator_sources_for_win_c.c
*
*	Created on: Dec 10, 2015
*	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
*
* //
*
*
*/

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <signal.h>
#include <sys/time.h>
#include <io.h>
#include <malloc.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <rfc/remote_function_caller.h>
#include <signal.h>
#include "pthread_private_for_source.h"

static _inline void* map_and_unmap_private(int a_is_map, void *__addr, uint64_t __len, int __prot, int __flags, int __fd, __off64_t __offset);

__BEGIN_C_DECLS

GEM_API int sigaction(int a_sig, const struct sigaction *RESTRICT a_act, struct sigaction *RESTRICT a_oact) __THROW;
GEM_VAR void* g_pWlacLoadCode = NULL;
GEM_VAR void* g_pProcessExitCallCode = NULL;
GEM_VAR int	  g_nLibraryCleanupStarted = 1;
//DWORD	g_tlsPthreadDataKey = 0;
DWORD	g_nLoaderThreadTID;
int IsDescriptorASocket(int a_d);
static void default_sa_restorer(void) {}
static BOOL init_wlac_functions(HINSTANCE a_hMod, void* a_pReserved);


/////////////////////////////////////////////////////////////////////////////

GEM_API int getpagesize(void)
{
	SYSTEM_INFO systemInfo;
	systemInfo.dwPageSize = 0;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwPageSize;
}


GEM_API int sigemptyset(sigset_t *a_set) __THROW
{
	memset(a_set->__val, 0, sizeof(a_set->__val));
	return 0;
}


GEM_API int sigfillset(sigset_t *a_set)
{
	memset(a_set->__val, 1, sizeof(a_set->__val));
	return 0;
}



GEM_API signal_handler_simple_type wlac_signal(int a_sig, signal_handler_simple_type a_handler)
{
#ifdef signal
#undef signal
#endif
	struct sigaction aAction;
	signal_handler_simple_type fpRet = signal(a_sig, a_handler);
	
	aAction.sa_flags = 0;
	sigemptyset(&aAction.sa_mask);
	aAction.sa_restorer = &default_sa_restorer;
	aAction.sa_handler = a_handler;
	sigaction(a_sig, &aAction, NULL);
	return fpRet;
}


GEM_API int sigaction(int a_sig, const struct sigaction *RESTRICT a_act, struct sigaction *RESTRICT a_oact) __THROW
{
	signal_handler_struct aHandlerIn, aHandlerOut;
	int nReturn = -1;
	BOOL bRetFromLib;

	if (a_act)
	{
		aHandlerIn.sa_flags = a_act->sa_flags;
		aHandlerIn.win_func_ptr = (void*)a_act->sa_handler;
		aHandlerIn.sa_restorer = a_act->sa_restorer;
		aHandlerIn.is_active = 1;
	}
	else {
		aHandlerIn.sa_flags = 0;
		aHandlerIn.win_func_ptr = (void*)SIG_DFL;
		aHandlerIn.sa_restorer = &default_sa_restorer;
		aHandlerIn.is_active = 0;
	}

	bRetFromLib = modify_signal_table(a_sig, &aHandlerIn, &aHandlerOut);

	if(!bRetFromLib){return -1;}

	if (a_act)
	{
		// suppress all masked signals
		int i=0;
		aHandlerIn.win_func_ptr = (void*)SIG_IGN;
		aHandlerIn.sa_restorer = &default_sa_restorer;
		aHandlerIn.is_active = 0;

		
		for (; i < _SIGSET_NWORDS; ++i){
			if ((a_act->sa_mask.__val)[i]){
				modify_signal_table(i, &aHandlerIn, NULL);
			}
		}  // for (; i < _SIGSET_NWORDS; ++i)
	}
	
	if (a_oact)
	{
		a_oact->sa_flags = aHandlerOut.sa_flags;
		a_oact->sa_handler = (__sighandler_t)aHandlerOut.win_func_ptr;
		a_oact->sa_restorer = aHandlerOut.sa_restorer;
	}
	
	return 0;
}


GEM_API int lockf(int a_fd, int a_cmd, off_t a_len)
{
	HANDLE hFile = (HANDLE)_get_osfhandle(a_fd);
	DWORD dwNumOfBytesLow = a_len & 0xffffffff;
	DWORD dwNumOfBytesHigh = /*a_len>>32*/0;
	OVERLAPPED sOverlapped;
	LONG lDistanceToMoveHigh = 0;
	BOOL bRet = TRUE;


	if (((long)((size_t)hFile)) == -1)
	{
		return -1;
	}

	sOverlapped.Offset = SetFilePointer(hFile, 0, &lDistanceToMoveHigh, FILE_BEGIN);
	sOverlapped.OffsetHigh = lDistanceToMoveHigh;

	switch (a_cmd)
	{
	case F_ULOCK:
		bRet = UnlockFileEx(hFile, 0, dwNumOfBytesLow, dwNumOfBytesHigh, &sOverlapped);
		break;
	case F_LOCK:
		bRet = LockFileEx(hFile, 0, 0, dwNumOfBytesLow, dwNumOfBytesHigh, &sOverlapped);
		break;
	case F_TLOCK:
		bRet = LockFileEx(hFile, LOCKFILE_FAIL_IMMEDIATELY, 0, dwNumOfBytesLow, dwNumOfBytesHigh, &sOverlapped);
		break;
	case F_TEST:
		bRet = LockFileEx(hFile, LOCKFILE_FAIL_IMMEDIATELY, 0, dwNumOfBytesLow, dwNumOfBytesHigh, &sOverlapped);
		UnlockFileEx(hFile, 0, dwNumOfBytesLow, dwNumOfBytesHigh, &sOverlapped);
		break;
	default:
		break;
	}

	return bRet ? 0 : -1;
}

GEM_API int read_common(int a_fd, void *a_buf, size_t a_count)
{
	//if (a_fd < 0)return a_fd; // WSANOTINITIALISED
	int nIsSocket = IsDescriptorASocket(a_fd);
	
	if (nIsSocket == 1)
		return recv((SOCKET)a_fd, (char*)a_buf, (int)a_count, 0);
	else if (nIsSocket == 0)
		return _read(a_fd, a_buf, (unsigned int)a_count);

	return 0;
}


GEM_API int write_common(int a_fd, const void *a_buf, size_t a_count)
{
	int nIsSocket = IsDescriptorASocket(a_fd);

	if (nIsSocket == 1)
		return send((SOCKET)a_fd, (const char*)a_buf, (int)a_count, 0);
	else if (nIsSocket == 0)
		return _write(a_fd, a_buf, (unsigned int)a_count);

	return 0;
}


GEM_API int close_common(int a_fd)
{
	int nIsSocket = IsDescriptorASocket(a_fd);

	if (nIsSocket == 1)
		return closesocket((SOCKET)a_fd);
	else if (nIsSocket == 0)
		return _close(a_fd);

	return 0;
}


GEM_API int wlac_poll(struct pollfd *a_fds, nfds_t a_nfds, int a_timeout)
{
	int nIsSocket = IsDescriptorASocket((int)a_fds[0].fd);

	if (nIsSocket == 1)
		return poll_sockets(a_fds, a_nfds, a_timeout);
	else if (nIsSocket == 0)
		return API_NOT_IMPLEMENTED;

	return API_NOT_IMPLEMENTED;
}

//////////////////////////////////////////////////////////////////////////////

GEM_API int strncasecmp(const char *a_s1, const char *a_s2, size_t a_n)
{
	size_t i = 0;
	int nC1, nC2;

	for (; i < a_n && a_s1[i] != 0 && a_s2[i] != 0; ++i)
	{
		nC1 = _toupper(a_s1[i]);
		nC2 = _toupper(a_s2[i]);
		if (nC2 != nC1) { return nC2 - nC1; }
	}

	return a_n ? ((int)(a_s2[i] - a_s1[i])) : 0;
}


GEM_API int strcasecmp(const char *a_s1, const char *a_s2)
{
	size_t unLen1 = strlen(a_s1);
	size_t unLen2 = strlen(a_s2);

	unLen1 = (unLen2 < unLen1) ? unLen2 : unLen1;
	return strncasecmp(a_s1, a_s2, unLen1);
}


GEM_API int thr_main(void)
{
	return GetCurrentThreadId() == g_nLoaderThreadTID;
}


GEM_API char *dirname(char *a_path)
{
	char* pcDelimer = strrchr(a_path,'\\');

	if (!pcDelimer)
	{
		pcDelimer = strrchr(a_path, '/');
		if (!pcDelimer) { return NULL; }
	}

	*pcDelimer = 0;
	return a_path;
}

GEM_API char *__xpg_basename(char *a_path)
{
	char* pcDelimer = strrchr(a_path, '\\');

	if (!pcDelimer)
	{
		pcDelimer = strrchr(a_path, '/');
		if (!pcDelimer) { return a_path; }
	}
	return pcDelimer + 1;
}


GEM_API DIR* opendir(const char* _a_path_)
{
	DIR* _pRet = (DIR*)calloc(1, sizeof(DIR));
	if (!_pRet) return NULL;
	_pRet->dir_handle = FindFirstFileA((_a_path_), &(_pRet->find_data));
	if (_pRet->dir_handle == INVALID_HANDLE_VALUE){ free(_pRet); return NULL; }
	return _pRet;
}


int closedir(DIR * a_dirp)
{
	if(!a_dirp){return -1;}
	FindClose(a_dirp->dir_handle); free(a_dirp);
	return 0;
}


GEM_API struct dirent *readdir(DIR *a_dirp)
{
	if (a_dirp->already_in_use){
		if (FindNextFileA(a_dirp->dir_handle, &a_dirp->find_data) == FALSE) return NULL;
		return &a_dirp->find_data;
	}else{
		a_dirp->already_in_use = 1;
		return &a_dirp->find_data;
	}
	return NULL;
}


GEM_API void *mmap64(void *__addr, size_t __len, int __prot,int __flags, int __fd, __off64_t __offset)
{
	return map_and_unmap_private(1, __addr,__len,__prot,__flags,__fd, __offset);
}



GEM_API int munmap(void *__addr, size_t __len) __THROW
{
	return (int)((size_t)map_and_unmap_private(0, 0, __len, 0, 0, 0, (__off64_t)0));
}


GEM_API int unsetenv(const char *a_name)
{
	size_t unStrLen;
	char* pcNewEnv;
	
	if (!a_name) return 0;
	unStrLen = strlen(a_name);
	if (!unStrLen) return 0;
	pcNewEnv = (char*)alloca(unStrLen + 2);
	snprintf(pcNewEnv, unStrLen, "%s=", a_name);
	_putenv(pcNewEnv);
	return 0;
}


GEM_API int64_t syscall(int64_t a_sysno, ...) __THROW
{
	switch (a_sysno)
	{
	case __NR_gettid:
		return (int64_t)GetCurrentThreadId();
	default:
		return (int64_t)-1;
	}

	return 0;
}


GEM_API int pthread_once_void_ptr(pthread_once_t *once_control, void(*init_routine)(void*), void* a_arg)
{
	//if (InterlockedCompareExchange(once_control, (pthread_once_t)1, PTHREAD_ONCE_INIT) == PTHREAD_ONCE_INIT)
	//if (InterlockedCompareExchangePointer(once_control, (pthread_once_t)1, PTHREAD_ONCE_INIT) == PTHREAD_ONCE_INIT)
	if (InterlockedCompareExchangePointerNew(once_control, (pthread_once_t)1, PTHREAD_ONCE_INIT) == PTHREAD_ONCE_INIT)

	{
		(*init_routine)(a_arg);
	}
	return 0;
}

static _inline void init_routine_caller(void* a_arg)
{
	void(*init_routine)(void) = (void(*)(void))a_arg;
	(*init_routine)();
}


GEM_API int pthread_once(pthread_once_t *once_control, void(*init_routine)(void))
{
	return pthread_once_void_ptr(once_control, init_routine_caller, init_routine);
}


GEM_API int pthread_key_create(pthread_key_t *key, void(*destructor)(void*))
{
	*key = (pthread_key_t)TlsAlloc();
	if (((DWORD)(*key)) != TLS_OUT_OF_INDEXES)
	{
		static struct key_struct* sKey_struct = NULL;
		return 0;
	}

	return ENOMEM;
}


int pthread_key_create_once_np(pthread_key_t *key, void(*destructor)(void*))
{
	return 0;
}



///////////////////////////////////////////////////////////////////////////////
//// Tool functions
int IsDescriptorASocket(int a_d)
{
	// Declarations
	fd_set rfds;
	int maxsd;
	int nSelectReturn;
	struct timeval	aTimeout;
	int nIsSocket;

	// First is to check descriptor
	if (a_d < 0)return 0;

	aTimeout.tv_sec = 0;
	aTimeout.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_SET((unsigned int)a_d, &rfds);
	maxsd = (int)(a_d + 1);
	nSelectReturn = select(maxsd, &rfds, &rfds, &rfds, &aTimeout);
	if (nSelectReturn == SOCKET_ERROR)
	{
		nSelectReturn = WSAGetLastError();
		if (nSelectReturn == WSANOTINITIALISED)
		{
			return WSANOTINITIALISED;
		}
	}
	nIsSocket = (nSelectReturn >= 0) ? 1 : 0;
	return nIsSocket;

}


static BOOL init_wlac_functions(HINSTANCE a_hMod, void* a_pReserved)
{
	lib_cons_dest_t aConstructDestruct = NULL;

	g_nLibraryCleanupStarted = 0;
	g_pWlacLoadCode = a_pReserved;
	g_pProcessExitCallCode = NULL;
	g_nLoaderThreadTID = GetCurrentThreadId();
	if (!initialize_windows_socket_library()) return FALSE;

	if(!a_hMod){a_hMod = GetModuleHandleA(NULL);}

	aConstructDestruct = (lib_cons_dest_t)GetProcAddress(a_hMod,
		__GET_FUNC_NAME__(LIBRARY_CONSTRUCTOR_FUNCTION));
	if (!aConstructDestruct) {
		aConstructDestruct = (lib_cons_dest_t)GetProcAddress(GetModuleHandleA(NULL),
			__GET_FUNC_NAME__(LIBRARY_CONSTRUCTOR_FUNCTION));
	}

	if (aConstructDestruct){
		(*aConstructDestruct)();
	}
	initialize_local_signals();

	return TRUE;
}


static void destroy_wlac_functions(HINSTANCE a_hMod, void* a_pReserved)
{
	// g_nProcessExitCalled

	lib_cons_dest_t aConstructDestruct = NULL;

	g_nLibraryCleanupStarted = 1;
	g_pProcessExitCallCode = a_pReserved;

	if(!a_hMod){a_hMod = GetModuleHandleA(NULL);}

	aConstructDestruct = (lib_cons_dest_t)GetProcAddress(a_hMod,
		__GET_FUNC_NAME__(LIBRARY_DESTRUCTOR_FUNCTION));
	if (!aConstructDestruct) {
		aConstructDestruct = (lib_cons_dest_t)GetProcAddress(GetModuleHandleA(NULL),
			__GET_FUNC_NAME__(LIBRARY_DESTRUCTOR_FUNCTION));
	}

	if (aConstructDestruct){
		(*aConstructDestruct)();
	}

	WSACleanup();
	g_nLoaderThreadTID = 0;

}

__END_C_DECLS


#include <vector>
#define stdn std

struct SPairs
{
	HANDLE	hMap;
	void*	address;
};

static _inline void* map_and_unmap_private(int a_is_map, void *__addr, uint64_t __len, int __prot, int __flags, int __fd, __off64_t __offset)
{

	static pthread_mutex_t sMutex = PTHREAD_MUTEX_INITIALIZER;
	static stdn::vector<SPairs> svEntries;  // Hash table with synchronisation mechanism should be introduced

	if (a_is_map)
	{
		SPairs aPair;
		DWORD dwMaximumSizeHigh, dwMaximumSizeLow;
		DWORD dwFileOffsetHigh, dwFileOffsetLow;
		void* pBuffer;
		HANDLE hMapFile;
		HANDLE hFile = (HANDLE)_get_osfhandle(__fd);

		if (((long int)((size_t)hFile)) == -1){ return NULL; }

		//int nPageAccess = PAGE_READONLY/*0x02*/ | PAGE_READWRITE | PAGE_WRITECOPY;
		//int nMapAccess = FILE_MAP_WRITE | FILE_MAP_READ | FILE_MAP_ALL_ACCESS;

		dwMaximumSizeLow = __len & 0xffffffff;
		dwMaximumSizeHigh = (__len >> 32) & 0xffffffff;

		hMapFile = CreateFileMappingA(hFile, NULL, __flags, dwMaximumSizeHigh, dwMaximumSizeLow, NULL);
		if (!hMapFile) return MAP_FAILED;

		dwFileOffsetLow = __offset & 0xffffffff;
		dwFileOffsetHigh = (__offset >> 32) & 0xffffffff;

		pBuffer = MapViewOfFile(hMapFile, __prot, dwFileOffsetHigh, dwFileOffsetLow, (DWORD)__len);
		if (!pBuffer) { CloseHandle(hMapFile); return MAP_FAILED; }
		aPair.hMap = hMapFile;
		aPair.address = pBuffer;
		pthread_mutex_lock(&sMutex);
		svEntries.push_back(aPair);
		pthread_mutex_unlock(&sMutex); /* end critical area */
		return pBuffer;
	}
	else
	{
		size_t i, unSize;
		int nIsFindet = 0;

		pthread_mutex_lock(&sMutex);
		unSize = svEntries.size();
		for (i = 0; i < unSize; ++i)
		{
			if (((char*)__addr) == ((char*)svEntries[i].address))
			{
				UnmapViewOfFile(__addr);
				CloseHandle(svEntries[i].hMap);
				svEntries.erase(svEntries.begin() + i);
				pthread_mutex_unlock(&sMutex); /* end critical area */
				return NULL;
			} // if (((char*)__addr) == ((char*)svEntries[i].address))
		} // for (i = 0; i < unSize; ++i)

		pthread_mutex_unlock(&sMutex); /* end critical area */
		return (void*)-1;
	}

	return MAP_FAILED;
}


#ifdef _USRDLL
__BEGIN_C_DECLS
BOOL WINAPI DllMain(HINSTANCE a_hinstDLL, DWORD fdwReason, LPVOID a_lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		return init_wlac_functions(a_hinstDLL,a_lpvReserved);
	case DLL_PROCESS_DETACH:
		destroy_wlac_functions(a_hinstDLL,a_lpvReserved);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		//ProperlyRemoveTlsInCurrentThread(NULL);
		break;
	default:
		break;
	}
	return TRUE;
}
__END_C_DECLS
#else  // #ifdef _USRDLL
class wlac_initer
{
public:
	wlac_initer() { init_wlac_functions((HINSTANCE)0, NULL); }
	~wlac_initer() { destroy_wlac_functions((HINSTANCE)0, NULL); }
};
static volatile wlac_initer s_gem_initer;
#endif  // #ifdef _USRDLL


