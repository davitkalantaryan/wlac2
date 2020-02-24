
#include <pthread.h>
#include <stdio.h>


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

static void* ThreadFunction(void*);

int main()
{
	//DWORD dwVal = 1;
	pthread_t aThread;

	union {
		unsigned short int val;
		unsigned char chVal[2];
	}testUn;

	testUn.val = 1;

	printf("chVal[0]=%d, chVal[1]=%d\n", testUn.chVal[0], testUn.chVal[1]);

	pthread_create(&aThread, nullptr, &ThreadFunction, nullptr);

	pthread_join(aThread, nullptr);

	return 0;
}


static void* ThreadFunction(void*) 
{
	printf("Thread function\n");
	return nullptr;
}
