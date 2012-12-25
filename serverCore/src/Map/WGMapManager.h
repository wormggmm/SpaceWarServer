#ifndef _WG_MAP_MANAGER_H_327
#define _WG_MAP_MANAGER_H_327
#include "def.h"
#include "ProtocolMap.h"
#include "MapDef.h"

class WGMapNodeBase;


class WGMapManager
{
private:
	WGMapManager();
	~WGMapManager();
public:
	static WGMapManager *Singleton();
public:
	int initMapData();
	const WGMapNodeBase *getMapInfo();

public:
	void c2sRequestMapInfo(int clientIdx, PROTOCOL_HEADER* protocol, int dataSize);
private:
	 WGMapNodeBase *m_mapNodes;
	 MapNodeInfo *m_mapInfo;
	 UINT m_mapNodeCount;

};





#endif