#ifndef _MAP_DEF_H_327
#define _MAP_DEF_H_327
#include "CommonStruct.h"

#define MAP_NODE_MAX_COUNT 50
#define MAP_NODE_MIN_COUNT 30

#define MAP_X_MAX 1024
#define MAP_Y_MAX 768
#define MAP_Z_MAX 100


struct MapNodeInfo
{
	WGPoint3 m_position;
};

#endif