
// usergroupdatabase.cpp
// 2018 Jan 02

#include "stdafx.h"
#include "usergroupdatabase.hpp"
#include <memory.h>
#include <stdlib.h>
#include <stddef.h>


UserGroupDatabase::UserGroupDatabase()
	:
	m_list()
	,m_hashByUserName(512)
	//,m_hashByGroupName(512)
	,m_hashByUserId(512)
	//,m_hashByGroupId(512)
{
}


UserGroupDatabase::~UserGroupDatabase()
{
}


int UserGroupDatabase::get_unix_user_group_idCls(INPUT_PARAMETER a_accessType,struct CUserDetails* a_usr)
{
	struct CUserDetails* pItem2 = NULL;
	int nReturn = 0;
	bool bFound = false;

	// enum INPUT_PARAMETER {UGID_USERNAME,UGID_USERID};
	switch (a_accessType)
	{
	case UGID_USERNAME:
		bFound=m_hashByUserName.FindEntry(a_usr->un.userName,strlen(a_usr->un.userName),&pItem2);
		break;
	case UGID_USERID:
		bFound=m_hashByUserId.FindEntry(&a_usr->m_UID,sizeof(a_usr->m_UID),&pItem2);
		break;
	default:
		break;
	}

	if((!pItem2)||(!bFound)){
		char* pcUserNameInitial = a_usr->un.userNamePrivate;
		char* pcGroupNameInitial = a_usr->gn.groupNamePrivate;
		nReturn = get_unix_user_group_id(a_accessType,a_usr);
		if(!nReturn){
			pItem2 = new CUserDetails(*a_usr);
			if(!pItem2){return 0;}
			if (pcUserNameInitial) {
				pItem2->un.userNamePrivate = _strdup(a_usr->un.userName);
				if (!pItem2->un.userNamePrivate) { delete pItem2; return 0; }
			}
			else {pItem2->un.userNamePrivate= a_usr->un.userNamePrivate;}

			if (pcGroupNameInitial) {
				pItem2->gn.groupNamePrivate = _strdup(a_usr->gn.groupName);
				if (!pItem2->gn.groupNamePrivate) { delete pItem2; return 0; }
			}
			else {pItem2->gn.groupNamePrivate= a_usr->gn.groupNamePrivate;}

			// time to cash it
			m_list.AddToTheEnd(pItem2);
			m_hashByUserName.AddEntry(pItem2->un.userName,strlen(pItem2->un.userName),pItem2);
			//m_hashByGroupName.AddEntry(pItem2->gn.groupName,strlen(pItem2->gn.groupName),pItem2);
			m_hashByUserId.AddEntry(&pItem2->m_UID,sizeof(pItem2->m_UID),pItem2);
			//m_hashByGroupId.AddEntry(&pItem2->m_GID,sizeof(pItem2->m_GID),pItem2);
		}
		else{return EFAULT;}
	} // if(!pItem){
	else{
		memcpy(a_usr, pItem2, sizeof(struct CUserDetails));
	}

	return nReturn;
}
