#include "StdAfx.h"
#include "ACuoi_VongQuay.h"
#include "DSProtocol.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "CashShop.h"
#include "User.h"
#include "Notice.h"
#include "ACuoi_MessLang.h"
#include "ServerInfo.h"
#include "Path.h"
#include "Log.h"
#include "MapServerManager.h"

CLuckySpin gLuckySpin;

CLuckySpin::CLuckySpin() { Init(); }
CLuckySpin::~CLuckySpin() {}

void CLuckySpin::Init()
{
	itemdata.clear();
	TicketType = TicketIndex = number = -1;
}

void CLuckySpin::Load() { Init(); Read(gPath.GetFullPath("Custom\\ThangCuoi\\CustomVongQuay.txt")); }

void CLuckySpin::Read(char* path)
{
	CMemScript* lpMemScript = new CMemScript;
	if (!lpMemScript) { ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path); return; }
	if (!lpMemScript->SetBuffer(path)) { ErrorMessageBox(lpMemScript->GetLastError()); delete lpMemScript; return; }

	try
	{
		while (lpMemScript->GetToken() != TOKEN_END)
		{
			int section = lpMemScript->GetNumber();
			while (true)
			{
				if (!strcmp("end", lpMemScript->GetAsString())) break;
				if (section == 0)
				{
					TicketType = lpMemScript->GetNumber();
					TicketIndex = lpMemScript->GetAsNumber();
					TicketCount = lpMemScript->GetAsNumber();
					TicketWcoinC = lpMemScript->GetAsNumber();
				}
				else if (section == 1)
				{
					LuckySpin_Data info;
					info.ItemType = lpMemScript->GetNumber();
					info.ItemIndex = lpMemScript->GetAsNumber();
					info.ItemLevel = lpMemScript->GetAsNumber();
					info.ItemSkill = lpMemScript->GetAsNumber();
					info.ItemLuck = lpMemScript->GetAsNumber();
					info.ItemOption = lpMemScript->GetAsNumber();
					info.ItemExc = lpMemScript->GetAsNumber();
					info.ItemTime = lpMemScript->GetAsNumber();
					info.Rate = lpMemScript->GetAsNumber();
					itemdata.push_back(info);
				}
				else break;
			}
		}
	}
	catch (...) { ErrorMessageBox(lpMemScript->GetLastError()); }

	delete lpMemScript;
}

void CLuckySpin::GCInitItem(int aIndex)
{
	if (!gObjIsConnected(aIndex)) return;
	GC_LuckySpinInit pMsg{};
	pMsg.h.set(0xF3, 0x34, sizeof(pMsg));
	pMsg.result = true;
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CLuckySpin::GCItemSend(int aIndex)
{
	if (!gObjIsConnected(aIndex)) return;
	GC_LuckySpin_Data pMsg{};
	pMsg.h.set(0xF3, 0x33, sizeof(pMsg));

	for (int i = 0; i < itemdata.size(); i++)
	{
		pMsg.ItemBmd[i] = GET_ITEM(itemdata[i].ItemType, itemdata[i].ItemIndex);
		pMsg.ItemLvl[i] = itemdata[i].ItemLevel;
		pMsg.ItemExc[i] = itemdata[i].ItemExc;
		pMsg.WCoinC = TicketWcoinC;
		DataSend(aIndex, (BYTE*)&pMsg, sizeof(pMsg));
	}

	GCInitItem(aIndex);
}

int CLuckySpin::GetItemCount(int UserIndex)
{
	if (!OBJMAX_RANGE(UserIndex)) return -1;
	int ItemCount = 0;
	LPOBJ lpUser = &gObj[UserIndex];

	for (int i = INVENTORY_WEAR_SIZE; i < INVENTORY_MAIN_SIZE; i++)
		if (lpUser->Inventory[i].IsItem() && lpUser->Inventory[i].m_Index == GET_ITEM(TicketType, TicketIndex))
			ItemCount++;

	return ItemCount;
}

int CLuckySpin::CheckItem(int UserIndex, int ItemID, BYTE ItemLevel)
{
	if (!OBJMAX_RANGE(UserIndex)) return -1;
	LPOBJ lpUser = &gObj[UserIndex];

	for (int i = INVENTORY_WEAR_SIZE; i < INVENTORY_MAIN_SIZE; i++)
		if (lpUser->Inventory[i].IsItem() && lpUser->Inventory[i].m_Index == ItemID && lpUser->Inventory[i].m_Level == ItemLevel)
			return i;

	return -1;
}

bool CLuckySpin::DeleteItemCount(int UserIndex, int SoulCount)
{
	if (!OBJMAX_RANGE(UserIndex) || GetItemCount(UserIndex) < SoulCount) return false;
	LPOBJ lpUser = &gObj[UserIndex];

	int DeletedItem = 0;
	while (DeletedItem < SoulCount)
	{
		int ItemPos = CheckItem(UserIndex, GET_ITEM(TicketType, TicketIndex), 0);
		if (ItemPos == -1) return false;
		gItemManager.DeleteInventoryItemCount(lpUser, ItemPos, 0, TicketCount);
		DeletedItem++;
	}
	return true;
}

void CLuckySpin::StartRoll(LPOBJ lpObj)
{
	if (lpObj->ThisCoin[0] < TicketWcoinC)
	{
		int CurrentWC = TicketWcoinC - lpObj->ThisCoin[0];
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(50), CurrentWC);
		return;
	}

	DeleteItemCount(lpObj->Index, TicketCount);
	GDSetCoinSend(lpObj->Index, -TicketWcoinC, 0, 0, 0, 0, "VongQuay");
	lpObj->LuckySpinTime = GetTickCount();

	GC_LuckySpinRoll pMsg{};
	pMsg.header.set(0xF3, 0x32, sizeof(pMsg));
	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CLuckySpin::MakeItem(int aIndex, LPOBJ lpObj)
{
	int rates[12], totalRate = 0;
	for (int i = 0; i < 12; i++) { rates[i] = itemdata[i].Rate; totalRate += rates[i]; }

	int randomNumber = rand() % totalRate, cumulativeRate = 0;
	for (int i = 0; i < 12; i++)
	{
		cumulativeRate += rates[i];
		if (randomNumber <= cumulativeRate) { number = i; break; }
	}

	DWORD iTime = (DWORD)time(NULL) + itemdata[number].ItemTime * 86400;
	GDCreateItemSend(aIndex, 0xEB, 0, 0, GET_ITEM(itemdata[number].ItemType, itemdata[number].ItemIndex),
		itemdata[number].ItemLevel, 0, itemdata[number].ItemSkill, itemdata[number].ItemLuck,
		itemdata[number].ItemOption, -1, itemdata[number].ItemExc, 0, 0, 0, 0, 0xFF, 0);

	WORD ItemIndex = GET_ITEM(itemdata[number].ItemType, itemdata[number].ItemIndex, itemdata[number].ItemLevel);
	gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(49), lpObj->Name, gItemManager.GetItemName(ItemIndex));

	gLog.Output(LOG_TRADE, "[VQMM] Tài Khoản:[%s] Nhân Vật:[%s] - Tiền: [%d] - Phần Phưởng: [%s]", lpObj->Account, lpObj->Name, TicketWcoinC, gItemManager.GetItemName(ItemIndex));
}
