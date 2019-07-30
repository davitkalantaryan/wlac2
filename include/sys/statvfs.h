// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/statvfs.h> For WINDOWS MFC
 *
 *	Created on: Aug 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_sys_statvfs_h___
#define __win_sys_statvfs_h___

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

#ifndef __fsblkcnt64_t
#define __fsblkcnt64_t __int64
#endif

#ifndef __fsfilcnt64_t
#define __fsfilcnt64_t __int64
#endif

struct statvfs64
{
	unsigned long int f_bsize;
	unsigned long int f_frsize;
	__fsblkcnt64_t f_blocks;
	__fsblkcnt64_t f_bfree;
	__fsblkcnt64_t f_bavail;
	__fsfilcnt64_t f_files;
	__fsfilcnt64_t f_ffree;
	__fsfilcnt64_t f_favail;
	unsigned long int f_fsid;
#ifdef _STATVFSBUF_F_UNUSED
	int __f_unused;
#endif
	unsigned long int f_flag;
	unsigned long int f_namemax;
	int __f_spare[6];
};

#ifndef fsblkcnt_t_defined
#define fsblkcnt_t_defined
typedef size_t fsblkcnt_t;
#endif

#ifndef fsfilcnt_t_defined
#define fsfilcnt_t_defined
typedef size_t fsfilcnt_t;
#endif

struct statvfs {
	unsigned long  f_bsize;    /* file system block size */
	unsigned long  f_frsize;   /* fragment size */
	fsblkcnt_t     f_blocks;   /* size of fs in f_frsize units */
	fsblkcnt_t     f_bfree;    /* # free blocks */
	fsblkcnt_t     f_bavail;   /* # free blocks for unprivileged users */
	fsfilcnt_t     f_files;    /* # inodes */
	fsfilcnt_t     f_ffree;    /* # free inodes */
	fsfilcnt_t     f_favail;   /* # free inodes for unprivileged users */
	unsigned long  f_fsid;     /* file system ID */
	unsigned long  f_flag;     /* mount flags */
	unsigned long  f_namemax;  /* maximum filename length */
};

__BEGIN_C_DECLS

GEM_API_FAR int statvfs64(const char * __file, struct statvfs64 * __buf);
GEM_API_FAR int statvfs(const char *path, struct statvfs *buf);				// https://linux.die.net/man/2/statvfs

__END_C_DECLS

#endif  // #ifndef __win_sys_statvfs_h___
