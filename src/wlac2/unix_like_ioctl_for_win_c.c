/*
 *	File: unix_emulator_sources_for_win_c.c
 *
 *	Created on: Dec 10, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * //
 *
 *
 */
#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <sys/time.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <sys/ioctl.h>
#include <net/if.h>

__BEGIN_C_DECLS

int IsDescriptorASocket(int d);
int ioctl_private(int fd, int request, va_list* arg_list);

GEM_API int ioctl(int a_d, int a_request, ...)
{
	// Declarations
	int nReturn;
	va_list args;

	va_start(args, a_request);
	nReturn = ioctl_private(a_d, a_request, &args);
	va_end(args);
	return nReturn;
}


GEM_API int fcntl(int a_d, int a_request, ... /* arg */)
{
	// Declarations
	int nReturn;
	va_list args;

	va_start(args, a_request);
	nReturn = ioctl_private(a_d, a_request, &args);
	va_end(args);
	return nReturn;
}


int ioctl_private(int a_d, int a_request, va_list* a_arg_list_p)
{
	// Declarations
	void* pLstArg;  // Last Argument
	INTERFACE_INFO*	pIfaceInfo;
	int nIsSocket = IsDescriptorASocket(a_d);
	int nReturn = 0;
	DWORD dwReturned;
	//DWORD dwInBufSize;
	DWORD dwOutBufSize, dwOutBufSizeReturned;
	struct ifconf* pIfConf;
	int nNumberOfInterfaces, nNumberOfInterfaces0, i;
	struct ifreq *ifr;
	unsigned long ulnNonBlocking;
	int* pnNonBlocking;

	// First is to check descriptor
	if (a_d < 0)return a_d;


	if (nIsSocket == 1)
	{
		switch (a_request)
		{
		case SIOCGIFCONF:
			pIfConf = va_arg(*a_arg_list_p, struct ifconf*);

			dwOutBufSize = 64 * sizeof(INTERFACE_INFO);
			pLstArg = pIfaceInfo = (INTERFACE_INFO*)_alloca(dwOutBufSize);
			nReturn = (int)WSAIoctl((SOCKET)a_d, SIO_GET_INTERFACE_LIST, NULL, 0,
				pLstArg, dwOutBufSize, &dwOutBufSizeReturned, 0, NULL);
			if (nReturn == SOCKET_ERROR)
			{
				nReturn = WSAGetLastError();
				nReturn = nReturn > 0 ? -nReturn : nReturn;
				//va_end(args);
				break;
			}
			nNumberOfInterfaces0 = pIfConf->ifc_len / sizeof(struct ifreq);
			nNumberOfInterfaces = dwOutBufSizeReturned / sizeof(INTERFACE_INFO);
			if (nNumberOfInterfaces > nNumberOfInterfaces0){ nNumberOfInterfaces = nNumberOfInterfaces0; }
			pIfConf->ifc_len = nNumberOfInterfaces*sizeof(struct ifreq);

			ifr = pIfConf->ifc_req;
			for (i = 0; i < nNumberOfInterfaces; ++i, ++ifr, ++pIfaceInfo)
			{
				memset(ifr, 0, sizeof(struct ifreq));

				ifr->ifr_flags = (short int)pIfaceInfo->iiFlags;
				//ifr->ifr_ifru.ifru_addr = pIfaceInfo->iiAddress.Address;
				//ifr->ifr_ifru.ifru_dstaddr = pIfaceInfo->iiAddress.Address;

				// Get the name of interface
				//ifr->ifr_ifrn.ifrn_name[IFNAMSIZ - 1] = (unsigned char)pIfaceInfo->iiFlags;

				if (pIfaceInfo->iiFlags&IFF_UP)
				{
					ifr->ifr_ifru.ifru_addr = pIfaceInfo->iiAddress.Address;
				}
				else if (pIfaceInfo->iiFlags&IFF_BROADCAST){ ifr->ifr_ifru.ifru_broadaddr = pIfaceInfo->iiBroadcastAddress.Address; }
				else if (pIfaceInfo->iiFlags&IFF_LOOPBACK){ ifr->ifr_ifru.ifru_dstaddr = pIfaceInfo->iiAddress.Address; }
				else if (pIfaceInfo->iiFlags&IFF_POINTTOPOINT){ ifr->ifr_ifru.ifru_hwaddr = pIfaceInfo->iiNetmask.Address; }
				else if (pIfaceInfo->iiFlags&IFF_MULTICAST){ ifr->ifr_ifru.ifru_netmask = pIfaceInfo->iiNetmask.Address; }
			}
			break;
		case SIOCGIFFLAGS:
			ifr = va_arg(*a_arg_list_p, struct ifreq*);
			//pIfaceInfo = (INTERFACE_INFO*)((void*)ifr->winInfo2);
			//ifr->ifr_flags = (short int)pIfaceInfo->iiFlags;
			break;
		case FIONBIO:  // pnNonBlocking
			pnNonBlocking = va_arg(*a_arg_list_p, int*);
			ulnNonBlocking = *pnNonBlocking;
			nReturn = ioctlsocket((SOCKET)a_d, FIONBIO, &ulnNonBlocking);
			break;
		default:
			pLstArg = va_arg(*a_arg_list_p, void*);
			nReturn = ioctlsocket((SOCKET)a_d, (long)a_request, pLstArg);
			break;
		}
	}
	else if (nIsSocket == 0)
	{
		switch (a_request)
		{
		case 0:
			break;
		default:
			pLstArg = va_arg(*a_arg_list_p, void*);
			nReturn = (int)DeviceIoControl((HANDLE)_get_osfhandle(a_d), a_request, pLstArg, 0, pLstArg, 0, &dwReturned, NULL);
			break;
		}
	}

	return nReturn;
}

__END_C_DECLS
