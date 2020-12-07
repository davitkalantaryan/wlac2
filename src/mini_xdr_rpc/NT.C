/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO 
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE 
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/

#include <stdio.h>
#include <winsock.h>

void sun_rpc_report(const char*);
int sun_rpc_init(void);
int sun_rpc_exit(void);
void svc_exit(void);
WSADATA WSAData;

static int init = 0;
int g = 0;
int u = 0;
int sun_rpc_init(void)
{
	if (! init)
		if (WSAStartup(0x0101, &WSAData)) {
			sun_rpc_report("WSAStartup failed\n");
			WSACleanup();
			return -1;
		}
	init = 1;
	return 0;
}

int sun_rpc_exit(void)
{
    svc_exit();
	return WSACleanup();
}


#if(_MSC_VER >= 1400)
//#define		_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif

void
sun_rpc_report(const char* lpszMsg)
{
    CHAR    chMsg[256];
    HANDLE  hEventSource;
    LPCSTR  lpszStrings[2];

    // Use event logging to log the error.
    //
    hEventSource = RegisterEventSourceA(NULL,
                            "DOOCS.log");
	/// Modified by DK
    //sprintf(chMsg, "sunrpc report: %d", GetLastError());
	_snprintf(chMsg, 255,"sunrpc report: %d", GetLastError());
    lpszStrings[0] = chMsg;
    lpszStrings[1] = lpszMsg;

    if (hEventSource != NULL) {
        ReportEventA(hEventSource, // handle of event source
            EVENTLOG_WARNING_TYPE, // event type
            0,                    // event category
            0,                    // event ID
            NULL,                 // current user's SID
            2,                    // strings in lpszStrings
            0,                    // no bytes of raw data
            lpszStrings,          // array of error strings
            NULL);                // no raw data

        (VOID) DeregisterEventSource(hEventSource);
    }
}

