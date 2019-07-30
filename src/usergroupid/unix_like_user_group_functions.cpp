
// unix_like_user_group_functions.cpp
// 2017 Dec 30

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include "unix_like_user_group_header.h"
#include "usergroupdatabase.hpp"


static int
get_uid_gid_ggroup_witch_cashing(
	INPUT_PARAMETER accessType,
	struct CUserDetails* a_usr);

__BEGIN_C_DECLS

#define RESOURCE_BUF_LEN_MIN1	1023

void InitUserGroupDetails(struct CUserDetails* a_usr)
{
	memset(a_usr, 0, sizeof(struct SUserDetails));
}


uid_t getuid(void)
{
	struct CUserDetails aData;
	char vcUserName[RESOURCE_BUF_LEN_MIN1+1];
	DWORD	dwBufLen= RESOURCE_BUF_LEN_MIN1;
	int nReturn;

	vcUserName[RESOURCE_BUF_LEN_MIN1]=0;
	if( LIKELY(GetUserNameA(vcUserName,&dwBufLen))){
		aData.un.userName = vcUserName;
		nReturn = get_uid_gid_ggroup_witch_cashing(UGID_USERNAME,&aData);
		if(nReturn){
			//errno = nReturn; // This function is always successfull // https://linux.die.net/man/2/getuid 
			// should be returnd windows ssid (later)
			return 0;
		}
		return aData.m_UID;
	}

	return 0;
}


gid_t getgid(void)
{
	struct CUserDetails aData;
	char vcUserName[RESOURCE_BUF_LEN_MIN1+1];
	DWORD	dwBufLen= RESOURCE_BUF_LEN_MIN1;
	int nReturn;

	vcUserName[RESOURCE_BUF_LEN_MIN1]=0;
	if( LIKELY(GetUserNameA(vcUserName,&dwBufLen))){
		aData.un.userName = vcUserName;
		nReturn = get_uid_gid_ggroup_witch_cashing(UGID_USERNAME,&aData);
		if(nReturn){
			//errno = nReturn; // This function is always successfull // https://linux.die.net/man/2/getgid 
			// should be returnd windows ssid (later)
			return 0;
		}
		return aData.m_GID;
	}

	return 0;
}


//
// https://linux.die.net/man/3/getgrouplist 
//
int getgrouplist(const char *a_user, gid_t a_group, gid_t *a_groups, int *a_ngroups)
{
	struct CUserDetails aData;
	int nReturn, i,nForLoop,nGroupIdIncluded;

	aData.un.userName = a_user;
	nReturn = get_uid_gid_ggroup_witch_cashing(UGID_USERNAME,&aData);
	if (nReturn) {*a_ngroups = 0;return 0;}  // error 

	if (aData.m_nNgroups>(*a_ngroups)){nReturn = -1;nForLoop=(*a_ngroups);}
	else{nReturn=aData.m_nNgroups;nForLoop=aData.m_nNgroups;}

	nGroupIdIncluded = 0;
	for(i=0;i<nForLoop;++i){
		a_groups[i] = aData.m_vGroups[i];
		if(a_groups[i]== a_group){nGroupIdIncluded=1;}
	}

	if(nGroupIdIncluded==0){
		if((aData.m_nNgroups+1)>(*a_ngroups)){nReturn = -1;}
		else {a_groups[aData.m_nNgroups]=a_group;}
		++aData.m_nNgroups;
	}

	(*a_ngroups) = aData.m_nNgroups;

	return aData.m_nNgroups;
}


//
// https://linux.die.net/man/2/getgroups 
//
int getgroups(int a_size, gid_t a_list[])
{
	struct CUserDetails aData;
	char vcUserName[RESOURCE_BUF_LEN_MIN1+1];
	DWORD	dwBufLen= RESOURCE_BUF_LEN_MIN1;

	vcUserName[RESOURCE_BUF_LEN_MIN1]=0;
	if( LIKELY(GetUserNameA(vcUserName,&dwBufLen))){
		aData.un.userName = vcUserName;
		if (get_uid_gid_ggroup_witch_cashing(UGID_USERNAME,&aData)){errno=EFAULT;return -1;}  // error 
		//if (aData.m_nNgroups>a_size) { errno= EINVAL; return -1; } // error prone
		if(a_size>aData.m_nNgroups){a_size=aData.m_nNgroups;}
		if(a_size){memcpy(a_list,aData.m_vGroups,sizeof(gid_t)*a_size);}
		return aData.m_nNgroups;
	}

	return -1;
}


__END_C_DECLS

static UserGroupDatabase	s_dataBase;

static int 
get_uid_gid_ggroup_witch_cashing(
	INPUT_PARAMETER a_accessType,
	struct CUserDetails* a_usr)
{

	return s_dataBase.get_unix_user_group_idCls(a_accessType,a_usr);
}


#ifdef __cplusplus

CUserDetails::CUserDetails()
{
	InitUserGroupDetails(this);
}


CUserDetails::~CUserDetails()
{
}

#endif  // #ifdef __cplusplus
