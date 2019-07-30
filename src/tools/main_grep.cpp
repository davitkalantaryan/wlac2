/*
 *	File		: 'main_grep.cpp' For WINDOWS
 *
 *	Created on	: Dec 22, 2015
 *	Author		: Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef DebugDbg_used
#ifdef _WIN32
#include <conio.h>
#else
#define _getch	getchar
#endif
#endif

#define LINE_MAX_LENGTH		1024

static void PrintHelp(void);

int main(int argc, char* argv[])
{
	char *pcBufTmp((char*)1),*pcBufferForGrep = (char*)malloc(LINE_MAX_LENGTH);
	const char* cpcStringToMatch;
	int cunInpStrLen;
	int unBufLenPrev,unOutBufferLen(LINE_MAX_LENGTH),unOffset(0);

#ifdef DebugDbg_used
	printf("Attach debugger and press any key and hit enter to continue!"); fflush(stdout);
	_getch();
	printf("\n");
#endif

	if(!pcBufferForGrep){
		fprintf(stderr,"No memory!\n");
		return 1;
	}
	unBufLenPrev = unOutBufferLen;

	if (argc < 2){
		fprintf(stderr,
			"Usage: grep [OPTION]... PATTERN [FILE]...\n"
			"Try 'grep --help' for more information.\n");
		return 2;
	}

	if (strcmp(argv[1], "--help")==0){
		PrintHelp();
		return 0;
	}
	cpcStringToMatch = argv[1];

	cunInpStrLen = (int)strlen(cpcStringToMatch);
	pcBufferForGrep[unOutBufferLen-2]=0;
	
	//while(fget()
	while(fgets(pcBufferForGrep+unOffset,unOutBufferLen-unOffset, stdin) != NULL){
		if((pcBufferForGrep[unOutBufferLen-2] != 0)&&(pcBufferForGrep[unOutBufferLen-2] !='\n')){
			unBufLenPrev = unOutBufferLen;
			unOutBufferLen <<= 1;
			pcBufTmp = (char*)realloc(pcBufferForGrep,unOutBufferLen);
			if(!pcBufTmp){goto returnPoint;}
			pcBufferForGrep = pcBufTmp;
			unOffset = unBufLenPrev - 1;
			pcBufferForGrep[unOutBufferLen - 2] = 0;
			continue;
		}
		if (strstr(pcBufferForGrep, cpcStringToMatch)) { printf("%s",pcBufferForGrep); }
		unOffset = 0;
		pcBufferForGrep[unOutBufferLen - 2] = 0;
	}

returnPoint:
	free(pcBufferForGrep);
	return pcBufTmp?0:1;
}


static void PrintHelp(void)
{
	printf(
		"Autor:         Davit Kalantaryan (davit.kalantaryan@desy.de)\n"
		"Functionality: Makes linux like grep\n"
		"Syntax:        grep pattern\n");
}
