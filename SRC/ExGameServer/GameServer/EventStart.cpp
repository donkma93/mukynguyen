// CEventStart.cpp: interface for the CEventStart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "EventStart.h"
#include "GameMaster.h"

#include "Monster.h"
#include "Notice.h"
#include "Protocol.h"
#include "ServerInfo.h"
#include "Util.h"
#include "ACuoi_MessLang.h"

CEventStart gEventStart;

CEventStart::CEventStart()
{
	this->m_RemainTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_Map = 0;
	this->m_MapX = 0;
	this->m_MapY = 0;
}

void CEventStart::Clear()
{
	this->m_RemainTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_Map = 0;
	this->m_MapX = 0;
	this->m_MapY = 0;
}

void CEventStart::MainProc()
{
	DWORD elapsed = GetTickCount()-this->m_TickCount;

	if(elapsed < 1000)
	{
		return;
	}

	this->m_TickCount = GetTickCount();

	if(this->m_RemainTime > 0)
	{ 

		if(this->m_RemainTime <= 10)
		{
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessageNew.GetMessage(470),m_RemainTime);
		}

		this->m_RemainTime--;

		if(this->m_RemainTime <= 0)
		{
			    this->m_EventStart = 0;
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(471));
				LogAdd(LOG_EVENT,"[EVENT START] Close");

		}

	}

}

void CEventStart::CommandEventStart(LPOBJ lpObj,char* arg) 
{

	if(gServerInfo.m_EventStartSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,1) == 0)
	{
		return;
	}

	if (this->m_RemainTime > 0) 
	{
		this->Clear();
		LogAdd(LOG_EVENT,"[EVENT START] Canceled");

		gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(472));
		return;
	}

	this->m_EventStart = 1;
	this->m_Map = lpObj->Map;
	this->m_MapX = lpObj->X;
	this->m_MapY = lpObj->Y-1;

	this->m_RemainTime = gServerInfo.m_EventStartTime;

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(473),lpObj->Name);
	gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(474));
	LogAdd(LOG_EVENT,"[EVENT START] Start");
}

void CEventStart::CommandEventStartJoin(LPOBJ lpObj,char* arg) 
{

	if(gServerInfo.m_EventStartSwitch == 0)
	{
		return;
	}

	if (this->m_EventStart == 0) 
	{
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(475),lpObj->Name);

	gObjTeleport(lpObj->Index,this->m_Map,this->m_MapX,this->m_MapY);

	LogAdd(LOG_EVENT,"[EVENT START] Member add (%s)",lpObj->Name);
}
