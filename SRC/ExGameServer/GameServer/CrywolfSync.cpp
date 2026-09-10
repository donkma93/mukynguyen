#include "stdafx.h"
#include "CrywolfSync.h"

CCrywolfSync gCrywolfSync;

CCrywolfSync::CCrywolfSync()
{
	this->m_CrywolfEnable = 0;
	this->m_ApplyBenefit = 0;
	this->m_ApplyPenalty = 0;
	this->m_CrywolfState = 0;
	this->m_OccupationState = 1;
	this->m_PlusChaosRate = 0;
	this->m_GemDropPenaltyRate = 100;
	this->m_GettingExpPenaltyRate = 100;
	this->m_MinusMonHPBenefitRate = 100;
	this->m_KundunHPRefillState = 1;
}

BOOL CCrywolfSync::CheckEnableCrywolf()
{
	return this->m_CrywolfEnable;
}

BOOL CCrywolfSync::CheckApplyBenefit()
{
	return this->m_ApplyBenefit;
}

BOOL CCrywolfSync::CheckApplyPenalty()
{
	return this->m_ApplyPenalty;
}

void CCrywolfSync::SetEnableCrywolf(int EnableCrywolf)
{
	this->m_CrywolfEnable = EnableCrywolf;
}

void CCrywolfSync::SetApplyBenefit(int ApplyBenefit)
{
	this->m_ApplyBenefit = ApplyBenefit;
}

void CCrywolfSync::SetApplyPenalty(int ApplyPenalty)
{
	this->m_ApplyPenalty = ApplyPenalty;
}

void CCrywolfSync::SetCrywolfState(int CrywolfState)
{
	this->m_CrywolfState = CrywolfState;
}

void CCrywolfSync::SetOccupationState(int OccupationState)
{
	this->m_OccupationState = OccupationState;
}

void CCrywolfSync::SetPlusChaosRate(int PlusChaosRate)
{
	this->m_PlusChaosRate = PlusChaosRate;
}

void CCrywolfSync::SetGemDropPenaltiyRate(int GemDropPenaltyRate)
{
	this->m_GemDropPenaltyRate = GemDropPenaltyRate;
}

void CCrywolfSync::SetGettingExpPenaltyRate(int GettingExpPenaltyRate)
{
	this->m_GettingExpPenaltyRate = GettingExpPenaltyRate;
}

void CCrywolfSync::SetMonHPBenefitRate(int MinusMonHPBenefitRate)
{
	this->m_MinusMonHPBenefitRate = MinusMonHPBenefitRate;
}

void CCrywolfSync::SetKundunHPRefillState(int KundunHPRefillState)
{
	this->m_KundunHPRefillState = KundunHPRefillState;
}

int CCrywolfSync::GetCrywolfState()
{
	return this->m_CrywolfState;
}

int CCrywolfSync::GetOccupationState()
{
	return this->m_OccupationState;
}

int CCrywolfSync::GetPlusChaosRate()
{
	return this->m_PlusChaosRate;
}

int CCrywolfSync::GetGemDropPenaltiyRate()
{
	return this->m_GemDropPenaltyRate;
}

int CCrywolfSync::GetGettingExpPenaltyRate()
{
	return this->m_GettingExpPenaltyRate;
}

int CCrywolfSync::GetMonHPBenefitRate()
{
	return this->m_MinusMonHPBenefitRate;
}

int CCrywolfSync::GetKundunHPRefillState()
{
	return this->m_KundunHPRefillState;
}
