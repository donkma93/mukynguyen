#include "stdafx.h"
#include "EventInventory.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "GameMain.h"
#include "ItemBagManager.h"
#include "ItemMove.h"
#include "ItemStack.h"
#include "Map.h"
#include "MuRummy.h"
#include "ServerInfo.h"
#include "Util.h"

CEventInventory gEventInventory;


bool CEventInventory::IsEventItem(int ItemIndex)
{
	return 0;
}

bool CEventInventory::IsEventItem(CItem* lpItem)
{
	return 0;
}

void CEventInventory::EventInventoryItemSet(int aIndex,int slot,BYTE type)
{
	
}

BYTE CEventInventory::EventInventoryRectCheck(int aIndex,int x,int y,int width,int height)
{
	return 0xFF;
}

BYTE CEventInventory::EventInventoryInsertItem(int aIndex,CItem item)
{
	return 0xFF;
}

BYTE CEventInventory::EventInventoryAddItem(int aIndex,CItem item,int slot)
{
	return 0xFF;
}

void CEventInventory::EventInventoryDelItem(int aIndex,int slot)
{
	
}

bool CEventInventory::EventInventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem)
{
	return 0;
}

bool CEventInventory::EventInventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot)
{
	return 0;
}

void CEventInventory::CGEventItemGetRecv(PMSG_EVENT_ITEM_GET_RECV* lpMsg,int aIndex)
{

}

void CEventInventory::CGEventItemDropRecv(PMSG_EVENT_ITEM_DROP_RECV* lpMsg,int aIndex)
{

}

void CEventInventory::CGEventInventoryOpenRecv(PMSG_EVENT_INVENTORY_OPEN_RECV* lpMsg,int aIndex)
{

}

void CEventInventory::GCEventItemListSend(int aIndex)
{

}

void CEventInventory::GCEventItemDurSend(int aIndex,BYTE slot,BYTE dur)
{

}

void CEventInventory::GCEventItemDeleteSend(int aIndex,BYTE slot,BYTE flag)
{
}

void CEventInventory::GCEventItemModifySend(int aIndex,BYTE slot)
{
	
}

void CEventInventory::GCEventItemChangeSend(int aIndex,BYTE slot)
{
	
}

void CEventInventory::GCEventInventoryEnableSend(int aIndex)
{
	
}

void CEventInventory::DGEventInventoryRecv(SDHP_EVENT_INVENTORY_RECV* lpMsg)
{
	
}

void CEventInventory::GDEventInventorySend(int aIndex)
{
	
}

void CEventInventory::GDEventInventorySaveSend(int aIndex)
{
	
}
