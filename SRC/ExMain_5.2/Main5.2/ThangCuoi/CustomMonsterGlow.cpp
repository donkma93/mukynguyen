#include "stdafx.h"
#include "CustomMonsterGlow.h"

cCustomGlow gCustomMonsterGlow;

cCustomGlow::cCustomGlow()
{
	Init();
}

void cCustomGlow::Init()
{
	m_CustombrightnessInfo.clear();
}

void cCustomGlow::Load(CUSTOM_GLOW_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_MONSTERGLOW; ++n)
	{
		if (info[n].Index >= 0 && info[n].Index < MAX_CUSTOM_MONSTERGLOW)
		{
			m_CustombrightnessInfo.push_back(info[n]);
		}
	}
}

bool cCustomGlow::IsGolden(int MonsterID) const
{
	for (const auto& data : m_CustombrightnessInfo)
	{
		if (data.MonsterID == MonsterID && data.Golden == 1)
			return true;
	}
	return false;
}

bool cCustomGlow::HasBrightness(int MonsterID) const
{
	for (const auto& data : m_CustombrightnessInfo)
	{
		if (data.MonsterID == MonsterID && (data.Golden == 1 || data.brightness == 1))
			return true;
	}
	return false;
}
