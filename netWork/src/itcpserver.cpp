#include "common/itcpserver.h"
#include "tcpserver.h"
ITCPServer* CreateTCPServer( )
{
	ITCPServer* pServer = ( ITCPServer*)(new tcpserver);
	return pServer;
}

bool CloseTCPServer( ITCPServer* pServer )
{
	if ( !pServer )
		return false;

	pServer->StopServer();
	return true;
}
