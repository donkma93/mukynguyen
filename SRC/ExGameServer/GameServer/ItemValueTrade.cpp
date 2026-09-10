#include "stdafx.h"
#include "ItemValueTrade.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "MemScript.h"

#include "Notice.h"
#include "DSProtocol.h"
#include "Util.h"
#include "ACuoi_MessLang.h"

CItemValueTrade gItemValueTrade;

CItemValueTrade::CItemValueTrade()
{
	this->m_ItemValueTradeInfo.clear();
}

void CItemValueTrade::Load(char* path)
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

	this->m_ItemValueTradeInfo.clear();

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

			ITEM_VALUETRADE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Value = lpMemScript->GetAsNumber();

			info.Coin1 = lpMemScript->GetAsNumber();

			info.Coin2 = lpMemScript->GetAsNumber();

			info.Coin3 = lpMemScript->GetAsNumber();


			this->m_ItemValueTradeInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemValueTrade::CheckItemValueTrade(LPOBJ lpObj,LPOBJ lpTarget)
{
	int Money = 0;
	int Coin1 = 0;
	int Coin2 = 0;
	int Coin3 = 0;

	int TargetMoney = 0;
	int TargetCoin1 = 0;
	int TargetCoin2 = 0;
	int TargetCoin3 = 0;

	for(std::vector<ITEM_VALUETRADE_INFO>::iterator it=this->m_ItemValueTradeInfo.begin();it != this->m_ItemValueTradeInfo.end();it++)
	{

		for(int n=0;n < TRADE_SIZE;n++)
		{
			//Check lpObj
			if(lpObj->Trade[n].IsItem() != 0)
			{
				if(it->Index == lpObj->Trade[n].m_Index)
				{
					if(gItemStack.GetItemMaxStack(it->Index) == 0 || it->Index == GET_ITEM(4,7) || it->Index == GET_ITEM(4,15))
					{
						Money += it->Value;
						Coin1 += it->Coin1;
						Coin2 += it->Coin2;
						Coin3 += it->Coin3;
					}
					else
					{
						Money = (int)(it->Value*lpObj->Trade[n].m_Durability);
						Coin1 = (int)(it->Coin1*lpObj->Trade[n].m_Durability);
						Coin2 = (int)(it->Coin2*lpObj->Trade[n].m_Durability);
						Coin3 = (int)(it->Coin3*lpObj->Trade[n].m_Durability);
					}
				}
			}

			//Check lpTarget
			if(lpTarget->Trade[n].IsItem() != 0)
			{
				if(it->Index == lpTarget->Trade[n].m_Index)
				{
					if(gItemStack.GetItemMaxStack(it->Index) == 0 || it->Index == GET_ITEM(4,7) || it->Index == GET_ITEM(4,15))
					{
						TargetMoney += it->Value;
						TargetCoin1 += it->Coin1;
						TargetCoin2 += it->Coin2;
						TargetCoin3 += it->Coin3;
					}
					else
					{
						TargetMoney += (int)(it->Value*lpTarget->Trade[n].m_Durability);
						TargetCoin1 += (int)(it->Coin1*lpTarget->Trade[n].m_Durability);
						TargetCoin2 += (int)(it->Coin2*lpTarget->Trade[n].m_Durability);
						TargetCoin3 += (int)(it->Coin3*lpTarget->Trade[n].m_Durability);
					}
				}
			}
		}
	}

	if(lpObj->Money < (DWORD)Money) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessageNew.GetMessage(644),Money);
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpObj->Name);
		return 0;
	}

	if (lpObj->ThisCoin[0] < Coin1) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(645),Coin1);
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpObj->Name);
		return 0;
	}

	if (lpObj->ThisCoin[1] < Coin2) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(646),Coin2);
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpObj->Name);
		return 0;
	}

	if (lpObj->ThisCoin[2] < Coin3) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(647),Coin3);
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpObj->Name);
		return 0;
	}

	if(lpTarget->Money < (DWORD)TargetMoney) //OK
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(644),TargetMoney);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpTarget->Name);
		return 0;
	}

	if (lpTarget->ThisCoin[0] < TargetCoin1) //OK
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(645),TargetCoin1);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpTarget->Name);
		return 0;
	}

	if (lpTarget->ThisCoin[1] < TargetCoin2) //OK
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(646),TargetCoin2);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpTarget->Name);
		return 0;
	}

	if (lpTarget->ThisCoin[2] < TargetCoin3) //OK
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0, gMessageNew.GetMessage(647),TargetCoin3);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(648),lpTarget->Name);
		return 0;
	}

	if (Coin1 > 0 || Coin2 > 0 || Coin3 > 0)
	{
		GDSetCoinSend(lpObj->Index, -(Coin1), -(Coin2), -(Coin3), 0, 0,"TradeValue");
	}

	if (TargetCoin1 > 0 || TargetCoin2 > 0 || TargetCoin3 > 0)
	{
		GDSetCoinSend(lpTarget->Index, -(TargetCoin1), -(TargetCoin2), -(TargetCoin3), 0, 0,"TradeValue");
	}

	lpObj->Money -= Money;
	GCMoneySend(lpObj->Index,lpObj->Money);

	lpTarget->Money -= TargetMoney;
	GCMoneySend(lpTarget->Index,lpTarget->Money);

	return 1;
}
