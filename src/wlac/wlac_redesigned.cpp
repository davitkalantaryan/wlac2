
// wlac_redesigned.cpp
// 2017 Dec 07

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <alloca.h>
#include <string.h>
#include <signal.h>
#include "pthread_private_for_source.h"

__BEGIN_C_DECLS


GEM_API int* wlac_errno(void)
{
	static int snWlacErrno = 0;

	snWlacErrno = *(_errno());
	if(!snWlacErrno){
		snWlacErrno = WSAGetLastError();
		if(snWlacErrno== WSAEWOULDBLOCK){
			snWlacErrno = EINPROGRESS;
		}
		
	}

	return &snWlacErrno;
}


GEM_API void wlac_exit(int a_status, int a_nSignal)
{

	if(a_nSignal>=0){
		static int snAttempt = 0;
		if (snAttempt == 0){
			snAttempt = 1;
			raise(a_nSignal);
			return;
		}
	}

	_exit(a_status);
}

GEM_API FILE* wlac_fopen(const char* a_fileName, const char* a_mode)
{
	FILE* fpFile = NULL;
	errno_t nError;
	int nIsBynary = 1;

	if(strchr(a_mode,'t')){
		nIsBynary = 0;
	}

	if(nIsBynary && !strchr(a_mode,'b')){
		size_t  unModeStrLen = strlen(a_mode);
		char* pcNewMode = (char*)_alloca(unModeStrLen + 2);
		memcpy(pcNewMode, a_mode, unModeStrLen);
		pcNewMode[unModeStrLen] = 'b';
		pcNewMode[unModeStrLen+1] = '\0';
		a_mode = pcNewMode;
	}

	nError = fopen_s(&fpFile, a_fileName, a_mode);

	if(nError){
		errno = nError;
		return NULL;
	}

	return fpFile;
}


GEM_API int wlac_rename(const char *a_oldname, const char *a_newname)
{
	DeleteFileA(a_newname);
#ifdef rename
#undef rename
#endif
	return rename(a_oldname, a_newname);
}


#ifdef strftime
#undef strftime
#endif

#define CHECK_FTIME_RANGE(__v_to_ck, __minm, __maxm )  (((__v_to_ck)<(__minm)) || ((__v_to_ck)>(__maxm)))

#define NOT_IMPLEMENTED_FTIME(__format,__index)	\
					(__format)[(__index)-1] = 'n'; (__format)[(__index)] = 'p'
 
GEM_API size_t wlac_strftime(char *a_strDest, size_t a_maxsize, const char *a_format, const struct tm *a_timeptr)
{
	int i, nFormatStrLenMin1 = (int)strlen(a_format)-1;
	char* pcNewFormat = (char*)alloca(nFormatStrLenMin1 + 2);

	if (CHECK_FTIME_RANGE(a_timeptr->tm_mday, 1, 31))  /// To be more correct all filds should be checked
	{
		return 0;
	}


	for (i = 0; i < nFormatStrLenMin1; ++i)
	{
		pcNewFormat[i] = a_format[i];

		if (a_format[i] == '%')
		{
			switch (a_format[++i])
			{
			case 'C':
				pcNewFormat[i] = 'Y';
				break;
			case 'D':
				pcNewFormat[i] = 'd';
				break;
			case 'e':
				pcNewFormat[i] = 'd';
				break;
			case 'E':
				NOT_IMPLEMENTED_FTIME(pcNewFormat,i);
				break;
			case 'F':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'g':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'G':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'h':
				pcNewFormat[i] = 'b';
				break;
			case 'k':
				pcNewFormat[i] = 'H';
				break;
			case 'n':
				pcNewFormat[i-1] = '\\';
				pcNewFormat[i] = 'n';
				break;
			case 'O':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'P':
				pcNewFormat[i] = 'p'; /// Almost correct
				break;
			case 'r':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'R':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 's':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 't':
				pcNewFormat[i - 1] = '\\';
				pcNewFormat[i] = 't';
				break;
			case 'T':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			case 'u':
				pcNewFormat[i] = 'w'; /// Almost implemented 'w'(0-6), 'u'(1-7)
				break;
			case 'V':
				pcNewFormat[i] = 'U'; /// Almost implemented 'V'(1-53), 'U'(0-53)
				break;
			case '+':
				NOT_IMPLEMENTED_FTIME(pcNewFormat, i);
				break;
			default:
				pcNewFormat[i] = a_format[i];
				break;
				
			}  // switch (a_format[++i])

		}  // if (a_format[i] == '%')

	}  // for (i = 0; i < nFormatStrLenMin1; ++i)

	pcNewFormat[nFormatStrLenMin1 + 1] = '\0';

	return strftime(a_strDest, a_maxsize, pcNewFormat, a_timeptr);
}


GEM_API char* wlac_getenv(const char* a_name)
{
	struct pthread_s_new* pThreadData = GetCurrentThreadDataPointer();
	if(LIKELY2(pThreadData)){
		DWORD dwCount = GetEnvironmentVariableA(a_name, pThreadData->resourse, pThreadData->resourseSize);
		if (!dwCount) { return NEWNULLPTR2; }
		if((dwCount+1)> pThreadData->resourseSize){
			char* pcFrmBuff = STATIC_CAST2(char*,realloc(pThreadData->resourse,(dwCount+1)));
			if(pcFrmBuff){
				pThreadData->resourse = pcFrmBuff;
				pThreadData->resourseSize = (dwCount + 1);
				GetEnvironmentVariableA(a_name, pThreadData->resourse, pThreadData->resourseSize);
			}
		}
		return pThreadData->resourse;
	}
	return NEWNULLPTR2;
}

#ifdef strerror
#undef strerror
#endif
GEM_API char* wlac_strerror(int a_nError)
{
	struct pthread_s_new* pThreadData = GetCurrentThreadDataPointer();
	if(LIKELY2(pThreadData)){
		errno_t tError = strerror_s(pThreadData->resourse, pThreadData->resourseSize, a_nError);

		if(tError){
			_snprintf(pThreadData->resourse, pThreadData->resourseSize, "wlac_strerror returned %d", (int)tError);
		}
		return pThreadData->resourse;

	}
	return "error to get thread data";
}


__END_C_DECLS
