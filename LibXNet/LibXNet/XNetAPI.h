#include <winsock2.h>
//#include <windows.h>
#include <stdio.h>

#define XNetComm SOCKET 

#define XNetAPI

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

/******************************************************************************
 *			启动服务，并初始化
 ******************************************************************************/
__declspec(dllexport) int XNetAPI XNetServerInit();


/******************************************************************************
 *			连接到服务并获取通信句柄
 ******************************************************************************/
__declspec(dllexport) XNetComm XNetAPI XNetCommunication();


/******************************************************************************
 *			建立远程连接
 ******************************************************************************/
__declspec(dllexport) BOOL XNetAPI SetupRemoteLink(XNetComm _comm,char* _id,char * _password);


/******************************************************************************
 *			查找设备
 ******************************************************************************/
__declspec(dllexport) int XNetAPI ComPortTypeFindDevice( XNetComm _comm, BYTE _com, BYTE _type, BYTE _series, BYTE _model );

__declspec(dllexport) int XNetAPI AutoTypeFindDevice( XNetComm _comm, BYTE _type, BYTE _series, BYTE _model );

__declspec(dllexport) int XNetAPI ComPortIDFindDevice( XNetComm _comm, BYTE _com, char* _id );

__declspec(dllexport) int XNetAPI AutoIDFindDevice( XNetComm _comm, char* _id );


/******************************************************************************
 *			字位对象读写
 * 无论是字还是位，都统一存于short数组内
 * 如果是位对象，数组每一项中1位ON，0为OFF
 ******************************************************************************/
__declspec(dllexport) int XNetAPI WriteRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _writeRegs);

__declspec(dllexport) int XNetAPI ReadRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _readRegs);

__declspec(dllexport) int XNetAPI WriteCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _writeCoils);

__declspec(dllexport) int XNetAPI ReadCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _readCoil);