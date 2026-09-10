#include "stdafx.h"
#include "RaklionBattleUser.h"

CRaklionBattleUser::CRaklionBattleUser()
{
	this->ResetData();
}

void CRaklionBattleUser::ResetData()
{
	this->m_Index = -1;
	this->m_IsUse = 0;
}

void CRaklionBattleUser::SetIndex(int aIndex)
{
	this->m_Index = aIndex;
	this->m_IsUse = 1;
}

BOOL CRaklionBattleUser::IsUseData()
{
	return this->m_IsUse;
}

int CRaklionBattleUser::GetIndex()
{
	return this->m_Index;
}
