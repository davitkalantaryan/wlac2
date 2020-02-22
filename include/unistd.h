// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <unistd.h> For WINDOWS MFC
 *
 *	Created on: Dec 10, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_unistd_h__
#define __win_unistd_h__

#include <first_includes/common_include_for_headers.h>
#include <sys/cdefs.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>  // for O_BINARY
#include <windows.h>
#include <sys/stat.h>
#include <direct.h>
#include <redesigned/sys/types.h>
#include <malloc.h>
#include <sched.h>
#include <unix_like_user_group_header.h>
#include <getopt.h>
#include <.tmp/wlac_temporary.h>

#ifndef pipe									// https://linux.die.net/man/2/pipe
#define pipe(_fds)	_pipe(_fds, 512, O_BINARY)	// https://msdn.microsoft.com/en-us/library/edze9h7e.aspx
#endif

#ifndef dirent
#define dirent _WIN32_FIND_DATAA
#endif
#if defined(d_name_to_cFileName_needed)  & !defined(d_name)
#ifndef d_name
#define d_name	cFileName
#endif
#endif
#if !defined(S_ISDIR) && !defined(S_ISDIR_not_needed)
//#define S_ISDIR(_a_path_)	(GetFileAttributesA((_a_path_)) & FILE_ATTRIBUTE_DIRECTORY)
#define S_ISDIR(_a_stat_st_mode_)	(S_IFDIR  &(_a_stat_st_mode_))
#endif

#ifndef getpid
#define getpid _getpid
#endif

#ifndef MAX_USER_NAME_LEN_MIN1
#define	MAX_USER_NAME_LEN_MIN1		127
#endif

#ifndef F_ULOCK
# define F_ULOCK 0	/* Unlock a previously locked region.  */
#endif
#ifndef F_LOCK
# define F_LOCK  1	/* Lock a region for exclusive use.  */
#endif
#ifndef F_TLOCK
# define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
#endif
#ifndef F_TEST
# define F_TEST  3	/* Test a region for other processes locks.  */
#endif

#ifdef lstat
#undef lstat
#endif
#define lstat stat

#ifndef S_IRUSR
#define	S_IRUSR	_S_IREAD	/* Read by owner.  */
#endif
#ifndef S_IWUSR
#define	S_IWUSR	_S_IWRITE	/* Write by owner.  */
#endif
#ifndef S_IXUSR
#define	S_IXUSR	_S_IEXEC	/* Execute by owner.  */
#endif
/* Read, write, and execute by owner.  */
#ifndef S_IRWXU
#define	S_IRWXU	(_S_IREAD|_S_IWRITE|_S_IEXEC)
#endif

#ifndef S_IRGRP
#define	S_IRGRP	(S_IRUSR >> 3)	/* Read by group.  */
#endif
#ifndef S_IWGRP
#define	S_IWGRP	(S_IWUSR >> 3)	/* Write by group.  */
#endif
#ifndef S_IXGRP
#define	S_IXGRP	(S_IXUSR >> 3)	/* Execute by group.  */
#endif
/* Read, write, and execute by group.  */
#ifndef S_IRWXG
#define	S_IRWXG	(S_IRWXU >> 3)
#endif

#ifndef S_IROTH
#define	S_IROTH	(S_IRGRP >> 3)	/* Read by others.  */
#endif
#ifndef S_IWOTH
#define	S_IWOTH	(S_IWGRP >> 3)	/* Write by others.  */
#endif
#ifndef S_IXOTH
#define	S_IXOTH	(S_IXGRP >> 3)	/* Execute by others.  */
#endif
/* Read, write, and execute by others.  */
#ifndef S_IRWXO
#define	S_IRWXO	(S_IRWXG >> 3)
#endif

// access flags
// https://msdn.microsoft.com/en-us/library/1w06ktdy.aspx
// https://linux.die.net/man/2/access
#ifndef F_OK
#define F_OK	0x0
#endif
#ifndef X_OK			// no X_OK for windows, this should be .exe .bat .com, to say X_OK true
#define X_OK	0x0		// so correct solution will be reimplement access function and in the case 
#endif					// if provided X_OK for check, check also file name
#ifndef W_OK
#define W_OK	0x2
#endif
#ifndef R_OK
#define R_OK	0x4
#endif

__BEGIN_C_DECLS

#if defined(_MSC_VER) & (_MSC_VER >= 1400)
#ifndef gmtime_r
#define gmtime_r(_a_time_,_a_tm_res_)	\
			(gmtime_s((_a_tm_res_),(_a_time_)) ? NULL : (_a_tm_res_))
#endif  // #ifndef gmtime_r
#else  // #if defined(_MSC_VER) & (_MSC_VER >= 1400)
GEM_API_FAR struct tm *gmtime_r(const time_t *timep, struct tm *result);
#endif // #if defined(_MSC_VER) & (_MSC_VER >= 1400)

#ifndef DIR_defined
typedef struct new_win_DIR{
	struct _WIN32_FIND_DATAA	find_data;
	HANDLE						dir_handle;
	int							already_in_use;
}DIR;
#define DIR_defined
#endif

GEM_API_FAR int pread(int a_fd, void *a_buf, size_t a_count, int a_offset);
GEM_API_FAR int pwrite(int a_fd, const void* a_buf, size_t a_count, int a_offset);
GEM_API_FAR int getpagesize(void);
GEM_API_FAR DIR *opendir(const char *name);
GEM_API_FAR int closedir(DIR * a_dirp);
GEM_API_FAR struct dirent *readdir(DIR *dirp);
GEM_API_FAR int lockf(int fd, int cmd, off_t len);
GEM_API_FAR int unsetenv(const char *a_name);
GEM_API_FAR long sysconf(int name);
GEM_API_FAR int getdtablesize(void);

// Implemented
GEM_API_FAR BOOL initialize_windows_socket_library(void);
GEM_API_FAR int close_common(int a_fd);
_DEPRICATED_TEXT_("Instead of this use 'readsocket' for sockets and 'readfile' for files for not having collision")
GEM_API_FAR int read_common(int fd, void *buf, size_t count);
_DEPRICATED_TEXT_("Instead of this use 'writesocket' for sockets and 'writefile' for files for not having collision")
GEM_API_FAR int write_common(int fd, const void *buf, size_t count);
GEM_API_FAR int usleep(int64_t a_usec);
//GEM_API_FAR char *getcwd(char *buf, size_t size); // implemented in <direct.h>
GEM_API_FAR int64_t syscall(int64_t __sysno, ...) __THROW;
GEM_API_FAR int fsync(int fildes);
GEM_API_FAR int daemon(int nochdir, int noclose);
GEM_API_FAR int setgid(gid_t gid);
GEM_API_FAR int setuid(uid_t uid);
GEM_API_FAR int setgroups(size_t size, const gid_t *list);

#ifndef geteuid
#define geteuid getuid
#endif
#ifndef getegid
#define getegid getgid
#endif

#ifndef readfile
#define readfile	_read
#endif // #ifndef readfile

#ifndef writefile
#define writefile	_write
#endif // #ifndef readfile

#ifndef readsocket
#define readsocket(_a_d_,_a_buf_,_a_size_)	recv((SOCKET)(_a_d_),(_a_buf_),(_a_size_),0)
#endif // #ifndef readsocket

#ifndef writesocket
#define writesocket(_a_d_,_a_buf_,_a_size_)	send((SOCKET)(_a_d_),(_a_buf_),(_a_size_),0)
#endif // #ifndef writesocket

#ifndef lseek
#define lseek _lseek
#endif

#ifndef sleep_not_needed
#ifdef sleep
#undef sleep
#endif
#define	sleep(_seconds_)	SleepEx((_seconds_)*1000,TRUE)
#endif // #ifndef sleep_not_needed

__END_C_DECLS

#if !defined(__cplusplus) | defined(read_write_close_needed)

#ifdef close
#undef close
#endif  //#ifdef close
#ifdef read
#undef read
#endif  // #ifdef read
#ifdef write
#undef write
#endif  // #ifdef write
#ifdef open
#undef open
#endif


#define read	read_common
#define write	write_common
#define	close	close_common
#define open _open

#endif // #if !defined(__cplusplus) | defined(read_write_close_needed)

/* Values for the argument to `sysconf'.  */
enum
{
	_SC_ARG_MAX,
#define	_SC_ARG_MAX			_SC_ARG_MAX
	_SC_CHILD_MAX,
#define	_SC_CHILD_MAX			_SC_CHILD_MAX
	_SC_CLK_TCK,
#define	_SC_CLK_TCK			_SC_CLK_TCK
	_SC_NGROUPS_MAX,
#define	_SC_NGROUPS_MAX			_SC_NGROUPS_MAX
	_SC_OPEN_MAX,
#define	_SC_OPEN_MAX			_SC_OPEN_MAX
	_SC_STREAM_MAX,
#define	_SC_STREAM_MAX			_SC_STREAM_MAX
	_SC_TZNAME_MAX,
#define	_SC_TZNAME_MAX			_SC_TZNAME_MAX
	_SC_JOB_CONTROL,
#define	_SC_JOB_CONTROL			_SC_JOB_CONTROL
	_SC_SAVED_IDS,
#define	_SC_SAVED_IDS			_SC_SAVED_IDS
	_SC_REALTIME_SIGNALS,
#define	_SC_REALTIME_SIGNALS		_SC_REALTIME_SIGNALS
	_SC_PRIORITY_SCHEDULING,
#define	_SC_PRIORITY_SCHEDULING		_SC_PRIORITY_SCHEDULING
	_SC_TIMERS,
#define	_SC_TIMERS			_SC_TIMERS
	_SC_ASYNCHRONOUS_IO,
#define	_SC_ASYNCHRONOUS_IO		_SC_ASYNCHRONOUS_IO
	_SC_PRIORITIZED_IO,
#define	_SC_PRIORITIZED_IO		_SC_PRIORITIZED_IO
	_SC_SYNCHRONIZED_IO,
#define	_SC_SYNCHRONIZED_IO		_SC_SYNCHRONIZED_IO
	_SC_FSYNC,
#define	_SC_FSYNC			_SC_FSYNC
	_SC_MAPPED_FILES,
#define	_SC_MAPPED_FILES		_SC_MAPPED_FILES
	_SC_MEMLOCK,
#define	_SC_MEMLOCK			_SC_MEMLOCK
	_SC_MEMLOCK_RANGE,
#define	_SC_MEMLOCK_RANGE		_SC_MEMLOCK_RANGE
	_SC_MEMORY_PROTECTION,
#define	_SC_MEMORY_PROTECTION		_SC_MEMORY_PROTECTION
	_SC_MESSAGE_PASSING,
#define	_SC_MESSAGE_PASSING		_SC_MESSAGE_PASSING
	_SC_SEMAPHORES,
#define	_SC_SEMAPHORES			_SC_SEMAPHORES
	_SC_SHARED_MEMORY_OBJECTS,
#define	_SC_SHARED_MEMORY_OBJECTS	_SC_SHARED_MEMORY_OBJECTS
	_SC_AIO_LISTIO_MAX,
#define	_SC_AIO_LISTIO_MAX		_SC_AIO_LISTIO_MAX
	_SC_AIO_MAX,
#define	_SC_AIO_MAX			_SC_AIO_MAX
	_SC_AIO_PRIO_DELTA_MAX,
#define	_SC_AIO_PRIO_DELTA_MAX		_SC_AIO_PRIO_DELTA_MAX
	_SC_DELAYTIMER_MAX,
#define	_SC_DELAYTIMER_MAX		_SC_DELAYTIMER_MAX
	_SC_MQ_OPEN_MAX,
#define	_SC_MQ_OPEN_MAX			_SC_MQ_OPEN_MAX
	_SC_MQ_PRIO_MAX,
#define	_SC_MQ_PRIO_MAX			_SC_MQ_PRIO_MAX
	_SC_VERSION,
#define	_SC_VERSION			_SC_VERSION
	_SC_PAGESIZE,
#define	_SC_PAGESIZE			_SC_PAGESIZE
#define	_SC_PAGE_SIZE			_SC_PAGESIZE
	_SC_RTSIG_MAX,
#define	_SC_RTSIG_MAX			_SC_RTSIG_MAX
	_SC_SEM_NSEMS_MAX,
#define	_SC_SEM_NSEMS_MAX		_SC_SEM_NSEMS_MAX
	_SC_SEM_VALUE_MAX,
#define	_SC_SEM_VALUE_MAX		_SC_SEM_VALUE_MAX
	_SC_SIGQUEUE_MAX,
#define	_SC_SIGQUEUE_MAX		_SC_SIGQUEUE_MAX
	_SC_TIMER_MAX,
#define	_SC_TIMER_MAX			_SC_TIMER_MAX

	/* Values for the argument to `sysconf'
	corresponding to _POSIX2_* symbols.  */
	_SC_BC_BASE_MAX,
#define	_SC_BC_BASE_MAX			_SC_BC_BASE_MAX
	_SC_BC_DIM_MAX,
#define	_SC_BC_DIM_MAX			_SC_BC_DIM_MAX
	_SC_BC_SCALE_MAX,
#define	_SC_BC_SCALE_MAX		_SC_BC_SCALE_MAX
	_SC_BC_STRING_MAX,
#define	_SC_BC_STRING_MAX		_SC_BC_STRING_MAX
	_SC_COLL_WEIGHTS_MAX,
#define	_SC_COLL_WEIGHTS_MAX		_SC_COLL_WEIGHTS_MAX
	_SC_EQUIV_CLASS_MAX,
#define	_SC_EQUIV_CLASS_MAX		_SC_EQUIV_CLASS_MAX
	_SC_EXPR_NEST_MAX,
#define	_SC_EXPR_NEST_MAX		_SC_EXPR_NEST_MAX
	_SC_LINE_MAX,
#define	_SC_LINE_MAX			_SC_LINE_MAX
	_SC_RE_DUP_MAX,
#define	_SC_RE_DUP_MAX			_SC_RE_DUP_MAX
	_SC_CHARCLASS_NAME_MAX,
#define	_SC_CHARCLASS_NAME_MAX		_SC_CHARCLASS_NAME_MAX

	_SC_2_VERSION,
#define	_SC_2_VERSION			_SC_2_VERSION
	_SC_2_C_BIND,
#define	_SC_2_C_BIND			_SC_2_C_BIND
	_SC_2_C_DEV,
#define	_SC_2_C_DEV			_SC_2_C_DEV
	_SC_2_FORT_DEV,
#define	_SC_2_FORT_DEV			_SC_2_FORT_DEV
	_SC_2_FORT_RUN,
#define	_SC_2_FORT_RUN			_SC_2_FORT_RUN
	_SC_2_SW_DEV,
#define	_SC_2_SW_DEV			_SC_2_SW_DEV
	_SC_2_LOCALEDEF,
#define	_SC_2_LOCALEDEF			_SC_2_LOCALEDEF

	_SC_PII,
#define	_SC_PII				_SC_PII
	_SC_PII_XTI,
#define	_SC_PII_XTI			_SC_PII_XTI
	_SC_PII_SOCKET,
#define	_SC_PII_SOCKET			_SC_PII_SOCKET
	_SC_PII_INTERNET,
#define	_SC_PII_INTERNET		_SC_PII_INTERNET
	_SC_PII_OSI,
#define	_SC_PII_OSI			_SC_PII_OSI
	_SC_POLL,
#define	_SC_POLL			_SC_POLL
	_SC_SELECT,
#define	_SC_SELECT			_SC_SELECT
	_SC_UIO_MAXIOV,
#define	_SC_UIO_MAXIOV			_SC_UIO_MAXIOV
	_SC_IOV_MAX = _SC_UIO_MAXIOV,
#define _SC_IOV_MAX			_SC_IOV_MAX
	_SC_PII_INTERNET_STREAM,
#define	_SC_PII_INTERNET_STREAM		_SC_PII_INTERNET_STREAM
	_SC_PII_INTERNET_DGRAM,
#define	_SC_PII_INTERNET_DGRAM		_SC_PII_INTERNET_DGRAM
	_SC_PII_OSI_COTS,
#define	_SC_PII_OSI_COTS		_SC_PII_OSI_COTS
	_SC_PII_OSI_CLTS,
#define	_SC_PII_OSI_CLTS		_SC_PII_OSI_CLTS
	_SC_PII_OSI_M,
#define	_SC_PII_OSI_M			_SC_PII_OSI_M
	_SC_T_IOV_MAX,
#define	_SC_T_IOV_MAX			_SC_T_IOV_MAX

	/* Values according to POSIX 1003.1c (POSIX threads).  */
	_SC_THREADS,
#define	_SC_THREADS			_SC_THREADS
	_SC_THREAD_SAFE_FUNCTIONS,
#define _SC_THREAD_SAFE_FUNCTIONS	_SC_THREAD_SAFE_FUNCTIONS
	_SC_GETGR_R_SIZE_MAX,
#define	_SC_GETGR_R_SIZE_MAX		_SC_GETGR_R_SIZE_MAX
	_SC_GETPW_R_SIZE_MAX,
#define	_SC_GETPW_R_SIZE_MAX		_SC_GETPW_R_SIZE_MAX
	_SC_LOGIN_NAME_MAX,
#define	_SC_LOGIN_NAME_MAX		_SC_LOGIN_NAME_MAX
	_SC_TTY_NAME_MAX,
#define	_SC_TTY_NAME_MAX		_SC_TTY_NAME_MAX
	_SC_THREAD_DESTRUCTOR_ITERATIONS,
#define	_SC_THREAD_DESTRUCTOR_ITERATIONS _SC_THREAD_DESTRUCTOR_ITERATIONS
	_SC_THREAD_KEYS_MAX,
#define	_SC_THREAD_KEYS_MAX		_SC_THREAD_KEYS_MAX
	_SC_THREAD_STACK_MIN,
#define	_SC_THREAD_STACK_MIN		_SC_THREAD_STACK_MIN
	_SC_THREAD_THREADS_MAX,
#define	_SC_THREAD_THREADS_MAX		_SC_THREAD_THREADS_MAX
	_SC_THREAD_ATTR_STACKADDR,
#define	_SC_THREAD_ATTR_STACKADDR	_SC_THREAD_ATTR_STACKADDR
	_SC_THREAD_ATTR_STACKSIZE,
#define	_SC_THREAD_ATTR_STACKSIZE	_SC_THREAD_ATTR_STACKSIZE
	_SC_THREAD_PRIORITY_SCHEDULING,
#define	_SC_THREAD_PRIORITY_SCHEDULING	_SC_THREAD_PRIORITY_SCHEDULING
	_SC_THREAD_PRIO_INHERIT,
#define	_SC_THREAD_PRIO_INHERIT		_SC_THREAD_PRIO_INHERIT
	_SC_THREAD_PRIO_PROTECT,
#define	_SC_THREAD_PRIO_PROTECT		_SC_THREAD_PRIO_PROTECT
	_SC_THREAD_PROCESS_SHARED,
#define	_SC_THREAD_PROCESS_SHARED	_SC_THREAD_PROCESS_SHARED

	_SC_NPROCESSORS_CONF,
#define _SC_NPROCESSORS_CONF		_SC_NPROCESSORS_CONF
	_SC_NPROCESSORS_ONLN,
#define _SC_NPROCESSORS_ONLN		_SC_NPROCESSORS_ONLN
	_SC_PHYS_PAGES,
#define _SC_PHYS_PAGES			_SC_PHYS_PAGES
	_SC_AVPHYS_PAGES,
#define _SC_AVPHYS_PAGES		_SC_AVPHYS_PAGES
	_SC_ATEXIT_MAX,
#define _SC_ATEXIT_MAX			_SC_ATEXIT_MAX
	_SC_PASS_MAX,
#define _SC_PASS_MAX			_SC_PASS_MAX

	_SC_XOPEN_VERSION,
#define _SC_XOPEN_VERSION		_SC_XOPEN_VERSION
	_SC_XOPEN_XCU_VERSION,
#define _SC_XOPEN_XCU_VERSION		_SC_XOPEN_XCU_VERSION
	_SC_XOPEN_UNIX,
#define _SC_XOPEN_UNIX			_SC_XOPEN_UNIX
	_SC_XOPEN_CRYPT,
#define _SC_XOPEN_CRYPT			_SC_XOPEN_CRYPT
	_SC_XOPEN_ENH_I18N,
#define _SC_XOPEN_ENH_I18N		_SC_XOPEN_ENH_I18N
	_SC_XOPEN_SHM,
#define _SC_XOPEN_SHM			_SC_XOPEN_SHM

	_SC_2_CHAR_TERM,
#define _SC_2_CHAR_TERM			_SC_2_CHAR_TERM
	_SC_2_C_VERSION,
#define _SC_2_C_VERSION			_SC_2_C_VERSION
	_SC_2_UPE,
#define _SC_2_UPE			_SC_2_UPE

	_SC_XOPEN_XPG2,
#define _SC_XOPEN_XPG2			_SC_XOPEN_XPG2
	_SC_XOPEN_XPG3,
#define _SC_XOPEN_XPG3			_SC_XOPEN_XPG3
	_SC_XOPEN_XPG4,
#define _SC_XOPEN_XPG4			_SC_XOPEN_XPG4

	_SC_CHAR_BIT,
#define	_SC_CHAR_BIT			_SC_CHAR_BIT
	_SC_CHAR_MAX,
#define	_SC_CHAR_MAX			_SC_CHAR_MAX
	_SC_CHAR_MIN,
#define	_SC_CHAR_MIN			_SC_CHAR_MIN
	_SC_INT_MAX,
#define	_SC_INT_MAX			_SC_INT_MAX
	_SC_INT_MIN,
#define	_SC_INT_MIN			_SC_INT_MIN
	_SC_LONG_BIT,
#define	_SC_LONG_BIT			_SC_LONG_BIT
	_SC_WORD_BIT,
#define	_SC_WORD_BIT			_SC_WORD_BIT
	_SC_MB_LEN_MAX,
#define	_SC_MB_LEN_MAX			_SC_MB_LEN_MAX
	_SC_NZERO,
#define	_SC_NZERO			_SC_NZERO
	_SC_SSIZE_MAX,
#define	_SC_SSIZE_MAX			_SC_SSIZE_MAX
	_SC_SCHAR_MAX,
#define	_SC_SCHAR_MAX			_SC_SCHAR_MAX
	_SC_SCHAR_MIN,
#define	_SC_SCHAR_MIN			_SC_SCHAR_MIN
	_SC_SHRT_MAX,
#define	_SC_SHRT_MAX			_SC_SHRT_MAX
	_SC_SHRT_MIN,
#define	_SC_SHRT_MIN			_SC_SHRT_MIN
	_SC_UCHAR_MAX,
#define	_SC_UCHAR_MAX			_SC_UCHAR_MAX
	_SC_UINT_MAX,
#define	_SC_UINT_MAX			_SC_UINT_MAX
	_SC_ULONG_MAX,
#define	_SC_ULONG_MAX			_SC_ULONG_MAX
	_SC_USHRT_MAX,
#define	_SC_USHRT_MAX			_SC_USHRT_MAX

	_SC_NL_ARGMAX,
#define	_SC_NL_ARGMAX			_SC_NL_ARGMAX
	_SC_NL_LANGMAX,
#define	_SC_NL_LANGMAX			_SC_NL_LANGMAX
	_SC_NL_MSGMAX,
#define	_SC_NL_MSGMAX			_SC_NL_MSGMAX
	_SC_NL_NMAX,
#define	_SC_NL_NMAX			_SC_NL_NMAX
	_SC_NL_SETMAX,
#define	_SC_NL_SETMAX			_SC_NL_SETMAX
	_SC_NL_TEXTMAX,
#define	_SC_NL_TEXTMAX			_SC_NL_TEXTMAX

	_SC_XBS5_ILP32_OFF32,
#define _SC_XBS5_ILP32_OFF32		_SC_XBS5_ILP32_OFF32
	_SC_XBS5_ILP32_OFFBIG,
#define _SC_XBS5_ILP32_OFFBIG		_SC_XBS5_ILP32_OFFBIG
	_SC_XBS5_LP64_OFF64,
#define _SC_XBS5_LP64_OFF64		_SC_XBS5_LP64_OFF64
	_SC_XBS5_LPBIG_OFFBIG,
#define _SC_XBS5_LPBIG_OFFBIG		_SC_XBS5_LPBIG_OFFBIG

	_SC_XOPEN_LEGACY,
#define _SC_XOPEN_LEGACY		_SC_XOPEN_LEGACY
	_SC_XOPEN_REALTIME,
#define _SC_XOPEN_REALTIME		_SC_XOPEN_REALTIME
	_SC_XOPEN_REALTIME_THREADS,
#define _SC_XOPEN_REALTIME_THREADS	_SC_XOPEN_REALTIME_THREADS

	_SC_ADVISORY_INFO,
#define _SC_ADVISORY_INFO		_SC_ADVISORY_INFO
	_SC_BARRIERS,
#define _SC_BARRIERS			_SC_BARRIERS
	_SC_BASE,
#define _SC_BASE			_SC_BASE
	_SC_C_LANG_SUPPORT,
#define _SC_C_LANG_SUPPORT		_SC_C_LANG_SUPPORT
	_SC_C_LANG_SUPPORT_R,
#define _SC_C_LANG_SUPPORT_R		_SC_C_LANG_SUPPORT_R
	_SC_CLOCK_SELECTION,
#define _SC_CLOCK_SELECTION		_SC_CLOCK_SELECTION
	_SC_CPUTIME,
#define _SC_CPUTIME			_SC_CPUTIME
	_SC_THREAD_CPUTIME,
#define _SC_THREAD_CPUTIME		_SC_THREAD_CPUTIME
	_SC_DEVICE_IO,
#define _SC_DEVICE_IO			_SC_DEVICE_IO
	_SC_DEVICE_SPECIFIC,
#define _SC_DEVICE_SPECIFIC		_SC_DEVICE_SPECIFIC
	_SC_DEVICE_SPECIFIC_R,
#define _SC_DEVICE_SPECIFIC_R		_SC_DEVICE_SPECIFIC_R
	_SC_FD_MGMT,
#define _SC_FD_MGMT			_SC_FD_MGMT
	_SC_FIFO,
#define _SC_FIFO			_SC_FIFO
	_SC_PIPE,
#define _SC_PIPE			_SC_PIPE
	_SC_FILE_ATTRIBUTES,
#define _SC_FILE_ATTRIBUTES		_SC_FILE_ATTRIBUTES
	_SC_FILE_LOCKING,
#define _SC_FILE_LOCKING		_SC_FILE_LOCKING
	_SC_FILE_SYSTEM,
#define _SC_FILE_SYSTEM			_SC_FILE_SYSTEM
	_SC_MONOTONIC_CLOCK,
#define _SC_MONOTONIC_CLOCK		_SC_MONOTONIC_CLOCK
	_SC_MULTI_PROCESS,
#define _SC_MULTI_PROCESS		_SC_MULTI_PROCESS
	_SC_SINGLE_PROCESS,
#define _SC_SINGLE_PROCESS		_SC_SINGLE_PROCESS
	_SC_NETWORKING,
#define _SC_NETWORKING			_SC_NETWORKING
	_SC_READER_WRITER_LOCKS,
#define _SC_READER_WRITER_LOCKS		_SC_READER_WRITER_LOCKS
	_SC_SPIN_LOCKS,
#define _SC_SPIN_LOCKS			_SC_SPIN_LOCKS
	_SC_REGEXP,
#define _SC_REGEXP			_SC_REGEXP
	_SC_REGEX_VERSION,
#define _SC_REGEX_VERSION		_SC_REGEX_VERSION
	_SC_SHELL,
#define _SC_SHELL			_SC_SHELL
	_SC_SIGNALS,
#define _SC_SIGNALS			_SC_SIGNALS
	_SC_SPAWN,
#define _SC_SPAWN			_SC_SPAWN
	_SC_SPORADIC_SERVER,
#define _SC_SPORADIC_SERVER		_SC_SPORADIC_SERVER
	_SC_THREAD_SPORADIC_SERVER,
#define _SC_THREAD_SPORADIC_SERVER	_SC_THREAD_SPORADIC_SERVER
	_SC_SYSTEM_DATABASE,
#define _SC_SYSTEM_DATABASE		_SC_SYSTEM_DATABASE
	_SC_SYSTEM_DATABASE_R,
#define _SC_SYSTEM_DATABASE_R		_SC_SYSTEM_DATABASE_R
	_SC_TIMEOUTS,
#define _SC_TIMEOUTS			_SC_TIMEOUTS
	_SC_TYPED_MEMORY_OBJECTS,
#define _SC_TYPED_MEMORY_OBJECTS	_SC_TYPED_MEMORY_OBJECTS
	_SC_USER_GROUPS,
#define _SC_USER_GROUPS			_SC_USER_GROUPS
	_SC_USER_GROUPS_R,
#define _SC_USER_GROUPS_R		_SC_USER_GROUPS_R
	_SC_2_PBS,
#define _SC_2_PBS			_SC_2_PBS
	_SC_2_PBS_ACCOUNTING,
#define _SC_2_PBS_ACCOUNTING		_SC_2_PBS_ACCOUNTING
	_SC_2_PBS_LOCATE,
#define _SC_2_PBS_LOCATE		_SC_2_PBS_LOCATE
	_SC_2_PBS_MESSAGE,
#define _SC_2_PBS_MESSAGE		_SC_2_PBS_MESSAGE
	_SC_2_PBS_TRACK,
#define _SC_2_PBS_TRACK			_SC_2_PBS_TRACK
	_SC_SYMLOOP_MAX,
#define _SC_SYMLOOP_MAX			_SC_SYMLOOP_MAX
	_SC_STREAMS,
#define _SC_STREAMS			_SC_STREAMS
	_SC_2_PBS_CHECKPOINT,
#define _SC_2_PBS_CHECKPOINT		_SC_2_PBS_CHECKPOINT

	_SC_V6_ILP32_OFF32,
#define _SC_V6_ILP32_OFF32		_SC_V6_ILP32_OFF32
	_SC_V6_ILP32_OFFBIG,
#define _SC_V6_ILP32_OFFBIG		_SC_V6_ILP32_OFFBIG
	_SC_V6_LP64_OFF64,
#define _SC_V6_LP64_OFF64		_SC_V6_LP64_OFF64
	_SC_V6_LPBIG_OFFBIG,
#define _SC_V6_LPBIG_OFFBIG		_SC_V6_LPBIG_OFFBIG

	_SC_HOST_NAME_MAX,
#define _SC_HOST_NAME_MAX		_SC_HOST_NAME_MAX
	_SC_TRACE,
#define _SC_TRACE			_SC_TRACE
	_SC_TRACE_EVENT_FILTER,
#define _SC_TRACE_EVENT_FILTER		_SC_TRACE_EVENT_FILTER
	_SC_TRACE_INHERIT,
#define _SC_TRACE_INHERIT		_SC_TRACE_INHERIT
	_SC_TRACE_LOG,
#define _SC_TRACE_LOG			_SC_TRACE_LOG

	_SC_LEVEL1_ICACHE_SIZE,
#define _SC_LEVEL1_ICACHE_SIZE		_SC_LEVEL1_ICACHE_SIZE
	_SC_LEVEL1_ICACHE_ASSOC,
#define _SC_LEVEL1_ICACHE_ASSOC		_SC_LEVEL1_ICACHE_ASSOC
	_SC_LEVEL1_ICACHE_LINESIZE,
#define _SC_LEVEL1_ICACHE_LINESIZE	_SC_LEVEL1_ICACHE_LINESIZE
	_SC_LEVEL1_DCACHE_SIZE,
#define _SC_LEVEL1_DCACHE_SIZE		_SC_LEVEL1_DCACHE_SIZE
	_SC_LEVEL1_DCACHE_ASSOC,
#define _SC_LEVEL1_DCACHE_ASSOC		_SC_LEVEL1_DCACHE_ASSOC
	_SC_LEVEL1_DCACHE_LINESIZE,
#define _SC_LEVEL1_DCACHE_LINESIZE	_SC_LEVEL1_DCACHE_LINESIZE
	_SC_LEVEL2_CACHE_SIZE,
#define _SC_LEVEL2_CACHE_SIZE		_SC_LEVEL2_CACHE_SIZE
	_SC_LEVEL2_CACHE_ASSOC,
#define _SC_LEVEL2_CACHE_ASSOC		_SC_LEVEL2_CACHE_ASSOC
	_SC_LEVEL2_CACHE_LINESIZE,
#define _SC_LEVEL2_CACHE_LINESIZE	_SC_LEVEL2_CACHE_LINESIZE
	_SC_LEVEL3_CACHE_SIZE,
#define _SC_LEVEL3_CACHE_SIZE		_SC_LEVEL3_CACHE_SIZE
	_SC_LEVEL3_CACHE_ASSOC,
#define _SC_LEVEL3_CACHE_ASSOC		_SC_LEVEL3_CACHE_ASSOC
	_SC_LEVEL3_CACHE_LINESIZE,
#define _SC_LEVEL3_CACHE_LINESIZE	_SC_LEVEL3_CACHE_LINESIZE
	_SC_LEVEL4_CACHE_SIZE,
#define _SC_LEVEL4_CACHE_SIZE		_SC_LEVEL4_CACHE_SIZE
	_SC_LEVEL4_CACHE_ASSOC,
#define _SC_LEVEL4_CACHE_ASSOC		_SC_LEVEL4_CACHE_ASSOC
	_SC_LEVEL4_CACHE_LINESIZE,
#define _SC_LEVEL4_CACHE_LINESIZE	_SC_LEVEL4_CACHE_LINESIZE
	/* Leave room here, maybe we need a few more cache levels some day.  */

	_SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50,
#define _SC_IPV6			_SC_IPV6
	_SC_RAW_SOCKETS,
#define _SC_RAW_SOCKETS			_SC_RAW_SOCKETS

	_SC_V7_ILP32_OFF32,
#define _SC_V7_ILP32_OFF32		_SC_V7_ILP32_OFF32
	_SC_V7_ILP32_OFFBIG,
#define _SC_V7_ILP32_OFFBIG		_SC_V7_ILP32_OFFBIG
	_SC_V7_LP64_OFF64,
#define _SC_V7_LP64_OFF64		_SC_V7_LP64_OFF64
	_SC_V7_LPBIG_OFFBIG,
#define _SC_V7_LPBIG_OFFBIG		_SC_V7_LPBIG_OFFBIG

	_SC_SS_REPL_MAX,
#define _SC_SS_REPL_MAX			_SC_SS_REPL_MAX

	_SC_TRACE_EVENT_NAME_MAX,
#define _SC_TRACE_EVENT_NAME_MAX	_SC_TRACE_EVENT_NAME_MAX
	_SC_TRACE_NAME_MAX,
#define _SC_TRACE_NAME_MAX		_SC_TRACE_NAME_MAX
	_SC_TRACE_SYS_MAX,
#define _SC_TRACE_SYS_MAX		_SC_TRACE_SYS_MAX
	_SC_TRACE_USER_EVENT_MAX,
#define _SC_TRACE_USER_EVENT_MAX	_SC_TRACE_USER_EVENT_MAX

	_SC_XOPEN_STREAMS,
#define _SC_XOPEN_STREAMS		_SC_XOPEN_STREAMS

	_SC_THREAD_ROBUST_PRIO_INHERIT,
#define _SC_THREAD_ROBUST_PRIO_INHERIT	_SC_THREAD_ROBUST_PRIO_INHERIT
	_SC_THREAD_ROBUST_PRIO_PROTECT
#define _SC_THREAD_ROBUST_PRIO_PROTECT	_SC_THREAD_ROBUST_PRIO_PROTECT
};

#endif  /* #ifndef __win_unistd_h__ */
