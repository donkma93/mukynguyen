#pragma once

#include "User.h"
#include "Protocol.h"

#define MAX_CTC_GUILD 20
#define MAX_CTC_CHAR 100

struct CTCMINI_CGPACKET
{
	PSBMSG_HEAD header;
	BYTE CongVao;
};

struct INFOCTCMINI_SENDCLIENT
{
	PSBMSG_HEAD header;
	BYTE OpenWindow;
	BYTE GuildWinOld;
	int ValueCoin[3];
	char GuildNameOld[9];
};

struct CTCMINIKILL_SENDCLIENT
{
	PSBMSG_HEAD header;
	int	TimeCTCMini;
	BYTE Cong[3];
	BYTE Tru[3];
	char GNameTru1[9];
	char GNameTru2[9];
	char GNameTru3[9];
};

enum eCTCState
{
	CTC_STATE_BLANK = 0,
	CTC_STATE_EMPTY = 1,
	CTC_STATE_START = 2,
};

struct CTC_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct SDHP_RANKING_CTC_GUILD_SAVE_SEND
{
	PBMSG_HEAD header; // C1:74
	char name[9];
	DWORD score;
};

struct SDHP_RANKING_CTC_PLAYER_SAVE_SEND
{
	PBMSG_HEAD header; // C1:75
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};


struct CTC_GUILD
{
	void Reset()
	{
		this->Number = -1;
		this->Owner = 0; //Chiem tru
		ZeroMemory(GuildOwner, sizeof(GuildOwner));
	}

	int Number;
	int Owner;
	char GuildOwner[9];
};

struct CTC_CHAR
{
	void Reset()
	{
		memset(this->Name, 0, sizeof(this->Name));
		this->Index = -1;
		this->CongVao = 0;
	}

	int Index;
	char Name[11];
	int CongVao;
};

struct BONUS_POINT
{
	int WCoin;
	int WCoinP;
	int GobinP;
	int Ruud;
	int IndexBonus;
};

class CastleStartGuild
{
	public:
	CastleStartGuild();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_START();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_START();
	void CheckSync();
	void Clear();
	bool Dialog(int aIndex, int aNpcIndex);
	bool CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget, int Type);
	void AddGuild(int gIndex);
	bool GetGuild(int gIndex);
	void SetChar(int cIndex, int CongVao);
	void AddChar(int cIndex, int CongVao);
	int GetCongVao(int cIndex);
	bool GetChar(int cIndex);
	void CheckChar();
	void SetGuildTop();

	int m_Active;
	void StartCTCMini();
	void TestCTCMini();
	bool MonsterDie(int aIndexMonster, int aIndexUser);

	int m_Enabled;

	BONUS_POINT b_PhaTru;
	BONUS_POINT b_PhaCong;
	BONUS_POINT b_GuildWin;
	int GateMove[5];
	bool GetUserRespawnLocation(LPOBJ lpObj, int* gate, int* map, int* x, int* y, int* dir, int* level);
	void CGPacketCTCMini(CTCMINI_CGPACKET* aRecv, int aIndex);
	char WinnerNameOLD[9];
	void SendKillCTCMini(int aIndex, int Type);
	int YeuCauCoin[3];

	private:
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	int m_WarningTime;
	int m_EventTime;
	int MinutesLeft;
	int WinnerNumber;
	int WinnerTime;
	char WinnerName[9];

	int m_RemainTimeWinner;

	std::vector<CTC_START_TIME> m_CTCMinitartTime;
	CTC_GUILD Guild[MAX_CTC_GUILD];
	CTC_CHAR Char[MAX_CTC_CHAR];

	void SetEntranceZone();
	void DelEntranceZone();
	void XoaTruChiDinh(BYTE n);
	void ThemTruGuild(int n, int GuidNumber);
	int SoTruDaChiem(int GuildNumber, int aIndex);
	int IndexCong[3];
	int IndexTru[3];

	int Class;
	int Map;
	int X;
	int Y;
	int GuildMinPlayer;

	int ClassTru;
	int ClassGate;
};

extern CastleStartGuild gCastleStartGuild;

// Tower erase positions (X1, Y1, X2, Y2)
inline constexpr BYTE XoaTru[3][4] =
{
	{  96, 165,  96, 165 }, // Tower 1
	{ 128, 165, 128, 165 }, // Tower 2
	{ 159, 165, 159, 165 }  // Tower 3
};

// Tower spawn positions (X, Y)
inline constexpr BYTE ThemTru[3][2] =
{
	{  96, 165 }, // Tower 1
	{ 128, 165 }, // Tower 2
	{ 159, 165 }  // Tower 3
};

// Gate erase positions (X1, Y1, X2, Y2)
inline constexpr BYTE XoaCong[3][4] =
{
	{  94, 131,  98, 133 }, // Gate 1
	{ 126, 131, 130, 133 }, // Gate 2
	{ 159, 131, 163, 133 }  // Gate 3
};

// Gate spawn positions (X, Y)
inline constexpr BYTE ThemCong[3][2] =
{
	{  96, 131 }, // Gate 1
	{ 128, 131 }, // Gate 2
	{ 161, 131 }  // Gate 3
};

// Entry zone positions to clear (X1, Y1, X2, Y2) — attribute = 16
inline constexpr BYTE XoaDuongVao[5][4] =
{
	{  79,  31,  85,  33 }, // Entry path 1
	{ 108,  31, 114,  33 }, // Entry path 2
	{ 138,  31, 144, 332 }, // Entry path 3
	{ 171,  31, 176,  33 }, // Entry path 4
	{ 122, 231, 136, 233 }  // Entry path 5
};
