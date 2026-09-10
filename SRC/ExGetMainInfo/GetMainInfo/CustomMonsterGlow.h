#pragma once

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
	virtual ~cCustomGlow();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_GLOW_INFO info);
	CUSTOM_GLOW_INFO m_CustomGlowInfo[MAX_CUSTOM_MONSTERGLOW];
};

extern cCustomGlow gCustomMonsterGlow;