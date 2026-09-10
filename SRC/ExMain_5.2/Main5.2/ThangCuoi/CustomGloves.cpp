#include "stdafx.h"
#include "CustomGloves.h"

cCustomGloves gCustomBattleGloves;

cCustomGloves::cCustomGloves() { m_CustomGlovesInfo.clear(); }

void cCustomGloves::Load(CUSTOM_RF_GLOVES* info)
{
	for (int n = 0; n < MAX_CUSTOM_GLOVES; n++)
	{
		if (info[n].Index < 0 || info[n].Index >= MAX_CUSTOM_GLOVES) return;
		m_CustomGlovesInfo[info[n].ItemIndex] = info[n];
	}
}

int cCustomGloves::GetItemIndex(int ItemIndex)
{
	auto it = m_CustomGlovesInfo.find(ItemIndex);
	return (it != m_CustomGlovesInfo.end()) ? it->second.ItemIndex : -1;
}

bool cCustomGloves::CheckGloves(int ItemIndex)
{
	return m_CustomGlovesInfo.find(ItemIndex) != m_CustomGlovesInfo.end();
}
