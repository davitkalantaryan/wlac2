
// main_make_symbols_exportable.cpp
// 2017 Dec 09

#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <direct.h>

#if defined(_MSC_VER) && (_MSC_VER>1400)
#pragma warning(disable:4996)
#endif

#define FILE_FROM_PATH(_path_)	(  strrchr((_path_),'\\')?(strrchr((_path_),'\\')+1) : \
												(strrchr((_path_),'/')?(strrchr((_path_),'/')+1):(_path_))   )

//#define EXPORT_SYMBOL		"__declspec(dllexport) "
#define CLASS_KEYWORD		"class"
#define STRUCT_KEYWORD		"struct"

static const size_t	s_cunClassKeyLen = strlen(CLASS_KEYWORD);
static const size_t	s_cunStructKeyLen = strlen(STRUCT_KEYWORD);

//static const size_t	s_cunClassKeyLenPlus1 = strlen(CLASS_KEYWORD) + 1;
//static const size_t	s_cunStructKeyLenPlus1 = strlen(STRUCT_KEYWORD) + 1;

int main(int argc, char* argv[])
{
	char *cpStrStart,*pcTemp,*pcAfterKeyword;
	FILE *fpInp(NULL), *fpOut(NULL);
	char** ppcClassNames = &argv[4];
	size_t unOffset;
	size_t unExpSymLen, unKeyLenToAdd;
	const int cnVectorSize(argc - 4);
	int nVectorIndex;
	int nReturn(0), nNumber(0);
#ifdef _DEBUG
	int nLine(0);
#endif 
	char data[1024];

	if(argc<4){
		fprintf(stderr,"Provide input and output file names and key to be added!\n");
		goto returnPoint;
	}

	unExpSymLen = strlen(argv[3]);

	fpInp = fopen(argv[1], "rb");
	if(!fpInp){
		fprintf(stderr, "Unable to open input file\"%s\"\n", argv[1]);
		goto returnPoint;
	}


	fpOut = fopen(argv[2], "wb");
	if(!fpOut){
		size_t unStrLen;
		char* pcDir;
		char* pcLastDelim = strrchr(argv[2], '\\');
		if(!pcLastDelim || (pcLastDelim == argv[2])){
			pcLastDelim = strrchr(argv[2], '/');
			if (!pcLastDelim || (pcLastDelim == argv[2])) {
				fprintf(stderr, "Unable to open output file\"%s\"\n", argv[2]);
				fprintf(stderr, "Probably no write permission!\n");
				goto returnPoint;
			}
		}

		unStrLen = pcLastDelim - argv[2];
		pcDir = (char*)_alloca(unStrLen + 1);
		memcpy(pcDir, argv[2], unStrLen);
		pcDir[unStrLen] = 0;

		_mkdir(pcDir);

		fpOut = fopen(argv[2], "wb");
		if (!fpOut) {
			fprintf(stderr, "Unable to open output file\"%s\"\n", argv[2]);
			goto returnPoint;
		}
		
	}  // if(!fpOut){


	while (fgets(data, 1023, fpInp)) {
		//size_t strspn(const char * str1, const char * str2);

		cpStrStart = data;
		pcAfterKeyword = nullptr;
		pcTemp = strstr(data,CLASS_KEYWORD);
		if(pcTemp){
			//pcAfterKeyword = pcTemp + s_cunClassKeyLenPlus1;
			pcAfterKeyword = pcTemp + s_cunClassKeyLen;
			unKeyLenToAdd = s_cunClassKeyLen;
		}
		else {
			pcTemp = strstr(data, STRUCT_KEYWORD);
			if (pcTemp) {
				//pcAfterKeyword = pcTemp + s_cunStructKeyLenPlus1;
				pcAfterKeyword = pcTemp + s_cunStructKeyLen;
				unKeyLenToAdd = s_cunStructKeyLen;
			}
		}

		if(pcAfterKeyword){
			if(cnVectorSize>0){
				for(nVectorIndex=0;nVectorIndex<cnVectorSize;++nVectorIndex){
					pcTemp = strstr(pcAfterKeyword,ppcClassNames[nVectorIndex]);
					if(pcTemp){
						unOffset = static_cast<size_t>(pcAfterKeyword - cpStrStart)+1;
						fwrite(data, 1, unOffset, fpOut);
						fwrite(argv[3], 1, unExpSymLen, fpOut);
						cpStrStart = pcAfterKeyword ;
						++nNumber;
						break;
					}
				} // for(nVectorIndex=0;nVectorIndex<cnVectorSize;++nVectorIndex){
			}  // if(cnVectorSize>0){
			else {  // for if(cnVectorSize>0){
				unOffset = static_cast<size_t>(pcAfterKeyword - cpStrStart) + 1;
				fwrite(data, 1, unOffset, fpOut);
				fwrite(argv[3], 1, unExpSymLen, fpOut);
				cpStrStart = pcAfterKeyword;
				++nNumber;
			}  // else {  // for if(cnVectorSize>0){
		}  // if(pcAfterKeyword){

		fputs(cpStrStart, fpOut);

#ifdef _DEBUG
		++nLine;
		if(strstr(data,"merge")){
			printf("Line of merge :  %d!\n", nLine);
		}
		else{
			//printf("%d. %s",nLine,data);
		}
#endif

	}

	printf("File:%s, NumberOfExportsCommentsIncluded=%d\n", FILE_FROM_PATH(argv[1]), nNumber);
returnPoint:
	if(fpOut){
		fclose(fpOut);
	}
	if (fpInp) {
		fclose(fpInp);
	}
#ifdef _DEBUG
	printf("Press any key to exit!"); fflush(stdout);
	_getch();
	printf("\n");
#endif
	return nReturn;
}
