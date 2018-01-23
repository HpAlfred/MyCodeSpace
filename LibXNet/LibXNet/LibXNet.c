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
void RegsToBuf( short * _regs, char * _buf, int _num );
void BufToRegs(char * _buf,short * _regs, int _num);
void CoilsToBuf(short * _coils, char * _buf, int _num);
void BufToCoils(char * _buf,short * _coils,BYTE _startBit ,int _num);
BOOL ConfigComPort(XNetComm _comm,BYTE _com);

/* ******************************************************************************* *
 * Name			: XNetServerInit()
 * Descripe		: 启动服务
 * Input		:
 * Output		:
 * Note			: 
 * ******************************************************************************* */
int XNetAPI XNetServerInit()
{
	char path[100]; 
	char path2[30]="\\XINJE\\XNET\\Config.ini";
	char ret[100];
	char serviceName[20]="\\X_Net_Windows.exe";
	int i;

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if(!SHGetSpecialFolderPathA(NULL, path, CSIDL_COMMON_APPDATA , FALSE))
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

XNetComm XNetAPI XNetCommunication()
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
			 XNetCommCBArray[i].sock = sock;
			 XNetCommCBArray[i].isfree = FALSE;
			 return i;
		 }
	}
	LastErrCode = TooMuchLink;
	return -1;
}

BOOL XNetAPI SetupRemoteLink(XNetComm _comm,char* _id,char * _password)
{
	SOCKET sock;
	char sendBuf[61];
	char recvBuf[3];
	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;

	memset(sendBuf,0x00,61);
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x3B;
	sendBuf[2] = 0x05;
	memcpy(&(sendBuf[3]),_id,10);
	memcpy(&(sendBuf[53]),_password,8);

	if(send(sock,sendBuf,61,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBuf, 3, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	switch (recvBuf[2])
	{
		case 0x00:
			{
				return TRUE;
			}
			break;
		case 0x01:
			{
				LastErrCode = SetupRemoteLinkFailed;
				return FALSE;
			}
			break;
		case 0x10:
			{
				LastErrCode = BuilderLogonError ;
				return FALSE;
			}
			break;
		case 0x11:
			{
				LastErrCode = ReceiverNoLogon;
				return FALSE;
			}
			break;
		case 0x12:
			{
				LastErrCode = SafeModeMismatch;
				return FALSE;
			}
			break;
		case 0x13:
			{
				LastErrCode = PasswordError;
				return FALSE;
			}
			break;
		case 0x20:
			{
				LastErrCode = BuilderNoRegister;
				return FALSE;
			}
			break;
		case 0x21:
			{
				LastErrCode = BuilderCommandError ;
				return FALSE;
			}
			break;
		case 0x22:
			{
				LastErrCode = BuilderNotAllowed;
				return FALSE;
			}
			break;
		case 0x23:
			{
				LastErrCode = ReceiverNoRegister;
				return FALSE;
			}
			break;
		case 0x24:
			{
				LastErrCode = ReceiverCommandError;
				return FALSE;
			}
			break;
		case 0xFF:
			{
				LastErrCode = LogonFailed;
				return FALSE;
			}
			break;
		default:
			{
				LastErrCode = DefaultError;
				return FALSE;
			}
			break;
	}
}

int XNetAPI ComPortTypeFindDevice( XNetComm _comm, BYTE _com, BYTE _type, BYTE _series, BYTE _model )
{
	SOCKET sock;
	int len;
	char sendBuf[7];
	char recvBufLen[2];
	char recvBuf[20];

	if(!ConfigComPort(_comm,_com))
	{
		LastErrCode = ComPortConfigFailure_1;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x05;
	sendBuf[2] = 0x00;
	sendBuf[3] = _com;
	sendBuf[4] = _type;
	sendBuf[5] = _series;
	sendBuf[6] = _model;


	if(send(sock,sendBuf,7,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}


	if(recvBuf [0]==0)
	{
		XNetCommCBArray[_comm].net = (((int)(recvBuf[4]))<<8)+((int)(recvBuf[5]));
		XNetCommCBArray[_comm].station = (((int)(recvBuf[6]))<<8)+((int)(recvBuf[7]));
		return 0;
	}
	else
	{
		switch(recvBuf [0])
		{
		case 1:
			{
				LastErrCode = ComPortConfigFailure_1;
			}
			break;
		case 2:
			{
				LastErrCode = WrongComPort_2 ;
			}
			break;
		case 3:
			{
				LastErrCode = CannotFindComPort_3;
			}
			break;
		case 4:
			{
				LastErrCode = FindTimeOut_4;
			}
			break;
		case 5:
			{
				LastErrCode = BindComPortFailure_5;
			}
			break;
		default:
			LastErrCode = DefaultError;
			break;
		}
		return -1;
	}
	

}

int XNetAPI AutoTypeFindDevice( XNetComm _comm, BYTE _type, BYTE _series, BYTE _model )
{
	SOCKET sock;
	int len;
	char sendBuf[7];
	char recvBufLen[2];
	char recvBuf[20];

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x05;
	sendBuf[2] = 0x00;
	sendBuf[3] = 0x00;
	sendBuf[4] = _type;
	sendBuf[5] = _series;
	sendBuf[6] = _model;


	if(send(sock,sendBuf,7,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}


	if(recvBuf [0]==0)
	{
		XNetCommCBArray[_comm].net = (((int)(recvBuf[4]))<<8)+((int)(recvBuf[5]));
		XNetCommCBArray[_comm].station = (((int)(recvBuf[6]))<<8)+((int)(recvBuf[7]));
		return 0;
	}
	else
	{
		switch(recvBuf [0])
		{
		case 1:
			{
				LastErrCode = ComPortConfigFailure_1;
			}
			break;
		case 2:
			{
				LastErrCode = WrongComPort_2 ;
			}
			break;
		case 3:
			{
				LastErrCode = CannotFindComPort_3;
			}
			break;
		case 4:
			{
				LastErrCode = FindTimeOut_4;
			}
			break;
		case 5:
			{
				LastErrCode = BindComPortFailure_5;
			}
			break;
		default:
			LastErrCode = DefaultError;
			break;
		}
		return -1;
	}
}

int XNetAPI ComPortIDFindDevice( XNetComm _comm, BYTE _com, char* _id )
{
	SOCKET sock;
	int len;
	char sendBuf[14];
	char recvBufLen[2];
	char recvBuf[20];

	if(!ConfigComPort(_comm,_com))
	{
		LastErrCode = ComPortConfigFailure_1;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x05;
	sendBuf[2] = 0x00;
	sendBuf[3] = _com;
	memcpy(&(sendBuf[4]),_id,10);


	if(send(sock,sendBuf,14,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}


	if(recvBuf [0]==0)
	{
		XNetCommCBArray[_comm].net = (((int)(recvBuf[4]))<<8)+((int)(recvBuf[5]));
		XNetCommCBArray[_comm].station = (((int)(recvBuf[6]))<<8)+((int)(recvBuf[7]));
		return 0;
	}
	else
	{
		switch(recvBuf [0])
		{
		case 1:
			{
				LastErrCode = ComPortConfigFailure_1;
			}
			break;
		case 2:
			{
				LastErrCode = WrongComPort_2 ;
			}
			break;
		case 3:
			{
				LastErrCode = CannotFindComPort_3;
			}
			break;
		case 4:
			{
				LastErrCode = FindTimeOut_4;
			}
			break;
		case 5:
			{
				LastErrCode = BindComPortFailure_5;
			}
			break;
		default:
			LastErrCode = DefaultError;
			break;
		}
		return -1;
	}

}

int XNetAPI AutoIDFindDevice( XNetComm _comm, char* _id )
{
	SOCKET sock;
	int len;
	char sendBuf[14];
	char recvBufLen[2];
	char recvBuf[20];

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x05;
	sendBuf[2] = 0x00;
	sendBuf[3] = 0x00;
	memcpy(&(sendBuf[4]),_id,10);


	if(send(sock,sendBuf,14,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}


	if(recvBuf [0]==0)
	{
		XNetCommCBArray[_comm].net = (((int)(recvBuf[4]))<<8)+((int)(recvBuf[5]));
		XNetCommCBArray[_comm].station = (((int)(recvBuf[6]))<<8)+((int)(recvBuf[7]));
		return 0;
	}
	else
	{
		switch(recvBuf [0])
		{
		case 1:
			{
				LastErrCode = ComPortConfigFailure_1;
			}
			break;
		case 2:
			{
				LastErrCode = WrongComPort_2 ;
			}
			break;
		case 3:
			{
				LastErrCode = CannotFindComPort_3;
			}
			break;
		case 4:
			{
				LastErrCode = FindTimeOut_4;
			}
			break;
		case 5:
			{
				LastErrCode = BindComPortFailure_5;
			}
			break;
		default:
			LastErrCode = DefaultError;
			break;
		}
		return -1;
	}
}

int XNetAPI WriteRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _writeRegs)
{
	char sendBuf[1500];
	char recvBuf[3];
	int totalLen,len;
	SOCKET sock;

	if(_num>500)
	{
		LastErrCode = RegNumShouldUnder500;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;

	totalLen = 14 + _num * 2;
	len = totalLen-2;
	sendBuf[0] = (BYTE)(len >> 8);//长度
	sendBuf[1] = (BYTE)(len);
	sendBuf[2] = 0x02;//IDBWO对象
	sendBuf[3] = (BYTE)(XNetCommCBArray[_comm].net>>8);
	sendBuf[4] = (BYTE)(XNetCommCBArray[_comm].net);
	sendBuf[5] = (BYTE)(XNetCommCBArray[_comm].station>>8);
	sendBuf[6] = (BYTE)(XNetCommCBArray[_comm].station);
	sendBuf[7] = 0x81;//可靠写
	sendBuf[8] = (BYTE)_regType;
	sendBuf[9] = (BYTE)(_offSet >> 16);
	sendBuf[10] = (BYTE)(_offSet >> 8);
	sendBuf[11] = (BYTE)(_offSet);
	sendBuf[12] = (BYTE)(_num >> 8);//个数
	sendBuf[13] = (BYTE)(_num);//个数
	RegsToBuf(_writeRegs, &(sendBuf[14]), _num);
	if(send(sock,sendBuf,totalLen,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBuf, 3, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recvBuf[2] != 0)
	{
		LastErrCode = CommuError;
	}
	return recvBuf[2];
}

int XNetAPI ReadRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _readRegs)
{
	char sendBuf[24];
	char recvBufLen[2];
	char recvBuf[9000];
	SOCKET sock;
	int len = 12;
	if(_num>500)
	{
		LastErrCode = RegNumShouldUnder500;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;

	sendBuf[0] = (BYTE)(len >> 8);//长度
	sendBuf[1] = (BYTE)(len);
	sendBuf[2] = 0x02;//BWO操作
	sendBuf[3] = (BYTE)(XNetCommCBArray[_comm].net>>8);
	sendBuf[4] = (BYTE)(XNetCommCBArray[_comm].net);
	sendBuf[5] = (BYTE)(XNetCommCBArray[_comm].station>>8);
	sendBuf[6] = (BYTE)(XNetCommCBArray[_comm].station);
	sendBuf[7] = 0x01;//可靠读
	sendBuf[8] = (BYTE)_regType;
	sendBuf[9] = (BYTE)(_offSet >> 16);
	sendBuf[10] = (BYTE)(_offSet >> 8);
	sendBuf[11] = (BYTE)(_offSet);
	sendBuf[12] = (BYTE)(_num >> 8);
	sendBuf[13] = (BYTE)(_num);

	if(send(sock,sendBuf,len+2,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	if (recvBuf[0] == 0x00)
	{
		BufToRegs(&(recvBuf[1]),_readRegs,_num);
	}
	else
	{
		LastErrCode = CommuError;
	}

	return recvBuf[0];
}

int XNetAPI WriteCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _writeCoils)
{
	char sendBuf[9000];
	char recvBuf[3];
	int totalLen,len;
	SOCKET sock;
	if(_num>65535)
	{
		LastErrCode = CoilNumShouldUnder65535;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;

	if (_num % 8 == 0)
	{
		totalLen = 15 + _num / 8;
	}
	else
	{
		totalLen = 15 + ( _num / 8 ) + 1;
	}
	len = totalLen - 2;

	sendBuf[0] = (BYTE)(len >> 8);//长度
	sendBuf[1] = (BYTE)(len);
	sendBuf[2] = 0x02;//IDBWO对象
	sendBuf[3] = (BYTE)(XNetCommCBArray[_comm].net>>8);
	sendBuf[4] = (BYTE)(XNetCommCBArray[_comm].net);
	sendBuf[5] = (BYTE)(XNetCommCBArray[_comm].station>>8);
	sendBuf[6] = (BYTE)(XNetCommCBArray[_comm].station);
	sendBuf[7] = 0x81;//可靠写
	sendBuf[8] = (BYTE)_coilType;
	sendBuf[9] = (BYTE)(_offSet >> 16);
	sendBuf[10] = (BYTE)(_offSet >> 8);
	sendBuf[11] = (BYTE)(_offSet);
	sendBuf[12] = (BYTE)((_num) >> 8);//个数
	sendBuf[13] = (BYTE)(_num);//个数
	sendBuf[14] = 0x00;//位偏移

	CoilsToBuf(_writeCoils, &(sendBuf[15]), _num);

	if(send(sock,sendBuf,totalLen,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBuf, 3, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recvBuf[2] != 0)
	{
		LastErrCode = CommuError;
	}
	return recvBuf[2];

}

int XNetAPI ReadCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _readCoil)
{
	char sendBuf[24];
	char recvBufLen[2];
	char recvBuf[9000];
	SOCKET sock;
	int len = 12;
	if(_num>65535)
	{
		LastErrCode = CoilNumShouldUnder65535;
		return -1;
	}

	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = (BYTE)(len >> 8);//长度
	sendBuf[1] = (BYTE)(len);
	sendBuf[2] = 0x02;//BWO操作
	sendBuf[3] = (BYTE)(XNetCommCBArray[_comm].net>>8);
	sendBuf[4] = (BYTE)(XNetCommCBArray[_comm].net);
	sendBuf[5] = (BYTE)(XNetCommCBArray[_comm].station>>8);
	sendBuf[6] = (BYTE)(XNetCommCBArray[_comm].station);
	sendBuf[7] = 0x01;//可靠读
	sendBuf[8] = (BYTE)_coilType;
	sendBuf[9] = (BYTE)(_offSet >> 16);
	sendBuf[10] = (BYTE)(_offSet >> 8);
	sendBuf[11] = (BYTE)(_offSet);
	sendBuf[12] = (BYTE)(_num >> 8);
	sendBuf[13] = (BYTE)(_num);

	if(send(sock,sendBuf,len+2,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recv(sock, recvBufLen, 2, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}
	len = (((int)(recvBufLen[0]))<<8)+((int)(recvBufLen[1]));

	if(recv(sock, recvBuf, len, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if (recvBuf[0] == 0x00)
	{
		BufToCoils(&recvBuf[2],_readCoil,recvBuf[1], _num);
	}
	else
	{
		LastErrCode = CommuError;
	}

	return recvBuf[0];

}


void RegsToBuf( short * _regs, char * _buf, int _num )
{
	int i;
	for (i = 0; i < _num ; i++)
	{
		_buf[i*2] = (BYTE)(_regs[i]);
		_buf[i*2+1] = (BYTE)(_regs[i] >> 8);
	}
}

void BufToRegs(char * _buf,short * _regs, int _num)
{
	int i;
	for (i = 0; i < _num; i++)
	{
		_regs[i] = (short)(((int)(_buf[ i * 2 ])) + (((int)(_buf[i * 2 + 1])) << 8));
	}
}


void CoilsToBuf(short * _coils, char * _buf, int _num)
{
	int i;
	memset(_buf,0x00,_num);//必须先清零
	for (i = 0; i < _num; i++)
	{
		BYTE value;
		value = (BYTE)(((BYTE)(_coils[i]))&0x01);
		_buf[(i / 8)] |= (BYTE)(value << (i % 8));
	}
}

void BufToCoils(char * _buf,short * _coils,BYTE _startBit, int _num)
{
	int i;
	for (i = 0; i < _num; i++)
	{
		BYTE value = 0x00;
		if(_startBit != 0)
		{
			_startBit--;
			continue;
		}
		value = (BYTE)(( _buf[(i / 8) ] >> (i % 8))&0x01);
		_coils[i] = (short)value;
	}
}


BOOL ConfigComPort(XNetComm _comm,BYTE _com)
{
	SOCKET sock;
	char sendBuf[4];
	char recvBuf[3];
	if( XNetCommCBArray[_comm].isfree)
	{
		LastErrCode = InvalidComm;
		return -1;
	}
	sock = XNetCommCBArray[_comm].sock;
	sendBuf[0] = 0x00;
	sendBuf[1] = 0x02;
	sendBuf[2] = 0x81;
	sendBuf[3] = _com;

	if(send(sock,sendBuf,4,0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}


	if(recv(sock, recvBuf, 3, 0)<=0)
	{
		LastErrCode = GetLastError();
		closesocket(sock);
		XNetCommCBArray[_comm].isfree = TRUE;
		return -1;
	}

	if(recvBuf[2]==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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