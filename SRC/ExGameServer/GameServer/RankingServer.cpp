#include "Stdafx.h"
#include "RankingServer.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Util.h"
#include "ItemManager.h"

cRanking gRanking;
void cRanking::SendDS()
{
	BYTE send[4096];
	GDTop pMsg{};
	pMsg.h.set(0x7A, 0x01, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}
void cRanking::RecvDS(DGCharTop* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingChar[i].Name,					  Recv->HA[i].Name, 11);
				this->RankingChar[i].Is_Class				= Recv->HA[i].Is_Class;
				this->RankingChar[i].Is_cLevel				= Recv->HA[i].Is_cLevel;
				this->RankingChar[i].Is_ResetCount			= Recv->HA[i].Is_ResetCount;
				this->RankingChar[i].Is_MasterResetCount	= Recv->HA[i].Is_MasterResetCount;
				this->RankingChar[i].Is_ResetTime			= Recv->HA[i].Is_ResetTime;
				this->RankingChar[i].Is_TotalPoint			= Recv->HA[i].Is_TotalPoint;
		strncpy(this->RankingChar[i].Is_SvSub,				  Recv->HA[i].Is_SvSub, 9);
				this->RankingChar[i].Is_VipStars			= Recv->HA[i].Is_VipStars;
	}
}
void cRanking::SendUser(LPOBJ lpObj)
{
	DGCharTop2	sClient{};
	sClient.h.set(0x04, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.HA[i].Name,					  this->RankingChar[i].Name, 11);
				sClient.HA[i].Is_Class				= this->RankingChar[i].Is_Class;
				sClient.HA[i].Is_cLevel				= this->RankingChar[i].Is_cLevel;
				sClient.HA[i].Is_ResetCount			= this->RankingChar[i].Is_ResetCount;
				sClient.HA[i].Is_MasterResetCount	= this->RankingChar[i].Is_MasterResetCount;
				sClient.HA[i].Is_ResetTime			= this->RankingChar[i].Is_ResetTime;
				sClient.HA[i].Is_TotalPoint			= this->RankingChar[i].Is_TotalPoint;
		strncpy(sClient.HA[i].Is_SvSub,				  this->RankingChar[i].Is_SvSub, 9);
				sClient.HA[i].Is_VipStars			= this->RankingChar[i].Is_VipStars;
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}

void cRanking::SendDSTP()
{
	BYTE send[4096];
	GDTopTP pMsg{};
	pMsg.h.set(0x7A, 0x02, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}
void cRanking::RecvDSTP(DGCharTopTP* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingCharTP[i].Name, Recv->HA[i].Name, 11);
		this->RankingCharTP[i].Is_Class = Recv->HA[i].Is_Class;
		this->RankingCharTP[i].Is_WcoinC = Recv->HA[i].Is_WcoinC;
		this->RankingCharTP[i].Is_WcoinP = Recv->HA[i].Is_WcoinP;
		this->RankingCharTP[i].Is_WcoinG = Recv->HA[i].Is_WcoinG;
		this->RankingCharTP[i].Is_WcoinR = Recv->HA[i].Is_WcoinR;
		this->RankingCharTP[i].Is_CoinAtm = Recv->HA[i].Is_CoinAtm;
	}
}
void cRanking::SendUserTP(LPOBJ lpObj)
{
	DGCharTop2TP	sClient{};
	sClient.h.set(0x05, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.HA[i].Name,			  this->RankingCharTP[i].Name, 11);
			    sClient.HA[i].Is_Class		= this->RankingCharTP[i].Is_Class;
			    sClient.HA[i].Is_WcoinC		= this->RankingCharTP[i].Is_WcoinC;
			    sClient.HA[i].Is_WcoinP		= this->RankingCharTP[i].Is_WcoinP;
			    sClient.HA[i].Is_WcoinG		= this->RankingCharTP[i].Is_WcoinG;
			    sClient.HA[i].Is_WcoinR		= this->RankingCharTP[i].Is_WcoinR;
			    sClient.HA[i].Is_CoinAtm	= this->RankingCharTP[i].Is_CoinAtm;

	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}

void cRanking::SendDSTG()
{
	BYTE send[4096];
	GDTopTG pMsg{};
	pMsg.h.set(0x7A, 0xF4, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}

void cRanking::RecvDSTG(DGCharTopTG* Recv)
{
	for (int i = 0; i < 20; i++)
	{
		strncpy(this->RankingCharTG[i].G_Master, Recv->HA[i].G_Master, 11);
		strncpy(this->RankingCharTG[i].G_Name, Recv->HA[i].G_Name, 11);

		this->RankingCharTG[i].Is_GLevel = Recv->HA[i].Is_GLevel;
		this->RankingCharTG[i].Is_GScore = Recv->HA[i].Is_GScore;
		this->RankingCharTG[i].Is_GMem = Recv->HA[i].Is_GMem;
		strncpy(this->RankingCharTG[i].Is_JoinDate, Recv->HA[i].Is_JoinDate, 20);
		this->RankingCharTG[i].KillBoss = Recv->HA[i].KillBoss;
		this->RankingCharTG[i].TotalReset = Recv->HA[i].TotalReset;
		this->RankingCharTG[i].TotalPoint = Recv->HA[i].TotalPoint;

		memcpy(this->RankingCharTG[i].GuildMark, Recv->HA[i].GuildMark, 32);

	}
}

void cRanking::SendUserTG(LPOBJ lpObj)
{
	DGCharTop2TG sClient;
	sClient.h.set(0x75, sizeof(sClient));
	for (int i = 0; i < 20; i++)
	{
		strncpy(sClient.HA[i].G_Master, this->RankingCharTG[i].G_Master, 11);
		strncpy(sClient.HA[i].G_Name, this->RankingCharTG[i].G_Name, 11);

		sClient.HA[i].Is_GLevel = this->RankingCharTG[i].Is_GLevel;
		sClient.HA[i].Is_GScore = this->RankingCharTG[i].Is_GScore;
		sClient.HA[i].Is_GMem = this->RankingCharTG[i].Is_GMem;
		strncpy(sClient.HA[i].Is_JoinDate, this->RankingCharTG[i].Is_JoinDate, 20);
		sClient.HA[i].KillBoss = this->RankingCharTG[i].KillBoss;
		sClient.HA[i].TotalReset = this->RankingCharTG[i].TotalReset;
		sClient.HA[i].TotalPoint = this->RankingCharTG[i].TotalPoint;

		memcpy(sClient.HA[i].GuildMark, this->RankingCharTG[i].GuildMark, 32);
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}


void cRanking::SendDSPK()
{
	BYTE send[4096];
	GDTopPK pMsg{};
	pMsg.h.set(0x7A, 0x05, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}
void cRanking::RecvDSPK(DGCharTopPK* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingCharPK[i].Name, Recv->tp[i].Name, 11);
		this->RankingCharPK[i].Is_Class = Recv->tp[i].Is_Class;
		this->RankingCharPK[i].Is_Kills = Recv->tp[i].Is_Kills;
		this->RankingCharPK[i].Is_Deads = Recv->tp[i].Is_Deads;
		this->RankingCharPK[i].Is_OnlineHours = Recv->tp[i].Is_OnlineHours;
		this->RankingCharPK[i].Is_Win = Recv->tp[i].Is_Win;
		this->RankingCharPK[i].Is_Lose = Recv->tp[i].Is_Lose;
		this->RankingCharPK[i].Is_Kboss = Recv->tp[i].Is_Kboss;
		strncpy(this->RankingCharPK[i].Is_JoinDate, Recv->tp[i].Is_JoinDate, 22);
	}
}
void cRanking::SendUserPK(LPOBJ lpObj)
{
	DGCharTop2PK	sClient{};
	sClient.h.set(0x08, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.tp[i].Name, this->RankingCharPK[i].Name, 11);
		sClient.tp[i].Is_Class = this->RankingCharPK[i].Is_Class;
		sClient.tp[i].Is_Kills = this->RankingCharPK[i].Is_Kills;
		sClient.tp[i].Is_Deads = this->RankingCharPK[i].Is_Deads;
		sClient.tp[i].Is_OnlineHours = this->RankingCharPK[i].Is_OnlineHours;
		sClient.tp[i].Is_Win = this->RankingCharPK[i].Is_Win;
		sClient.tp[i].Is_Lose = this->RankingCharPK[i].Is_Lose;
		sClient.tp[i].Is_Kboss = this->RankingCharPK[i].Is_Kboss;
		strncpy(sClient.tp[i].Is_JoinDate, this->RankingCharPK[i].Is_JoinDate, 22);
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}

void cRanking::SendDSET()
{
	BYTE send[4096];
	GDTopET pMsg;
	pMsg.h.set(0x7A, 0x03, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}
void cRanking::RecvDSET(DGCharTopET* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingCharET[i].Name, Recv->tp[i].Name, 11);
		this->RankingCharET[i].Is_Class = Recv->tp[i].Is_Class;
		this->RankingCharET[i].Is_BloodCS = Recv->tp[i].Is_BloodCS;
		this->RankingCharET[i].Is_ChaosCS = Recv->tp[i].Is_ChaosCS;
		this->RankingCharET[i].Is_DevilCS = Recv->tp[i].Is_DevilCS;
	}
}
void cRanking::SendUserET(LPOBJ lpObj)
{
	DGCharTop2ET	sClient;
	sClient.h.set(0x06, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.tp[i].Name, this->RankingCharET[i].Name, 11);
		sClient.tp[i].Is_Class = this->RankingCharET[i].Is_Class;
		sClient.tp[i].Is_BloodCS = this->RankingCharET[i].Is_BloodCS;
		sClient.tp[i].Is_ChaosCS = this->RankingCharET[i].Is_ChaosCS;
		sClient.tp[i].Is_DevilCS = this->RankingCharET[i].Is_DevilCS;
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}

void cRanking::SendDSMT()
{
	BYTE send[4096];
	GDTopMT pMsg;
	pMsg.h.set(0x7A, 0x06, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	gDataServerConnection.DataSend(send, size);
}
void cRanking::RecvDSMT(DGCharTopMT* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingCharMT[i].Name, Recv->tp[i].Name, 11);
		this->RankingCharMT[i].cLevel = Recv->tp[i].cLevel;
		this->RankingCharMT[i].cPoint = Recv->tp[i].cPoint;
		this->RankingCharMT[i].cExp = Recv->tp[i].cExp;
	}
}
void cRanking::SendUserMT(LPOBJ lpObj)
{
	DGCharTop2MT	sClient{};
	sClient.h.set(0x09, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.tp[i].Name, this->RankingCharMT[i].Name, 11);
		sClient.tp[i].cLevel = this->RankingCharMT[i].cLevel;
		sClient.tp[i].cPoint = this->RankingCharMT[i].cPoint;
		sClient.tp[i].cExp = this->RankingCharMT[i].cExp;
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}

void cRanking::SendDSWQ()
{
	BYTE send[4096];
	GDTopWQ pMsg{};
	pMsg.h.set(0x7A, 0x07, 0);
	int size = sizeof(pMsg);
	pMsg.Result = 1;
	pMsg.h.size[0] = SET_NUMBERHB(size);
	pMsg.h.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, size);
	gDataServerConnection.DataSend(send, size);
}

void cRanking::RecvDSWQ(DGCharTopWQ* Recv)
{
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(this->RankingCharWQ[i].Name, Recv->tp[i].Name, 11);
		this->RankingCharWQ[i].Start = Recv->tp[i].Start;
		this->RankingCharWQ[i].Number = Recv->tp[i].Number;
		this->RankingCharWQ[i].CurQuest = Recv->tp[i].CurQuest;
	}
}

void cRanking::SendUserWQ(LPOBJ lpObj)
{
	DGCharTop2WQ sClient{};
	sClient.h.set(0x0A, sizeof(sClient));
	for (int i = 0; i < MAXTOP; i++)
	{
		strncpy(sClient.tp[i].Name, this->RankingCharWQ[i].Name, 11);
		sClient.tp[i].Start = this->RankingCharWQ[i].Start;
		sClient.tp[i].Number = this->RankingCharWQ[i].Number;
		sClient.tp[i].CurQuest = this->RankingCharWQ[i].CurQuest;
	}
	DataSend(lpObj->Index, (BYTE*)&sClient, sizeof(sClient));
}
