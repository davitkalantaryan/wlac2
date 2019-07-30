/*
 *	File: registry_functions.h
 *
 *	Created on: Sep 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */
#ifndef __registry_functions_h__
#define __registry_functions_h__

#ifndef CINTERFACE
#define CINTERFACE
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

int OpenSystemEnvironmentRegistry(HKEY* phKey, REGSAM mode);
int OpenUserEnvironmentRegistry(HKEY* phKey, REGSAM mode);
#define	EnvironmentRegistryKeyClose(__hKey) RegCloseKey((__hKey))
#if 0
char* GetRegistryContent2(
	HKEY hKey, const char* a_cpcName, DWORD* pType,BYTE* a_pbData, 
	DWORD* a_pdwDataSize, char* a_pNameBuffer, size_t a_unBufLen, int a_nCaseInsensitive);
#endif
int  ModifyRegistry(HKEY hKey, const char* name, DWORD type, const BYTE* data, DWORD data_size);
void UpdateEnvironmentalVariables(void);
int  RegistryInfoQuery(HKEY hKey);
BYTE* GetRegistryContent4(HKEY a_hKey, const char* a_cpcName, DWORD* a_pType, BYTE* a_pbData, DWORD* a_pdwDataSize);

#define GetRegistryContent3(_hKey, _name,_typePtr,_buffer,_dataSizeInOutPtr) \
						RegQueryValueExA((_hKey),(_name),NULL,(_typePtr),(_buffer),(_dataSizeInOutPtr))


#ifdef __cplusplus
}
#endif


#endif  // #ifndef __registry_functions_h__
