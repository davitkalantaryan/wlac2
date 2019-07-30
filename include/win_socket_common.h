// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <win_raw_socket.h> For WINDOWS MFC
 *
 *	Created on: Dec 17, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_raw_socket_h__
#define __win_raw_socket_h__

/// Test area
//#include <WinSock2.h>
//SOCKET a_s;
//sockaddr_storage s_a;
//ADDRESS_FAMILY a;

#include <first_includes/common_definations_wul.h>

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

//#define WSA_POLL_IMPLEMENTED

#ifdef _WINDOWS_
#if defined(_WINSOCK2API_)
#if(_MSC_VER >= 1400)
#define POLFD_STRUCTURE_DEFINED
#endif
#else
#define _WINSOCK2_API_DEFS_NEEDED_
#endif
#if !defined(_WS2TCPIP_H_)
#define _WS2TCPIP_DEFINITION_NEEDED
#endif
#endif  // #ifdef _WINDOWS_

#ifndef _WINDOWS_
#ifdef _INC_WINDOWS
#undef _INC_WINDOWS
#endif
#include <WinSock2.h>
#include <Ws2tcpip.h>
#if(_MSC_VER >= 1400)
#define  POLFD_STRUCTURE_DEFINED
#endif  // #if(_MSC_VER >= 1400)
#else   // #ifndef _WINDOWS_

#if !defined(_WINSOCK2API_) & !defined(_WINSOCKAPI_)

typedef UINT_PTR        SOCKET;
typedef USHORT ADDRESS_FAMILY;
typedef unsigned int    u_int;
#ifndef FD_SETSIZE
#define FD_SETSIZE      64
#endif /* FD_SETSIZE */

struct timeval {
	long    tv_sec;         // seconds 
	long    tv_usec;        // and microseconds 
};

typedef struct fd_set {
	u_int fd_count;               /* how many are SET? */
	SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

//
// IPv4 Internet address
// This is an 'on-wire' format structure.
//
typedef struct in_addr {
	union {
		struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { USHORT s_w1,s_w2; } S_un_w;
		ULONG S_addr;
	} S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;

typedef struct sockaddr_in {

#if(_WIN32_WINNT < 0x0600)
	short   sin_family;
#else //(_WIN32_WINNT < 0x0600)
	ADDRESS_FAMILY sin_family;
#endif //(_WIN32_WINNT < 0x0600)

	USHORT sin_port;
	IN_ADDR sin_addr;
	CHAR sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN;

#endif  // #if !defined(_WINSOCK2API_) & !defined(_WINSOCKAPI_)

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// File ws2def.h ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(_WS2DEF_)
//#include <WinSock2.h>
//
// Desired design of maximum size and alignment.
// These are implementation specific.
//
#define _SS_MAXSIZE 128                 // Maximum size
#define _SS_ALIGNSIZE (sizeof(__int64)) // Desired alignment

#if(_WIN32_WINNT >= 0x0600)
#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof(USHORT))
#define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof(USHORT) + _SS_PAD1SIZE + _SS_ALIGNSIZE))
#else
#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof (short))
#define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof (short) + _SS_PAD1SIZE \
                                                    + _SS_ALIGNSIZE))
#endif //(_WIN32_WINNT >= 0x0600)
typedef USHORT ADDRESS_FAMILY;
typedef struct sockaddr_storage {
	ADDRESS_FAMILY ss_family;      // address family

	CHAR __ss_pad1[_SS_PAD1SIZE];  // 6 byte pad, this is to make
	//   implementation specific pad up to
	//   alignment field that follows explicit
	//   in the data structure
	__int64 __ss_align;            // Field to force desired structure
	CHAR __ss_pad2[_SS_PAD2SIZE];  // 112 byte pad to achieve desired size;
	//   _SS_MAXSIZE value minus size of
	//   ss_family, __ss_pad1, and
	//   __ss_align fields is 112
} SOCKADDR_STORAGE_LH, *PSOCKADDR_STORAGE_LH, FAR *LPSOCKADDR_STORAGE_LH;
typedef struct addrinfo {
	int             ai_flags;
	int             ai_family;
	int             ai_socktype;
	int             ai_protocol;
	size_t          ai_addrlen;
	char            *ai_canonname;
	struct sockaddr  *ai_addr;
	struct addrinfo  *ai_next;
} ADDRINFOA, *PADDRINFOA;
#define _WS2DEF_
#endif  // #if !defined(_WS2DEF_) 
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// end File ws2def.h ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// File in6addr.h ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(s6_addr) & !defined(_WS2TCPIP_H_)
//
// IPv6 Internet address (RFC 2553)
// This is an 'on-wire' format structure.
//
typedef struct in6_addr {
	union {
		UCHAR       Byte[16];
		USHORT      Word[8];
	} u;
} IN6_ADDR, *PIN6_ADDR, FAR *LPIN6_ADDR;
#define in_addr6 in6_addr
#define s6_addr
#endif   // #ifndef s6_addr
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// end File in6addr.h ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#endif  // #ifndef _WINDOWS_


#ifndef POLFD_STRUCTURE_DEFINED
typedef struct pollfd {

	SOCKET  fd;
	SHORT   events;
	SHORT   revents;

} WSAPOLLFD, *PWSAPOLLFD, FAR *LPWSAPOLLFD;
#ifndef POLLRDNORM
#define POLLRDNORM  0x0100
#define POLLRDBAND  0x0200
#define POLLIN      (POLLRDNORM | POLLRDBAND)
#define POLLPRI     0x0400

#define POLLWRNORM  0x0010
#define POLLOUT     (POLLWRNORM)
#define POLLWRBAND  0x0020

#define POLLERR     0x0001
#define POLLHUP     0x0002
#define POLLNVAL    0x0004
#endif  // #ifndef POLLRDNORM
#define POLFD_STRUCTURE_DEFINED
#endif  // #ifndef POLFD_STRUCTURE_DEFINED

#ifdef _WINSOCK2_API_DEFS_NEEDED_
////////////////////////////////////////////////////////////////////////////////////
//
// TCP/IP specific Ioctl codes.
//
//#define SIO_GET_INTERFACE_LIST     _IOR('t', 127, ULONG)
#define SIO_GET_INTERFACE_LIST_EX  _IOR('t', 126, ULONG)
#define SIO_SET_MULTICAST_FILTER   _IOW('t', 125, ULONG)
#define SIO_GET_MULTICAST_FILTER   _IOW('t', 124 | IOC_IN, ULONG)
#define SIOCSIPMSFILTER            SIO_SET_MULTICAST_FILTER
#define SIOCGIPMSFILTER            SIO_GET_MULTICAST_FILTER
////////////////////////////////////////////////////////////////////////////////////
#define _WINSOCK2_API_DEFINED_HERE_
#endif  /* #ifdef _WINSOCK2_API_DEFS_NEEDED_ */

#ifdef _WS2TCPIP_DEFINITION_NEEDED
////////////////////////////////////////////////////////////////////////////////////
#define IFF_UP              0x00000001 // Interface is up.
#define IFF_BROADCAST       0x00000002 // Broadcast is  supported.
#define IFF_LOOPBACK        0x00000004 // This is loopback interface.
#define IFF_POINTTOPOINT    0x00000008 // This is point-to-point interface.
#define IFF_MULTICAST       0x00000010 // Multicast is supported.
////////////////////////////////////////////////////////////////////////////////////
#define _WS2TCPIP__DEFINED_HERE_
#endif  /* #ifdef _WINSOCK2_API_DEFS_NEEDED_ */


#ifndef _WINSOCK2API_
#define _WINSOCK2API_
#endif
#ifndef _WS2TCPIP_H_
#define _WS2TCPIP_H_
#endif

#ifndef SOCKET_INPROGRESS
//#define	SOCKET_INPROGRESS(e)	(WSAGetLastError() == WSAEWOULDBLOCK)
#endif

__BEGIN_C_DECLS

#ifndef WINSOCK_API_LINKAGE
#ifdef DECLSPEC_IMPORT
#define WINSOCK_API_LINKAGE DECLSPEC_IMPORT
#else
#define WINSOCK_API_LINKAGE
#endif
#endif

#ifndef WSAAPI
#define WSAAPI                  FAR PASCAL
#endif

#if !defined(sleep) && !defined(sleep_defined) && !defined(__sleep_defined) && !defined(sleep_not_needed)
#define sleep(__x__) Sleep(1000*(__x__))
#endif

WINSOCK_API_LINKAGE
VOID
WSAAPI
freeaddrinfo(
	_In_opt_        PADDRINFOA      pAddrInfo
);

WINSOCK_API_LINKAGE
INT
WSAAPI
getaddrinfo(
	_In_opt_        PCSTR               pNodeName,
	_In_opt_        PCSTR               pServiceName,
	_In_opt_        const ADDRINFOA *   pHints,
	_Outptr_        PADDRINFOA *        ppResult
);

#ifdef _NEW_SOCKET_DEFINE_FOR_64_BIT
#ifdef _WIN64
#ifndef __win64_socket_defined
#ifdef socket
#undef socket
#endif  // #ifdef socket
#ifndef WINSOCK_API_LINKAGE
#ifdef DECLSPEC_IMPORT
#define WINSOCK_API_LINKAGE DECLSPEC_IMPORT
#else  // #ifdef DECLSPEC_IMPORT
#define WINSOCK_API_LINKAGE
#endif  // #ifdef DECLSPEC_IMPORT
#endif  // #ifndef WINSOCK_API_LINKAGE
#ifndef WSAAPI
#define WSAAPI                  FAR PASCAL
#endif  // #ifndef WSAAPI
WINSOCK_API_LINKAGE
_Must_inspect_result_
SOCKET
WSAAPI
WSASocketA(
_In_ int af,
_In_ int type,
_In_ int protocol,
_In_opt_ struct _WSAPROTOCOL_INFOA* lpProtocolInfo,
_In_ unsigned int/*GROUP*/ g,
_In_ DWORD dwFlags
);
#if 0
#define	socket(_af_,_type_,_protocol_)	((int)WSASocketA((_af_),(_type_),(_protocol_),NULL,0,0))
#else
#endif
#define	__win64_socket_defined
#endif  //#ifndef __win64_socket_defined
#endif  // #ifdef _WIN64
#endif  // #if _NEW_SOCKET_DEFINE_FOR_64_BIT

#include <errno.h>
#if 1
#ifdef h_errno
#undef h_errno
#endif
#define h_errno	(*_errno())
#else
#ifndef h_errno
#define h_errno	(*_errno())
#endif
#endif


__END_C_DECLS

#endif  /* #ifndef __win_raw_socket_h__ */
