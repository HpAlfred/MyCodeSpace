#include <stdio.h>
#pragma comment(lib, "LibXNet.lib" )
#include "XNetAPI.h"

void main()
{
	XNetComm comm;
	short writeregs[20];
	short readregs[20];
	int i=0;
	int ret;
	BOOL retb;
	ret = XNetServerInit();

	comm=XNetCommunication();

	ret = ComPortTypeFindDevice( comm, 5, 0x03, 0x00,0x00 );

	for (i = 0; i < 20 ; i++)
	{
		writeregs[i] = i*i; 
	}
	ret = WriteRegs(comm,0x80,0,20,writeregs);
	ret = ReadRegs(comm,0x80,0,20,readregs);

	for (i = 0; i < 10 ; i++)
	{
		writeregs[i*2] = 1; 
		writeregs[i*2+1] = 0;
		readregs[i*2] = 0; 
		readregs[i*2+1] = 0;
	}

	ret = WriteCoils(comm,M,0,20,writeregs);
	ret = ReadCoils(comm,M,1,20,readregs);
	return ;
}