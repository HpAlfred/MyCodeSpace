#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define XNetComm SOCKET 

#define ServiceInstallationError       1
#define FailedToStartService           2
#define TooMuchLink                    3

struct XNetCommCB
{
	BOOL     isfree;
	XNetComm comm;
	WORD     net;
	WORD     station;
};
#define MaxLinkNum 200