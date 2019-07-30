
// main_make_symbols_exportable.cpp
// 2017 Dec 09

#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <direct.h>

#define NUMBER_OF_BRACES_TO_EXPORT	1  // <0 exports in any case

#if defined(_MSC_VER) && (_MSC_VER>1400)
#pragma warning(disable:4996)
#endif

#define FILE_FROM_PATH(_path_)	(  strrchr((_path_),'\\')?(strrchr((_path_),'\\')+1) : \
												(strrchr((_path_),'/')?(strrchr((_path_),'/')+1):(_path_))   )

//#define EXPORT_SYMBOL		"__declspec(dllexport) "
#define MLcommentStart		"/*"
#define MLcommentEnd		"*/"
#define PUBLIC_KEYWORD		"public:"
#define PROTECTED_KEYWORD	"protected:"
#define PRIVATE_KEYWORD		"private:"

static const size_t s_cunMLcommentEndLen = strlen(MLcommentEnd);
static const size_t	s_cunPubKeyLen = strlen(PUBLIC_KEYWORD);
static const size_t	s_cunProtKeyLen = strlen(PROTECTED_KEYWORD);
static const size_t	s_cunPrivKeyLen = strlen(PRIVATE_KEYWORD);

int main(int argc, char* argv[])
{
	char *cpStrStart,*pcTemp,*pcSingleLineComment,*pcMLcommentStart,*pcCommentStart;
	FILE *fpInp(NULL), *fpOut(NULL);
	size_t unOffset;
	int nReturn(0), nNumber(0), nPackCount(0);
#ifdef _DEBUG
	int nLine(0);
#endif 
	bool bInMLcomment2(false), bInPublic(false), bPackCountChanged;
	char data[1024];
	char cComment2;

	if(argc<4){
		fprintf(stderr,"Provide input and output file names and key to be added!\n");
		goto returnPoint;
	}

	size_t unExpSymLen = strlen(argv[3]);

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


	while (fgets(data, 1023, fpInp) != NULL) {
		//size_t strspn(const char * str1, const char * str2);

		pcMLcommentStart = NULL;
		pcSingleLineComment = NULL;
		pcCommentStart = NULL;
		bPackCountChanged = false;

#ifdef _DEBUG
		++nLine;
		if(strstr(data,"merge")){
			printf("Line of merge :  %d!\n", nLine);
		}
		else{
			//printf("%d. %s",nLine,data);
		}
#endif

		if (bInMLcomment2) {
			pcTemp = strstr(data, MLcommentEnd);
			if (!pcTemp) {
				fputs(data, fpOut);
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
			fputs(data, fpOut);
			continue;
		}


		if (cpStrStart[0] == '#') {

			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, fpOut);
			continue;
		}


		if (!bInPublic) {
			if (strncmp(cpStrStart, PUBLIC_KEYWORD, s_cunPubKeyLen) == 0) {
				bInPublic = true;
			}
			
			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, fpOut);
			continue;
		}
		

		if (strncmp(cpStrStart,PROTECTED_KEYWORD,s_cunProtKeyLen)==0) {
			bInPublic = false;

			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, fpOut);
			continue;
		}

		if (strncmp(cpStrStart, PRIVATE_KEYWORD, s_cunPrivKeyLen) == 0) {
			bInPublic = false;

			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, fpOut);
			continue;
		}

		if((NUMBER_OF_BRACES_TO_EXPORT>=0)&&(nPackCount!=NUMBER_OF_BRACES_TO_EXPORT)){
			if (pcCommentStart) {
				pcCommentStart[0] = '/';
				pcCommentStart[1] = cComment2;
			}
			fputs(data, fpOut);
			continue;
		}

		fwrite(data, 1, unOffset, fpOut);
		pcTemp = strchr(cpStrStart, '(');

		if(pcTemp){
			fwrite(argv[3], 1, unExpSymLen, fpOut);
			fwrite(" ", 1, 1, fpOut);
			++nNumber;
		}

		if (pcCommentStart) {
			pcCommentStart[0] = '/';
			pcCommentStart[1] = cComment2;
		}
		fputs(cpStrStart, fpOut);

	}

	printf("File:%s, NumberOfExports=%d\n", FILE_FROM_PATH(argv[1]), nNumber);
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
