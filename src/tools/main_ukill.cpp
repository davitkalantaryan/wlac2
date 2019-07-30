

// main_ukill.cpp
// 2017 Dec 13


#define ERR_BUF_LEN_MIN1	1023
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "remote_function_caller.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>

#ifdef MAKE_DEBUG
#ifdef _WIN32
#include <conio.h>
#else   // #ifdef WIN32
#define _getch(...)
#endif  // #ifdef WIN32
#endif  // #ifdef MAKE_DEBUG

int main(int argc, char* argv[])
{
	int nReturn = 0;
	int nSig, nPid;
	char vcErrBuff[ERR_BUF_LEN_MIN1 + 1];

	vcErrBuff[0] = 0;

	if (argc < 3 || (strlen(argv[1])<2) || ((argv[1])[0]!='-'))
	{
		nReturn = -1;
		goto returnPoint;
	}

	nSig = atoi((argv[1]+1));
	nPid = atoi(argv[2]);
	nReturn = kill(nPid, nSig);

	goto returnPoint;

returnPoint:
	if (nReturn)
	{
		if (vcErrBuff[0])
		{
			fprintf(stderr, "%s", vcErrBuff);
		}
		else
		{
			fprintf(stderr,"kill: kill %d failed: no such process",nPid);
			fflush(stderr);
		}
		//perror("\n");
	}
#ifdef MAKE_DEBUG
	printf("nReturn = %d\nPress any key to exit!", nReturn);
	fflush(stdout);
	_getch();
#endif  // #ifdef MAKE_DEBUG

	return nReturn;
}

