
// dll_exit_test_exe.cpp
// 2017 Nov 29
// created by D. Kalantaryan

//#define USE_LIB_STATIC

#define TEST_LIBRARY_NAME_BASE	"dll_exit_test_dll."

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stddef.h>

typedef void(*TypeFunction)(void);

#ifdef USE_LIB_STATIC
#pragma comment(lib,TEST_LIBRARY_NAME_BASE "lib")
void TestLibraryFunction1(void);
#else // #ifdef USE_LIB_STATIC
#endif  // #ifdef USE_LIB_STATIC


int main()
{
	TypeFunction fpLibFunction = NULL;
#ifdef USE_LIB_STATIC
	fpLibFunction = &TestLibraryFunction1;
#else   // #ifndef USE_LIB_STATIC
	HMODULE hmodTestLib = LoadLibraryA("W:\\udoocs\\contrib\\sys\\win32\\dll\\" TEST_LIBRARY_NAME_BASE "dll");
	printf("hmodTestLib=%p\n",(void*)hmodTestLib);
	if(!hmodTestLib){
		goto returnPoint;
	}
	fpLibFunction=(TypeFunction)GetProcAddress(hmodTestLib, "TestLibraryFunction1");
#endif  // #ifndef USE_LIB_STATIC


	if(fpLibFunction){
		(*fpLibFunction)();
	}

#ifndef USE_LIB_STATIC
#endif  // #ifndef USE_LIB_STATIC

returnPoint:
	printf("Press any key to exit!");
	fflush(stdout);
	_getch();
	printf("\n");

	return 0;

}
