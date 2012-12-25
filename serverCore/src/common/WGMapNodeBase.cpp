
#include "WGMapNodeBase.h"
#include "random.h"
#include "MapDef.h"


WGMapNodeBase::WGMapNodeBase()
{
	m_mapInfo = NULL;
}

WGMapNodeBase::~WGMapNodeBase()
{
	m_mapInfo = NULL;
}

void WGMapNodeBase::createNode(MapNodeInfo *info)
{
	m_mapInfo = info;
}

