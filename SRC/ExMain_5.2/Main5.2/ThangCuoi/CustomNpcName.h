#pragma once

#define MAX_CUSTOM_NPCNAME 100

struct IsNameData
{
    int Index;
    int MonsterID;
    int Map;
    int X;
    int Y;
    char Name[32];
};

class IsCustomNameNPC
{
public:
    IsCustomNameNPC();
    void Load(IsNameData* info);
    IsNameData* GetNPCName(int MonsterID, int Map, int X, int Y);

    void Init();
    void SetInfo(const IsNameData& info);
    void SetNPCNameAttribute(CHARACTER* pNpc, int MonsterID, int X, int Y);
    IsNameData* GetNPCNameByIndex(int index);
    IsNameData m_CustomNpcName[MAX_CUSTOM_NPCNAME];
};

extern IsCustomNameNPC gNPCName;
