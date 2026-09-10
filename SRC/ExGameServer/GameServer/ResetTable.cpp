#include "stdafx.h"
#include "ResetTable.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"
#include "ResetTableVIP.h"
#include "CommandManager.h"
#include "ResetLimiter.h"

CResetTable gResetTable;

CResetTable::CResetTable()
{
	this->m_ResetTableInfo.clear();
}

void CResetTable::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ResetTableInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			RESET_TABLE_INFO info;

			info.MinReset = lpMemScript->GetNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Level[0] = lpMemScript->GetAsNumber();

			info.Level[1] = lpMemScript->GetAsNumber();

			info.Level[2] = lpMemScript->GetAsNumber();

			info.Level[3] = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			info.Point[0] = lpMemScript->GetAsNumber();

			info.Point[1] = lpMemScript->GetAsNumber();

			info.Point[2] = lpMemScript->GetAsNumber();

			info.Point[3] = lpMemScript->GetAsNumber();

			this->m_ResetTableInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CResetTable::GetResetLevel(LPOBJ lpObj)
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Level[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
			}
			else
			{
				return it->Level[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
}

int CResetTable::GetResetMoney(LPOBJ lpObj)
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Money[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
			}
			else
			{
				return it->Money[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
}

int CResetTable::GetResetPoint(LPOBJ lpObj)
{
	int point = 0;

	for(int n=1;n <= lpObj->Reset;n++)
	{
		int AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];

		for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
		{
			if(n >= it->MinReset && n <= it->MaxReset)
			{
				if(it->Point[lpObj->AccountLevel] == -1)
				{
					AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];
					break;
				}
				else
				{
					AddPoint = it->Point[lpObj->AccountLevel];
					break;
				}
			}
		}

		point += AddPoint;
	}

	return point;
}
int CResetTable::GetResetPointKeep(LPOBJ lpObj)
{

	for (std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if ((lpObj->Reset + 1) >= it->MinReset && (lpObj->Reset + 1) <= it->MaxReset)
		{
			return it->Point[lpObj->AccountLevel];
		}
	}

	return m_ResetTable.Point[lpObj->AccountLevel];
}

void CResetTable::SendRequestAuto(CGPACKET_AUTO* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	if (lpMsg->Status == 1 && lpMsg->Vip == 0)
	{
		// Tắt ResetVip
		lpObj->ResetVip = 0;

		// Gọi hàm auto reset
		gCommandManager.CommandResetAuto(&gObj[aIndex], "auto", 0);
		if (lpObj->AutoResetVipEnable)
		{
			lpObj->AutoResetVipEnable = 0;
		}
	}
	else if (lpMsg->Status == 0 && lpMsg->Vip == 1)
	{
		// Bật ResetVip
		lpObj->ResetVip = 1;

		// Gọi hàm auto reset vip
		gCommandManager.CommandResetVipAuto(&gObj[aIndex], "auto", 0);

		if (lpObj->AutoResetEnable)
		{
			lpObj->AutoResetEnable = 0;
		}
	}
}

void CResetTable::SendRequest(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	SendResetInfo(lpObj, lpObj->Reset, lpObj->AccountLevel);
}

void CResetTable::SendResetInfo(LPOBJ lpObj, int reset, int accountlevel)
{
	PMSG_RESET_TABLE_INFO pMsg = { 0 };
	pMsg.h.set(0xF3, 0xF6, sizeof(pMsg));

	pMsg.m_Reset = reset + 1;
	pMsg.GHRS = gResetLitmit.GioiHanRS;
	pMsg.Level = GetResetLevel(lpObj);
	pMsg.Money = GetResetMoney(lpObj);
	if (gServerInfo.m_CommandResetType == 1)
	{
		pMsg.Point = GetResetPoint(lpObj);
	}
	else
	{
		pMsg.Point = GetResetPointKeep(lpObj);

	}
	pMsg.VipLevel = gResetTableVip.GetResetLevel(lpObj);
	pMsg.VipMoney = gResetTableVip.GetResetMoney(lpObj);
	if (gServerInfo.m_CommandResetType == 1)
	{
		pMsg.VipPoint = gResetTableVip.GetResetPoint(lpObj);
	}
	else
	{
		pMsg.VipPoint = gResetTableVip.GetResetPointkeep(lpObj);

	}
	pMsg.Coin[0] = gResetTableVip.GetResetTienTeWCoinC(lpObj);
	//pMsg.Coin[1] = gResetTableVip.GetResetTienTeWCoinP(lpObj);
	pMsg.Ngoc[0] = gResetTableVip.GetResetBless(lpObj);
	pMsg.Ngoc[1] = gResetTableVip.GetResetSoul(lpObj);
	pMsg.Ngoc[2] = gResetTableVip.GetResetLife(lpObj);
	pMsg.Ngoc[3] = gResetTableVip.GetResetCre(lpObj);
	pMsg.Ngoc[4] = gResetTableVip.GetResetChaos(lpObj);
	DataSend(lpObj->Index, (BYTE*)&pMsg, sizeof(pMsg));
}