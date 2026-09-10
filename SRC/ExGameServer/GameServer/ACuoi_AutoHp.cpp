#include "stdafx.h"
#include "CommandManager.h"
#include "ACuoi_AutoHp.h"
#include "GensSystem.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ACuoi_MessLang.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"
#include "User.h"

CAUTOHP gAUTOHP;

void CAUTOHP::AutoHp(LPOBJ lpObj)
{
	if (lpObj->CmdAutoHp == 0)
	{
		return;
	}
	if (lpObj->Life > 0 && lpObj->Life < ((lpObj->MaxLife + lpObj->AddLife) * 95 / 100))
	{
		PMSG_ITEM_USE_RECV pMsg{};

		pMsg.header.set(0x26, sizeof(pMsg));
		pMsg.SourceSlot = 0xFF;
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 3), -1) : pMsg.SourceSlot);
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 2), -1) : pMsg.SourceSlot);
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 1), -1) : pMsg.SourceSlot);
		pMsg.TargetSlot = 0xFF;
		pMsg.type = 0;

		if (INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
		{
			gItemManager.CGItemUseRecv(&pMsg, lpObj->Index);
		}
	}
}

void CAUTOHP::BomMana(LPOBJ lpObj)
{
	if (lpObj->Mana > 0 && lpObj->Mana <= ((lpObj->MaxMana + lpObj->AddMana) * 20 / 100))
	{
		PMSG_ITEM_USE_RECV pMsg{};

		pMsg.header.set(0x27, sizeof(pMsg));
		pMsg.SourceSlot = 0xFF;
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 4), -1) : pMsg.SourceSlot);
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 5), -1) : pMsg.SourceSlot);
		pMsg.SourceSlot = ((pMsg.SourceSlot == 0xFF) ? gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 6), -1) : pMsg.SourceSlot);
		pMsg.TargetSlot = 0xFF;
		pMsg.type = 0;

		if (INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
		{
			gItemManager.CGItemUseRecv(&pMsg, lpObj->Index);
		}
	}
}
void CAUTOHP::MainProc()
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			gAUTOHP.AutoHp(&gObj[n]);
			gAUTOHP.BomMana(&gObj[n]);
		}
	}
}

bool CAUTOHP::CommandHp(LPOBJ lpObj)
{
	if(lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(46));
		return 0;
	}

	if ((GetTickCount() - lpObj->ClickClientSend) < 5 * 1000)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(38));
		return false;
	}
	if (lpObj->CmdAutoHp == 1)
	{
		lpObj->CmdAutoHp = 0;
		//gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(59));
		return 0;
	}
	else
	{
		lpObj->CmdAutoHp == 0;
		//gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0, gMessageNew.GetMessage(58));
		lpObj->CmdAutoHp = 1;
		return 1;
	}
	lpObj->ClickClientSend = GetTickCount();
}
