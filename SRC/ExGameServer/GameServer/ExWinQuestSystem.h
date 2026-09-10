#pragma once
#include "Protocol.h"

#define EW_MAX_ACT				1
#define EW_MAX_MISSION			5
#define EW_MAX_REWARD			4
#define EW_MAX_QUEST			1000
#define EW_MAX_DROP				100

#define EX_QUEST_SYSTEM_SIZE	39
#define EW_NEW_DROP				TRUE

enum ExWinMission
{
	Hunting = 1
};

enum ExWinReward
{
	ewZen = 1,
	ewFreePoint = 2,
	ewItem = 3,
	ewBonus = 4,
};

#pragma pack(push, 1) 
struct WinQuest_Table
{
	char NameQuest[25];
	BYTE b_accept;
	BYTE b_done;
	BYTE b_End;
	BYTE Start;
	WORD Number;
	WORD Count[5];
	BYTE b_Level;
	WORD NeedLevel;
	BYTE b_Reset;
	WORD NeedReset;
	BYTE b_Grand;
	WORD NeedGrand;
	BYTE b_Mission[5];
	BYTE MissionType[5];
	WORD MissionID[5];
	BYTE MissionLevel[5];
	WORD MissionCount[5];
	WORD RewardType[4];
	DWORD RewardCount[4];
	WORD RewardItemType[4];
	WORD RewardItemIndex[4];
	WORD RewardItemLevel[4];
	WORD ExWQuestCurrent[1];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GC_MainPacket
{
	PSWMSG_HEAD h;
	BYTE isOpenWindow;
	WinQuest_Table exQuest[1];
};
#pragma pack(pop)

struct WinQuest_Mission
{
	char NameQuest[25];
	BYTE Start;
	WORD ExWQuestCount[EW_MAX_MISSION];
	BYTE MissionType[EW_MAX_MISSION];
	WORD MissionID[EW_MAX_MISSION];
	BYTE MissionLevel[EW_MAX_MISSION];
	WORD MissionCount[EW_MAX_MISSION];
};

struct GC_ExWinMiniInfo
{
	PSWMSG_HEAD h;  //PSWMSG_HEAD set
	WinQuest_Mission exQuest[EW_MAX_ACT];
};

struct GC_ExWinMiniInfoEx
{
	PSBMSG_HEAD h;
	BYTE a;
	BYTE m;
};

struct CG_Accept_Done
{
	PSBMSG_HEAD h;
	BYTE Result;
};

struct GC_ExWinStart
{
	PSBMSG_HEAD h;
	BYTE Act;
	BYTE Start;
};

struct GC_ExWinMiniInfoExItem
{
	PSBMSG_HEAD h;
	BYTE a;
	BYTE m;
	BYTE c;
};

struct QUESTCHANGE_REQ
{
	PSBMSG_HEAD h;
	int	numchange;
};

struct ExWinQuest_DATA
{
	char NameQuest[25];

	int NeedLevel;
	int NeedReset;
	int NeedGrand;

	int MissionType[EW_MAX_MISSION];
	int MissionID[EW_MAX_MISSION];
	BYTE MissionLevel[EW_MAX_MISSION];
	int MissionCount[EW_MAX_MISSION];

	int RewardType[EW_MAX_REWARD];
	int RewardCount[EW_MAX_REWARD];
	int RewardItemType[EW_MAX_REWARD];
	int RewardItemIndex[EW_MAX_REWARD];
	int RewardItemLevel[EW_MAX_REWARD];
};

struct ExWinDrop_DATA
{
	int iAct;
	int iNeedQuest;
	int ItemType;
	int ItemIndex;
	int ItemMinLevel;
	int ItemMaxLevel;
	int ItemMaxOption;
	int ItemLuck;
	int ItemSkill;
	int ItemExcellent;
	int ItemAncent;
	int ItemDur;
	int ItemDropRate;
	int MonsterMinLevel;
	int MonsterMaxLevel;
	int MonsterMap;
};

class ExWinQuestSystem
{
public:
	ExWinQuestSystem();
	//-> Load
	void Init();
	void Load();
	void Read(char* File);
	void ReadQuest(int a, char* File);
	//-> Game Comunication
	bool Dialog(int aIndex, int aNpcIndex);
	void GC_MainInfo(int aIndex, bool isOpenWindow);
	void GC_OpenWindow(int aIndex);
	void GC_MiniInfo(int aIndex);
	void GC_MiniInfoEx(int aIndex, int a, int m);
	void CG_AcceptQuest(int aIndex, CG_Accept_Done* aRecv);
	void GC_Start(int aIndex, int a, int s);
	//Mission
	bool MonsterKiller(int aIndexMonster, int aIndexUser);
	bool MonsterKill(int aIndexMonster, int aIndexUser);
	bool CheckMission(int aIndex, int a);
	void RewardQuest(int aIndex, int Act);
	void QuestChange(int aIndex, QUESTCHANGE_REQ* lpMsg);

	int Enable;
	int NPC_Class;
	int NPC_Map;
	int NPC_X;
	int NPC_Y;

	int ReloadLap;
	ExWinQuest_DATA ewdata[EW_MAX_ACT][EW_MAX_QUEST];
	ExWinDrop_DATA m_Drop[EW_MAX_DROP];

	int iLoaded[EW_MAX_ACT];
	int iDropLoaded;
};
extern ExWinQuestSystem g_ExWinQuestSystem;
