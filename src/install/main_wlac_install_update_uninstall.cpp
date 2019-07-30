//
// file:		main_wlac_install_update_uninstall.cpp
// created on:	2018 Dec 28
// created by:	Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

//
// main_wlac_install_update_uninstall.cpp
// 2018 Dec 28
//
// In order to install WLAC library following steps should be confirmed
//	1.	Files should be copied in proper directories 
//		in the case of MSI installer will be done by it during installation
//	2.	ENSHOST environmental variable should be set 
//		in the case of Zeuthen DESY doocsens1:doocsens1
//		generally this parameter can be selected
//	3.	%DOOCSROOT%\AMD64\bin;%DOOCSROOT_X64%/AMD64/dll should be added to the Path
//		environmental variable. In the case of 32 bit windows DOOCS installation on 64 
//		bit WINDOWS following trick should be done in order to skip colision
//		 a) or WOW64 
//		 b) or different naming should be used 
//		 c) or with different values environmental variables should be used
//		https://msdn.microsoft.com/en-us/library/windows/desktop/aa384232(v=vs.85).aspx
//		https://msdn.microsoft.com/en-us/library/windows/desktop/aa384274(v=vs.85).aspx
//		%UDOOCSROOT%/AMD64/bin;%UDOOCSROOT%/AMD64/dll   for 32 bit windows
//		Conclusion for path value:	
//		Path=%Path%;%DOOCSROOT%/%PROCESSOR_ARCHITECTURE%/bin;%DOOCSROOT%/%PROCESSOR_ARCHITECTURE%/dll
//	4.	MATLABPATH environmental variable should be extended if necessary
//		https://de.mathworks.com/help/matlab/matlab_env/add-folders-to-matlab-search-path-at-startup.html
//		MATLABPATH=%DOOCSROOT%/mscripts;%DOOCSROOT%/%PROCESSOR_ARCHITECTURE%/mbin
//	5.	Firewall for DOOCS ports should be open
//	6.	Information on uninstalling the DOOCS shoulod be added to the corresponding registry
//		http://nsis.sourceforge.net/Add_uninstall_information_to_Add/Remove_Programs
//		In the case of usage standard installer (MSI), this will be done by installer
//
// Following steps are necessary in order to install server library
//	1.	All steps for the client should be confirmed
//	2.	Necessary files (dll,exe(util and watchdog),doocs.bat script)
//		should be copied to the correspondiong places
//	3.	rpc_bind service should be registered
//	4.	watchdog_server  should be scheduled as a startup task
// 
// ARM installers will be always servers
//
// Steps to install development version of WINDOWS DOOCS
//	1.	All steps for the client should be confirmed
//	2.	Necessary files (dll,exe(util and watchdog),doocs.bat script)
//		should be copied to the correspondiong places
//	3.	rpc_bind service should be registered (in order to give cjhance developer
//		to debug servers under development)
//	4.	All sources will be added to the %UDOOCSROOT%/dev/src
//		following directories will exist
//		%UDOOCSROOT%/dev/src/clientlib ,  %UDOOCSROOT%/dev/src/serverlib
//	5.	Visual Studio new project template for server and client development 
//		will be added
//		to add:		https://msdn.microsoft.com/en-us/library/dd885241.aspx
//		to rem:		https://stackoverflow.com/questions/789095/how-can-i-delete-bad-visual-studio-templates
//		to rem:		delete zip file from 
//					'\\win.desy.de\home\kalantar\My Documents\Visual Studio 2015\Templates\ProjectTemplates'
//					folder
//

//#define _CONSOLE
//#define _DEBUG_INSTALLERS

#ifndef CINTERFACE
#define CINTERFACE
#endif

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#ifdef _WIN32
#define strcmpi	_strcmpi
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#endif

#if defined(_MSC_VER) && (_MSC_VER>1400)
#pragma warning(disable:4996)
#endif

#if defined(_DEBUG_INSTALLERS) && !defined(_CONSOLE)
#define AllocConsoleMcr	AllocConsole
#else
#define AllocConsoleMcr()
#endif

#if defined(_M_ARM) || !defined(_CONSOLE)
#define FreeConsoleMcr()
#ifdef _M_ARM
#define FinalizeInstaller(...)	do{printf(__VA_ARGS__);printf("\n");}while(0)
#else
#define FinalizeInstaller(...)
#endif   // #ifdef _M_ARM
#else
#define FreeConsoleMcr			FreeConsole
#ifdef _DEBUG_INSTALLERS
#define FinalizeInstaller(...)	\
	do{ \
		AllocConsole(); \
		printf(__VA_ARGS__); \
		printf("\nPress any key to exit!");fflush(stdout);_getch();printf("\n"); \
	}while(0)
#else
#define FinalizeInstaller(...)	printf(__VA_ARGS__)
#endif
#endif

#if defined(_DEBUG_INSTALLERS) && defined(_DEBUG)
#define WaitDebuggerIfNeeded(...)	\
	do{ \
		printf(__VA_ARGS__);\
		printf("\nAttach debugger and Press any key to continue!"); fflush(stdout);\
		_getch(); \
		printf("\n"); \
	}while(0)
#elif defined(_M_ARM)
#define WaitDebuggerIfNeeded(...)	printf(__VA_ARGS__)
#else
#define WaitDebuggerIfNeeded(...)
#endif

typedef int (*TypeActionFunction)( char* installerDir, int isSystem);

struct SOptionFuncPair {
	const char* option;
	TypeActionFunction	func;
};

int WlacInstaller( char* a_wlacRoot, int isSystem);
int WlacUninstaller( char* a_wlacRoot, int isSystem);
int WlacUpdater(char* a_installerDir, int isSystem);


static const SOptionFuncPair s_vAllOptions[] = {
	{"/Install",&WlacInstaller},
	{"/Uninstall",WlacUninstaller},
	{ "/Rollback",WlacUninstaller },
	{ "/Update",WlacUpdater },
};

//#include "registry_functions.h"
#ifndef _CONSOLE
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	int argc = 2;
	char* argv[2];

	argv[0] = lpCmdLine;
	argv[1]=lpCmdLine;
	AllocConsoleMcr();
#else
int main(int argc, char* argv[])
{
#endif
	char* pcFlStarts;
	const size_t cunOptionsVectSize(sizeof(s_vAllOptions)/sizeof(SOptionFuncPair));
	size_t i;
	int nReturn = -1;
	int nIsSystem;
	char vcWlacRootPath[_MAX_PATH];
	char vcUserName[128];
	DWORD dwUserNameLen=127;

	WaitDebuggerIfNeeded("version 3\n%s",argc>1?argv[1]:argv[0]);

	//GetAnyRegistryContent(HKEY_CURRENT_USER,"Software\\Classes\\http\\shell\\open\\command","",
	//	&dwDataType, (BYTE*)vcDoocsRootPath, &dwDataLen, 1);
	//return 0;

	if (argc<2) { return 1; }

	FreeConsoleMcr();

	GetUserNameA(vcUserName, &dwUserNameLen);
	if(strcmpi("SYSTEM",vcUserName)==0){nIsSystem=1;}
	else{nIsSystem=0;}

	GetModuleFileNameA(NULL, vcWlacRootPath,_MAX_PATH);
	pcFlStarts = strrchr(vcWlacRootPath, '\\');
	if(!pcFlStarts){goto returnPoint;}
	*pcFlStarts = 0;

	for(i=0; i<cunOptionsVectSize;++i){
		if(strcmpi(s_vAllOptions[i].option,argv[1])==0){
			nReturn=(*s_vAllOptions[i].func)(vcWlacRootPath,nIsSystem);
			goto returnPoint;
		}
	}

	nReturn = 0;
	goto returnWithoutPromt;
returnPoint:
	FinalizeInstaller("action=%s, return=%d",argv[1],nReturn);
returnWithoutPromt:
	return nReturn;
}
