
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment (lib,"Ws2_32.lib")

#define MAX_NAME_LEN	512

static int GetHostSinAddr(IN_ADDR* a_sin_addr_p);

int main()
{
	int nReturn = 1;
	WSADATA wsaData;
	int iResult;
	IN_ADDR sin_addr;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	nReturn = GetHostSinAddr(&sin_addr);
	WSACleanup();

	return nReturn;
}


static int GetHostSinAddr(IN_ADDR* a_sin_addr_p)
{
	int nReturn = 1;
	char my_name[MAX_NAME_LEN];
	ADDRINFOA* pAddrInfo = nullptr;
	ADDRINFOA* ptr;
	INT getAddrInfoRet;
	struct sockaddr_in* ipv4_addr_ptr;

#ifdef __INTELLISENSE__
	INT WSAAPI getaddrinfo(
		PCSTR           pNodeName,
		PCSTR           pServiceName,
		const ADDRINFOA * pHints,
		PADDRINFOA * ppResult
	);
#endif

	gethostname(my_name, MAX_NAME_LEN);
	//printf("Local host name is: %s\n", my_name);
	getAddrInfoRet = getaddrinfo(my_name, nullptr, nullptr, &pAddrInfo);
	if (getAddrInfoRet) {
		goto returnPoint;
	}

	for (ptr = pAddrInfo; ptr != NULL; ptr = ptr->ai_next) {
		switch (ptr->ai_family) {
		case AF_INET:
			ipv4_addr_ptr = (struct sockaddr_in*)ptr->ai_addr;
			//printf("In the xdr rpc get my addr w need fierld addr->sin_addr, that is here: %uln\n",
			//	ipv4_addr_ptr->sin_addr.S_un.S_addr);
			nReturn = 0;
			goto returnPoint;
			break;
		default:
			break;
		}
	}

	nReturn = 0;
returnPoint:
	if (pAddrInfo) { freeaddrinfo(pAddrInfo); }

	return nReturn;
}
