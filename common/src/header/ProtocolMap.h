
#ifndef _PROTOCOL_MAP_H_327
#define _PROTOCOL_MAP_H_327
#include "ProtocolStruct.h"
#pragma	pack(push, 1)

struct S2C_SendMapInfo : PROTOCOL_HEADER
{
	UINT mapNodeCount;
	char mapInfo[1];
};






#endif