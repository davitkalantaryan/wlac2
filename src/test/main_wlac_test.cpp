
#include <first_includes/common_include_for_headers.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>

int main()
{
	int i1_data = 1;
	float f1_data = (float)2;
	float f2_data = (float)3;
	char vcBuffer[128];
	const char* tmpx = "tmpx";
	time_t curTime = time(0);

	//snprintf(vcBuffer,128,"!!!!!!! tm=%ld\n", curTime);
	snprintf(vcBuffer, sizeof(vcBuffer), "%d \t%g \t %g \t\042%s\042 \t%ld \t ",
		i1_data,
		f1_data,
		f2_data,
		tmpx, curTime);
	printf("pageSize=%d\n%s", getpagesize(),vcBuffer);

	printf("\nPress any key to exit!"); fflush(stdout);
	_getch();
	printf("\n");

	return 0;
}


#define DOOCS_CLNT_EXPORT

#ifdef _WIN32
DOOCS_CLNT_EXPORT
#else  
static
#endif  
void doocs_client_cleanup(void)
{
	// on non Windows cases no need for sync, because the routine will be called once, by ~PrivateArea
#ifdef _WIN32  
	static volatile SHORT  ssnCleanupCalled = 0;
	if(InterlockedExchange16(&ssnCleanupCalled,1)){return;/* Already cleaned */}
#endif  
	// below is current actual cleanup code  
	//delete resp_;
	//delete ensp_;

	//ensp_ = nullptr;
	//resp_ = nullptr;

	//// destroy protocol modules
	//pmod_fini();
}
