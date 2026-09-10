#include "stdafx.h"
#include "CustomNpcName.h"
#include <Utilities/Log/muConsoleDebug.h>
#include <MapManager.h>

IsCustomNameNPC gNPCName;

IsCustomNameNPC::IsCustomNameNPC()
{
    Init();
}

void IsCustomNameNPC::Init()
{
    for (auto& npc : m_CustomNpcName)
    {
        npc.Index = -1;
    }
}

void IsCustomNameNPC::Load(IsNameData* info)
{
    for (int n = 0; n < MAX_CUSTOM_NPCNAME; ++n)
    {
        SetInfo(info[n]);
    }
}

void IsCustomNameNPC::SetInfo(const IsNameData& info)
{
    if (info.Index < 0 || info.Index >= MAX_CUSTOM_NPCNAME)
    {
        return;
    }

    m_CustomNpcName[info.Index] = info;
}

IsNameData* IsCustomNameNPC::GetNPCName(int MonsterID, int Map, int X, int Y)
{
    for (int n = 0; n < MAX_CUSTOM_NPCNAME; ++n)
    {
        IsNameData* lpInfo = GetNPCNameByIndex(n);

        if (!lpInfo)
        {
            continue;
        }

        if ((lpInfo->MonsterID == MonsterID && lpInfo->Map == Map &&
            (lpInfo->X == X || lpInfo->X == -1) &&
            (lpInfo->Y == Y || lpInfo->Y == -1)))
        {
            return lpInfo;
        }
    }
    return nullptr;
}

IsNameData* IsCustomNameNPC::GetNPCNameByIndex(int index)
{
    if (index < 0 || index >= MAX_CUSTOM_NPCNAME || m_CustomNpcName[index].Index != index)
    {
        return nullptr;
    }
    return &m_CustomNpcName[index];
}

void IsCustomNameNPC::SetNPCNameAttribute(CHARACTER* pNpc, int MonsterID, int X, int Y)
{
    IsNameData* info = this->GetNPCName(MonsterID, gMapManager.WorldActive, X, Y);

    if (info)
    {
        strncpy_s(pNpc->ID, sizeof(pNpc->ID), info->Name, _TRUNCATE);
        /*g_ConsoleDebug->Write(3,
            "[SetNPCNameAttribute] MonsterID: %d | Map: %d | X: %d | Y: %d | Name: %s",
            MonsterID, gMapManager.WorldActive, X, Y, info->Name);*/
    }
}