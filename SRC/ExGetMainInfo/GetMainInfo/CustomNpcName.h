#pragma once

#define MAX_CUSTOM_NPCNAME	100

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
	void Init();
	void Load(char * path);
	void SetInfo(IsNameData info);
	IsNameData m_CustomNpcName[MAX_CUSTOM_NPCNAME];
}; 
extern IsCustomNameNPC gNPCName;