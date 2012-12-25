
#include <stdio.h>
#include "itcpserver.h"
#include "tools.h"
#include "Core.h"
#include "WGNetProtocolProcess.h"

int main()
{

	//ITCPServer* Server = CreateTCPServer();
	//if ( !Server )
	//	return 1;



	//Server->StartServer( 3345 );
	startCore();
	g_Server->StartServer(3345, 10);

	TimerStart();

	unsigned int dataSize = 0;
	while (true)
	{
		
		do 
		{
			char*data = (char *)g_Server->GetDataFromRecvBuff(dataSize);
			if (dataSize > 0)
			{
				//char recvRet[50] = {0};
				TCPNetPack *pack = (TCPNetPack*)data;
				//sprintf_s(recvRet, "Recv Over:%s", pack->buff);
				//OutPutDebugStringToCMD(pack->buff);
				//g_Server->PushDataToSendBuff(pack->ConnectionID,  recvRet, strlen(recvRet));


				//减去TCPNetPack的开头ConnectIdx的size
				g_NetProtocolProcess.ProcessNetProtocol(pack->ConnectionID, (PROTOCOL_HEADER*)(pack->buff), dataSize-4);

				OutPutStringToCMD("recv msg from Client :%d\n", pack->ConnectionID);
			}
		}while (dataSize>0);



		Sleep(10);
	}
	

	g_Server->StopServer();



	return 0;

	/*
	ITCPServer* Server = CreateTCPServer();
	if ( !Server )
		return 1;

	Server->StartServer( 3345 );

	TimerStart();
	
	char md5[1024] = {0};
	char* filename="d:\\aa.txt";
	file_md5( filename, md5 );

	OutPutDebugStringToCMD("filemd5:%s\n", md5);

	char* stringtest ="md5test";
	string_md5( stringtest, md5 );

	OutPutDebugStringToCMD("stringMd5:%s\n", md5);

	//


	unsigned int dataSize = 0;

	while (true)
	{
		char*data = (char *)Server->GetDataFromRecvBuff(dataSize);
		if (dataSize > 0)
		{
			char recvRet[50] = {0};
			TCPNetPack *pack = (TCPNetPack*)data;
			sprintf_s(recvRet, "Recv Over:%s", pack->buff);
			OutPutDebugStringToCMD(pack->buff);
			Server->PushDataToSendBuff(pack->ConnectionID,  recvRet, strlen(recvRet));
		}
		Sleep(10);
	}

	getchar();

	Server->StopServer();
	return 0;
	*/
}