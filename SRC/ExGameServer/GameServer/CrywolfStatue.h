#pragma once

#include "User.h"

class CCrywolfShield
{
	public:
	CCrywolfShield()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_PriestNumber = 0;
		this->m_ShieldHP = 0;
		this->m_ShieldMaxHP = 0;
		this->m_ShieldState = 0;
	}

	int GetHPPercentage()
	{
		return ((this->m_ShieldMaxHP==0)?0:((this->m_ShieldHP*100)/this->m_ShieldMaxHP));
	}
	int m_ShieldMaxHP;
	int m_ShieldHP;
	int m_ShieldState;
	int m_PriestNumber;
};

class CCrywolfStatue
{
	public:
	void CrywolfStatueAct(int aIndex);
	void SetStatueViewState(LPOBJ lpObj,int PriestNumber);
	CCrywolfShield m_Shield;
};

extern CCrywolfStatue gCrywolfStatue;
