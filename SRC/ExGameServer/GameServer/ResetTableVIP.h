// ResetTable.h: interface for the CResetTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct RESET_TABLE_INFO_VIP
{
	int MinReset;
	int MaxReset;
	int Level[MAX_ACCOUNT_LEVEL];
	int Money[MAX_ACCOUNT_LEVEL];
	int Point[MAX_ACCOUNT_LEVEL];
	int PointKeep[MAX_ACCOUNT_LEVEL];
	int WCoinC[MAX_ACCOUNT_LEVEL];
	//int WCoinP[MAX_ACCOUNT_LEVEL];
	int Bless[MAX_ACCOUNT_LEVEL];
	int Soul[MAX_ACCOUNT_LEVEL];
	int Life[MAX_ACCOUNT_LEVEL];
	int Chaos[MAX_ACCOUNT_LEVEL];
	int Cre[MAX_ACCOUNT_LEVEL];

};

class CResetTableVip
{
public:
	CResetTableVip();
	virtual ~CResetTableVip();
	void Load(char* path);
	int GetResetLevel(LPOBJ lpObj);
	int GetResetMoney(LPOBJ lpObj);
	int GetResetPoint(LPOBJ lpObj);
	int GetResetPointkeep(LPOBJ lpObj);
	//int GetResetTienTeWCoinP(LPOBJ lpObj);
	int GetResetTienTeWCoinC(LPOBJ lpObj);
	int GetResetChaos(LPOBJ lpObj);
	int GetResetBless(LPOBJ lpObj);
	int GetResetSoul(LPOBJ lpObj);
	int GetResetLife(LPOBJ lpObj);
	int GetResetCre(LPOBJ lpObj);
private:
	std::vector<RESET_TABLE_INFO_VIP> m_ResetTableInfo;
	RESET_TABLE_INFO_VIP m_ResetTable;
};

extern CResetTableVip gResetTableVip;
