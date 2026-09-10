#pragma once

#include "User.h"

struct CUSTOM_MONSTER_INFO
{
	int Index;
	int MapNumber;
	int MaxLife;
	int DamageMin;
	int DamageMax;
	int Defense;
	int AttackRate;
	int DefenseRate;
	int ExperienceRate;
	int KillMessage;
	int InfoMessage;
	int RewardValue1;
	int RewardValue2;
	int RewardValue3;
	int RewardValue4;
	int SummonMonster;
	int SummonMonsterCount;
	int SummonMonsterRate;
	int DrawClient;
	int KillCountBoss;
};

class CCustomMonster
{
public:
	CCustomMonster();
	void Load(char* path);
	void SetCustomMonsterInfo(LPOBJ lpObj);
	void MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	long GetCustomMonsterExperienceRate(int index,int map);
	long GetCustomMonsterMasterExperienceRate(int index,int map);
	bool GetCustomMonsterInfo(int index,int map,CUSTOM_MONSTER_INFO* lpInfo);
	bool CCustomMonster::GetCustomMonsterDrawBXH(int index,int map,CUSTOM_MONSTER_INFO* lpInfo);
	void RewardKill(LPOBJ lpObj, LPOBJ lpTarget);
	void GDRankingBosSaveSend(int aIndex, DWORD Score);
private:
	std::vector<CUSTOM_MONSTER_INFO> m_CustomMonsterInfo;
};

extern CCustomMonster gCustomMonster;
