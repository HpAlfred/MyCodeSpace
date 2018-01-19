#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <Shlobj.h>
#include "LibXNet.h"
#pragma comment(lib, "ws2_32.lib" )
#pragma comment(lib, "Shell32.lib" )

int LastErrCode = 0;//10000以上为Windows文件接口异常
XNetCommCB XNetCommCBArray[MaxLinkNum];
CRITICAL_SECTION 	CS_XNetComm;

void LibXNetInit();

/* ******************************************************************************* *
 * Name			: XNetServerInit()
 * Descripe		: 启动服务
 * Input		:
 * Output		:
 * Note			: 
 * ******************************************************************************* */
int XNetServerInit()
{
	char path[100]; 
	char path2[30]="\\XINJE\\XNET\\Config.ini";
	char ret[100];
	char serviceName[20]="\\X_Net_Windows.exe";
	int i;

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if(!SHGetSpecialFolderPathA(NULL, path, CSIDL_COMMON_APPDATA , NULL))
	{
		LastErrCode = ServiceInstallationError;
		return -1;
	}
	for (i = 0; i < 100 ; i++)
	{
		if(path[i]==0x00)
		{
			memcpy(&(path[i]),path2,30);
			break;
		}
	}

	if(GetPrivateProfileStringA("common","serverPath",NULL,ret,256,path)<=0)
	{
		LastErrCode = ServiceInstallationError;
		return -1;
	}

	

	for (i = 0; i < 100 ; i++)
	{
		if(ret[i]==0x00)
		{
			memcpy(&(ret[i]),serviceName,20);
			break;
		}
	}

	if (!CreateProcessA(NULL,ret,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		LastErrCode = FailedToStartService;
		return -1;
	}
	
	//启动服务需要给服务一定的初始化时间
	Sleep(1000);

	//初始化控制块
	LibXNetInit();
	return 0;
}

void LibXNetInit()
{
	int i;
	InitializeCriticalSection( &CS_XNetComm );
	for (i = 0; i < MaxLinkNum ; i++)
	{//本身就是慢过程，不考虑把时间复杂度从n压榨到logn了
		XNetCommCBArray[i].isfree = TRUE;
	}
}

XNetComm XNetCommunication()
{
	SOCKET sock;
	struct sockaddr_in serviceAddr;
	WORD sockVersion;
	WSADATA wsaData;
	int i;
	

	sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &wsaData)!=0)
	{
		LastErrCode = GetLastError();
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == INVALID_SOCKET)
	{
		LastErrCode = GetLastError();
		return -1;
	}

	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_port = htons(2323);
	serviceAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 

	if (connect(sock, (struct sockaddr *)&serviceAddr, sizeof(serviceAddr)) == SOCKET_ERROR)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		return -1;
	}

	for (i = 0; i < MaxLinkNum ; i++)
	{//本身就是慢过程，不考虑把时间复杂度从n压榨到logn了
		 if(XNetCommCBArray[i].isfree)
		 {
			 XNetCommCBArray[i].comm = sock;
			 XNetCommCBArray[i].isfree = FALSE;
			 return i;
		 }
	}
	LastErrCode = TooMuchLink;
	return -1;
}


int ComPortTypeFindDevice( XNetComm _comm, BYTE _com, BYTE _type, BYTE _series, BYTE _model )
{

}

int AutoTypeFindDevice( XNetComm _comm, BYTE _type, BYTE _series, BYTE _model )
{

}

int ComPortIDFindDevice( XNetComm _comm, BYTE _com, char* _id )
{

}

int AutoIDFindDevice( XNetComm _comm, char* _id )
{

}




