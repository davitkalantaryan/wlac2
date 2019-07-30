// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <sys/vfs.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * 
 */
#ifndef __win_sys_vfs_h__
#define __win_sys_vfs_h__

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>
#include <bits/mman.h>

__BEGIN_C_DECLS


#if !defined(__fsword_t_defined) && !defined(__fsword_t)
typedef unsigned int __fsword_t;
#define __fsword_t_defined
#endif

struct statfs
{
	__fsword_t f_type;
	__fsword_t f_bsize;
#ifndef __USE_FILE_OFFSET64
	__fsblkcnt_t f_blocks;
	__fsblkcnt_t f_bfree;
	__fsblkcnt_t f_bavail;
	__fsfilcnt_t f_files;
	__fsfilcnt_t f_ffree;
#else
	__fsblkcnt64_t f_blocks;
	__fsblkcnt64_t f_bfree;
	__fsblkcnt64_t f_bavail;
	__fsfilcnt64_t f_files;
	__fsfilcnt64_t f_ffree;
#endif
	__fsid_t f_fsid;
	__fsword_t f_namelen;
	__fsword_t f_frsize;
	__fsword_t f_flags;
	__fsword_t f_spare[4];
};

//GEM_API_FAR int semget(key_t key, int nsems, int semflg);

__END_C_DECLS

#endif  /* #ifndef __win_sys_vfs_h__ */
