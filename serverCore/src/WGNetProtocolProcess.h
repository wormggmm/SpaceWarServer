
#ifndef _WG_NET_PROTOCOL_PROCESS_H_327
#define _WG_NET_PROTOCOL_PROCESS_H_327
#include "Protocol.h"
#include "ProtocolStruct.h"


class WGNetProtocolProcess
{
public:
	WGNetProtocolProcess();
	~WGNetProtocolProcess();
	int ProcessNetProtocol(int clientIdx, PROTOCOL_HEADER*protocol, int dataSize);

private:
	void (WGNetProtocolProcess::*ProcessFunc[c2s_end])(int clientIdx, PROTOCOL_HEADER* protocol, int dataSize);
private:
	void c2sRequestMapInfo(int clientIdx, PROTOCOL_HEADER *protocal, int dataSize);


};


extern WGNetProtocolProcess g_NetProtocolProcess;


#endif