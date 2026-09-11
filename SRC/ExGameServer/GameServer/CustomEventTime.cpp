#include "stdafx.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ServerDisplayer.h"
#include "Util.h"

#include "User.h"
#include "Path.h"
#include "ServerInfo.h"
#include "CustomEventTime.h"
#include "MemScript.h"
#include "Notice.h"
#include "InvasionManager.h"
#include "MapManager.h"

CCustomEventTime gCustomEventTime;

void CCustomEventTime::Init()
{
	for (int n = 0; n < MAX_EVENTTIME; n++)
	{
		this->m_CustomEventInfo[n].Index = -1;
		this->m_CustomEventInfo[n].Name[0] = 0;
		this->m_CustomEventInfo[n].Map[0] = 0;
		this->m_CustomEventInfo[n].Gate[0] = 0;
	}
}

void CCustomEventTime::SetInfo(CUSTOM_EVENTTIME_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_EVENTTIME)
	{
		return;
	}

	this->m_CustomEventInfo[info.Index] = info;
}

void CCustomEventTime::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		LogAdd(LOG_RED, "[CustomEventTime] Load failed: %s", path);
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_EVENTTIME_INFO info;
			memset(&info, 0, sizeof(info));
			info.Index = lpMemScript->GetNumber();
			strcpy_s(info.Name, lpMemScript->GetAsString());
			strcpy_s(info.Map, lpMemScript->GetAsString());
			strcpy_s(info.Gate, lpMemScript->GetAsString());
			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
	LogAdd(LOG_BLUE, "[CustomEventTime] loaded successfully");
}

void CCustomEventTime::FillEventData(int n, CUSTOM_EVENTTIME_DATA* info)
{
	memset(info, 0, sizeof(*info));
	info->index = n;

	if (n == 0) info->time = gServerDisplayer.EventBc;
	else if (n == 1) info->time = gServerDisplayer.EventDs;
	else if (n == 2) info->time = gServerDisplayer.EventCc;
	else if (n == 3) info->time = gServerDisplayer.EventCTCMini;
	else if (n >= 4 && n < (4 + MAX_INVASION)) info->time = gServerDisplayer.EventInvasion[n - 4];
	else if (n == 34) info->time = gServerDisplayer.EventIt;
	else if (n == 35) info->time = gServerDisplayer.EventCs;
	else if (n == 36) info->time = gServerDisplayer.EventCryWolf;
	else if (n == 37) info->time = gServerDisplayer.EventCastleDeep;
	else if (n == 38) info->time = gServerDisplayer.EventMoss;
	else if (n == 39) info->time = gServerDisplayer.EventKing;
	else if (n == 40) info->time = gServerDisplayer.EventDrop;
	else if (n == 41) info->time = gServerDisplayer.EventCustomArena[0];
	else info->time = -1;

	if (n >= 0 && n < MAX_EVENTTIME && this->m_CustomEventInfo[n].Index >= 0)
	{
		strcpy_s(info->Name, this->m_CustomEventInfo[n].Name);
		strcpy_s(info->Map, this->m_CustomEventInfo[n].Map);
	}

	if (info->Name[0] == 0 && n >= 4 && n < (4 + MAX_INVASION))
	{
		INVASION_INFO* lpInvasion = &gInvasionManager.m_InvasionInfo[n - 4];
		if (lpInvasion->AlertMessage[0] != 0)
		{
			strncpy_s(info->Name, lpInvasion->AlertMessage, _TRUNCATE);
		}

		if (info->Map[0] == 0)
		{
			for (int group = 0; group < MAX_INVASION_RESPAWN_GROUP; group++)
			{
				if (lpInvasion->RespawnInfo[group].empty() == 0)
				{
					char* mapName = gMapManager.GetMapName(lpInvasion->RespawnInfo[group][0].Map);
					if (mapName != 0)
					{
						strncpy_s(info->Map, mapName, _TRUNCATE);
					}
					break;
				}
			}
		}
	}

	if (info->Name[0] == 0)
	{
		if (n == 0) strcpy_s(info->Name, "Blood Castle");
		else if (n == 1) strcpy_s(info->Name, "Devil Square");
		else if (n == 2) strcpy_s(info->Name, "Chaos Castle");
		else if (n == 3) strcpy_s(info->Name, "Castle Siege Mini");
		else if (n == 34) strcpy_s(info->Name, "Illusion Temple");
		else if (n == 35) strcpy_s(info->Name, "Castle Siege");
		else if (n == 36) strcpy_s(info->Name, "Crywolf");
		else if (n == 37) strcpy_s(info->Name, "Castle Deep");
		else if (n == 38) strcpy_s(info->Name, "Moss Merchant");
		else if (n == 39) strcpy_s(info->Name, "Golden Invasion");
		else if (n == 40) strcpy_s(info->Name, "Event Drop");
		else if (n == 41) strcpy_s(info->Name, "Custom Arena");
	}

	if (info->Map[0] == 0)
	{
		if (n == 0) strcpy_s(info->Map, "Devias");
		else if (n == 1) strcpy_s(info->Map, "Noria");
		else if (n == 2) strcpy_s(info->Map, "Lorencia");
		else if (n == 3) strcpy_s(info->Map, "Guild War");
		else if (n == 34) strcpy_s(info->Map, "Illusion Temple");
		else if (n == 35) strcpy_s(info->Map, "Castle Siege");
		else if (n == 36) strcpy_s(info->Map, "Crywolf");
		else if (n == 37) strcpy_s(info->Map, "Lorencia");
		else if (n >= 38) strcpy_s(info->Map, "Server");
	}
}

void CCustomEventTime::GCReqEventTime(int Index, PMSG_CUSTOM_EVENTTIME_RECV* lpMsg)
{
	if (gServerInfo.m_CustomEventTimeSwitch == 0)
	{
		return;
	}

	// Main client (H board) expects CUSTOM_EVENTTIME_DATA with Name/Map.
	// Legacy packets (index+time only) mis-align the client parser and leave the list empty.
	CUSTOM_EVENTTIME_DATA info[MAX_EVENTTIME];
	memset(info, 0, sizeof(info));

	int count = 0;
	int indexes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 34, 35, 36, 37, 38, 39, 40, 41 };
	for (int i = 0; i < (int)(sizeof(indexes) / sizeof(indexes[0])) && count < MAX_EVENTTIME; ++i)
	{
		CUSTOM_EVENTTIME_DATA row{};
		this->FillEventData(indexes[i], &row);
		if (row.Name[0] == 0)
		{
			continue;
		}
		// Keep time < 0 rows so the H board still lists offline events.
		info[count++] = row;
	}

	BYTE send[4096];
	PMSG_CUSTOM_EVENTTIME_SEND pMsg{};
	pMsg.header.set(0xF3, 0xE8, 0);
	int size = sizeof(pMsg);
	pMsg.count = count;
	pMsg.RegLineEvent = (BYTE)count;

	for (int n = 0; n < count; ++n)
	{
		memcpy(&send[size], &info[n], sizeof(info[n]));
		size += sizeof(info[n]);
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(Index, send, size);
}

static bool DashboardEventValid(const CUSTOM_EVENTTIME_DATA* info)
{
	if (info->Name[0] == 0)
	{
		return false;
	}
	if (info->time < -1 || info->time > 86400 * 40)
	{
		return false;
	}
	for (int i = 0; i < 40 && info->Name[i] != 0; ++i)
	{
		unsigned char ch = (unsigned char)info->Name[i];
		if (ch < 32 || ch > 126)
		{
			return false;
		}
	}
	return true;
}

void CCustomEventTime::GCReqDashboardEvents(int Index)
{
	BYTE send[4096];
#pragma pack(push, 1)
	struct Record
	{
		int time;
		char name[40];
		char mapName[40];
	};
	struct Header
	{
		PSWMSG_HEAD header;
		int count;
	};
#pragma pack(pop)

	Header pMsg{};
	pMsg.header.set(0xF3, 0xEE, 0);
	int size = sizeof(pMsg);
	pMsg.count = 0;

	int indexes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 34, 35, 36, 37, 38, 39, 40, 41 };
	for (int i = 0; i < (int)(sizeof(indexes) / sizeof(indexes[0])); ++i)
	{
		CUSTOM_EVENTTIME_DATA info{};
		this->FillEventData(indexes[i], &info);
		if (DashboardEventValid(&info) == false)
		{
			continue;
		}
		if (info.time < 0)
		{
			continue;
		}

		Record rec{};
		rec.time = info.time;
		strncpy_s(rec.name, info.Name, _TRUNCATE);
		strncpy_s(rec.mapName, info.Map[0] ? info.Map : "Server", _TRUNCATE);
		memcpy(&send[size], &rec, sizeof(rec));
		size += sizeof(rec);
		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(Index, send, size);
}

void CCustomEventTime::GCReqOnlinePlayers(int Index)
{
	BYTE send[4096];
	PMSG_DASHBOARD_PLAYERS_SEND pMsg{};
	pMsg.header.set(0xF3, 0xEA, 0);
	int size = sizeof(pMsg);
	pMsg.count = 0;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		LPOBJ lpObj = &gObj[n];
		if (gObjIsConnectedGP(n) == 0 || lpObj->Live == 0 || lpObj->Name[0] == 0)
		{
			continue;
		}
		if (lpObj->AttackCustomOffline != 0 || lpObj->m_OfflineMode != 0 ||
			lpObj->PShopCustomOffline != 0 || lpObj->IsFakeOnline != 0 ||
			lpObj->Socket == INVALID_SOCKET)
		{
			continue;
		}

		DASHBOARD_ONLINE_PLAYER info{};
		strncpy_s(info.name, lpObj->Name, _TRUNCATE);
		char* mapName = gMapManager.GetMapName(lpObj->Map);
		if (mapName != 0)
		{
			strncpy_s(info.mapName, mapName, _TRUNCATE);
		}
		info.level = lpObj->Level;
		info.reset = lpObj->Reset;
		info.masterReset = lpObj->MasterReset;
		info.x = lpObj->X;
		info.y = lpObj->Y;
		info.hp = (int)lpObj->Life;
		info.maxHp = (int)lpObj->MaxLife + lpObj->AddLife;
		info.sd = lpObj->Shield;
		info.maxSd = lpObj->MaxShield + lpObj->AddShield;
		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
		pMsg.count++;
		if (size + (int)sizeof(info) >= (int)sizeof(send))
		{
			break;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(Index, send, size);
}
