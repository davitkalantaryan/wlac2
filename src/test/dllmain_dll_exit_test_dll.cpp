
#include <windows.h>
#include <stdio.h>

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved)
{
	printf("hinstDLL=%p, fdwReason=%d, lpvReserved=%p\n", (void*)hinstDLL,(int)fdwReason,lpvReserved);


	return TRUE;
}
