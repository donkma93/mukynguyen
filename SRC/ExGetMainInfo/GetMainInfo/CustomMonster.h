#pragma once
#define MAX_CUSTOMMONSTER	100
#define MAX_BOSSCLASS		100

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
class cCustomMonsters
{
	public:
	cCustomMonsters();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOMMONSTER_DATA info);
	void SetInfoBossClass(BOSSCLASS_DATA info);
	CUSTOMMONSTER_DATA m_Monsters[MAX_CUSTOMMONSTER];
	BOSSCLASS_DATA m_BossClass[MAX_BOSSCLASS];
}; 
extern cCustomMonsters gMonsters;
