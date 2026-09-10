#pragma once

#define MAX_CUSTOM_MONSTERGLOW 100

struct CUSTOM_GLOW_INFO
{
	int Index;
	int MonsterID;
	int Golden;
	int brightness;
};

class cCustomGlow
{
	public:
	cCustomGlow();
	void Init();
	void Load(CUSTOM_GLOW_INFO* info);
	bool IsGolden(int MonsterID) const;
	bool HasBrightness(int MonsterID) const;
	std::vector<CUSTOM_GLOW_INFO> m_CustombrightnessInfo;
};

extern cCustomGlow gCustomMonsterGlow;
