//
// file:		main_victim_test.cpp
// created on:	2018 Dec 18
//

#ifndef CINTERFACE
#define CINTERFACE
#endif // !CINTERFACE

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

static volatile int s_nRun = 1;

int main()
{
	int nPid = GetCurrentProcessId();

	printf("pid=%d, lodaLibraryAddress=%p\n", nPid,&LoadLibraryA);

	s_nRun = 1;
	while(s_nRun){
		SleepEx(INFINITE, TRUE);
	}
}
