#include "stdafx.h"
#include "RankingData.h"
#include "SocketManager.h"
#include "QueryManager.h"
#include "Util.h"

RankingDataClass gRankingDataClass;

void RankingDataClass::CharacterRanking(GDTop* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTop rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x01, sizeof(DGCharTop));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_ALL") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}

		char NickName[11] = { 0 };
		char JoinDate[22] = { 0 };
		char LastIP[16] = { 0 };
		char SvSub[9] = { 0 };

		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.HA[iCharCount].Name, NickName, sizeof(NickName));

		gQueryManager.GetAsString("ServerName", SvSub, sizeof(SvSub));
		strncpy(rSend.HA[iCharCount].Is_SvSub, SvSub, sizeof(SvSub));

		rSend.HA[iCharCount].Is_Class = gQueryManager.GetAsInteger("Class");
		rSend.HA[iCharCount].Is_cLevel = gQueryManager.GetAsInteger("cLevel");
		rSend.HA[iCharCount].Is_ResetCount = gQueryManager.GetAsInteger("ResetCount");
		rSend.HA[iCharCount].Is_MasterResetCount = gQueryManager.GetAsInteger("MasterResetCount");
		rSend.HA[iCharCount].Is_ResetTime = gQueryManager.GetAsInteger("Resets_Time");
		rSend.HA[iCharCount].Is_TotalPoint = gQueryManager.GetAsInteger("TotalPoint");
		rSend.HA[iCharCount].Is_VipStars = gQueryManager.GetAsInteger("rNewVip");

		iCharCount++;
		if (iCharCount >= MAXTOP)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTop));
}
void RankingDataClass::CharacterRankingTP(GDTopTP* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopTP rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x02, sizeof(DGCharTopTP));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_TP") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}
		char NickName[11] = { 0 };

		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.HA[iCharCount].Name, NickName, sizeof(NickName));

		rSend.HA[iCharCount].Is_Class = gQueryManager.GetAsInteger("Class");
		rSend.HA[iCharCount].Is_WcoinC = gQueryManager.GetAsInteger("WC");
		rSend.HA[iCharCount].Is_WcoinP = gQueryManager.GetAsInteger("WP");
		rSend.HA[iCharCount].Is_WcoinG = gQueryManager.GetAsInteger("WG");
		rSend.HA[iCharCount].Is_WcoinR = gQueryManager.GetAsInteger("RD");
		rSend.HA[iCharCount].Is_CoinAtm = gQueryManager.GetAsInteger("AT");

		iCharCount++;
		if (iCharCount >= MAXTOP)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopTP));
}

void RankingDataClass::CharacterRankingTG(GDTopTG* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopTG rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0xF4, sizeof(DGCharTopTG));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_GD") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}

		char Guild_Name[11] = { 0 };
		char Guild_Master[11] = { 0 };
		char JoinDate[20] = { 0 };

		gQueryManager.GetAsString("G_Master", Guild_Master, sizeof(Guild_Master));
		strncpy(rSend.tp[iCharCount].G_Master, Guild_Master, sizeof(Guild_Master));
		gQueryManager.GetAsString("G_Name", Guild_Name, sizeof(Guild_Name));
		strncpy(rSend.tp[iCharCount].G_Name, Guild_Name, sizeof(Guild_Name));
		gQueryManager.GetAsString("G_Created", JoinDate, sizeof(JoinDate));
		strncpy(rSend.tp[iCharCount].Is_JoinDate, JoinDate, sizeof(JoinDate));

		rSend.tp[iCharCount].Is_GLevel = gQueryManager.GetAsInteger("LevelDevote");
		rSend.tp[iCharCount].Is_GScore = gQueryManager.GetAsInteger("G_Score");
		rSend.tp[iCharCount].Is_GMem = gQueryManager.GetAsInteger("G_Count");

		rSend.tp[iCharCount].KillBoss = gQueryManager.GetAsInteger("G_PointTotal");
		rSend.tp[iCharCount].TotalReset = gQueryManager.GetAsInteger("G_TopRS");
		rSend.tp[iCharCount].TotalPoint = gQueryManager.GetAsInteger("G_TopPoint");

		gQueryManager.GetAsBinary("G_Mark", rSend.tp[iCharCount].GuildMark, sizeof(rSend.tp[iCharCount].GuildMark));

		iCharCount++;
		if (iCharCount >= 20)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopTG));
}

void RankingDataClass::CharacterRankingPK(GDTopPK* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopPK rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x05, sizeof(DGCharTopPK));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_PK") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}

		char NickName[11] = { 0 };
		char JoinDate[22] = { 0 };
		char SvSub[9] = { 0 };

		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.tp[iCharCount].Name, NickName, sizeof(NickName));

		gQueryManager.GetAsString("MDate", JoinDate, sizeof(JoinDate));
		strncpy(rSend.tp[iCharCount].Is_JoinDate, JoinDate, sizeof(JoinDate));

		rSend.tp[iCharCount].Is_Class = gQueryManager.GetAsInteger("Class");
		rSend.tp[iCharCount].Is_Kills = gQueryManager.GetAsInteger("Kills");
		rSend.tp[iCharCount].Is_Deads = gQueryManager.GetAsInteger("Deads");
		rSend.tp[iCharCount].Is_OnlineHours = gQueryManager.GetAsInteger("OnlineHours");

		rSend.tp[iCharCount].Is_Win = gQueryManager.GetAsInteger("WinScore");
		rSend.tp[iCharCount].Is_Lose = gQueryManager.GetAsInteger("LoseScore");
		rSend.tp[iCharCount].Is_Kboss = gQueryManager.GetAsInteger("KBoss");

		iCharCount++;
		if (iCharCount >= MAXTOP)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopPK));
}
void RankingDataClass::CharacterRankingET(GDTopET* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopET rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x03, sizeof(DGCharTopET));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_ET") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}
		char NickName[11] = { 0 };

		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.tp[iCharCount].Name, NickName, sizeof(NickName));

		rSend.tp[iCharCount].Is_Class = gQueryManager.GetAsInteger("Class");
		rSend.tp[iCharCount].Is_BloodCS = gQueryManager.GetAsInteger("BC");
		rSend.tp[iCharCount].Is_ChaosCS = gQueryManager.GetAsInteger("CC");
		rSend.tp[iCharCount].Is_DevilCS = gQueryManager.GetAsInteger("DS");

		iCharCount++;
		if (iCharCount >= MAXTOP)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopET));
}

void RankingDataClass::CharacterRankingMT(GDTopMT* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopMT rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x06, sizeof(DGCharTopMT));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_MT") == FALSE)
	{
		gQueryManager.Close();
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);

		if (iReturnCode < 0)
		{
			break;
		}

		char NickName[11] = { 0 };

		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.tp[iCharCount].Name, NickName, sizeof(NickName));

		rSend.tp[iCharCount].cLevel = gQueryManager.GetAsInteger("MasterLevel");
		rSend.tp[iCharCount].cPoint = gQueryManager.GetAsInteger("MasterPoint");
		rSend.tp[iCharCount].cExp = gQueryManager.GetAsInteger("MasterExperience");

		iCharCount++;
		if (iCharCount >= MAXTOP)
		{
			break;
		}
		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopMT));
}

void RankingDataClass::CharacterRankingWQ(GDTopWQ* lpMsg, int pIndex)
{
	int iCharCount = 0;
	DGCharTopWQ rSend;
	ZeroMemory(&rSend, sizeof(rSend));
	rSend.h.set(0x7A, 0x07, sizeof(DGCharTopWQ));

	if (gQueryManager.ExecQuery("EXEC EX_RANKING_WQ") == FALSE)
	{
		gQueryManager.Close();
		return;
	}

	short i = gQueryManager.Fetch();

	while (i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		int iReturnCode = gQueryManager.GetResult(1);
		if (iReturnCode < 0)
			break;

		char NickName[11] = { 0 };
		gQueryManager.GetAsString("Name", NickName, sizeof(NickName));
		strncpy(rSend.tp[iCharCount].Name, NickName, sizeof(NickName));

		rSend.tp[iCharCount].Start = gQueryManager.GetAsInteger("Start");
		rSend.tp[iCharCount].Number = gQueryManager.GetAsInteger("Number");
		rSend.tp[iCharCount].CurQuest = gQueryManager.GetAsInteger("CurQuest");

		iCharCount++;
		if (iCharCount >= MAXTOP)
			break;

		i = gQueryManager.Fetch();
	}

	gQueryManager.Close();
	gSocketManager.DataSend(pIndex, (BYTE*)&rSend, sizeof(DGCharTopWQ));
}
