#include <winsock2.h>
//#include <windows.h>
#include <stdio.h>

#define XNetComm SOCKET 

#define XNetAPI

/******************************************************************************
 *			�������񣬲���ʼ��
 ******************************************************************************/
int XNetAPI XNetServerInit();


/******************************************************************************
 *			���ӵ����񲢻�ȡͨ�ž��
 ******************************************************************************/
XNetComm XNetAPI XNetCommunication();


/******************************************************************************
 *			����Զ������
 ******************************************************************************/
BOOL XNetAPI SetupRemoteLink(XNetComm _comm,char* _id,char * _password);


/******************************************************************************
 *			�����豸
 ******************************************************************************/
int XNetAPI ComPortTypeFindDevice( XNetComm _comm, BYTE _com, BYTE _type, BYTE _series, BYTE _model );

int XNetAPI AutoTypeFindDevice( XNetComm _comm, BYTE _type, BYTE _series, BYTE _model );

int XNetAPI ComPortIDFindDevice( XNetComm _comm, BYTE _com, char* _id );

int XNetAPI AutoIDFindDevice( XNetComm _comm, char* _id );


/******************************************************************************
 *			��λ�����д
 * �������ֻ���λ����ͳһ����short������
 * �����λ��������ÿһ����1λON��0ΪOFF
 ******************************************************************************/
int XNetAPI WriteRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _writeRegs);

int XNetAPI ReadRegs(XNetComm _comm,BYTE _regType,int _offSet,int _num,short * _readRegs);

int XNetAPI WriteCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _writeCoils);

int XNetAPI ReadCoils(XNetComm _comm,BYTE _coilType,int _offSet,int _num,short * _readCoil);