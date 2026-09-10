#pragma once

#define MAX_CUSTOM_BOW 100

struct CUSTOM_BOW_INFO
{
	int Index;
	int ItemIndex;
	int Type;
	int Skill;
	char ModelName[32];

	int ColorR;
	int ColorG;
	int ColorB;

};

class CCustomBow
{
public:
	CCustomBow();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_BOW_INFO info);
	CUSTOM_BOW_INFO m_CustomBow[MAX_CUSTOM_BOW];
};

extern CCustomBow gCustomBow;
