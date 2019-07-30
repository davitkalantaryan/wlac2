/*
 *	File: main_set_env.cpp
 *
 *	Created on: Sep 14, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */

// _M_ARM is the standard macro shows if ARM is used

#include "registry_functions.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include "common/argument_parser.hpp"

#define _DEBUG_APP_MAC_

#ifdef _MSC_VER
#if(_MSC_VER >= 1400)
#pragma warning(disable : 4996)
#endif
#endif

#if !defined(_DEBUG) | defined(_M_ARM)
#ifdef _DEBUG_APP_MAC_
#undef _DEBUG_APP_MAC_
#endif
#endif

#ifdef _DEBUG_APP_MAC_
#define DEFAULT_CONIO	1
#else
#define DEFAULT_CONIO	0
#endif

#if (!defined(_M_ARM) | defined(BroadcastSystemMessage_for_arm_fixed)) & !defined(BroadcastSystemMessage_not_needed)
#pragma comment(lib,"User32.lib")
#endif

static void PrintHelp(void);

typedef int (*TypeOpenEnvironmentRegistry)(HKEY* phKey, REGSAM mode);

int main(int a_argc, char* a_argv[])
{
	TypeOpenEnvironmentRegistry fpFunc = &OpenSystemEnvironmentRegistry;
	char**		argv2 = a_argv+1;
	char*		pcVarNameFromReg;
	const char* cpcEnvVar = "";
	const char* cpcErrorString = "Unknown error!";
	const char* cpcPartFound = NULL;
	HKEY hKey = (HKEY)0;
	size_t unStrLenForNewValue=0;
	size_t unStrLenForValue;
	int nReturn2 = -1;
	int nDebugApp = DEFAULT_CONIO;
	int	argc2 = a_argc-1;
	//int nInsensitive = 0;
	DWORD dwType = REG_SZ;
	DWORD dwDataSize = 2048;
	char vcData[2048];
	//char vcEnvNameBuf[4096];
	common::argument_parser aParser;

	aParser
		<< "-h,--help:display this message"
		<< "-a,--append:append to the existing or create new"
		<< "-p,--prepend:prepend to the existion or create new"
		<< "-u,--user:Work with local user registry";;

	aParser.ParseCommandLine(argc2, argv2);
	
	if(aParser["-h"]||(!argc2)){
		PrintHelp();
		std::cout<< aParser.HelpString()<<std::endl;
		return aParser["-h"]?0:1;
	}

	if(aParser["--user"]){fpFunc=&OpenUserEnvironmentRegistry;}

	if(argc2==1){
		if ((*fpFunc)(&hKey, KEY_READ)){goto returnPoint;}
		vcData[0] = 0;
		//pcVarNameFromReg=GetRegistryContent2(hKey,argv2[0], &dwType,(BYTE*)vcData,&dwDataSize,vcEnvNameBuf,4095,nInsensitive);
		pcVarNameFromReg = (char*)GetRegistryContent4(hKey, argv2[0], &dwType, (BYTE*)vcData, &dwDataSize);
		if(pcVarNameFromReg){
			printf(
				"System Environment: %s\n"
				"Value             : %s\n"
				"Type              : %d\n",pcVarNameFromReg,vcData,(int)dwType);
			nReturn2 = 0;
		}
		goto returnPoint;
	}

	if (((*fpFunc))(&hKey, KEY_READ | KEY_WRITE)){goto returnPoint;}

	cpcEnvVar = argv2[1];
	
	if(aParser["-a"]){
		vcData[0] = 0;
		pcVarNameFromReg = (char*)GetRegistryContent4(hKey,argv2[0],&dwType,(BYTE*)vcData,&dwDataSize);
		if (pcVarNameFromReg){
			cpcPartFound = strstr(vcData, argv2[1]);
			unStrLenForValue = strlen(argv2[1]);
			while (cpcPartFound) {
				cpcPartFound += unStrLenForValue;
				if ((*cpcPartFound == ';') || (*cpcPartFound == 0)) {
					printf("Environmental variable '%s' already contains '%s'\n", argv2[0], argv2[1]);
					nReturn2 = 0;
					goto returnPoint;
				}
				cpcPartFound = strstr(cpcPartFound, argv2[1]);
			}
			strncat(vcData, ";", 2047);
			strncat(vcData, argv2[1], 2047);
			cpcEnvVar = vcData;
		} // if(aParser["-a"]){
	}
	else if (aParser["-p"]) {
		unStrLenForNewValue = strlen(argv2[1]) + 1;
		vcData[unStrLenForNewValue] = 0;
		dwDataSize -= ((DWORD)unStrLenForNewValue);
		pcVarNameFromReg = (char*)GetRegistryContent4(hKey, argv2[0], &dwType, (BYTE*)(vcData + unStrLenForNewValue), &dwDataSize);
		if (pcVarNameFromReg) {
			cpcPartFound = strstr(vcData + unStrLenForNewValue, argv2[1]);
			unStrLenForValue = strlen(argv2[1]);
			while (cpcPartFound) {
				cpcPartFound += unStrLenForValue;
				if ((*cpcPartFound == ';') || (*cpcPartFound == 0)) {
					printf("Environmental variable '%s' already contains '%s'\n", argv2[0], argv2[1]);
					nReturn2 = 0;
					goto returnPoint;
				}
				cpcPartFound = strstr(cpcPartFound, argv2[1]);
			}
			memcpy(vcData, argv2[1], unStrLenForNewValue - 1);
			vcData[unStrLenForNewValue - 1] = ';';
			cpcEnvVar = vcData;
		} // if (pcVarNameFromReg) {
	}

	printf("Value to set: \"%s\"\n", cpcEnvVar);
	nReturn2 = ModifyRegistry(hKey,argv2[0],dwType,(BYTE*)cpcEnvVar,(DWORD)strlen(cpcEnvVar));
	UpdateEnvironmentalVariables();

	printf("return=%d\n", nReturn2);
#if defined(_M_ARM) & defined(BroadcastSystemMessage_for_arm_fixed)
	printf("Open new shell to apply changes!\n");
#endif

returnPoint:
	if (hKey) { RegCloseKey(hKey); }
	if(nReturn2){fprintf(stderr,"%s\n", cpcErrorString);}

	if(nDebugApp){
		printf("Press any key to exit"); fflush(stdout);
		_getch();
		printf("\n");
	}
	
	return nReturn2;
}


static void PrintHelp(void)
{
	printf(
		"\nProgram set_environment.\n"
		"\tSets or modifies the system environmental variable.\n"
		"\tUsage:  set_environment [--append,--prepend] varName ValueToSetOrAdd.\n"
		"\tFollowing options are available\n"
		"\t\t1. --help    (-h) : displays this message and return\n"
		"\t\t2. --append  (-a) : appends last argument to the existing environmental variable\n"
		"\t\t3. --prepend (-p) : prepends last argument to the existing environmental variable\n"
		"\n"
		"\tExamples of usage"
		"\t\tset_environment Path --help\n                        :: to display this message\n"
		"\t\tset_environment Path --appent  C:\\controlszn\\dll64 :: to append\n"
		"\t\tset_environment Path --prepend C:\\controlszn\\dll64 :: to prepend\n"
		"\t\tset_environment ENSHOST doocsens1:doocsens2          :: to set (or modify if it exists)\n"
		"\tFollowing output is generated by argument parser API\n"
	);
}
