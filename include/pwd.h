// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <pwd.h> For WINDOWS MFC
 *
 *	Created on: Jan 20, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_pwd_h__
#define __win_pwd_h__

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

/* The passwd structure.  */
struct passwd
{
	char *pw_name;		/* Username.  */
	char *pw_passwd;		/* Password.  */
	__uid_t pw_uid;		/* User ID.  */
	__gid_t pw_gid;		/* Group ID.  */
	char *pw_gecos;		/* Real name.  */
	char *pw_dir;			/* Home directory.  */
	char *pw_shell;		/* Shell program.  */
};

__BEGIN_C_DECLS

GEM_API_FAR int getpwnam_r(const char *name, struct passwd *pwd,
	char *buf, size_t buflen, struct passwd **result);
GEM_API_FAR struct passwd *getpwnam(const char *name);
GEM_API_FAR struct passwd *getpwuid(uid_t uid);
GEM_API_FAR int getpwuid_r(uid_t uid, struct passwd *pwd,char *buf, size_t buflen, struct passwd **result);

__END_C_DECLS


#endif  // #ifndef __win_pwd_h__
