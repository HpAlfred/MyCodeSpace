#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define XNetComm int

#define ServiceInstallationError       1
#define FailedToStartService           2
#define TooMuchLink                    3
#define InvalidComm                    4

#define ComPortConfigFailure_1           5
#define WrongComPort_2                   6
#define CannotFindComPort_3              7
#define FindTimeOut_4                    8
#define BindComPortFailure_5             9

#define SetupRemoteLinkFailed           10
#define BuilderLogonError               11
#define ReceiverNoLogon					12
#define SafeModeMismatch                13
#define PasswordError                   14
#define BuilderNoRegister               15
#define BuilderCommandError             16
#define BuilderNotAllowed               17
#define ReceiverNoRegister              18
#define ReceiverCommandError            19
#define LogonFailed                     20

#define CommuError                      21
#define RegNumShouldUnder500            22
#define CoilNumShouldUnder65535         23
#define DefaultError                   100

typedef struct XNetCommCB_Struct
{
	BOOL     isfree;
	SOCKET   sock;
	WORD     net;
	WORD     station;
}XNetCommCB;

#define MaxLinkNum 200