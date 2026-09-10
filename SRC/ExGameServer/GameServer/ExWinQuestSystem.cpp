#include "StdAfx.h"
#include "ExWinQuestSystem.h"
#include "user.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "Path.h"
#include "Log.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "Item.h"
#include "Map.h"
#include "Monster.h"
#include "ItemOptionRate.h"
#include "Notice.h"
#include "CommandManager.h"
#include "CashShop.h"
#include "ObjectManager.h"
#include "Party.h"
#include "User.h"
#include "ReadScript.h"
#include "CustomRankUser.h"

ExWinQuestSystem g_ExWinQuestSystem;

BOOL gObjIsConnectedEx(int aIndex)
{
	if (aIndex < 0 || aIndex > MAX_OBJECT - 1)
	{
		return FALSE;
	}

	OBJECTSTRUCT* lpObj = (OBJECTSTRUCT*)&gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return FALSE;
	}

	if (lpObj->Connected < OBJECT_ONLINE)
	{
		return FALSE;
	}

	if (lpObj->CloseCount >= 0)
	{
		return FALSE;
	}

	return TRUE;
}

ExWinQuestSystem::ExWinQuestSystem()
{
	this->Init();
}

void ExWinQuestSystem::Init()
{
	ZeroMemory(&this->ewdata, sizeof(this->ewdata));

	this->Enable = false;
	this->NPC_Class = 0;
	this->NPC_Map = 0;
	this->NPC_X = 0;
	this->NPC_Y = 0;
	this->iDropLoaded = 0;
}

void ExWinQuestSystem::Load()
{
	this->Init();

	this->Read(gPath.GetFullPath("Custom\\NhiemVu\\ConfigNhiemVu.ini"));
	this->ReadQuest(0, gPath.GetFullPath("Custom\\NhiemVu\\NhiemVu.ini"));
}

void ExWinQuestSystem::Read(char* File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	if (!SMDFile)
	{
		ErrorMessageBox("[NhiemVu] %s file not found", File);
		return;
	}

	this->Enable = GetPrivateProfileInt("Common", "Enable", 0, File);
	this->NPC_Class = GetPrivateProfileInt("Common", "NPC_Class", 0, File);
	this->NPC_Map = GetPrivateProfileInt("Common", "NPC_Map", 0, File);
	this->NPC_X = GetPrivateProfileInt("Common", "NPC_X", 0, File);
	this->NPC_Y = GetPrivateProfileInt("Common", "NPC_Y", 0, File);

	this->ReloadLap = GetPrivateProfileInt("Common", "ResetNhiemVu", 0, File);
}

void ExWinQuestSystem::ReadQuest(int a, char* File)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, File);
		return;
	}

	if (lpMemScript->SetBuffer(File) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		int n = 0;

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

			this->ewdata[a][n].NeedLevel = lpMemScript->GetNumber();
			this->ewdata[a][n].NeedReset = lpMemScript->GetAsNumber();
			this->ewdata[a][n].NeedGrand = lpMemScript->GetAsNumber();
			strncpy(this->ewdata[a][n].NameQuest, lpMemScript->GetAsString(), 25);

			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				this->ewdata[a][n].MissionType[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionID[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionLevel[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionCount[m] = lpMemScript->GetAsNumber();
			}

			for (int r = 0; r < EW_MAX_REWARD; r++)
			{
				this->ewdata[a][n].RewardType[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemType[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemIndex[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemLevel[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardCount[r] = lpMemScript->GetAsNumber();
			}

			n++;
		}

		this->iLoaded[a] = n;
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}
	delete lpMemScript;
}

bool ExWinQuestSystem::Dialog(int aIndex, int aNpcIndex)
{
	LPOBJ lpUser = &gObj[aIndex];
	LPOBJ lpNpc = &gObj[aNpcIndex];

	if (!this->Enable)
	{
		GCChatTargetSend(lpUser, lpNpc->Index, "Tính năng này đang tạm khóa rồi bạn!");
		return false;
	}

	if (!gObjIsConnectedEx(aIndex))
	{
		return false;
	}

	if (lpNpc->Class == this->NPC_Class
		&& lpNpc->Map == this->NPC_Map
		&& lpNpc->X == this->NPC_X
		&& lpNpc->Y == this->NPC_Y
		)
	{
		this->GC_OpenWindow(aIndex);
		return true;
	}
	return false;
}

void ExWinQuestSystem::GC_MainInfo(int aIndex, bool isOpenWindow)
{
	LPOBJ lpUser = &gObj[aIndex];

	int n = 0;

	GC_MainPacket pReq;
		pReq.h.set(0xF3, 0x82, sizeof(pReq));

	pReq.isOpenWindow = isOpenWindow;

	int a = 0;
	{
		bool sb_accept = true;
		bool sb_done = true;

		bool sb_level = true;
		bool sb_reset = true;
		bool sb_grand = true;

		bool b_mission[EW_MAX_MISSION];

		for (int i = 0; i < EW_MAX_MISSION; i++)
		{
			b_mission[i] = true;
		}

		n = lpUser->ExWQuestNum;

		if (n >= this->iLoaded[a])
		{
			pReq.exQuest[a].b_End = true;
		}
		else
		{
			pReq.exQuest[a].b_End = false;
		}
		// -> user send
		strncpy(pReq.exQuest[a].NameQuest, this->ewdata[a][n].NameQuest, 25);
		pReq.exQuest[a].Start = lpUser->ExWQuestStart;
		pReq.exQuest[a].Number = lpUser->ExWQuestNum;
		pReq.exQuest[a].ExWQuestCurrent[a] = lpUser->ExWQuestCurrent;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			if (this->ewdata[a][n].MissionType[m] == Hunting)
			{
				if (lpUser->ExWQuestCount[m] > this->ewdata[a][n].MissionCount[m])
				{
					lpUser->ExWQuestCount[m] = this->ewdata[a][n].MissionCount[m];
				}
				pReq.exQuest[a].Count[m] = lpUser->ExWQuestCount[m];
			}
		}
		// -> need send
		if (this->ewdata[a][n].NeedLevel > lpUser->Level)
		{
			sb_level = false;
		}

		pReq.exQuest[a].NeedLevel = this->ewdata[a][n].NeedLevel;

		if (this->ewdata[a][n].NeedReset > lpUser->Reset)
		{
			sb_reset = false;
		}

		pReq.exQuest[a].NeedReset = this->ewdata[a][n].NeedReset;


		if (this->ewdata[a][n].NeedGrand > lpUser->MasterReset)
		{
			sb_grand = false;
		}
		pReq.exQuest[a].NeedGrand = this->ewdata[a][n].NeedGrand;
		// -> mission send
		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			if (this->ewdata[a][n].MissionType[m] == Hunting)
			{
				if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[m])
				{
					b_mission[m] = false;
					sb_done = false;
				}
			}

			pReq.exQuest[a].b_Mission[m] = b_mission[m];

			pReq.exQuest[a].MissionType[m] = this->ewdata[a][n].MissionType[m];
			pReq.exQuest[a].MissionID[m] = this->ewdata[a][n].MissionID[m];
			pReq.exQuest[a].MissionLevel[m] = this->ewdata[a][n].MissionLevel[m];
			pReq.exQuest[a].MissionCount[m] = this->ewdata[a][n].MissionCount[m];
		}
		// -> reward send
		for (int r = 0; r < EW_MAX_REWARD; r++)
		{
			pReq.exQuest[a].RewardType[r] = this->ewdata[a][n].RewardType[r];
			pReq.exQuest[a].RewardCount[r] = this->ewdata[a][n].RewardCount[r];
			pReq.exQuest[a].RewardItemType[r] = this->ewdata[a][n].RewardItemType[r];
			pReq.exQuest[a].RewardItemIndex[r] = this->ewdata[a][n].RewardItemIndex[r];
			pReq.exQuest[a].RewardItemLevel[r] = this->ewdata[a][n].RewardItemLevel[r];
		}

		if (lpUser->ExWQuestStart == FALSE)
		{
			if (sb_level == false || sb_reset == false || sb_grand == false)
			{
				sb_accept = false;
			}
		}

		//LogAdd(LOG_RED, "%d/%d, %d/%d, %d/%d"
		//	, this->ewdata[a][n].NeedLevel, lpUser->Level
		//	, this->ewdata[a][n].NeedReset, lpUser->Reset
		//	, this->ewdata[a][n].NeedGrand, lpUser->MasterReset
		//);

		pReq.exQuest[a].b_done = sb_done;
		pReq.exQuest[a].b_accept = sb_accept;

		pReq.exQuest[a].b_Level = sb_level;
		pReq.exQuest[a].b_Reset = sb_reset;
		pReq.exQuest[a].b_Grand = sb_grand;
	}
	GDCharacterInfoSaveSend(lpUser->Index);
	DataSend(aIndex, (LPBYTE)&pReq, sizeof(pReq));
}
//
void ExWinQuestSystem::GC_OpenWindow(int aIndex)
{
	if (!gObjIsConnectedEx(aIndex))
	{
		return;
	}
	this->GC_MainInfo(aIndex, true);
}
//
void ExWinQuestSystem::GC_MiniInfo(int aIndex)
{
	LPOBJ lpUser = &gObj[aIndex];

	GC_ExWinMiniInfo pReq = { 0 };

		pReq.h.set(0xF3, 0x83, sizeof(pReq));

	int n = 0;

	int a = 0;
	{
		n = lpUser->ExWQuestNum;
		strncpy(pReq.exQuest[a].NameQuest, this->ewdata[a][n].NameQuest, 25);
		pReq.exQuest[a].Start = lpUser->ExWQuestStart;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			if (this->ewdata[a][n].MissionType[m] == Hunting)
			{
				pReq.exQuest[a].ExWQuestCount[m] = lpUser->ExWQuestCount[m];
			}

			pReq.exQuest[a].MissionType[m] = this->ewdata[a][n].MissionType[m];
			pReq.exQuest[a].MissionID[m] = this->ewdata[a][n].MissionID[m];
			pReq.exQuest[a].MissionLevel[m] = this->ewdata[a][n].MissionLevel[m];
			pReq.exQuest[a].MissionCount[m] = this->ewdata[a][n].MissionCount[m];
		}
	}

	DataSend(aIndex, (LPBYTE)&pReq, sizeof(pReq));
}

void ExWinQuestSystem::GC_MiniInfoEx(int aIndex, int a, int m)
{
	if (!gObjIsConnectedEx(aIndex))
	{
		return;
	}

	GC_ExWinMiniInfoEx exSend = { 0 };
	exSend.h.set(0xF3, 0x80, sizeof(exSend));

	exSend.a = a;
	exSend.m = m;

	DataSend(aIndex, (LPBYTE)&exSend, exSend.h.size);
}

void ExWinQuestSystem::CG_AcceptQuest(int aIndex, CG_Accept_Done* aRecv)
{
	if (!gObjIsConnectedEx(aIndex))
	{
		return;
	}

	if (aRecv->Result != 0 && aRecv->Result != 1 && aRecv->Result != 2)
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	int a = aRecv->Result;
	int n = lpUser->ExWQuestNum;

	if (!lpUser->ExWQuestStart)
	{
		if (this->ewdata[a][n].NeedLevel > lpUser->Level ||
			this->ewdata[a][n].NeedReset > lpUser->Reset ||
			this->ewdata[a][n].NeedGrand > lpUser->MasterReset)
		{
			return;
		}

		lpUser->ExWQuestStart = true;

		this->GC_Start(aIndex, a, true);
	}
	else
	{
		this->RewardQuest(aIndex, a);
	}
}

void ExWinQuestSystem::GC_Start(int aIndex, int a, int s)
{
	//LogAdd(LOG_RED,"GC_Start");
	GC_ExWinStart pReq = { 0 };
	pReq.h.set(0xF3, 0x81, sizeof(pReq));
	pReq.Act = a;
	pReq.Start = s;
	DataSend(aIndex, (LPBYTE)&pReq, pReq.h.size);

}

bool ExWinQuestSystem::MonsterKiller(int aIndexMonster, int aIndexUser)
{
	bool Result = false;

	if (!this->Enable)
	{
		return Result;
	}

	LPOBJ mainObj = &gObj[aIndexUser];
	LPOBJ monsterObj = &gObj[aIndexMonster];
	PARTY_INFO* lpParty = &gParty.m_PartyInfo[mainObj->PartyNumber];

	if (OBJECT_RANGE(mainObj->PartyNumber) != 0)
	{
		for (int j = 0; j < MAX_PARTY_USER; j++)
		{
			LPOBJ lpTarget = &gObj[gParty.m_PartyInfo[mainObj->PartyNumber].Index[j]];

			if (OBJECT_RANGE(lpTarget->Index) != 0)
			{
				if (monsterObj->Map == mainObj->Map && gObjCalcDistance(monsterObj, &gObj[lpParty->Index[j]]) < MAX_PARTY_DISTANCE)
				{
					bool res = this->MonsterKill(aIndexMonster, lpTarget->Index);
					if (res == true)
					{
						Result = res;
					}
				}
				else
				{
					Result = false;
				}
			}
		}
	}

	else
	{
		Result = this->MonsterKill(aIndexMonster, aIndexUser);
	}
	return Result;
}

bool ExWinQuestSystem::MonsterKill(int aIndexMonster, int aIndexUser)
{
	bool Result = false;

	if (!this->Enable)
	{
		return Result;
	}

	LPOBJ lpUser = &gObj[aIndexUser];
	LPOBJ lpMonster = &gObj[aIndexMonster];

	int n = 0;
	int a = 0;
	{
		n = lpUser->ExWQuestNum;

		if (lpUser->ExWQuestStart == TRUE)
		{
			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				if (this->ewdata[a][n].MissionType[m] == Hunting)
				{
					if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[m])
					{
						if (this->ewdata[a][n].MissionLevel[m] == lpMonster->Map || this->ewdata[a][n].MissionLevel[m] == -1)
						{
							if (this->ewdata[a][n].MissionID[m] == lpMonster->Class)
							{
								lpUser->ExWQuestCount[m]++;
								this->GC_MiniInfoEx(aIndexUser, a, m);
								GDCharacterInfoSaveSend(lpUser->Index);
							}
						}
					}
				}
			}
		}
	}
	return Result;
}

bool ExWinQuestSystem::CheckMission(int aIndex, int a)
{
	LPOBJ lpUser = &gObj[aIndex];

	int n = lpUser->ExWQuestNum;

	for (int m = 0; m < EW_MAX_MISSION; m++)
	{
		if (this->ewdata[a][n].MissionType[m] == Hunting)
		{
			if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[m])
			{
				return false;
			}
		}
	}

	return true;
}

void ExWinQuestSystem::RewardQuest(int aIndex, int Act)
{
	if (!gObjIsConnectedEx(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	int a = Act;
	int n = lpUser->ExWQuestNum;

	if (!this->CheckMission(aIndex, a))
	{
		return;
	}

	int rewCount = 0;
	int X = 0;
	int Y = 0;

	for (int i = 0; i < EW_MAX_REWARD; i++)
	{
		if (this->ewdata[a][n].RewardType[i] == ewItem)
		{
			int iItemIndex = this->ewdata[a][n].RewardItemIndex[i];
			int iItemType = this->ewdata[a][n].RewardItemType[i];

			ITEM_INFO ItemInfo;

			gItemManager.GetInfo(GET_ITEM(iItemType, iItemIndex), &ItemInfo);

			X += ItemInfo.Width;

			if (Y < ItemInfo.Height)
			{
				Y = ItemInfo.Height;
			}
		}
	}

	if (!gItemManager.CheckItemInventorySpace(&gObj[aIndex], X, Y))
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Không đủ chỗ trong thùng đồ. Cần %dx%d ô trống", X, Y);
		return;
	}

	int UserMap = lpUser->Map;
	int UserX = lpUser->X;
	int UserY = lpUser->Y;

	int rType = 0;
	int rItemType = 0;
	int rItemIndex = 0;
	int rItemLevel = 0;
	int rCount = 0;

	for (int m = 0; m < EW_MAX_MISSION; m++)
	{
		lpUser->ExWQuestCount[m] = 0;

	}

	for (int r = 0; r < EW_MAX_REWARD; r++)
	{
		rType = this->ewdata[a][n].RewardType[r];
		rItemType = this->ewdata[a][n].RewardItemType[r];
		rItemIndex = this->ewdata[a][n].RewardItemIndex[r];
		rItemLevel = this->ewdata[a][n].RewardItemLevel[r];
		rCount = this->ewdata[a][n].RewardCount[r];

		if (rType == ewZen)
		{
			if (!MAP_RANGE(UserMap))
			{
				return;
			}

			if (lpUser->Money + rCount > MAX_MONEY)
			{
				lpUser->Money = MAX_MONEY;
			}
			else
			{
				lpUser->Money += rCount;
			}

			GCMoneySend(lpUser->Index, lpUser->Money);
		}
		if (rType == ewFreePoint)
		{
			lpUser->LevelUpPoint += rCount;

			gObjectManager.CharacterCalcAttribute(lpUser->Index);

			GCNewCharacterInfoSend(lpUser);

			GDCharacterInfoSaveSend(lpUser->Index);
		}

		if (rType == ewItem)
		{
			int level, skill, luck, option, exc, set, socket;

			BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

			ITEM_INFO TargetItem;
			gItemManager.GetInfo(GET_ITEM(rItemType, rItemIndex), &TargetItem);

			level = rItemLevel;
			skill = TargetItem.Skill;
			luck = 0;
			option = 0;
			exc = 0;
			set = 0;
			socket = 0;

			for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
			{
				ItemSocketOption[n] = 0xFF;
			}

			for (int d = 0; d < rCount; d++)
			{
				GDCreateItemSend(aIndex, 0xEB, 0, 0, GET_ITEM(rItemType, rItemIndex), level, 0, skill, luck, option, -1, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);
			}
		}
		if (rType == ewBonus)
		{
			GDSetCoinSend(aIndex, rCount, 0, 0, 0, 0, "NhiemVu");
			gObjectManager.CharacterCalcAttribute(lpUser->Index);
			GDCharacterInfoSaveSend(lpUser->Index);
		}
	}

	lpUser->ExWQuestNum++;
	lpUser->ExWQuestStart = false;
	this->GC_MainInfo(aIndex, true);
}

void ExWinQuestSystem::QuestChange(int aIndex, QUESTCHANGE_REQ* lpMsg)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpMsg->numchange == 111)
	{
		lpObj->ExWQuestStart = false;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			lpObj->ExWQuestCount[m] = 0;
		}

		this->GC_MainInfo(aIndex, true);
	}
	else
	{
		if (lpMsg->numchange < 0 )
		{
			lpObj->ExWQuestNum = 0;
			return;
		}

		if (lpMsg->numchange > this->ReloadLap)
		{
			lpObj->ExWQuestNum = this->ReloadLap;
			return;
		}

		if (lpMsg->numchange > lpObj->ExWQuestCurrent)
		{
			lpObj->ExWQuestNum = lpObj->ExWQuestCurrent;
		}
		else
		{
			lpObj->ExWQuestNum = lpMsg->numchange;
		}
	}
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	GDResetInfoSaveSend(lpObj->Index, 0, 0, 0);
	this->GC_MainInfo(aIndex, true);
}