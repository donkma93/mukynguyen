#include "stdafx.h"
#include "CrywolfUtil.h"
#include "DSProtocol.h"
#include "Map.h"
#include "MapServerManager.h"
#include "Notice.h"
#include "Util.h"

CCrywolfUtil gCrywolfUtil;

void CCrywolfUtil::SendMapServerGroupMsg(char* lpMsg,...)
{
	if(lpMsg == 0)
	{
		return;
	}

	char buff[512] = {0};

	va_list arg;
	va_start(arg,lpMsg);
	vsprintf_s(buff,lpMsg,arg);
	va_end(arg);

	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,buff);
}

void CCrywolfUtil::SendAllUserAnyData(BYTE* lpMsg,int size)
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER)
		{
			DataSend(n,lpMsg,size);
		}
	}
}

void CCrywolfUtil::SendAllUserAnyMsg(int type,char* lpMsg,...)
{
	if(lpMsg == 0)
	{
		return;
	}

	char buff[512] = {0};

	va_list arg;
	va_start(arg,lpMsg);
	vsprintf_s(buff,lpMsg,arg);
	va_end(arg);

	gNotice.GCNoticeSendToAll(type,0,0,0,0,0,buff);
}

void CCrywolfUtil::SendCrywolfUserAnyData(BYTE* lpMsg,int size)
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER && gObj[n].Map == MAP_CRYWOLF)
		{
			DataSend(n,lpMsg,size);
		}
	}
}

void CCrywolfUtil::SendCrywolfUserAnyMsg(int type,char* lpMsg,...)
{
	if(lpMsg == 0)
	{
		return;
	}

	char buff[512] = {0};

	va_list arg;
	va_start(arg,lpMsg);
	vsprintf_s(buff,lpMsg,arg);
	va_end(arg);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER && gObj[n].Map == MAP_CRYWOLF)
		{
			gNotice.GCNoticeSend(n,type,0,0,0,0,0,buff);
		}
	}
}

bool CCrywolfUtil::CrywolfAllUserScoreSort(LPOBJ const &lpObj,LPOBJ const &lpTarget)
{
	return ((lpObj->CrywolfMVPScore>=lpTarget->CrywolfMVPScore)?1:0);
}
