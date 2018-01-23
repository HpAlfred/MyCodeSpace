#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define XNetComm int
#define XNetAPI

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


typedef	enum
{
	X = 1 ,
	Y = 2 ,
	M = 3 ,
	S = 4 ,
	T = 5 ,
	C = 6 ,
	ET = 7 ,
	HM = 8 ,
	HS = 9 ,
	HT = 10 ,
	HC = 11 ,
	HSC = 12 ,
	SM = 13 ,
	SSM = 14 ,
	TG = 15 ,
	HTG = 16 ,
	PF = 17 ,
	SEM = 18 ,
	STG = 19,//add 2016.4.8
	HSTG = 20,
}CoilType;

typedef	enum
{
	D = 128 ,
	TD = 129 ,
	CD = 130 ,
	SD = 131 ,
	ETD = 133 ,
	ID = 134 ,
	QD = 135 ,
	HD = 136 ,
	HTD = 137 ,
	HCD = 138 ,
	HSCD = 139 ,
	HSD = 140 ,
	FD = 141 ,
	SFD = 142 ,
	SSFD = 143 ,
	SSD = 144 ,
	FS = 145,
}RegType;

#define MaxLinkNum 200