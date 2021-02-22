
//
//
// here is test file to check functionality of getaddrinfo:
// see:	https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo


// C:\Windows\System32\drivers\etc

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#endif
#include <stdio.h>

#ifdef _MSC_VER
#pragma comment (lib,"Ws2_32.lib")
#endif

#define MAX_NAME_LEN	512

static int GetHostSinAddrAndReturnProto(struct in_addr* a_sin_addr_p, const char* a_hostName, const char* a_protoName, const char* portAsStrOrServiceName);

int main()
{
	int nReturn = 1;
	WSADATA wsaData;
	int iResult;
	IN_ADDR sin_addr;
	char my_name[MAX_NAME_LEN];

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	gethostname(my_name, MAX_NAME_LEN);
	nReturn = GetHostSinAddrAndReturnProto(&sin_addr, my_name,"TCP",nullptr);
	WSACleanup();

	return nReturn;
}


static int GetHostSinAddrAndReturnProto(struct in_addr* a_sin_addr_p,const char* a_hostName, const char* a_protoName, const char* a_portAsStrOrServiceName)
{
#ifdef _WIN32

	int nReturn = -1;
	struct protoent* prot = NULL;
	struct addrinfo* pAddrInfo = NULL;
	struct addrinfo* ptr;
	struct addrinfo hints;
	INT getAddrInfoRet;
	struct sockaddr_in* ipv4_addr_ptr;

	if(a_protoName){
		prot = getprotobyname(a_protoName);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;  // 0 means unspecified
	hints.ai_protocol = prot? prot->p_proto:0;

	getAddrInfoRet = getaddrinfo(a_hostName,a_portAsStrOrServiceName,&hints, &pAddrInfo);
	if (getAddrInfoRet) {
		goto returnPoint;
	}

	for (ptr = pAddrInfo; ptr != NULL; ptr = ptr->ai_next) {
		switch (ptr->ai_family) {
		case AF_INET:
			ipv4_addr_ptr = (struct sockaddr_in*)ptr->ai_addr;
			memcpy(a_sin_addr_p, &ipv4_addr_ptr->sin_addr,sizeof(struct in_addr));
			nReturn = hints.ai_protocol;
			goto returnPoint;
			break;
		default:
			break;
		}
	}

	nReturn = hints.ai_protocol;
returnPoint:
	if (pAddrInfo) { freeaddrinfo(pAddrInfo); }
	return nReturn;

#else   // #ifdef _WIN32

	// todo: implement case of UNIX
	(void)a_hostName;
	(void)a_sin_addr_p;
	return 0;

#endif  // #ifdef _WIN32
}
