
// main_ldd.cpp
// updated on 2018 Feb 28


/*
 *	File		: 'main_ldd.cpp' For WINDOWS
 *
 *	Created on	: Dec 22, 2015
 *	Author		: Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

//#define	IMAGE_TO_DISCOVER	"simple_tcp_server"
//#define DO_DEBUG

#if defined(_MSC_VER) & (_MSC_VER>1400)
#pragma warning(disable : 4996)  //  This function or variable may be unsafe
#pragma warning(disable : 4091)  // 'typedef ': ignored on left of '' when no variable is declared  #8.1\include\um\imagehlp.h
#endif

#include <windows.h>
#include <Imagehlp.h>
#include <stdio.h>
#include <stddef.h>
#ifdef DO_DEBUG
#include <conio.h>
#endif  // #ifdef DO_DEBUG
#include <tchar.h>

#define BUF_STR_LEN_MIN1	4095

typedef void(*fnc_to_handle_dll_type)(const char*name,const char*fullPath, void* a_mappedAddress,void*userData);

#pragma comment(lib, "Imagehlp.lib")

static int FindDependency(const char* a_name, fnc_to_handle_dll_type a_fnc, void* a_pUserData,int a_bCall);
static void FunctToHandleDll(const char*name, const char*fullPath, void* a_mappedAddress,void*userData);
static void FunctToHandleItself(const char*a_name, const char*a_fullPath, void* a_mappedAddress, void*a_userData);
static void PrintHelp(void);
static void show_error(void);

static int s_nVerbose = 0;

#include <vector>
#include <string>

static std::vector<std::string>	s_weakDependencies;
static std::vector<std::string> s_DllsFound;

int main(int argc, char* argvn[])
{
	const char* cpcBinaryName;
	int nReturn = -1;
	char vcBufString[BUF_STR_LEN_MIN1 + 1];

	vcBufString[0] = 0;
	if (argc < 2){
		_snprintf(vcBufString, BUF_STR_LEN_MIN1, "ldd: missing file arguments\nTry `ldd --help' for more information.\n");
		goto returnPoint;
	}

	if ((strcmp(argvn[1],"--help")==0)||(strcmp(argvn[1], "-h")) == 0){
		PrintHelp();
		goto returnPoint;
	}
	else if( (strcmp(argvn[1], "-v") == 0) || (strcmp(argvn[1], "-a")==0)  ){
		if (argc<3) {
			fprintf(stderr,"Provide name of binary!\n");
			goto returnPoint;
		}
		cpcBinaryName = argvn[2];
		s_nVerbose = 1;
	}
	else {
		cpcBinaryName = argvn[1];
		if (  (argc>2)&&   (  (strcmp(argvn[2], "-v") == 0) || (strcmp(argvn[2], "-a")==0 ) )   ) {s_nVerbose = 1;}
	}


	if(s_nVerbose){
		nReturn = GetEnvironmentVariableA("Path", vcBufString, BUF_STR_LEN_MIN1);
		if ((nReturn != ERROR_ENVVAR_NOT_FOUND) && (nReturn<BUF_STR_LEN_MIN1)) {
			strncpy(vcBufString + nReturn, ";C:\\Windows\\SysWOW64\\downlevel", BUF_STR_LEN_MIN1 - nReturn);
			SetEnvironmentVariableA("Path", vcBufString);
		}
	}
	else{
		FindDependency(argvn[0], FunctToHandleItself, NULL, 0);
		s_DllsFound.clear();
	}


	if(FindDependency(cpcBinaryName,FunctToHandleDll,NULL,0)>=0){ nReturn =0;}
	printf("\n");
	goto returnPoint;
returnPoint:

	if (nReturn<0) {/*perror("\n");*/show_error();if (vcBufString[0]){printf(vcBufString);} }

#ifdef DO_DEBUG
	printf("press any key to exit!"); fflush(stdout);
	_getch();
#endif  // #ifdef DO_DEBUG
	return nReturn;
}


static bool IsWeekDependency(const char* a_name)
{
	const size_t cunSize(s_weakDependencies.size());
	if(strncmp(a_name,"api-ms-win-",11)==0){return true;}
	for(size_t i(0);i<cunSize;++i){if(s_weakDependencies[i]==a_name){return true;}}
	return false;
}


static void AddToTheWeekDependent(const char* a_name)
{
	s_weakDependencies.push_back(a_name);
}


static void FunctToHandleDll(const char*a_name, const char*a_fullPath, void* a_mappedAddress,void*a_userData)
{
	if((!s_nVerbose)&&IsWeekDependency(a_name)){return;}
	if(a_fullPath){printf ( "%s => %s (%p)\n", a_name,a_fullPath, a_mappedAddress );}
	else{printf("%s => not found\n", a_name);}
}


static void FunctToHandleItself(const char*a_name, const char*a_fullPath, void* a_mappedAddress, void*a_userData)
{
	if (!a_fullPath) { AddToTheWeekDependent(a_name);}
}


static void PrintHelp(void)
{
	printf(
		"Autor:         Davit Kalantaryan (davit.kalantaryan@desy.de)\n"
		"Functionality: Makes linux like ldd\n"
		"Syntax:        ldd binary_name\n");
}



template <class T> static 
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, T* pNTHeader) // 'T' == PIMAGE_NT_HEADERS 
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
	unsigned i;

	for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		// This 3 line idiocy is because Watcom's linker actually sets the
		// Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
		DWORD size = section->Misc.VirtualSize;
		if (0 == size)
			size = section->SizeOfRawData;

		// Is the RVA within this section?
		if ((rva >= section->VirtualAddress) &&
			(rva < (section->VirtualAddress + size)))
			return section;
	}

	return 0;
}


template <class T> static
LPVOID GetPtrFromRVA(DWORD rva, T* pNTHeader, PBYTE imageBase) // 'T' = PIMAGE_NT_HEADERS 
{
	PIMAGE_SECTION_HEADER pSectionHdr;
	INT delta;

	pSectionHdr = GetEnclosingSectionHeader(rva, pNTHeader);
	if (!pSectionHdr)
		return 0;

	delta = (INT)(pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData);
	return (PVOID)(imageBase + rva - delta);
}


static bool CheckIfExistAndAddIfNeeded(const char* a_name)
{
	const size_t cunSize(s_DllsFound.size());
	for(size_t i(0);i<cunSize;++i){if(a_name==s_DllsFound[i]){return true;}}

	s_DllsFound.push_back(a_name);
	return false;
}

static int FindDependencyPrivate(fnc_to_handle_dll_type a_fnc, void* a_pUserData, PLOADED_IMAGE a_pImage);

static int FindDependency(const char* a_name,fnc_to_handle_dll_type a_fnc,void* a_pUserData,int a_bCall)
{
	PLOADED_IMAGE pImage = ImageLoad(a_name, NULL);
	
	if (!pImage) {
		( *a_fnc )( a_name, NULL, 0, a_pUserData );
		return -1; 
	}

	if (pImage->FileHeader->OptionalHeader.NumberOfRvaAndSizes >= 2){
		if(a_bCall){(*a_fnc)(a_name, pImage->ModuleName, pImage->MappedAddress, a_pUserData);}
		FindDependencyPrivate(a_fnc, a_pUserData, pImage);
	}

	ImageUnload(pImage);
	return 1;
}


static int FindDependencyPrivate(fnc_to_handle_dll_type a_fnc,void* a_pUserData,PLOADED_IMAGE a_pImage)
{
	char* pcName;
	PIMAGE_IMPORT_DESCRIPTOR importDesc =
		(PIMAGE_IMPORT_DESCRIPTOR)GetPtrFromRVA(
			a_pImage->FileHeader->OptionalHeader.DataDirectory[1].VirtualAddress,
			a_pImage->FileHeader, a_pImage->MappedAddress);

	while (1) {
		// See if we've reached an empty IMAGE_IMPORT_DESCRIPTOR
		if ( !importDesc || ( (importDesc->TimeDateStamp == 0) && (importDesc->Name == 0) )  ) { break; }
		pcName = (char*)GetPtrFromRVA(importDesc->Name, a_pImage->FileHeader, a_pImage->MappedAddress);

		if (CheckIfExistAndAddIfNeeded(pcName)) { goto nextDll; }
		FindDependency(pcName, a_fnc, a_pUserData, 1);
	nextDll:
		importDesc++;
	}

	return 1;
}


static void show_error(void)
{
	DWORD dwErrCode = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrCode,
		0, // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
	_ftprintf(stderr, _T("code=%d, msg=\"%s\"\n"), dwErrCode, (LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree(lpMsgBuf);
}

