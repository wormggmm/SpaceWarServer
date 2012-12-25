#include "Core.h"
#include "Map/WGMapManager.h"
#include "def.h"
#include "header_debug.h"

ITCPServer* g_Server = NULL;



int initServer()
{
	if (g_Server != NULL)
		return INVALID_VALUE;

	g_Server = CreateTCPServer();

	if (g_Server == NULL)
		return INVALID_VALUE;


	return SUCCESS_VALUE;
}

int initCore()
{
	if(SUCCESS_VALUE != WGMapManager::Singleton()->initMapData())
	{
		OutPutStringToCMD("initCore ERROR::initMapData error!\n");
		return INVALID_VALUE;
	}

	return 0;
}

int startCore()
{
	if (SUCCESS_VALUE != initServer())
	{
		OutPutStringToCMD("initServer error!\n");
		return INVALID_VALUE;
	}

	if (SUCCESS_VALUE != initCore())
	{
		OutPutStringToCMD("initCore error!\n");
		return INVALID_VALUE;
	}

	return SUCCESS_VALUE;
}
