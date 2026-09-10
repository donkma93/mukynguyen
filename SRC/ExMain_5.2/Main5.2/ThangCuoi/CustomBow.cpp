#include "stdafx.h"
#include "CustomBow.h"

CCustomBow gCustomBow;

CCustomBow::CCustomBow() { Init(); }

void CCustomBow::Init() { m_CustomBowInfo.clear(); }

void CCustomBow::Load(CUSTOM_BOW_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_BOW; n++) SetInfo(info[n]);
}

void CCustomBow::SetInfo(CUSTOM_BOW_INFO info)
{
	if (info.ItemIndex < 0 || info.Index < 0 || info.Index >= MAX_CUSTOM_BOW) return;
	m_CustomBowInfo[info.ItemIndex] = info;
}

bool CCustomBow::CheckIsBow(int ItemIndex)
{
	auto it = m_CustomBowInfo.find(ItemIndex);
	return (it != m_CustomBowInfo.end() && it->second.Type == 1);
}

bool CCustomBow::CheckIsCrossBow(int ItemIndex)
{
	auto it = m_CustomBowInfo.find(ItemIndex);
	return (it != m_CustomBowInfo.end() && it->second.Type == 2);
}

int CCustomBow::GetSkillNumber(int ItemIndex)
{
	auto it = m_CustomBowInfo.find(ItemIndex);
	return (it != m_CustomBowInfo.end()) ? it->second.Skill : 0;
}

int CCustomBow::CheckIsIndexCross(int ItemIndex)
{
	auto it = m_CustomBowInfo.find(ItemIndex);
	return (it != m_CustomBowInfo.end() && it->second.Type == 2) ? ItemIndex + MODEL_ITEM : -1;
}
