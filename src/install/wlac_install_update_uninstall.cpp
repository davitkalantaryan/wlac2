//
// file:		wlac_install_update_uninstall.cpp
// created on:	2018 Dec 28
// created by:	Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

// wlac_installer.cpp
// 2018 Feb 09

#define SKIP_DETAILED_SEARCH

#if defined(_M_ARM) && !defined(SKIP_DETAILED_SEARCH)  && !defined(FORCE_DETAILED_SEARCH)
#define SKIP_DETAILED_SEARCH
#endif

#ifndef SKIP_DETAILED_SEARCH
#pragma comment(lib,"Netapi32.lib")
#endif


#include "registry_functions.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <malloc.h>
#ifndef SKIP_DETAILED_SEARCH
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <Lm.h>
#endif

#pragma warning (disable:4996)

#define PATH_ENV_NAME			"Path"
#define REG_DATA_SIZE_MIN1		4095
#define PATH_ENV_ADD_LEN_MIN1	1023

static const char* EnvVarAddForPath(char* a_wlacRoot, char* a_pcBuffer)
{
	char* pcLastSlash = strrchr(a_wlacRoot,'\\');
	*pcLastSlash = '\0';
	snprintf(a_pcBuffer,PATH_ENV_ADD_LEN_MIN1,"%s\\dll;%s\\exe",a_wlacRoot,a_wlacRoot);
	return a_pcBuffer;
}


static bool RemoveEnvVariableFromPath(HKEY a_hKey, const char* a_cpcPathAdd, size_t a_unStrLen)
{
	DWORD dwType;
	DWORD dwDataSize = REG_DATA_SIZE_MIN1;
	LSTATUS retFromRegFuncs;
	char vcData[REG_DATA_SIZE_MIN1 + 1];
	char* pcEnvPlace;
	bool bModify(false);

	retFromRegFuncs = GetRegistryContent3(a_hKey, PATH_ENV_NAME, &dwType, (BYTE*)vcData, &dwDataSize);
	if (retFromRegFuncs != ERROR_SUCCESS) { goto returnPoint; }

	pcEnvPlace = strstr(vcData, a_cpcPathAdd);
	while (pcEnvPlace) {
		if ((pcEnvPlace[a_unStrLen] == ';') || (pcEnvPlace[a_unStrLen] == '\0')) {
			*pcEnvPlace = '\0';
			bModify = true;
			break;
		}
		pcEnvPlace = strstr(pcEnvPlace + a_unStrLen, a_cpcPathAdd);
	}

returnPoint:
	if (bModify) {
		a_unStrLen = strlen(vcData);
		ModifyRegistry(a_hKey, PATH_ENV_NAME, dwType, (BYTE*)vcData, (DWORD)a_unStrLen);
		UpdateEnvironmentalVariables();
	}
	return bModify;
}


int WlacInstaller(char* a_wlacRoot, int a_isSystem)
{
	HKEY hKey = (HKEY)0;
	int nReturn = -1;
	char vcData[REG_DATA_SIZE_MIN1+1];
	char vcPathAdd[PATH_ENV_ADD_LEN_MIN1+1];
	DWORD dwDataSize = REG_DATA_SIZE_MIN1;
	DWORD dwType;
	LSTATUS retFromRegFuncs;
	int nPathVarLen;

	if(a_isSystem){nReturn=OpenSystemEnvironmentRegistry(&hKey, KEY_READ | KEY_WRITE);}
	else{nReturn=OpenUserEnvironmentRegistry(&hKey, KEY_READ | KEY_WRITE);}
	if (nReturn) { goto returnPoint; }

	retFromRegFuncs = GetRegistryContent3(hKey,PATH_ENV_NAME, &dwType, (BYTE*)vcData, &dwDataSize);
	if(retFromRegFuncs!=ERROR_SUCCESS){ vcData[0]='\0';}

	nPathVarLen=snprintf(vcData,REG_DATA_SIZE_MIN1,"%s;%s",vcData,EnvVarAddForPath(a_wlacRoot,vcPathAdd));
	ModifyRegistry(hKey,PATH_ENV_NAME, dwType,(BYTE*)vcData,nPathVarLen);
	UpdateEnvironmentalVariables();
		
	nReturn = 0;
returnPoint:
	if (hKey) { RegCloseKey(hKey); }
	return nReturn;
}


int WlacUninstaller(char* a_wlacRoot, int a_isSystem)
{
	HKEY hKey=(HKEY)0;
	size_t unStrLen;
	char vcPathAdd[PATH_ENV_ADD_LEN_MIN1 + 1];
	int nReturn;

	if(a_isSystem){nReturn=OpenSystemEnvironmentRegistry(&hKey, KEY_READ | KEY_WRITE);}
	else{nReturn=OpenUserEnvironmentRegistry(&hKey, KEY_READ | KEY_WRITE);}
	if (nReturn) { goto returnPoint; }

	EnvVarAddForPath(a_wlacRoot, vcPathAdd);
	unStrLen = strlen(vcPathAdd);

	if((!a_isSystem)||(!RemoveEnvVariableFromPath(hKey,vcPathAdd,unStrLen))){

		const size_t	cunStrLenEnv(strlen("\\Environment"));
		size_t			unWholeBufLen;
		HKEY			hKeyLocal;
		char*			pcSubKeyName;
		FILETIME		ftLastWriteTime;			// last write time 
		char			achClass[MAX_PATH] = "";	// buffer for class name 
		DWORD			cchClassName = MAX_PATH;	// size of class string 
		DWORD			cSubKeys = 0;				// number of subkeys 
		DWORD			cbMaxSubKey;				// longest subkey size 
		DWORD			cchMaxClass;				// longest class string 
		DWORD			cValues;					// number of values for key 
		DWORD			cchMaxValue;				// longest value name 
		DWORD			cbMaxValueData;				// longest value data 
		DWORD			cbSecurityDescriptor;		// size of security descriptor 
		DWORD			i;

		DWORD retCode = RegQueryInfoKeyA(
			HKEY_USERS,              // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 

		if (retCode != ERROR_SUCCESS){/*fprintf(stderr,"RegQueryInfoKeyA failed!\n");*/ goto returnPoint;}

		unWholeBufLen = (++cbMaxSubKey) + 1 + cunStrLenEnv;
		pcSubKeyName = (char*)alloca(unWholeBufLen);
		// Enumerate all subkeys. 
		for (i = 0; i < cSubKeys; i++){

			cValues = cbMaxSubKey;
			if(RegEnumKeyExA(HKEY_USERS,i,pcSubKeyName,&cValues,NULL,NULL,NULL,&ftLastWriteTime)!=ERROR_SUCCESS){continue;}

			strncat(pcSubKeyName,"\\Environment",unWholeBufLen);
			if(RegOpenKeyExA(HKEY_USERS,pcSubKeyName,0,KEY_READ|KEY_WRITE,&hKeyLocal) != ERROR_SUCCESS){continue;}
			if(RemoveEnvVariableFromPath(hKeyLocal,vcPathAdd,unStrLen)){
				RegCloseKey(hKeyLocal);
				break;
			}
			RegCloseKey(hKeyLocal);

		} // for (i = 0; i<cValues; i++)

#ifndef SKIP_DETAILED_SEARCH
		// let's find the user installed the soft
		// https://docs.microsoft.com/en-us/windows/desktop/api/lmaccess/nf-lmaccess-netuserenum 
		NET_API_STATUS nStatus;
		USER_INFO_0* pTmpBuf;
		USER_INFO_0* pUserInfo;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		DWORD dwResumeHandle = 0;
		DWORD i;

		nStatus = NetUserEnum(NULL, // means this host
			0,  // Return user account names. The bufptr parameter points to an array of USER_INFO_0 structures.
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pUserInfo,
			MAX_PREFERRED_LENGTH,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);

		if (   ((nStatus!=NERR_Success)&&(nStatus!=ERROR_MORE_DATA))||(!pUserInfo)  ){goto returnPoint;}

		for (i = 0, pTmpBuf = pUserInfo; (i < dwEntriesRead); ++i,++pTmpBuf){
			wprintf(L"\t-- %s\n", pTmpBuf->usri0_name);
		}
#endif  // #ifndef SKIP_DETAILED_SEARCH
	}
	
returnPoint:
	if (hKey) { RegCloseKey(hKey); }
	return 0;
}


int WlacUpdater(char* a_installerDir, int a_isSystem)
{
	return 0;
}
