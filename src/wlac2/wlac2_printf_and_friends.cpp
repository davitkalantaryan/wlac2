
// wlac_printf_and_friends.cpp
// 2017 Dec 22

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <string.h>
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN64
#include <inttypes.h> /* For PRIu64 */
static const size_t s_cunFormatLenStandardMin1 = strlen(PRId64);
static const size_t s_cunFormatLenStandard = s_cunFormatLenStandardMin1 + 1;  // "%" PRId64
static const size_t s_cunFormatLenNotCorrect = 3;  // %ld
static const char* ChangeFormatting2(const char* a_format, char* a_buffer, size_t a_unFormatLenPlus1);
#endif  // #ifdef _WIN64


__BEGIN_C_DECLS

#if defined(_MSC_VER) & (_MSC_VER>1400)
#pragma warning (disable:4996)
#endif

#ifdef _WIN64

GEM_API int wlac_snprintf(char* a_buffer, size_t a_count,const char* a_format,...)
{
	const size_t unInitialFormatSizePlus1 = strlen(a_format)+1;
	char* pcNewFormat = (char*)_alloca(unInitialFormatSizePlus1*2);
	va_list vaArgs;
	int nReturn;
	
	va_start(vaArgs, a_format);
	nReturn = _vsnprintf(a_buffer, a_count, ChangeFormatting2(a_format, pcNewFormat,unInitialFormatSizePlus1), vaArgs);
	va_end(vaArgs);

	return nReturn;
}


GEM_API int wlac_sprintf(char* a_buffer, const char* a_format, ...)
{
	const size_t unInitialFormatSizePlus1 = strlen(a_format) + 1;
	char* pcNewFormat = (char*)_alloca(unInitialFormatSizePlus1*2);
	va_list vaArgs;
	int nReturn;

	va_start(vaArgs, a_format);
	nReturn = vsprintf(a_buffer, ChangeFormatting2(a_format, pcNewFormat,unInitialFormatSizePlus1), vaArgs);
	va_end(vaArgs);

	return nReturn;
}


GEM_API int wlac_sscanf(const char* a_buffer, const char* a_format,...)
{
	const size_t unInitialFormatSizePlus1 = strlen(a_format)+1;
	char* pcNewFormat = (char*)_alloca(unInitialFormatSizePlus1*2);
	va_list vaArgs;
	int nReturn;
		
	va_start(vaArgs, a_format);
	nReturn = vsscanf(a_buffer, ChangeFormatting2(a_format, pcNewFormat,unInitialFormatSizePlus1), vaArgs);
	va_end(vaArgs);

	return nReturn;
}

#endif  // #ifdef _WIN64


__END_C_DECLS

#ifdef _WIN64

static const char* ChangeFormatting2(const char* a_format, char* a_buffer2, size_t a_unFormatLenPlus1)
{
	//ptrdiff_t nMove;
	char *pcBufTmp = a_buffer2;
	char *pcFoundSrc;
	size_t unMove;
	size_t unIndex;

	memcpy(a_buffer2, a_format, a_unFormatLenPlus1);

	pcFoundSrc = strstr(pcBufTmp, "%ld");
	if (!pcFoundSrc) {pcFoundSrc = strstr(pcBufTmp, "%lu");}

	while(pcFoundSrc){
		unIndex = (size_t)(pcFoundSrc - pcBufTmp);
		unMove = a_unFormatLenPlus1 - unIndex - s_cunFormatLenNotCorrect;
		if(s_cunFormatLenStandard!=s_cunFormatLenNotCorrect){memmove(pcFoundSrc+s_cunFormatLenStandard,pcFoundSrc+s_cunFormatLenNotCorrect,unMove);}
		memcpy(pcFoundSrc+1,PRId64,s_cunFormatLenStandardMin1);
		pcBufTmp = pcFoundSrc + s_cunFormatLenStandard;
		a_unFormatLenPlus1 -= (s_cunFormatLenNotCorrect + unIndex);

		pcFoundSrc = strstr(pcBufTmp, "%ld");
		if (!pcFoundSrc) { pcFoundSrc = strstr(pcBufTmp, "%lu"); }
	}

	return a_buffer2;
}

#endif  // #ifdef _WIN64
