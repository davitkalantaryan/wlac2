// new_winldap.cpp
/*
 *	File: new_winldap.cpp For WINDOWS MFC
 *
 *	Created on: Dec 24, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

//#include "stdafx.h"
#include "stdafx_new_winldap.h"
#include <Windows.h>
#include <Winldap.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>
//#include <sdef_gem_windows.h>


#if(_MSC_VER < 1400) || !defined(_In_)

#ifndef _In_
#define _In_
#endif

#ifndef _In_opt_
#define _In_opt_
#endif

#ifndef _Outptr_result_maybenull_
#define _Outptr_result_maybenull_
#endif

#ifndef _Null_terminated_
#define _Null_terminated_
#endif

#ifndef PZPSTR
#define PZPSTR _Null_terminated_ PSTR*
#endif 

#endif // #if(_MSC_VER < 1400)

#ifdef __cplusplus
extern "C"
{
#endif

NWLDAP_API int ldap_initialize(LDAP **a_ldp, char *a_uri)
{
	//WINLDAPAPI LDAP * LDAPAPI ldap_initA( _In_ const PSTR HostName, ULONG PortNumber );
	size_t unHostNameLen;
	int nPort;
	char* pcHostName;
	char *pcPort, *pcTerm;
	
	//func_ldyp_init = (LDAP(LDAPAPI*)(PCHAR, ULONG))GetProcAddress(s_ldap_lib_handle, "ldap_initA");

	pcPort = strstr(a_uri, "://");

	if(pcPort){ // Linux style
		nPort = strtol(a_uri, &pcTerm, 10);
		if(pcTerm== a_uri){  // Port should be default
			nPort = LDAP_PORT;
		}
		pcHostName = pcPort + strlen("://");
	}
	else{  // windows style
		pcPort = strchr(a_uri, ':');
		if(pcPort){
			nPort = strtol(pcPort, &pcTerm, 10);
			if (pcTerm == pcPort) {  // Port should be default
				nPort = LDAP_PORT;
			}
			unHostNameLen = (size_t)(pcPort - a_uri);
			pcHostName = (char*)_alloca(unHostNameLen + 1);
			memcpy(pcHostName, a_uri, unHostNameLen);
			pcHostName[unHostNameLen] = '\0';
		}
		else {
			pcHostName = a_uri;
			nPort = LDAP_PORT;
		}
	}  // else{  // windows style

	*a_ldp = ldap_initA(pcHostName, (ULONG)nPort);
	return 0;
}


NWLDAP_API int ldap_unbind_ext(LDAP *a_ld, LDAPControl *a_sctrls[], LDAPControl *a_cctrls[])
{
	// WINLDAPAPI ULONG LDAPAPI ldap_unbind( LDAP *ld );
	// ldap_unbind(a_ld);
	//ULONG(LDAPAPI* fpldap_unbind)(LDAP*);
	
	//if (!s_ldap_lib_handle) return -1;
	//fpldap_unbind = (ULONG(LDAPAPI*)(LDAP*))GetProcAddress(s_ldap_lib_handle, "ldap_unbind");
	//if (!fpldap_unbind) return -2;
	return (int)ldap_unbind(a_ld);  // or ldap_unbind_s
}

#ifdef __cplusplus
}
#endif
