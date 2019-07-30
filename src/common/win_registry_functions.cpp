/*
 *	File: registry_functions.cpp
 *
 *	Created on: Sep 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */

#include "registry_functions.h"
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH	255
#define MAX_VALUE_NAME2	4096

#ifdef __cplusplus
extern "C"
{
#endif

int OpenSystemEnvironmentRegistry(HKEY* a_phKey, REGSAM a_mode)
{
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\Session Manager\\Environment",
		0,
		a_mode, // KEY_READ | KEY_WRITE
		a_phKey) != ERROR_SUCCESS
		)
	{
		return -1;
	}
	return 0;
}


int OpenUserEnvironmentRegistry(HKEY* a_phKey, REGSAM a_mode)
{
	if (RegOpenKeyExA(HKEY_CURRENT_USER,
		"Environment",
		0,
		a_mode, // KEY_READ | KEY_WRITE
		a_phKey) != ERROR_SUCCESS
		)
	{
		return -1;
	}
	return 0;
}


int ModifyRegistry(HKEY hKey, const char* a_name, DWORD a_dwType, const BYTE* a_data, DWORD a_data_size)
{
	return RegSetValueExA(hKey, a_name, NULL, a_dwType, a_data, a_data_size)== ERROR_SUCCESS ? 0 : -1;
}


void UpdateEnvironmentalVariables(void)
{
	//int nRet=ModifyRegistry(hKey, a_name, a_dwType, (const BYTE*)a_data, a_data_size);
#if (!defined(_M_ARM) | defined(BroadcastSystemMessage_for_arm_fixed)) & !defined(BroadcastSystemMessage_not_needed)
	ULONG_PTR aPtr;
	//BroadcastSystemMessageA(0, NULL, WM_SETTINGCHANGE, (WPARAM)0, (LPARAM)0);
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, (WPARAM)0, (LPARAM)0, SMTO_ABORTIFHUNG, 100, &aPtr);
	//SetEnvironmentVariableA(a_name, a_data);
#endif
	//return nRet;
}


BYTE* GetRegistryContent4(HKEY a_hKey, const char* a_cpcName, DWORD* a_pType, BYTE* a_pbData,DWORD* a_pdwDataSize)
{
	if(RegQueryValueExA(a_hKey,a_cpcName,NULL,a_pType,a_pbData,a_pdwDataSize)== ERROR_SUCCESS){return a_pbData;}
	return NULL;
}


char* GetRegistryContent2(
	HKEY hKey, const char* a_cpcName, 
	DWORD* a_pdwType,BYTE* a_pbData, DWORD* a_pdwDataSize,
	char* a_pNameBuffer, size_t a_unBufLen, int a_nCaseInsensitive)
{
	char		achClass[MAX_PATH] = "";	// buffer for class name 
	DWORD		cchClassName = MAX_PATH;	// size of class string 
	DWORD		cSubKeys = 0;				// number of subkeys 
	DWORD		cbMaxSubKey;				// longest subkey size 
	DWORD		cchMaxClass;				// longest class string 
	DWORD		cValues;					// number of values for key 
	DWORD		cchMaxValue;				// longest value name 
	DWORD		cbMaxValueData;				// longest value data 
	DWORD		cbSecurityDescriptor;		// size of security descriptor 
	FILETIME	ftLastWriteTime;			// last write time 
	DWORD		i, retCode;
	DWORD		cchValue2;
	DWORD		outDataSize0 = *a_pdwDataSize;

	if(!a_pNameBuffer || !a_unBufLen){return NULL;}

	// Get the class name and the value count. 
	retCode = RegQueryInfoKeyA(
		hKey,                    // key handle 
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


	if (retCode != ERROR_SUCCESS){fprintf(stderr,"RegQueryInfoKeyA failed!\n");return NULL;}
	
	// Subkeys are not important
	
	// Enumerate the key values. 
	for (i = 0; i<cValues; i++)
	{
		cchValue2 = (DWORD)a_unBufLen;
		a_pNameBuffer[0] = '\0';
		retCode = RegEnumValueA(hKey, i,
			a_pNameBuffer,
			&cchValue2,
			NULL,
			NULL,
			NULL,
			NULL);

		if (retCode == ERROR_SUCCESS){
			*a_pdwDataSize = outDataSize0;
			RegQueryValueExA(hKey, a_pNameBuffer, NULL, a_pdwType, a_pbData, a_pdwDataSize);
			if((strcmp(a_pNameBuffer,a_cpcName)==0)||(a_nCaseInsensitive && (_stricmp(a_pNameBuffer, a_cpcName)==0))){return a_pNameBuffer;}
		} // if (retCode == ERROR_SUCCESS)
	} // for (i = 0; i<cValues; i++)
	return NULL;
}


int RegistryInfoQuery(HKEY hKey)
{
#define DATA_OUT_SIZE	1024
	//char		achKey[MAX_KEY_LENGTH];		// buffer for subkey name
	//DWORD		cbName;						// size of name string 
	char		achClass[MAX_PATH] = "";	// buffer for class name 
	DWORD		cchClassName = MAX_PATH;	// size of class string 
	DWORD		cSubKeys = 0;				// number of subkeys 
	DWORD		cbMaxSubKey;				// longest subkey size 
	DWORD		cchMaxClass;				// longest class string 
	DWORD		cValues;					// number of values for key 
	DWORD		cchMaxValue;				// longest value name 
	DWORD		cbMaxValueData;				// longest value data 
	DWORD		cbSecurityDescriptor;		// size of security descriptor 
	FILETIME	ftLastWriteTime;			// last write time 
	DWORD		i, retCode;

	char		achValue[MAX_VALUE_NAME2];
	DWORD		cchValue = MAX_VALUE_NAME2;
	DWORD		dwType;
	BYTE		pbData[DATA_OUT_SIZE];
	DWORD		outDataSize = DATA_OUT_SIZE;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKeyA(
		hKey,                    // key handle 
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

	// Subkeys are not important
	
	// Enumerate the key values. 

	for (i = 0; i<cValues; i++)
	{
		cchValue = MAX_VALUE_NAME2;
		achValue[0] = '\0';
		retCode = RegEnumValueA(hKey, i,
			achValue,
			&cchValue,
			NULL,
			NULL,
			NULL,
			NULL);

		if (retCode == ERROR_SUCCESS)
		{
			outDataSize = DATA_OUT_SIZE;
			RegQueryValueExA(hKey, achValue, NULL, &dwType, pbData, &outDataSize);

			printf("(%d) %s\t", i + 1, achValue);
			bool bIsString = false;

			switch (dwType)
			{
			case REG_EXPAND_SZ:
				bIsString = true;
				printf("REG_EXPAND_SZ");
				break;
			case REG_SZ:
				bIsString = true;
				printf("REG_SZ");
				break;
			case REG_MULTI_SZ:
				bIsString = true;
				printf("REG_MULTI_SZ");
				break;
			default:
				bIsString = false;
				_tprintf(TEXT("default"));
				break;
			} // switch (dwType)
			_tprintf(TEXT("\t"));
			if (bIsString)
			{
				printf("%s", (char*)pbData);
			}
			_tprintf(TEXT("\n"));

		} // if (retCode == ERROR_SUCCESS)
	} // for (i = 0; i<cValues; i++)

	//
	return 0;
}


#ifdef __cplusplus
}
#endif
