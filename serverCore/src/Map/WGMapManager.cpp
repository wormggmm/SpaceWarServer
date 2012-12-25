
#include "WGMapManager.h"
#include "MapDef.h"
#include "random.h"
#include "WGMapNodeBase.h"
#include "ProtocolMap.h"
#include "Protocol.h"
#include "Core.h"

WGMapManager::WGMapManager()
{
	m_mapNodeCount = MAP_NODE_MIN_COUNT + g_Random(MAP_NODE_MAX_COUNT - MAP_NODE_MIN_COUNT);
	m_mapNodes = new WGMapNodeBase[m_mapNodeCount];
	m_mapInfo = new MapNodeInfo[m_mapNodeCount];
 
}

WGMapManager::~WGMapManager()
{
	if (m_mapNodes)
		delete m_mapNodes;
}

WGMapManager *WGMapManager::Singleton()
{
	static WGMapManager *mgr = NULL;
	if (mgr == NULL)
	{
		mgr = new WGMapManager;
	}
	return mgr;
}


int WGMapManager::initMapData()
{
	for (UINT i = 0; i < m_mapNodeCount; i++)
	{
		m_mapInfo[i].m_position.x = g_Random(MAP_X_MAX);
		m_mapInfo[i].m_position.y = g_Random(MAP_Y_MAX);
		m_mapInfo[i].m_position.z = g_Random(MAP_Z_MAX);
		m_mapNodes[i].createNode(&(m_mapInfo[i]));
	}

	return 0;
}

const WGMapNodeBase *WGMapManager::getMapInfo()
{
	//const WGMapNodeBase* info = m_mapNodes;
	return m_mapNodes;
}


void WGMapManager::c2sRequestMapInfo(int clientIdx, PROTOCOL_HEADER* protocol, int dataSize)
{
	char *buffer = (char*)malloc(1000);
	S2C_SendMapInfo *data = (S2C_SendMapInfo *)buffer;
	data->Protocol = s2c_sendMapInfo;
	data->mapNodeCount = m_mapNodeCount;
	//memcpy_s(data->mapInfo,4000, m_mapInfo, sizeof(MapInfo)*m_mapNodeCount);
	memcpy(data->mapInfo, m_mapInfo, sizeof(MapNodeInfo)*m_mapNodeCount);
	data->Length = sizeof(data)+sizeof(MapNodeInfo)*m_mapNodeCount -1;
	g_Server->PushDataToSendBuff(clientIdx, data, data->Length);
}