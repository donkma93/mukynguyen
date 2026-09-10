#include "stdafx.h"
#include "CashShop.h"
#include "ResetChange.h"
#include "DSProtocol.h"
#include "JSProtocol.h"
#include "Util.h"
#include "MemScript.h"

#include "Notice.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "CustomRankUser.h"
#include "Util.h"
#include "ItemBagManager.h"
#include "ACuoi_MessLang.h"

CResetChange gResetChange;

CResetChange::CResetChange()
{
	this->Init();
}

void CResetChange::Init()
{
	for (int n = 0; n < MAX_RESET_CHANGE; n++)
	{
		this->m_CustomResetChangeInfo[n].Index = -1;
	}
}

void CResetChange::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
			
			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}
			RESETCHANGE_INFO info;
			memset(&info, 0, sizeof(info));

			info.Index			= lpMemScript->GetNumber();
			info.ReqReset		= lpMemScript->GetAsNumber();
			info.Coin1			= lpMemScript->GetAsNumber();
			info.LevelUpPoint	= lpMemScript->GetAsNumber();
			info.TangQua		= lpMemScript->GetAsNumber();
			info.ItemLevel		= lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}
	delete lpMemScript;
}

void CResetChange::SetInfo(RESETCHANGE_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_RESET_CHANGE)
	{
		return;
	}
	this->m_CustomResetChangeInfo[info.Index] = info;
}

RESETCHANGE_INFO* CResetChange::GetInfo(int index)
{
	if (index < 0 || index >= MAX_RESET_CHANGE)
	{
		return 0;
	}

	if (this->m_CustomResetChangeInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomResetChangeInfo[index];
}

void CResetChange::ResetChange(int aIndex, RESETCHANGE_REQ* lpMsg)
{
	LPOBJ lpObj = &gObj[aIndex];
	if ((GetTickCount() - lpObj->ClickClientSend) < 2000)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(38));
		return;
	}
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpMsg->numchange < 1 || lpMsg->numchange > 5)
	{
		return;
	}
	
	if (lpObj->Reset < m_CustomResetChangeInfo[lpMsg->numchange - 1].ReqReset)
	{
		this->SoLieuReset = m_CustomResetChangeInfo[lpMsg->numchange - 1].ReqReset - lpObj->Reset;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(40),SoLieuReset);
		return;
	}
	
	if (lpObj->LevelUpPoint < m_CustomResetChangeInfo[lpMsg->numchange - 1].LevelUpPoint)
	{
		this->SoLieuPoint = m_CustomResetChangeInfo[lpMsg->numchange - 1].LevelUpPoint - lpObj->LevelUpPoint;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(41),SoLieuPoint);
		return;
	}
	
	lpObj->Reset			= (lpObj->Reset - m_CustomResetChangeInfo[lpMsg->numchange - 1].ReqReset);
	lpObj->LevelUpPoint		= (lpObj->LevelUpPoint - m_CustomResetChangeInfo[lpMsg->numchange - 1].LevelUpPoint);

	lpObj->TangQua			= (m_CustomResetChangeInfo[lpMsg->numchange - 1].TangQua);
	lpObj->ItemLevel		= (m_CustomResetChangeInfo[lpMsg->numchange - 1].ItemLevel);

	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, lpObj->TangQua, lpObj->ItemLevel, 1, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0);

	GDSetCoinSend(lpObj->Index, +(m_CustomResetChangeInfo[lpMsg->numchange - 1].Coin1), 0, 0, 0, 0, "ResetChange");
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	GDResetInfoSaveSend(lpObj->Index, 0, 0, 0);
	gCashShop.CGCashShopPointRecv(lpObj->Index);
	if (gServerInfo.m_CustomRankUserType == 0)
	{
		gCustomRankUser.CheckUpdate(lpObj);
	}
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(42));
	lpObj->ClickClientSend = GetTickCount();
}
