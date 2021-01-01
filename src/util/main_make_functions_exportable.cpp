
// main_make_functions_exportable.cpp
// 2017 Dec 12


#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <direct.h>
#include <ctype.h>

#define NUMBER_OF_BRACES_TO_EXPORT	-1  // <0 exports in any case

#if defined(_MSC_VER) && (_MSC_VER>1400)
#pragma warning(disable:4996)
#endif

#define FILE_FROM_PATH(_path_)	(  strrchr((_path_),'\\')?(strrchr((_path_),'\\')+1) : \
												(strrchr((_path_),'/')?(strrchr((_path_),'/')+1):(_path_))   )

#define MLcommentStart	"/*"
#define MLcommentEnd	"*/"
#define DefineString	"#define"

// todo: think about whether this is usefull
#define STR_LEN_FOR_USING	5

typedef const char* TypeConstCharPtr;

static const size_t s_cunMLcommentEndLen = strlen(MLcommentEnd);
static const size_t s_cunDefineStringLen = strlen(DefineString);
static int AddStringToTheLine(FILE* a_fpInp, FILE* a_fpOut, const TypeConstCharPtr* a_vcpcSymbols, size_t a_unCount, const char* a_cpcStrToAdd);

int main(int argc, char* argv[])
{
	FILE *fpIn(NULL), *fpOut(NULL);
	int i2,nReturn(0), nNumber(0);
	TypeConstCharPtr* vcpcSymbolsNames;

	if(argc<5){
		fprintf(stderr,"Provide input and output file names, key to be added and the names of functions or variables!\n");
		goto returnPoint;
	}

	fpIn = fopen(argv[1], "rb");
	if(!fpIn){
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

	vcpcSymbolsNames = (TypeConstCharPtr*)_alloca(sizeof(TypeConstCharPtr)*(argc - 4));

	for (i2 = 4; i2<argc; ++i2) {
		vcpcSymbolsNames[i2 - 4] = argv[i2];
	}

	nNumber = AddStringToTheLine(fpIn, fpOut, vcpcSymbolsNames, i2 - 4, argv[3]);

	nReturn = 0;
	printf("File:%s, NumberOfExports=%d\n", FILE_FROM_PATH(argv[1]), nNumber);
returnPoint:
	if(fpOut){
		fclose(fpOut);
	}
	if (fpIn) {
		fclose(fpIn);
	}
#ifdef _DEBUG
	printf("Press any key to exit!"); fflush(stdout);
	_getch();
	printf("\n");
#endif
	return nReturn;
}


static int AddStringToTheLine(FILE* a_fpInp, FILE* a_fpOut, const TypeConstCharPtr* a_vcpcSymbols, size_t a_unCount, const char* a_cpcStrToAdd)
{
	char *cpStrStart,*pcTemp,*pcSingleLineComment,*pcMLcommentStart,*pcCommentStart;
	const size_t unExpSymLen = strlen(a_cpcStrToAdd);
	size_t i,unOffset, unSymbolLen;
	int nNumber(0), nPackCount(0);
#ifdef _DEBUG
	int nLine(0);
#endif
	bool bInMLcomment2(false), bInPublic(false), bPackCountChanged;
	char data[1024];
	char cComment2;

	while (fgets(data, 1023, a_fpInp) != NULL) {
		//size_t strspn(const char * str1, const char * str2);
#ifdef _DEBUG
		++nLine;
#endif
		pcMLcommentStart = NULL;
		pcSingleLineComment = NULL;
		pcCommentStart = NULL;
		bPackCountChanged = false;

		if (bInMLcomment2) {
			pcTemp = strstr(data, MLcommentEnd);
			if (!pcTemp) {
				fputs(data, a_fpOut);
				continue;
			}
			bInMLcomment2 = false;
			cpStrStart = pcTemp + s_cunMLcommentEndLen;
		}

		unOffset = strspn(data, " \t");
		cpStrStart = data + unOffset;

		pcSingleLineComment = strstr(cpStrStart, "//");
		pcMLcommentStart = strstr(cpStrStart, MLcommentStart);
		if (pcSingleLineComment||pcMLcommentStart){
			if(pcSingleLineComment&&pcMLcommentStart){
				if(((size_t)pcSingleLineComment)<((size_t)pcMLcommentStart)){
					pcCommentStart = pcSingleLineComment;
				}
				else{
					pcCommentStart = pcMLcommentStart;
					bInMLcomment2 = true;
				}
			}
			else {
				if (!pcSingleLineComment) {
					pcCommentStart = pcMLcommentStart;
					bInMLcomment2 = true;
				}
				if (!pcMLcommentStart) {
					pcCommentStart = pcSingleLineComment;
				}
			}
			if(bInMLcomment2){
				if(strstr(pcCommentStart+2,MLcommentEnd)){
					bInMLcomment2 = false;
				}
				cComment2 = '*';
			}
			else{
				cComment2 = '/';
			}
			pcCommentStart[0]='\n';
			pcCommentStart[1]=0;
		}

		pcTemp = strchr(cpStrStart, '{');
		if (pcTemp) {
			++nPackCount;
			bPackCountChanged = true;
		}

		pcTemp = strchr(cpStrStart, '}');
		if (pcTemp) {
			--nPackCount;
			bPackCountChanged = true;
		}

		if(bPackCountChanged){
			
			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, a_fpOut);
			continue;
		}

		if (cpStrStart[0] == '#') {

			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, a_fpOut);
			continue;
		}

		/*public, protected and private story*/

		if((NUMBER_OF_BRACES_TO_EXPORT>=0)&&(nPackCount!=NUMBER_OF_BRACES_TO_EXPORT)){
			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, a_fpOut);
			continue;
		}

		fwrite(data, 1, unOffset, a_fpOut);
		pcTemp = NULL;
		for(i=0;i<a_unCount;++i){
			if(!strstr(cpStrStart, "using")){pcTemp=strstr(cpStrStart,a_vcpcSymbols[i]);}
			if(pcTemp){
				unSymbolLen = strlen(a_vcpcSymbols[i]);
				if( (pcTemp[unSymbolLen]==';')||(pcTemp[unSymbolLen]=='(')||
					(pcTemp[unSymbolLen]=='[')|| (pcTemp[unSymbolLen] == '{')||
					(!pcTemp[unSymbolLen])||isspace(pcTemp[unSymbolLen]))
				{break;}
			}
			pcTemp = NULL;
		}

		if(pcTemp) {
			fwrite(a_cpcStrToAdd, 1, unExpSymLen, a_fpOut);
			fwrite(" ", 1, 1, a_fpOut);
			++nNumber;
		}

		if (pcCommentStart) {
			pcCommentStart[0] = '/';
			pcCommentStart[1] = cComment2;
		}
		fputs(cpStrStart, a_fpOut);

	}

	return nNumber;
}

