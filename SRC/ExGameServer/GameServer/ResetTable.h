// ResetTable.h: interface for the CResetTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Protocol.h"
#include "User.h"

struct CGPACKET_AUTO
{
	PSBMSG_HEAD header; // C3:F3:03
	DWORD Status;
	DWORD Vip;
};

struct PMSG_RESET_TABLE_INFO
{
	PSBMSG_HEAD h;
	int ResetThuong;
	int ResetVip;
	int GHRS;
	int m_Reset;
	int Level;
	int Money;
	int Point;
	int VipLevel;
	int VipMoney;
	int VipPoint;
	int Coin[2];
	int Ngoc[5];
};

struct RESET_TABLE_INFO
{
	int MinReset;
	int MaxReset;
	int Level[MAX_ACCOUNT_LEVEL];
	int Money[MAX_ACCOUNT_LEVEL];
	int Point[MAX_ACCOUNT_LEVEL];
	int PointKeep[MAX_ACCOUNT_LEVEL];
};

class CResetTable
{
public:
	CResetTable();
	void Load(char* path);
	int GetResetLevel(LPOBJ lpObj);
	int GetResetMoney(LPOBJ lpObj);
	int GetResetPoint(LPOBJ lpObj);
	int GetResetPointKeep(LPOBJ lpObj);
	void SendResetInfo(LPOBJ lpObj, int reset, int accountlevel);
	void SendRequestAuto(CGPACKET_AUTO* lpMsg, int aIndex);
	void SendRequest(int aIndex);
	RESET_TABLE_INFO m_ResetTable;
private:
	std::vector<RESET_TABLE_INFO> m_ResetTableInfo;
};

extern CResetTable gResetTable;
