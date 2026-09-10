#pragma once

#define MAX_CUSTOM_BOW 100
#include <map>

struct CUSTOM_BOW_INFO
{
	int Index, ItemIndex, Type, Skill;
	char ModelName[32];
	int ColorR, ColorG, ColorB;
};

class CCustomBow
{
public:
	CCustomBow();
	void Init();
	void Load(CUSTOM_BOW_INFO* info);
	void SetInfo(CUSTOM_BOW_INFO info);
	bool CheckIsBow(int ItemIndex);
	bool CheckIsCrossBow(int ItemIndex);
	int GetSkillNumber(int ItemIndex);
	int CheckIsIndexCross(int ItemIndex);

	std::map<int, CUSTOM_BOW_INFO> m_CustomBowInfo;
};

extern CCustomBow gCustomBow;
