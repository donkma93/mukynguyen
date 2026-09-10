#pragma once

struct CustomModelNpcInfo
{
	int Index;
	int MonsterID;
	int Class;
	int SetItemID;
	int LevelItemID;
	int WingItemID;
	short CurrentAction;
	float Scale;
	float BoundingBoxMax;
	int Weapon[2];
	int Type;
};

class CustomModelNpc
{
	public:
	CustomModelNpc();
	void Init();
	void Load(char* path);
	void SetInfo(CustomModelNpcInfo info);
	CustomModelNpcInfo m_CustomModelInfo[50];
};

extern CustomModelNpc gCustomModelNpc;