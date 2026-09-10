#include "user.h"
#include "Protocol.h"
#pragma once

#define MAXSIZERL	10
#define MAXSUBSIZE	5

struct RelifeRequest
{
	PSBMSG_HEAD h;
	int Number;
};

struct DieuKienNgoc
{
	int CheckEventJw[MAXSUBSIZE];
};

class TaiSinh
{
	public:
	void GetProcRelife(LPOBJ lpObj, RelifeRequest* aRecv, BYTE CheckDk);
	void Load(char* path);
	void RunRelifeToProc(LPOBJ lpObj, RelifeRequest *aRecv);
	void TaiSinhCheckDataSend(int aIndex);
	void TaiSinhDiemDataSend(int aIndex);
	void DieuKienNgoc01DataSend(int aIndex);

	int m_Enabled;
	int m_TaiSinh;
	int ThoiGian;
	int RelogChar;
	int GioiHanRelife;
	int m_DieuKienLevel;
	int m_AfterRelifeLevel;
	int m_AfterRelifeReset;
	int m_AfterRelifeUpPoint;
	int m_RelifeFreeTime;

	int Spk_DieuKien[MAXSIZERL];
	int Spk_DiemPoin[MAXSIZERL];

	char Msg01[256];
	char Msg02[256];

	DieuKienNgoc g_DieuKienJw[MAXSIZERL];

	char MaxCharChaos[20];
	char MaxCharBless[20];
	char MaxCharSoul[20];
	char MaxCharLife[20];
	char MaxCharCrea[20];
}; 
extern TaiSinh gTaiSinh;
