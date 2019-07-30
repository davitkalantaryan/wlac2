// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <grp.h> For WINDOWS MFC
 *
 *	Created on: Dec 21, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_grp_h__
#define __win_grp_h__

#include <first_includes/common_include_for_headers.h>
#include <sys/cdefs.h>
#include <redesigned/sys/types.h>
#include <unix_like_user_group_header.h>


__BEGIN_C_DECLS

struct group {
	char   *gr_name;       /* group name */
	char   *gr_passwd;     /* group password */
	gid_t   gr_gid;        /* group ID */
	char  **gr_mem;        /* group members */
};

GEM_API_FAR int getgrgid_r(gid_t gid, struct group *grp,char *buf, size_t buflen, struct group **result);

__END_C_DECLS


#endif  // #ifndef __win_grp_h__
