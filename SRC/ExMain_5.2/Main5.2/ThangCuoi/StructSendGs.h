#pragma once
#include "WSClient.h"
#include "ZzzToolKit.h"
//#include "NewUISystem.h"
#define MAX_COIN_POINT 5
#define MAXTOP 20
#define MAX_ITEM_SLOT 10
#define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))

typedef struct
{
	char      ID[32];
	char      Union[30];
	char      Guild[30];
	char      szShopTitle[16];
	char      Color;
	char      GuildColor;
	int       IDLifeTime;
	char      Text[2][256];
	int       LifeTime[2];
	CHARACTER* Owner;
	int       x, y;
	int       Width;
	int       Height;
	vec3_t    Position;
} CHAT;

struct CharTopAll
{
	char Name[11];
	BYTE Is_Class;
	int Is_cLevel;
	int Is_ResetCount;
	int Is_MasterResetCount;
	int Is_ResetTime;
	int Is_TotalPoint;
	char Is_SvSub[11];
	int Is_VipStars;
};
struct DGCharTopAll
{
	PWMSG_HEAD Header;
	CharTopAll HA[MAXTOP];
};
//------------------
struct CharTopTP
{
	char Name[11];
	BYTE Is_Class;
	int Is_WcoinC;
	int Is_WcoinP;
	int Is_WcoinG;
	int Is_WcoinR;
	int Is_CoinAtm;
};
struct DGCharTopTP
{
	PWMSG_HEAD h;
	CharTopTP	HA[MAXTOP];
};
//------------------
 struct CharTopTG
{
	char G_Name[11];
	char G_Master[11];
	int Is_GLevel;
	int Is_GScore;
	int Is_GMem;
	char Is_JoinDate[20];
	int KillBoss;
	int TotalReset;
	int TotalPoint;
	BYTE GuildMark[32];
};

struct DGCharTopTG
{
	PWMSG_HEAD h;
	CharTopTG	HA[MAXTOP];
};
//------------------
struct CharTopPK
{
	char Name[11];
	BYTE Is_Class;
	int Is_Kills;
	int Is_Deads;
	int Is_OnlineHours;
	int Is_Win;
	int Is_Lose;
	int Is_Kboss;
	char Is_JoinDate[22];
};

struct DGCharTopPK
{
	PWMSG_HEAD h;
	CharTopPK	HA[MAXTOP];
};
//------------------
struct CharTopET
{
	char Name[11];
	BYTE Is_Class;
	int Is_BloodCS;
	int Is_ChaosCS;
	int Is_DevilCS;
};
struct DGCharTopET
{
	PWMSG_HEAD h;
	CharTopET	HA[MAXTOP];
};
//------------------
struct CharTopMT
{
	char Name[11];
	int cLevel;
	int cPoint;
	int cExp;
};

struct DGCharTopMT
{
	PWMSG_HEAD h;
	CharTopMT	HA[MAXTOP];
};
//------------------

struct CharTopWQ
{
	char Name[11];
	int Start;
	int Number;
	int CurQuest;
};

struct DGCharTopWQ
{
	PWMSG_HEAD h;
	CharTopWQ tp[MAXTOP];
};


class Ranking
{
	public:
	void GsSendMain(DGCharTopAll* lpMsg);
	CharTopAll RankingChar[MAXTOP];

	void RecvInfoTP(DGCharTopTP* lpMsg);
	CharTopTP RankingCharTP[MAXTOP];

	void RecvInfoTG(DGCharTopTG* lpMsg);
	CharTopTG RankingCharTG[MAXTOP];

	void RecvInfoPK(DGCharTopPK* lpMsg);
	CharTopPK RankingCharPK[MAXTOP];

	void RecvInfoET(DGCharTopET* lpMsg);
	CharTopET RankingCharET[MAXTOP];

	void RecvInfoMT(DGCharTopMT* lpMsg);
	CharTopMT RankingCharMT[MAXTOP];

	void RecvInfoWQ(DGCharTopWQ* lpMsg);
	CharTopWQ RankingCharWQ[MAXTOP];

};
extern Ranking gRanking;

extern char* CharacterCode(int a);

struct CUSTOM_JEWELBANK_DATA
{
	int ItemBank[MAX_ITEM_SLOT];
};
struct PSBMSG_JEWELBANK_RECV
{
	PSBMSG_HEAD h;

	int ItemBank[MAX_ITEM_SLOT];
	BYTE JewelEnable;
	BYTE JewelMaxCount;
};
struct PSBMSG_JEWELBANKWITHDRAW_SEND
{
	PSBMSG_HEAD header;
	int type;
	int count;
};

struct PMSG_TICKET_SEND
{
	PSBMSG_HEAD header;
};

//
struct PMSG_NEW_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:E0
	WORD Level;
	DWORD LevelUpPoint;//
	DWORD Experience;
	DWORD NextExperience;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Leadership;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	DWORD Shield;
	DWORD MaxShield;
	short FruitAddPoint;
	short MaxFruitAddPoint;
	DWORD FruitSubPoint;
	DWORD MaxFruitSubPoint;
	//EXTRA
	DWORD ViewReset;
	DWORD ViewMReset;
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewLeadership;
	DWORD ViewMaxReset;
	BYTE InventoryExtensions;
	DWORD ViewAddPoint[5];
	WORD ManaClassReg[MAX_CLASS_MAGIC];
};

struct PMSG_NEW_CHARACTER_CALC_SEND
{
	PSBMSG_HEAD header; // C1:F3:E1
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	DWORD ViewAddStrength;
	DWORD ViewAddDexterity;
	DWORD ViewAddVitality;
	DWORD ViewAddEnergy;
	DWORD ViewAddLeadership;
	DWORD Experience;
	DWORD NextExperience;

	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Leadership;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	DWORD Shield;
	DWORD MaxShield;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewLeadership;	
	BYTE InventoryExtensions;
	DWORD ViewPhysiSpeed;
	DWORD ViewMagicSpeed;
	DWORD ViewPhysiDamageMin;
	DWORD ViewPhysiDamageMax;
	DWORD ViewMagicDamageMin;
	DWORD ViewMagicDamageMax;
	DWORD ViewCurseDamageMin;
	DWORD ViewCurseDamageMax;
	DWORD ViewMulPhysiDamage;
	DWORD ViewDivPhysiDamage;
	DWORD ViewMulMagicDamage;
	DWORD ViewDivMagicDamage;
	DWORD ViewMulCurseDamage;
	DWORD ViewDivCurseDamage;
	DWORD ViewMagicDamageRate;
	DWORD ViewCurseDamageRate;
	//DWORD ViewAttackSuccessRate;
	//DWORD ViewAttackSuccessRatePvP;
	DWORD ViewDefense;
	//DWORD ViewDefenseSuccessRate;
	//DWORD ViewDefenseSuccessRatePvP;
	DWORD ViewDamageMultiplier;
	DWORD ViewRFDamageMultiplierA;
	DWORD ViewRFDamageMultiplierB;
	DWORD ViewRFDamageMultiplierC;
	DWORD ViewDarkSpiritAttackDamageMin;
	DWORD ViewDarkSpiritAttackDamageMax;
	DWORD ViewDarkSpiritAttackSpeed;
	DWORD ViewDarkSpiritAttackSuccessRate;
};

struct GetReceivedCoinData
{
	PSBMSG_HEAD header; // From GS
	int  ThisCoin[MAX_COIN_POINT];
};

class GetCoinReceived
{
	public:
	int  ThisCoin[MAX_COIN_POINT]; // SaveToMain
};
extern GetCoinReceived pGetCoin;
void GCRecvCoin(const BYTE* lpMsg);

struct GetValueSpeedAttack
{
	PSBMSG_HEAD header;
	BYTE  ThisValue[14];
};

class GetValueSpeedAttackClass
{
	public:
	BYTE  ThisValue[14];
};
extern GetValueSpeedAttackClass pGetSPeedClass;
void GCRecvSpeedAttack(const BYTE* lpMsg);

struct PMSG_CUSTOM_RANKUSER
{
	PSBMSG_HEAD h;
	char szName[20];
	int iIndex;
	int iLevel;
	int rDanhHieu;
	int rQuanHam;
	int rTuLuyen;
	int rHonHoan;
	BYTE AccountType;
	int ReqResetChange[5];
	int ReqResetCoin[5];
	int ReqResetUpPoint[5];
};
struct CUSTOM_RANKUSER_DATA
{
	char szName[20];
	int m_Index;
	WORD m_Level;
	int rDanhHieu;
	int rQuanHam;
	int rTuLuyen;
	int rHonHoan;
	BYTE AccountType;
};
class RankUserClass
{
	public:
	void GCReqRankLevelUser(const BYTE* SPK);
	void RenderShowLogRank(int index, int PosX, int PosY);
	void RenderBarName();

	int ReqResetChange[5];
	int ReqResetCoin[5];
	int ReqResetUpPoint[5];

	POINT m_Pos;

	CUSTOM_RANKUSER_DATA m_Data[400];
};
extern RankUserClass gRank;


struct SolvedDameBoss
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};

struct CountListDanhHieu
{
	PSWMSG_HEAD header;
	int Count;
};

struct GetDataDanhHieu
{
	int AllData[11];
};

class cDanhHieu
{
	public:
	void SendStartLoad();
	void SendPackButton();
	void SendWsClient(BYTE* lpMsg);
	void Render(int a, int b);
	GetDataDanhHieu* IsCurrentLevel;
	GetDataDanhHieu* IsNextLevel;
	BYTE IsData[4];
	std::map<int, GetDataDanhHieu> m_LoadData;
	POINT SetPos;
};
extern cDanhHieu gDanhHieu;


struct CountListQuanHam
{
	PSWMSG_HEAD header;
	int Count;
};

struct GetDataQuanHam
{
	int AllData[11];
};


class cQuanHam
{
public:
	void SendStartLoad();
	void SendPackButton();
	void SendWsClient(BYTE* lpMsg);
	void Render(int a, int b);
	GetDataQuanHam* IsCurrentLevel;
	GetDataQuanHam* IsNextLevel;
	BYTE IsData[4];
	std::map<int, GetDataQuanHam> m_LoadData;
	POINT SetPos;
};
extern cQuanHam gQuanHam;



struct CountListTuLuyen
{
	PSWMSG_HEAD header;
	int Count;
};

struct GetDataTuLuyen
{
	int AllData[11];
};


class cTuLuyen
{
public:
	void SendStartLoad();
	void SendPackButton();
	void SendWsClient(BYTE* lpMsg);
	void Render(int a, int b);
	GetDataTuLuyen* IsCurrentLevel;
	GetDataTuLuyen* IsNextLevel;
	BYTE IsData[4];
	std::map<int, GetDataTuLuyen> m_LoadData;
	POINT SetPos;
};
extern cTuLuyen gTuLuyen;


struct CountListHonHoan
{
	PSWMSG_HEAD header;
	int Count;
};

struct GetDataHonHoan
{
	int AllData[11];
};

struct HonHoanRenderCache
{
	int CharacterKey = -1;
	vec3_t LastPosition = { 0.f, 0.f, 0.f };
	int TextureID = -1;
	bool IsActive = false;
};

class cHonHoan
{
	public:
	void SendWsClient(BYTE* lpMsg);
	
	GetDataHonHoan* IsCurrentLevel;
	GetDataHonHoan* IsNextLevel;
	BYTE IsData[4];
	std::map<int, GetDataHonHoan> m_LoadData;
	POINT SetPos;
};
extern cHonHoan gHonHoan;

class CustomJewelBank
{
public:
	BYTE JewelEnable;
	BYTE JewelMaxCount;
	DWORD StatusTick;
	CUSTOM_JEWELBANK_DATA m_Data;
	int ItemBank[MAX_ITEM_SLOT];
	void GCCustomBankInfoRecv(const BYTE* lpMsg);

	POINT MaxPosition;
	POINT CurPosition;
	bool  DragItem;
	DWORD ActionTime;
};
extern CustomJewelBank gJwBanking;

extern const char* ItemNameBank[20];

class cResetChange
{
	public:
	POINT MaxPosition;
	POINT CurPosition;
	bool  DragItem;
	DWORD ActionTime;
}; extern cResetChange gResetChange;


struct ClassLifeData
{
	WORD index;
	BYTE type;
	BYTE rate;
	BYTE rate2;
	short Level;
	float Life;
	WORD MonsID;
};

struct RecvLifeCount
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};

struct RecvLifeData
{
	WORD index;
	BYTE type;
	BYTE rate;
	BYTE rate2;
	short Level;
	float Life;
	WORD MonsID;
};

class ClassLifeBar
{
public:
	void Clear();
	void Insert(WORD index, BYTE type, BYTE rate, BYTE rate2, short Level, float Life, WORD MonsID);
	ClassLifeData* Get(WORD index, BYTE type);
	void Receive(const BYTE* lpMsg);
	void Render();

	private:
	ClassLifeData gNewHealthBar[MAX_CHARACTERS_CLIENT];
};
extern ClassLifeBar gClassLifeBar;

struct GetValueFromGs
{
	WORD monsterid;
	int Level;
	float MinLife;
	float MaxLife;
	char szName[11];
	SPK Kill;
	BYTE Rate;
};

class CNewUIRenderDameBossExt
{
	public:
	WORD IndexMonster;
	int Level;
	float MinLife;
	float MaxLife;
	BYTE Rate;
	int CalcString;
	void DamageGetInfo(const BYTE* lpMsg);
	void SendProtocolClient(BYTE* ReceiveBuffer);
	GetValueFromGs SavedValue[10];
	void DamageClear();
	bool CheckDrawBxhDame;
	BYTE ADD_OFF_ANTOAN;
	void Initz();
	POINT m_PosDmg;
	//private:
	bool SetWindows();
	POINT MaxPosition;
};
extern CNewUIRenderDameBossExt gDmgBoss;


struct InvasionDataReceived
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE Counter;
};

struct InvasionsData
{
	InvasionsData(int Counter, int MaxCounter) : IsCount(Counter), IsMaxCount(MaxCounter) {	}
	InvasionsData() : IsCount(0), IsMaxCount(0) {}
	int IsCount;
	int IsMaxCount;
};

struct InvasionDataActive
{
	int MonsterIndex;
	InvasionsData Counter;
};

struct InvasionMonsterReceived
{
	PSBMSG_HEAD header; // C2:F3:E2
	int MonsterIndex;
	int Counter;
};

struct InvasionActive
{
	int MonsterIndex;
	InvasionsData Counter;
};

#define MAXSLOTLIST 1

class CNewUIRenderInvasionExt
{
	public:
	void GetInfoReceived(const BYTE* lpMsg);
	void GetUpdateMonster(const BYTE* lpMsg);
	int Count[MAXSLOTLIST];
	int ListPage[MAXSLOTLIST];
	int CurPos[MAXSLOTLIST];
	int DataList[MAXSLOTLIST];
	bool UpdateMaxPos[MAXSLOTLIST];
	int ListMaxPer[MAXSLOTLIST];
	void Initz();
	POINT m_Pos;

	std::vector <InvasionActive> m_Data;
	bool is_move;

}; extern CNewUIRenderInvasionExt gInvasionExt;


struct CG_OFFMODE_RESULT
{
	PBMSG_HEADX h;
};

struct cCustomSpinGetData
{
	PSBMSG_HEAD h;
	int ItemBmd[12];
	BYTE ItemLvl[12];
	BYTE ItemExc[12];
	int ItemTime;
	int ThisCoin;
};

struct cCustomSpinDataUnit
{
	PSBMSG_HEAD h;
	bool result;
};


struct cCustomSpinDataUpdate
{
	PSBMSG_HEAD header;
	int	RollNumber;
};

class cCustomSpin
{
	public:
	void Render3DItem(float xs, float ys) const;
	void LuckySpinRoll();
	void GCLuckySpinInit(const BYTE* Protocol);
	void GCItemSend(const BYTE* Protocol);
	void LuckySpinData(const BYTE* Protocol);

	DWORD StartRoll;
	int RollNumber;
	int ThisCoin;
	BYTE RandThis;

	int ItemBmd[12];
	BYTE ItemLvl[12];
	BYTE ItemExc[12];
};
extern cCustomSpin gCustomSpin;

struct RecvPreviewCharListCount
{
	PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
};

struct RecvPreviewCharlistItem
{
	char Name[10];
	WORD WingIndex;
};

struct RecvPreviewCharlistPort
{
	PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
	BYTE Viewport;
};

struct RecvPreviewCharSetPort
{
	char Name[11];
	WORD WingIndex;
	WORD index;
};

struct PMSG_MASTER_SKILL_LIST_SEND
{
	PWMSG_HEADER header; // C2:F3:E2
	BYTE subcode;
	DWORD count;
};

struct PMSG_MASTER_SKILL_LIST
{
	BYTE skill;
	BYTE level;
	float MainValue;
	float NextValue;
};

class AccountLevelID
{
	public:
	BYTE GetLevel;
};
extern AccountLevelID gGetID;

class cMuOffHelper
{
public:
	bool m_Disconnect;
};
extern cMuOffHelper gMuHelper;


struct SendOffTradeType
{
	PSBMSG_HEAD header;
	int Type;
};

struct RecvOffTradeType
{
	PSBMSG_HEAD header;
	int Type;
};

struct LoadOffTradeData
{
	PSBMSG_HEAD header;
	int Active;
	int Type;
};

class cOffStore
{
public:
	void RecvPShop(const BYTE* Data);
	void PShopActiveRecv(const BYTE* Data);

	int TypeShop;
	int ShopActive;
	int ShopList;
	bool StartCheck;
};
extern cOffStore gOffStore;

struct SendRequestAction
{
	PSBMSG_HEAD header;
	DWORD Action;
};
class ViewCharItem
{
	public:
	void RecvProtocol(BYTE* Recv);
	void ClearData();
	void SendRequestViewItem(int aIndex);

	ITEM CharItemInfo[MAX_EQUIPMENT];

	struct RecvViewItemData
	{
		PSWMSG_HEAD header;
		int Count;
		int pPlayerInfo[2];
		//char Name[11];
	};
	struct RecvInfoItemData
	{
		BYTE Dur;
		BYTE Item[12];
		int  PeriodTime;
	};
	std::vector<RecvInfoItemData> m_CharItemInfo;

	char CharViewName[11];
	int pPlayerInfo[2];
};
extern ViewCharItem gViewCharItem;

void GCNewCharacterInfoRecv(const BYTE* lpMsg);
void GCNewCharacterCalcSend(const BYTE* lpMsg);


struct WsClientMocNap
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	int NhanMocNap;
	int TongNap;
};

struct ListMocNapSend
{
	int IndexMocNap;
	int GiaTriNap;
};
struct ClientListItemSend
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	int WC;
	int WP;
	int GP;
	int Ruud;
	int CoinAtm;
};
struct InfoListMocNapSend
{
	float SizeBMD;
	int Count;
	short Index;
	BYTE GetTypeItem;
	BYTE Dur;
	BYTE Item[12];
	int  PeriodTime;
};
//==Struct Client
struct InfoItemLocalSend
{
	float SizeBMD;
	int Count;
	short Index;
	BYTE GetTypeItem;
	ITEM Item;
};
struct ItemListClienRecvSend
{
	int WC;
	int WP;
	int GP;
	int Ruud;
	int CoinAtm;
	std::vector<InfoItemLocalSend> g_listmocnap;

	void Clear()
	{
		WC = 0;
		WP = 0;
		GP = 0;
		Ruud = 0;
		CoinAtm = 0;
		g_listmocnap.clear();
	}
};
struct InfoClientRecvSendData
{
	int NhanMocNap;
	int TongNap;
	std::vector<ListMocNapSend> DanhSachMocNap;
	void Clear()
	{
		NhanMocNap = 0;
		TongNap = 0;
		DanhSachMocNap.clear();
	}
};
extern InfoClientRecvSendData g_dataclient;
extern ItemListClienRecvSend g_mocapclient;
void WsClientSend9A(const BYTE* SPK);
void WsClientSend9B(BYTE* SPK);
void IsVisibleWindow();
void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg);

struct SPK_SHARED_INFO
{
	char AccountID[11];
	char CharacterName[11];
	char ServerIP[32];
	HWND hWnd;
	char ComputerName[64];
	char OSName[64];
	char OSVersion[16];
	char InstallDate[32];
	char SystemArch[16];
	char TotalRAM[32];
	char GPUName[48];
	char GPUVRAM[32];
};

extern "C" __declspec(dllexport) SPK_SHARED_INFO g_SPKSharedInfo;

#if WinQuestTurn
#define EW_MAX_ACT			1
#define EW_MAX_MISSION		5
#define EW_MAX_REWARD		4
#define EW_MAX_QUEST		1000

enum ExWinMission
{
	Hunting = 1,
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
	WORD ExWQuestCurrent;
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

struct CG_Accept_Done
{
	PSBMSG_HEAD h;
	BYTE Result;
};
// ----------------------------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------------------------

struct GC_ExWinMiniInfo
{
	PSWMSG_HEAD h;
	WinQuest_Mission exQuest[EW_MAX_ACT];
};
// ----------------------------------------------------------------------------------------------

struct GC_ExWinMiniInfoEx
{
	PSBMSG_HEAD h;
	BYTE a;
	BYTE m;
};

struct GC_ExWinStart
{
	PSBMSG_HEAD h;
	BYTE Act;
	BYTE Start;
};

struct ExWinQuest_DATA
{
	char NameQuest[25];
	BYTE b_accept;
	BYTE b_done;
	BYTE b_End;
	BYTE Start;
	WORD Number;
	WORD Count[EW_MAX_MISSION];
	BYTE b_Level;
	WORD NeedLevel;
	BYTE b_Reset;
	WORD NeedReset;
	BYTE b_Grand;
	WORD NeedGrand;
	BYTE b_Mission[EW_MAX_MISSION];
	BYTE MissionType[EW_MAX_MISSION];
	WORD MissionID[EW_MAX_MISSION];
	BYTE MissionLevel[EW_MAX_MISSION];
	WORD MissionCount[EW_MAX_MISSION];
	WORD RewardType[EW_MAX_REWARD];
	DWORD RewardCount[EW_MAX_REWARD];
	WORD RewardItemType[EW_MAX_REWARD];
	WORD RewardItemIndex[EW_MAX_REWARD];
	WORD RewardItemLevel[EW_MAX_REWARD];
	WORD ExWQuestCurrent;
};

struct QUESTCHANGE_REQ
{
	PSBMSG_HEAD h;
	int	numchange;
};
struct PMSG_QUEST_SEND
{
	PBMSG_HEADX h;
};
class ExWinQuestSystem
{
	public:
	void Load();
	void Draw(int x, int y);
	void DrawMiniInfo();

	void GC_RecvMain(GC_MainPacket* aRecv);
	void CG_AcceptQuest();
	void GC_MiniInfoRecv(GC_ExWinMiniInfo* aRecv);
	void GC_MiniInfoRecvEx(GC_ExWinMiniInfoEx* aRecv);
	void GC_Start(GC_ExWinStart* aRecv);

	void QuestChange(int numchange);

	int OpenTab;
	int DrawMiniInfoFlag;
	int CurrentQuestIndex;
	bool Finish;
	POINT m_IsPos;
	ExWinQuest_DATA ewdata[EW_MAX_ACT];
	WinQuest_Table m_QuestDataCache[EW_MAX_ACT][EW_MAX_QUEST];

};
extern ExWinQuestSystem g_ExWinQuestSystem;
#endif

struct CTCMINI_CGPACKET
{
	PSBMSG_HEAD header;
	BYTE CongVao;

};

struct EventCastleSendClient
{
	PSBMSG_HEAD header;
	BYTE OpenWindow;
	BYTE GuildWinOld;
	int ValueCoin[3];
	char GuildNameOld[9];
};

struct EventCastleKillSendClient
{
	PSBMSG_HEAD header;
	int	EventCurrentTime;
	BYTE EventGate[3];
	BYTE EventClassMonster[3];
};

void GCCTCMiniShowWindowRecv(const BYTE* Data);
void GCCTCMiniKillRecv(const BYTE* Data);