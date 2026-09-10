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

class CustomModelNPC
{
	public:
	CustomModelNPC();
	void Init();
	void Load(CustomModelNpcInfo* info);
	bool IsMonster(int MonsterID) const;
	void MakeNpcModel(CHARACTER* c, int MonsterID);
	std::vector<CustomModelNpcInfo> m_CustomModelInfo;
};

extern CustomModelNPC gCustomModelNPC;