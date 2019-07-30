/*
 *	File: main_ufand.cpp
 *
 *	Created on : 24 Nov, 2016
 *	Created by : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions ...
 *		1) ...
 *		2) ...
 *		...
 *
 *
 */

#include <windows.h>
#include <string>
#include <stdio.h>
#include <common_argument_parser.hpp>
#include "util/directory_iterator.h"


static int IterFunc(const char* dir, const FIND_DATAA* file_info, void*user, int isDir);

static int _____s_will_be_stop_on_find = 1;

static void SearchFile(const char* a_dir, const char* a_file,int a_stop_on_find)
{
	int nSub=1;
	_____s_will_be_stop_on_find = a_stop_on_find ? -1 : 1;
	IterateOverDirectoryFiles(a_dir, IterFunc, (void*)a_file, &nSub);
}

int main(int argc, char* argv[])
{
	common::argument_parser aParser;
	int	stop_on_find = 0;
	int nReturn = 0;
	int argcMin1 = argc - 1;
	const char* pOptionValue = NULL;
	const char* cpcFileName;
	const char* cpcDirectory;

	aParser.AddOption("-name", 1,"").AddOption("--stop-on-find",1,"") << "-sof"<<"-nsof";
	aParser.ParseCommandLine<int&, char*>(argcMin1, argv+1);

	if (argcMin1 == 0)
	{
		fprintf(stderr, "directory to search is not provided!\n");
		goto returnPoint;
	}

	pOptionValue = aParser["-name"];
	if (!pOptionValue)
	{
		fprintf(stderr, "directory to search is not provided!\n");
		goto returnPoint;
	}
	cpcFileName = pOptionValue;

	pOptionValue = aParser["-sof"];
	if (pOptionValue){stop_on_find = 1;}
	else
	{
		pOptionValue = aParser["--stop-on-find"];
		if (pOptionValue){ stop_on_find = atoi(pOptionValue); }
	}

	if (argcMin1 < 1)
	{
		fprintf(stderr,"directory for search is not provided!\n");
		goto returnPoint; 
	}
	cpcDirectory = argv[1];

	SearchFile(cpcDirectory, cpcFileName, stop_on_find);

returnPoint:
	return nReturn;
}


static int IterFunc(const char* a_dir,const WIN32_FIND_DATAA* a_file_info, void* a_user, int isDir)
{
	char *pcFound,*pcUsrTmp;
	size_t unStrLen0;
	const char *cpcUserString = (const char*)a_user, *cpcFileName(a_file_info->cFileName);
	int nMatch = 0;

	if (!cpcUserString || ((unStrLen0 = strlen(cpcUserString)) == 0))
	{
		//report
		return -1;
	}

	pcUsrTmp = (char*)_alloca(unStrLen0 + 1);
	memcpy(pcUsrTmp, cpcUserString, unStrLen0 + 1);

	pcFound = strchr(pcUsrTmp, '*');
	while (pcFound)
	{
		pcFound[0] = 0;
		if ((cpcFileName=strstr(cpcFileName, pcUsrTmp)) == NULL){ return 0; }
		++cpcFileName;
		pcUsrTmp = pcFound + 1;
		pcFound = strchr(pcUsrTmp, '*');
	}
	if (strcmp(pcUsrTmp, cpcFileName) == 0)
	{
		printf("%s\\%s\n", a_dir, a_file_info->cFileName);
		return _____s_will_be_stop_on_find;
	}
	return 0;
}
