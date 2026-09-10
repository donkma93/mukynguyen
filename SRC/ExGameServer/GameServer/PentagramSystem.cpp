// PentagramSystem.cpp: implementation of the CPentagramSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PentagramSystem.h"
#include "CastleSiegeSync.h"
#include "ChaosBox.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "Util.h"

CPentagramSystem gPentagramSystem;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPentagramSystem::CPentagramSystem()
{

}

void CPentagramSystem::Load(char* path)
{
}

void CPentagramSystem::LoadJewel(char* path)
{
}

void CPentagramSystem::LoadMixRate(char* path)
{
}

bool CPentagramSystem::IsPentagramItem(int ItemIndex)
{
	return 0;
}

bool CPentagramSystem::IsPentagramItem(CItem* lpItem)
{
	return 0;
}

bool CPentagramSystem::IsPentagramJewel(int ItemIndex)
{
	return 0;
}

bool CPentagramSystem::IsPentagramJewel(CItem* lpItem)
{
	return 0;
}

bool CPentagramSystem::IsPentagramMithril(int ItemIndex)
{
	return 0;
}

bool CPentagramSystem::IsPentagramMithril(CItem* lpItem)
{
	return 0;
}

PENTAGRAM_JEWEL_INFO* CPentagramSystem::GetPentagramJewelInfo(LPOBJ lpObj,int index,int type)
{
	return 0;
}

PENTAGRAM_JEWEL_INFO* CPentagramSystem::AddPentagramJewelInfo(LPOBJ lpObj,CItem* lpItem,int type)
{
	return 0;
}

PENTAGRAM_JEWEL_INFO* CPentagramSystem::AddPentagramJewelInfo(LPOBJ lpObj,PENTAGRAM_JEWEL_INFO* lpInfo,int type)
{
	return 0;
}

void CPentagramSystem::DelPentagramJewelInfo(LPOBJ lpObj,int index,int type)
{

}

void CPentagramSystem::DelAllPentagramJewelInfo(LPOBJ lpObj,CItem* lpItem,int type)
{
}

bool CPentagramSystem::CheckExchangePentagramItem(LPOBJ lpObj)
{
	return 1;
}

bool CPentagramSystem::CheckExchangePentagramItem(LPOBJ lpObj,CItem* lpItem)
{
	return 1;
}

bool CPentagramSystem::CheckPentagramOption(LPOBJ lpObj,PENTAGRAM_OPTION_INFO* lpInfo)
{
	return 0;
}

void CPentagramSystem::MovePentagramToInventoryFromWarehouse(LPOBJ lpObj,CItem* lpItem)
{
}

void CPentagramSystem::MovePentagramToWarehouseFromInventory(LPOBJ lpObj,CItem* lpItem)
{
}

void CPentagramSystem::ExchangePentagramItem(LPOBJ lpObj,LPOBJ lpTarget)
{
}

void CPentagramSystem::ExchangePentagramItem(LPOBJ lpObj,LPOBJ lpTarget,CItem* lpItem)
{
}

bool CPentagramSystem::GetPentagramTypeInfo(int index,PENTAGRAM_TYPE_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramOptionInfo(int index,PENTAGRAM_OPTION_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramJewelOptionInfo(int ItemIndex,int RankNumber,int RankOption,PENTAGRAM_JEWEL_OPTION_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramJewelRemoveInfo(int index,PENTAGRAM_JEWEL_REMOVE_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramJewelUpgradeRankInfo(int index,PENTAGRAM_JEWEL_UPGRADE_RANK_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramJewelUpgradeLevelInfo(int index,PENTAGRAM_JEWEL_UPGRADE_LEVEL_INFO* lpInfo)
{
	return 0;
}

bool CPentagramSystem::GetPentagramRandomJewelOption(int ItemIndex,int RankNumber,BYTE* option)
{
	return 0;
}

void CPentagramSystem::GetPentagramRelationshipDamage(LPOBJ lpObj,LPOBJ lpTarget,int* damage)
{
}

void CPentagramSystem::GetPentagramRelationshipDefense(LPOBJ lpObj,LPOBJ lpTarget,int* defense)
{
}

void CPentagramSystem::CalcPentagramOption(LPOBJ lpObj,bool flag)
{
}

void CPentagramSystem::CalcPentagramJewelOption(LPOBJ lpObj,bool flag)
{
}

bool CPentagramSystem::CheckPentagramSocket(LPOBJ lpObj,CItem* lpItem)
{
	return 0;
}

void CPentagramSystem::InsertOption(LPOBJ lpObj,int index,int value,bool flag)
{
}

void CPentagramSystem::InsertJewelOption(LPOBJ lpObj,int index,int value,bool flag)
{
}

void CPentagramSystem::CGPentagramJewelInsertRecv(PMSG_PENTAGRAM_JEWEL_INSERT_RECV* lpMsg,int aIndex)
{
}

void CPentagramSystem::CGPentagramJewelRemoveRecv(PMSG_PENTAGRAM_JEWEL_REMOVE_RECV* lpMsg,int aIndex)
{
}

void CPentagramSystem::CGPentagramJewelRefineRecv(PMSG_PENTAGRAM_JEWEL_REFINE_RECV* lpMsg,int aIndex)
{
}

void CPentagramSystem::CGPentagramJewelUpgradeRecv(PMSG_PENTAGRAM_JEWEL_UPGRADE_RECV* lpMsg,int aIndex)
{
}

void CPentagramSystem::GCPentagramJewelResultSend(int aIndex,int result)
{
}

void CPentagramSystem::GCPentagramJewelInfoSend(int aIndex,int type)
{
}

void CPentagramSystem::GCPentagramJewelTradeInfoSend(int aIndex,int bIndex,CItem* lpItem)
{
}

void CPentagramSystem::GCPentagramJewelPShopInfoSend(int aIndex,int bIndex)
{
}

void CPentagramSystem::DGPentagramJewelInfoRecv(SDHP_PENTAGRAM_JEWEL_INFO_RECV* lpMsg)
{
}

void CPentagramSystem::GDPentagramJewelInfoSend(int aIndex,int type)
{
}

void CPentagramSystem::GDPentagramJewelInfoSaveSend(int aIndex,int type)
{
}

void CPentagramSystem::GDPentagramJewelInsertSaveSend(int aIndex,PENTAGRAM_JEWEL_INFO* lpInfo)
{
}

void CPentagramSystem::GDPentagramJewelDeleteSaveSend(int aIndex,int type,int index)
{
}
