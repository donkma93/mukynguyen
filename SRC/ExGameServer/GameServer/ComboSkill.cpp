#include "stdafx.h"
#include "ComboSkill.h"
#include "CustomCombo.h"
#include "ServerInfo.h"
#include "SkillManager.h"


void CComboSkill::Init()
{
	this->m_time = 0;
	this->m_skill[0] = 0xFFFF;
	this->m_skill[1] = 0xFFFF;
	this->m_index = -1;
}

int CComboSkill::GetSkillType(WORD skill)
{
	if (gServerInfo.m_EnableComboToAllSwitch == 1)
	{
		if(gCustomCombo.CheckSkillPrimary(skill) == 1)
		{
			return 0;
		}
		else if(gCustomCombo.CheckSkillSecundary(skill) == 1)
		{
			return 1;
		}
		else
		{
			return -1;
		}	
	}
	else
	{
		if(skill == SKILL_FALLING_SLASH || skill == SKILL_LUNGE || skill == SKILL_UPPERCUT || skill == SKILL_CYCLONE || skill == SKILL_SLASH)
		{
			return 0;
		}
		else if(skill == SKILL_TWISTING_SLASH || skill == SKILL_RAGEFUL_BLOW || skill == SKILL_DEATH_STAB || skill == SKILL_FROZEN_STAB || skill == SKILL_BLOOD_STORM)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

bool CComboSkill::CheckCombo(WORD skill)
{
	int type = this->GetSkillType(skill);
#if(ACuoi_COMBO_1SKILL)
	if (skill == SKILL_ICE_STORM && ((rand() % 100) < gServerInfo.m_TyLeRaComboDW))
	{
		return 1;
	}


	if (skill == SKILL_DEATH_STAB && ((rand() % 100) < gServerInfo.m_TyLeRaComboDK))
	{
		return 1;
	}

	if (skill == SKILL_FIVE_SHOT && ((rand() % 100) < gServerInfo.m_TyLeRaComboEF))
	{
		return 1;
	}

	if (skill == SKILL_SWORD_SLASH && ((rand() % 100) < gServerInfo.m_TyLeRaComboMG))
	{
		return 1;
	}

	if (skill == SKILL_FIRE_BURST && ((rand() % 100) < gServerInfo.m_TyLeRaComboDL))
	{
		return 1;
	}

	if (skill == SKILL_DRAIN_LIFE && ((rand() % 100) < gServerInfo.m_TyLeRaComboSM))
	{
		return 1;
	}

	if (skill == SKILL_DARK_SIDE && ((rand() % 100) < gServerInfo.m_TyLeRaComboRF))
	{
		return 1;
	}

#endif
	if(type == -1)
	{
		this->Init();
		return 0;
	}

	if(type == 0)
	{
		this->m_time = GetTickCount()+3000;
		this->m_skill[0] = skill;
		this->m_index = 0;
		return 0;
	}

	if(type == 1)
	{
		if(this->m_time < GetTickCount())
		{
			this->Init();
			return 0;
		}

		if(this->m_skill[0] == 0xFFFF)
		{
			this->Init();
			return 0;
		}

		if(this->m_index == 0)
		{
			this->m_time = GetTickCount()+3000;
			this->m_skill[1] = skill;
			this->m_index = 1;
			return 0;
		}
		
		if(this->m_index == 1 && this->m_skill[1] != skill)
		{
			this->Init();
			return 1;
		}
	}

	this->Init();
	return 0;
}
