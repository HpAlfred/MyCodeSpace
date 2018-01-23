#include <winsock2.h>
//#include <windows.h>
#include <stdio.h>

#define XNetComm SOCKET 

#define XNetAPI

/******************************************************************************
 *			启动服务，并初始化
 ******************************************************************************/
int XNetAPI XNetServerInit();


/******************************************************************************
 *			连接到服务并获取通信句柄
 ******************************************************************************/
XNetComm XNetAPI XNetCommunication();


/******************************************************************************
 *			建立远程连接
 ******************************************************************************/
BOOL XNetAPI SetupRemoteLink(XNetComm _comm,char* _id,char * _password);


/******************************************************************************
 *			查找设备
 ******************************************************************************/
int XNetAPI ComPortTypeFindDevice( XNetComm _comm, BYTE _com, BYTE _type, BYTE _series, BYTE _model );

int XNetAPI AutoTypeFindDevice( XNetComm _comm, BYTE _type, BYTE _series, BYTE _model );

int XNetAPI ComPortIDFindDevice( XNetComm _comm, BYTE _com, char* _id );

int XNetAPI AutoIDFindDevice( XNetComm _comm, char* _id );


/******************************************************************************
 *			字位对象读写
 * 无论是字还是位，都统一存于short数组内
 * 如果是位对象，数组每一项中1位ON，0为OFF
 ******************************************************************************/
int XNetAPI WriteRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _writeRegs);

int XNetAPI ReadRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _readRegs);

int XNetAPI WriteCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _writeCoils);

int XNetAPI ReadCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _readCoil);