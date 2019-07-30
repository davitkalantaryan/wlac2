
// 2017 Dec 11

#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>

#define alloca _alloca

#if defined(_MSC_VER) && (_MSC_VER>1400)
#pragma warning(disable:4996)
#endif

#define FILE_FROM_PATH(_path_)	(  strrchr((_path_),'\\')?(strrchr((_path_),'\\')+1) : \
												(strrchr((_path_),'/')?(strrchr((_path_),'/')+1):(_path_))   )

#define WindowsTargetPlatformVersionStart	"<WindowsTargetPlatformVersion>"
#define WindowsTargetPlatformVersionEnd		"</WindowsTargetPlatformVersion>"

const size_t s_cunPlatformStartLen(strlen(WindowsTargetPlatformVersionStart));

struct SMem {
	char* buff() { return m_buff; }
	int resize(size_t a_newSize);
	SMem():m_buff(NULL),m_size(0){}
	~SMem() { free(m_buff); }
private:
	char*	m_buff;
	size_t	m_size;
};

static int SetWindowsTargetPlatform(SMem* a_pMem, const char* a_fileName, const char* a_targetPlatformName);

int main(int argc, char* argv[])
{
	SMem aMem;
	int i,nReturn(0);

	if(argc<3){
		fprintf(stderr,"Provide project files and WindowsTargetPlatformVersion\n");
		nReturn = 1;
		goto returnPoint;
	}

	--argc;
	for(i=1; i<argc;++i){
		nReturn = SetWindowsTargetPlatform(&aMem, argv[i], argv[argc]);
		if(nReturn){
			goto returnPoint;
		}
	}

	
returnPoint:
#ifdef _DEBUG
	printf("Press any key to exit!");
	fflush(stdout);
	_getch();
	printf("\n");
#endif
	return nReturn;
}



static int SetWindowsTargetPlatform(SMem* a_pMem, const char* a_fileName, const char* a_targetPlatformName)
{
	char* pcOldPlatform;
	char *pcPlatformStart, *pcPlatformEnd;
	FILE* fpProjFile(NULL);
	struct stat fStat;
	size_t unCount;
	const int cnTargetNamelen((int)strlen(a_targetPlatformName));
	int nTargetNamelenIn;
	int nReturn(0);

	fpProjFile = fopen(a_fileName, "r+b");
	if (!fpProjFile) {
		fprintf(stderr, "Unable to open the file \"%s\"\n", a_fileName);
		nReturn = 2;
		goto returnPoint;
	}
	if (fstat(fileno(fpProjFile), &fStat)) {
		fprintf(stderr, "fstat error\n");
		nReturn = 3;
		goto returnPoint;
	}

	if(a_pMem->resize(fStat.st_size + 1 + cnTargetNamelen)){
		fprintf(stderr, "Low memory\n");
		nReturn = 4;
		goto returnPoint;
	}

	fread(a_pMem->buff(), 1, fStat.st_size, fpProjFile);
	(a_pMem->buff())[fStat.st_size] = 0;

	pcPlatformStart = strstr(a_pMem->buff(), WindowsTargetPlatformVersionStart);
	if (!pcPlatformStart) {
		fprintf(stderr, "Unable to find platform start!\n");
		nReturn = 5;
		goto returnPoint;
	}
	pcPlatformStart += s_cunPlatformStartLen;

	pcPlatformEnd = strstr(pcPlatformStart, WindowsTargetPlatformVersionEnd);
	if (!pcPlatformEnd) {
		fprintf(stderr, "Unable to find platform end!\n");
		nReturn = 6;
		goto returnPoint;
	}
	nTargetNamelenIn = (int)(((size_t)pcPlatformEnd) - ((size_t)pcPlatformStart));

	if(   (nTargetNamelenIn==cnTargetNamelen)&&(memcmp(pcPlatformStart, a_targetPlatformName, cnTargetNamelen)==0)   ){
		nReturn = 0;
		goto returnPoint;
	}

	pcOldPlatform = (char*)alloca(nTargetNamelenIn + 1);
	memcpy(pcOldPlatform, pcPlatformStart, nTargetNamelenIn);
	pcOldPlatform[nTargetNamelenIn] = 0;
	unCount = ((size_t)fStat.st_size) - (((size_t)pcPlatformEnd) - ((size_t)a_pMem->buff()));
	memmove(pcPlatformStart + cnTargetNamelen, pcPlatformEnd, unCount);
	memcpy(pcPlatformStart, a_targetPlatformName, cnTargetNamelen);
	fclose(fpProjFile);
	fpProjFile = fopen(a_fileName, "wb");
	if (!fpProjFile) {
		fprintf(stderr, "Unable to open the file2 \"%s\"\n", a_fileName);
		nReturn = 7;
		goto returnPoint;
	}
	unCount = (size_t)(((int)fStat.st_size) + cnTargetNamelen - nTargetNamelenIn);
	fwrite(a_pMem->buff(), 1, unCount, fpProjFile);
	
	printf("File:%s, TargetPlatform(%s->%s) was set\n", FILE_FROM_PATH(a_fileName), pcOldPlatform,a_targetPlatformName);

returnPoint:
	if (fpProjFile) {
		fclose(fpProjFile);
	}
	return nReturn;
}


int SMem::resize(size_t a_newSize)
{
	if(a_newSize>m_size){
		char* pcBuffer = (char*)realloc(m_buff, a_newSize);
		if(!pcBuffer){
			return 1;
		}
		m_buff = pcBuffer;
		m_size = a_newSize;
	}
	return 0;
}
