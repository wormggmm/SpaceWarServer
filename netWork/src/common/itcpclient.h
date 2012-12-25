
#ifndef _I_TCP_CLIENT_
#define _I_TCP_CLIENT_

#ifdef WIN32
#include <WinSock.h>
#else
#endif

#include "networkthread.h"


#define MIN_CONN 1
#define MAX_CONN 10



interface ITCPClient: public ItcpThreadOwner
{
	virtual int StartClient( char* serverIP,  PORT listenPort, int maxCon = MIN_CONN ) = 0;
	virtual int StopClient( ) = 0;
	virtual int ActionClient() = 0;

	virtual int ConnectToServer( ) = 0;
	virtual int DisConnectFromServer(  int ConntedIdx  ) = 0;

};

extern ITCPClient* CreateTCPClient( );
extern bool CloseTCPClient( ITCPClient* pClient);






#endif
