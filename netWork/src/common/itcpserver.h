
#ifndef _I_TCP_SERVER_
#define _I_TCP_SERVER_

#ifdef WIN32
#include <WinSock.h>
#else
#endif

#include "networkthread.h"

#define MIN_CONN 1
#define MAX_CONN 10

#define MIN_PORT 1024
#define MAX_PORT 49151

interface ITCPServer: public ItcpThreadOwner
{
	virtual int StartServer(  PORT listenPort, int maxCon = MIN_CONN ) = 0;
	virtual int StopServer( ) = 0;
	virtual int ActionServer() = 0;
};

extern ITCPServer* CreateTCPServer( );
extern bool CloseTCPServer( ITCPServer* pServer );






#endif
