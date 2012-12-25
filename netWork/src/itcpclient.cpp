#include "common/itcpclient.h"
#include "tcpclient.h"
ITCPClient* CreateTCPClient( )
{
	ITCPClient* pClient = ( ITCPClient*)(new tcpclient);
	return pClient;
}

bool CloseTCPClient( ITCPClient* pClient )
{
	if ( !pClient )
		return false;

	pClient->StopClient();
	return true;
}
