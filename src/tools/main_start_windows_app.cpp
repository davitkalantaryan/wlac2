//
// main_start_windows_app.cpp
// 2018 Feb 28

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <rfc/remote_function_caller.h>
#include <rfc/remote_function_caller_base.h>

#define		ERR_BUF_LEN_MIN1		1023

#if defined(_MSC_VER) & (_MSC_VER >= 1400)
#pragma warning(disable : 4996)
#endif

static long CreateNewProcess33(const char* a_cpcExecute, HANDLE a_hInp, HANDLE a_hOut, HANDLE a_hErr, PROCESS_INFORMATION* a_ProcInfo);
static void PrintHelp(void);

int main(int argc, char* argv[])
{

	int nReturn = 0;
	int nPid = 0;
	int nDebug = 0;
	int nDebug_target = 0;
	FILE* pflPidFile = NULL;
	char vcErrBuffer[ERR_BUF_LEN_MIN1 + 1];
	PROCESS_INFORMATION aProcInf;
	HANDLE hChildOutput, hChildInput;
	SECURITY_ATTRIBUTES saAttr;
	const char *cpcCommandLine=NULL, *cpcInpFileName=NULL,*cpcOutFileName=NULL, *cpcErrFileName=NULL;
	const char *cpcPidFile = NULL, *cpcCurrentDirectory=NULL;

	vcErrBuffer[0] = 0;
	aProcInf.hProcess = NULL;
	aProcInf.hThread = NULL;

	for (int i(1); i < argc; ++i)
	{
		if ((strcmp(argv[i], "--command-line") == 0) || (strcmp(argv[i], "-cl") == 0))
		{
			if (i<(argc-1))
			{
				cpcCommandLine = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--pid") == 0))
		{
			if (i<(argc - 1))
			{
				nPid = atoi(argv[++i]);
			}
		}

		else if ((strcmp(argv[i], "--std-input") == 0) || (strcmp(argv[i], "-si") == 0))
		{
			
			if (i<(argc - 1))
			{
				cpcInpFileName = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--std-output") == 0) || (strcmp(argv[i], "-so") == 0))
		{
			if (i<(argc - 1))
			{
				cpcOutFileName = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--std-error") == 0) || (strcmp(argv[i], "-se") == 0))
		{
			if (i<(argc - 1))
			{
				cpcErrFileName = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--current-directory") == 0) || (strcmp(argv[i], "-cd") == 0))
		{
			if (i<(argc - 1))
			{
				cpcCurrentDirectory = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--pid-file") == 0) || (strcmp(argv[i], "-pf") == 0))
		{
			if (i<(argc - 1))
			{
				cpcPidFile = argv[++i];
			}
		}

		else if ((strcmp(argv[i], "--debug-target") == 0))
		{
			nDebug_target = 1;
		}

		else if ((strcmp(argv[i], "--debug") == 0))
		{
			nDebug = 1;
		}

		else if ((strcmp(argv[i], "--help") == 0))
		{
			PrintHelp();
			goto returnPoint;
		}

	}  // for (int i(0); i < argc; ++i)

	if (nDebug)
	{
		printf("Press any key to continue!\n"); fflush(stdout);
		_getch();
	}

	if ((cpcCommandLine == NULL) && (nPid==0))
	{
		_snprintf(vcErrBuffer, ERR_BUF_LEN_MIN1, "Program to handle is not specified!");
		nReturn = 1;
		PrintHelp();
		goto returnPoint;
	}

	if (cpcCurrentDirectory)
	{
		SetCurrentDirectoryA(cpcCurrentDirectory);
	}

	if (cpcCommandLine)
	{
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;
		CreatePipe(&hChildOutput, &hChildInput, &saAttr, 0);
		CreateNewProcess33(cpcCommandLine, hChildInput, hChildOutput, hChildOutput, &aProcInf);
	}
	else
	{
		aProcInf.dwProcessId = nPid;
		aProcInf.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (nPid));

		if (!aProcInf.hProcess)
		{
			_snprintf(vcErrBuffer, ERR_BUF_LEN_MIN1, "PID(%d) is not correct!",nPid);
			nReturn = -60;
			goto returnPoint;
		}
		aProcInf.dwThreadId = 0;
		aProcInf.hThread = NULL;
	}

#ifdef DEBUG_IS_ACTIVE2
	printf("Press any key to continue!");
	fflush(stdout);
	_getch();
#endif
	if (cpcInpFileName)
	{
		freopen_std_new_fast(aProcInf.hProcess, aProcInf.dwProcessId, cpcInpFileName,"r",0);
	}

	if (cpcOutFileName)
	{
		freopen_std_new_fast(aProcInf.hProcess, aProcInf.dwProcessId, cpcOutFileName, "a+", 1);
	}

	if (cpcErrFileName)
	{
		freopen_std_new_fast(aProcInf.hProcess, aProcInf.dwProcessId, cpcErrFileName, "a+", 2);
	}

	if (cpcPidFile)
	{
		pflPidFile = fopen(cpcPidFile,"w");
		if (!pflPidFile)
		{
			_snprintf(vcErrBuffer, 1023, "Unable to open pid file!");
			nReturn = -31;
			goto returnPoint;
		}

		fprintf(pflPidFile, "%d\n", (int)aProcInf.dwProcessId);
		fclose(pflPidFile);
		pflPidFile = NULL;
	}

returnPoint:
	
	if (pflPidFile){ fclose(pflPidFile); }
	if (aProcInf.hThread)
	{ 
		if (nDebug_target)
		{
			printf("Attach debugger to the process %d and press any key to resume that process!\n", 
				(int)aProcInf.dwProcessId); fflush(stdout);
			_getch();
		}
		ResumeThread(aProcInf.hThread); CloseHandle(aProcInf.hThread);
	}
	if (aProcInf.hProcess){ CloseHandle(aProcInf.hProcess); }

	if (nReturn && vcErrBuffer[0])
	{
		fprintf(stderr, "%s", vcErrBuffer);
		perror("\n");
	}

#ifdef DEBUG_IS_ACTIVE1
	printf("\nnReturn=%d\nPress any key to finish the program!", nReturn);
	fflush(stdout);
	_getch();
#endif
		 
	return nReturn;
}

static void PrintHelp(void)
{
	printf(
		"Author: D. Kalantaryan (davit.kalantaryan@desy.de)\n"
		"This appliation starts new program or handles already existing program\n"
		"Already running program can be specified by pid\n"
		"                                 (--pid 1045)\n"
		"For starting new program command line should be specified\n"
		"                                 (--command-line \"simple_tcp_server 1124\")\n"
		"\nFollowing are the list of all possible command line arguments\n"
		"  --command-line      (-cl), arg=command_line      : Specifies the command line for starting new application\n"
		"  --pid               (   ), arg=pid_of_programm   : Specifies the running program to handle\n"
		"  --std-input         (-si), arg=input_file_name   : Specifies the name of file\n"
		"                                                     from which programm should read standard input\n"
		"  --std-output        (-so), arg=output_file_name  : Specifies the name of file\n"
		"                                                     which will be used by programm for standard output\n"
		"  --std-error         (-se), arg=error_file_name   : Specifies the name of file\n"
		"                                                     which will be used by programm for error output\n"
		"  --current-directory (-cd), arg=current_directory : Specifies the current directory for target program\n"
		"  --run-as-service    (   ), No argument           : If this option is set, then program runs as windows service\n"
		"                                                     even if program initially created as application\n"
		"  --pid-file          (-pf), arg=file_name_for_pid : Specifies the name of file\n"
		"                                                     where the pid of target program will be written\n"
		"  --debug-target      (   ), No argument           : If this option is set, then target program\n"
		"                                                     stops the execution and waits untill debugger is attached\n"
		"  --debug             (   ), No argument           : If this option is set, then this starter program\n"
		"                                                     stops the execution and waits untill debugger is attached\n"
		"  --help              (   ), No argument           : If this option is set, then this starter program\n"
		"                                                     prints this help and exits\n"
		"\n");
}

static long CreateNewProcess33(const char* a_cpcExecute, HANDLE a_hInp, HANDLE a_hOut, HANDLE a_hErr, PROCESS_INFORMATION* a_ProcInfo)
{
	DWORD dwErrCode;
	STARTUPINFOA aStartInfo;

	//STARTUPINFOA aStartInfo;
	ZeroMemory(&aStartInfo, sizeof(aStartInfo));
	aStartInfo.dwFlags = STARTF_USESTDHANDLES;
	aStartInfo.hStdInput = a_hInp;
	aStartInfo.hStdOutput = a_hOut;
	aStartInfo.hStdError = a_hErr;
	aStartInfo.wShowWindow = SW_HIDE;
	//aStartInfo.wShowWindow = SW_SHOW;
	aStartInfo.dwFlags = STARTF_USESTDHANDLES;

	//PROCESS_INFORMATION aProcInfo; 
	ZeroMemory(a_ProcInfo, sizeof(*a_ProcInfo));

	dwErrCode = CreateProcessA(
		NULL,
		//const_cast<char*>(a_cpcExecute),
		(char*)a_cpcExecute,
		NULL,
		NULL,
		TRUE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_SUSPENDED,
		NULL,
		NULL,
		&aStartInfo,
		a_ProcInfo
		);

	return (long)((size_t)a_ProcInfo->hProcess);
}

#if 0
static long CreateNewProcess22(const char* a_cpcExecute, int a_nFD_Inp, int a_nFD_Out, int a_nFD_Err, PROCESS_INFORMATION* a_ProcInfo)
{
	long lnPipe;
	HANDLE rPipeInp, wPipeOut, wPipeErr;
	//////////////////////////////////////////////////////////////////////
	////// Creating system pipes from input file descriptors  ////////////
	//////////////////////////////////////////////////////////////////////
	//Create input pipe
	if ((lnPipe = (long)_get_osfhandle(a_nFD_Inp)) == -1){ return 0L; }
	rPipeInp = (HANDLE)lnPipe;
	//Create output pipe
	if ((lnPipe = (long)_get_osfhandle(a_nFD_Out)) == -1){ return 0L; }
	wPipeOut = (HANDLE)lnPipe;
	//Create error pipe
	if ((lnPipe = (long)_get_osfhandle(a_nFD_Err)) == -1){ return 0L; }
	wPipeErr = (HANDLE)lnPipe;

	return CreateNewProcess33(a_cpcExecute, rPipeInp, wPipeOut, wPipeErr, a_ProcInfo);
}
#endif // if 0

