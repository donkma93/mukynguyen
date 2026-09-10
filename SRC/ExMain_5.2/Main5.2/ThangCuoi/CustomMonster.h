#pragma once

#define MAX_CUSTOMMONSTER 100
#define MAX_BOSSCLASS 100
enum eMonsterType
{
	CUSTOM_MONSTER_TYPE_NPC = 0,
	CUSTOM_MONSTER_TYPE_MOB = 1,
};

struct CUSTOMMONSTER_DATA
{
	int Index;
	WORD ID;
	BYTE Type;
	char Name[125];
	char Dir[50];
	char Folder[50];
	char BMDFile[50];
	float Size;
};

struct BOSSCLASS_DATA
{
	int Index;
	WORD ID;
	char Name[125];
	int SlotElement[5];
	float Size;
};
class CCustomMonster
{
public:
	CCustomMonster();
	virtual ~CCustomMonster();
	void Init();
	void Load(CUSTOMMONSTER_DATA* info);
	void SetInfo(CUSTOMMONSTER_DATA info);
	CUSTOMMONSTER_DATA* GetInfoByMonsterID(int MonsterID);
	CUSTOMMONSTER_DATA* GetInfoByIndex(int Index);
	int CheckCustomMonster(int MonsterID);


	void LoadBossClass(BOSSCLASS_DATA* info);
	void SetInfoBossClass(BOSSCLASS_DATA info);
	BOSSCLASS_DATA* GetInfoByIDBoss(int MonsterID);
	BOSSCLASS_DATA* GetInfoBossByIndex(int Index);

	CUSTOMMONSTER_DATA m_CustomMonster[MAX_CUSTOMMONSTER];
	BOSSCLASS_DATA m_CustomBossClass[MAX_BOSSCLASS];

	char* CCustomMonster::GetNameMonterCustom(int Type);
};

extern CCustomMonster gCustomMonster;