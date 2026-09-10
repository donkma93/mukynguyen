#pragma once
#include "DataServerProtocol.h"
#define MAXTOP 20

struct GDTop
{
	PSWMSG_HEAD h;
	BYTE Result;
};

struct CharTop
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

struct DGCharTop
{
	PSWMSG_HEAD h;
	CharTop	HA[MAXTOP];
};

struct GDTopTP
{
	PSWMSG_HEAD h;
	BYTE Result;
};

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
	PSWMSG_HEAD h;
	CharTopTP	HA[MAXTOP];
};

struct GDTopTG
{
	PSWMSG_HEAD h;
	BYTE Result;
};

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
	PSWMSG_HEAD h;
	CharTopTG	tp[MAXTOP];
};

struct GDTopPK
{
	PSWMSG_HEAD h;
	BYTE Result;
};

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
	PSWMSG_HEAD h;
	CharTopPK	tp[MAXTOP];
};

struct GDTopET
{
	PSWMSG_HEAD h;
	BYTE Result;
};

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
	PSWMSG_HEAD h;
	CharTopET	tp[MAXTOP];
};

struct GDTopMT
{
	PSWMSG_HEAD h;
	BYTE Result;
};

struct CharTopMT
{
	char Name[11];
	int cLevel;
	int cPoint;
	int cExp;
};

struct DGCharTopMT
{
	PSWMSG_HEAD h;
	CharTopMT	tp[MAXTOP];
};

struct GDTopWQ
{
	PSWMSG_HEAD h;
	BYTE Result;
};

struct CharTopWQ
{
	char Name[11];
	int Start;
	int Number;
	int CurQuest;
};

struct DGCharTopWQ
{
	PSWMSG_HEAD h;
	CharTopWQ tp[MAXTOP];
};

class RankingDataClass
{
	public:
	void CharacterRanking(GDTop* lpMsg, int pIndex);
	void CharacterRankingTP(GDTopTP* lpMsg, int pIndex);
	void CharacterRankingTG(GDTopTG* lpMsg, int pIndex);
	void CharacterRankingPK(GDTopPK* lpMsg, int pIndex);
	void CharacterRankingET(GDTopET* lpMsg, int pIndex);
	void CharacterRankingMT(GDTopMT* lpMsg, int pIndex);
	void CharacterRankingWQ(GDTopWQ* lpMsg, int pIndex);
};
extern RankingDataClass gRankingDataClass;