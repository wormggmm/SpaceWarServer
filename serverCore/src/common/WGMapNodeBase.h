#ifndef _WG_MAP_NODE_BASE_H_327
#define _WG_MAP_NODE_BASE_H_327
#include "CommonStruct.h"
#include "ProtocolMap.h"
#include "MapDef.h"
class WGMapNodeBase
{
public:
	WGMapNodeBase();
	~WGMapNodeBase();
public:
	void createNode(MapNodeInfo* info);


private:
	MapNodeInfo *m_mapInfo;

};



#endif