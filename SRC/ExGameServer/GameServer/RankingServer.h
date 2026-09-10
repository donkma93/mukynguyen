#pragma once
#include "protocol.h"

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
	CharTop HA[MAXTOP];
};

struct DGCharTop2
{
	PWMSG_HEAD h;
	CharTop	HA[MAXTOP];
};
//------------------
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
	CharTopTP HA[MAXTOP];
};

struct DGCharTop2TP
{
	PWMSG_HEAD h;
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
	CharTopTG HA[MAXTOP];
};

struct DGCharTop2TG
{
	PWMSG_HEAD h;
	CharTopTG	HA[MAXTOP];
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
	CharTopPK tp[MAXTOP];
};

struct DGCharTop2PK
{
	PWMSG_HEAD h;
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
	CharTopET tp[MAXTOP];
};

struct DGCharTop2ET
{
	PWMSG_HEAD h;
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
	CharTopMT tp[MAXTOP];
};

struct DGCharTop2MT
{
	PWMSG_HEAD h;
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

struct DGCharTop2WQ
{
	PWMSG_HEAD h;
	CharTopWQ tp[MAXTOP];
};

class cRanking
{
	public:
	void SendDS();
	void RecvDS(DGCharTop* Recv);
	void SendUser(LPOBJ lpObj);
	CharTop RankingChar[MAXTOP];

	void SendDSTP();
	void RecvDSTP(DGCharTopTP* Recv);
	void SendUserTP(LPOBJ lpObj);
	CharTopTP RankingCharTP[MAXTOP];

	void SendDSTG();
	void RecvDSTG(DGCharTopTG* Recv);
	void SendUserTG(LPOBJ lpObj);
	CharTopTG RankingCharTG[MAXTOP];

	void SendDSPK();
	void RecvDSPK(DGCharTopPK* Recv);
	void SendUserPK(LPOBJ lpObj);
	CharTopPK RankingCharPK[MAXTOP];

	void SendDSET();
	void RecvDSET(DGCharTopET* Recv);
	void SendUserET(LPOBJ lpObj);
	CharTopET RankingCharET[MAXTOP];

	void SendDSMT();
	void RecvDSMT(DGCharTopMT* Recv);
	void SendUserMT(LPOBJ lpObj);
	CharTopMT RankingCharMT[MAXTOP];

	void SendDSWQ();
	void RecvDSWQ(DGCharTopWQ* Recv);
	void SendUserWQ(LPOBJ lpObj);
	CharTopWQ RankingCharWQ[MAXTOP];

};

extern cRanking gRanking;
