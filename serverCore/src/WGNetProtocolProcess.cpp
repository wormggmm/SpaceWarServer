

#include "WGNetProtocolProcess.h"
#include "Map/WGMapManager.h"

WGNetProtocolProcess g_NetProtocolProcess;



WGNetProtocolProcess::WGNetProtocolProcess()
{
#ifdef WIN32
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
#else
	memset(ProcessFunc, 0, sizeof(ProcessFunc));
#endif
	ProcessFunc[c2s_requestMapInfo] = &WGNetProtocolProcess::c2sRequestMapInfo;
}

WGNetProtocolProcess::~WGNetProtocolProcess()
{

}

int WGNetProtocolProcess::ProcessNetProtocol( int clientIdx, PROTOCOL_HEADER*protocol, int dataSize )
{
	if (protocol->Protocol <= c2s_begin  || protocol->Protocol >= c2s_end)
		return INVALID_VALUE;

	if (dataSize != protocol->Length)
		return INVALID_VALUE;

	if (ProcessFunc[protocol->Protocol])
	{
		(this->*ProcessFunc[protocol->Protocol])(clientIdx, protocol, dataSize);
		return SUCCESS_VALUE;
	}
	return INVALID_VALUE;
}


//协议处理函数开始


//s2c_requestMapInfo
void WGNetProtocolProcess::c2sRequestMapInfo( int clientIdx, PROTOCOL_HEADER *protocal, int dataSize )
{
	WGMapManager::Singleton()->c2sRequestMapInfo(clientIdx, protocal, dataSize);
}
