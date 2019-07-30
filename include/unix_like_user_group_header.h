// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// unix_like_user_group_header.h
// 2017 Dec 30


#ifndef __unix_like_user_group_header_h__
#define __unix_like_user_group_header_h__

#include <first_includes/common_include_for_headers.h>
#include <redesigned/sys/types.h>
#include <stddef.h>

#define	MAX_NUMBER_OF_GROUPS	32
/* For the Single Unix specification we must define this type here.  */
#if (defined(__USE_XOPEN) || defined(__USE_XOPEN2K)) && !defined(__gid_t_defined) && defined(____gid_t_defined)
typedef __gid_t gid_t;
# define __gid_t_defined
#define ____gid_t_defined
#endif

#ifndef UNDEFINED_FUNCTION
#define UNDEFINED_FUNCTION
#endif

__BEGIN_C_DECLS

enum INPUT_PARAMETER {UGID_USERNAME,UGID_USERID};


#ifndef usergroupid_lib_not_needed

UNDEFINED_FUNCTION int get_unix_user_group_id(int accessType, struct CUserDetails* a_usr);

uid_t getuid(void);
gid_t getgid(void);
int getgrouplist(const char *user, gid_t group, gid_t *groups, int *ngroups);
int getgroups(int size, gid_t list[]);

void InitUserGroupDetails(struct CUserDetails* a_usr);

#endif  // #ifndef usergroupid_lib_not_needed

struct SUserDetails{
	
	union {
		const char* userName;
		char*		userNamePrivate;  // used by lib
	}un;
	union {
		const char* groupName;
		char*		groupNamePrivate;  // used by lib
	}gn;
	uid_t		m_UID;
	gid_t		m_GID;
	int			m_nNgroups;
	gid_t		m_vGroups[MAX_NUMBER_OF_GROUPS];
};

__END_C_DECLS


#ifdef __cplusplus
struct CUserDetails : public SUserDetails
{
	CUserDetails();
	~CUserDetails();
};
#else  // #ifdef __cplusplus

#define  CUserDetails SUserDetails

#endif  // #ifdef __cplusplus


#endif  // #ifndef __unix_like_user_group_header_h__
