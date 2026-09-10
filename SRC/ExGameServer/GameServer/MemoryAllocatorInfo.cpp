#include "stdafx.h"
#include "MemoryAllocatorInfo.h"
#include "EffectManager.h"
#include "EventInventory.h"
#include "GensSystem.h"
#include "HackPacketCheck.h"
#include "MuRummy.h"
#include "MuunSystem.h"
#include "PentagramSystem.h"
#include "Quest.h"
#include "QuestWorld.h"
#include "SkillManager.h"
#include "SocketManager.h"

CMemoryAllocatorInfo::CMemoryAllocatorInfo()
{
	this->Clear();
}

void CMemoryAllocatorInfo::Clear()
{
	m_Index = -1;
	m_Active = 0;
	m_ActiveTime = 0;
	m_SkillBackup = 0;
	m_Skill = 0;
	m_VpPlayer = 0;
	m_VpPlayer2 = 0;
	m_VpPlayerItem = 0;
	m_HitDamage = 0;
	m_Effect = 0;
	m_PerSocketContext = 0;
	m_Inventory1 = 0;
	m_Inventory2 = 0;
	m_InventoryMap1 = 0;
	m_InventoryMap2 = 0;
	m_Trade = 0;
	m_TradeMap = 0;
	m_Warehouse = 0;
	m_WarehouseMap = 0;
	m_ChaosBox = 0;
	m_ChaosBoxMap = 0;
	m_MasterSkill = 0;
	m_QuestKillCount = 0;
	m_QuestWorldList = 0;
	m_GensVictimList = 0;
	m_SkillDelay = 0;
	m_HackPacketDelay = 0;
	m_HackPacketCount = 0;
}

void CMemoryAllocatorInfo::Alloc()
{
	this->m_SkillBackup = new CSkill[MAX_SKILL_LIST];
	this->m_Skill = new CSkill[MAX_SKILL_LIST];
	this->m_VpPlayer = new VIEWPORT_STRUCT[MAX_VIEWPORT];
	this->m_VpPlayer2 = new VIEWPORT_STRUCT[MAX_VIEWPORT];
	this->m_VpPlayerItem = new VIEWPORT_STRUCT[MAX_VIEWPORT];
	this->m_HitDamage = new HIT_DAMAGE_STRUCT[MAX_HIT_DAMAGE];
	this->m_Effect = new CEffect[MAX_EFFECT_LIST];

	if(OBJECT_USER_RANGE(this->m_Index) != 0)
	{
		this->m_PerSocketContext = new PER_SOCKET_CONTEXT;
		this->m_Inventory1 = new CItem[INVENTORY_SIZE];
		this->m_Inventory2 = new CItem[INVENTORY_SIZE];
		this->m_InventoryMap1 = new BYTE[INVENTORY_SIZE];
		this->m_InventoryMap2 = new BYTE[INVENTORY_SIZE];
		this->m_Trade = new CItem[TRADE_SIZE];
		this->m_TradeMap = new BYTE[TRADE_SIZE];
		this->m_Warehouse = new CItem[WAREHOUSE_SIZE];
		this->m_WarehouseMap = new BYTE[WAREHOUSE_SIZE];
		this->m_ChaosBox = new CItem[CHAOS_BOX_SIZE];
		this->m_ChaosBoxMap = new BYTE[CHAOS_BOX_SIZE];
		this->m_MasterSkill = new CSkill[MAX_MASTER_SKILL_LIST];
		this->m_QuestKillCount = new QUEST_KILL_COUNT[MAX_QUEST_KILL_COUNT];
		this->m_QuestWorldList = new QUEST_WORLD_LIST[MAX_QUEST_WORLD_LIST];
		this->m_GensVictimList = new GENS_SYSTEM_VICTIM_LIST[MAX_GENS_SYSTEM_VICTIM];
		this->m_SkillDelay = new DWORD[MAX_SKILL];
		this->m_HackPacketDelay = new DWORD[MAX_HACK_PACKET_INFO];
		this->m_HackPacketCount = new DWORD[MAX_HACK_PACKET_INFO];
	}
}
