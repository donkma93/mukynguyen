#include "stdafx.h"
#include "KanturuStateInfo.h"

CKanturuStateInfo::CKanturuStateInfo()
{
	this->ResetTimeInfo();
}

void CKanturuStateInfo::ResetTimeInfo()
{
	this->SetStateInfo(0);

	this->SetCondition(0);

	this->SetValue(0);
}

void CKanturuStateInfo::SetStateInfo(int state)
{
	this->m_State = state;
}

void CKanturuStateInfo::SetCondition(int condition)
{
	this->m_Condition = condition;
}

void CKanturuStateInfo::SetValue(int value)
{
	this->m_Value = value;
}

void CKanturuStateInfo::SetAppliedTime()
{
	this->m_AppliedTickCount = GetTickCount();
}

void CKanturuStateInfo::SetConditionAppliedTime()
{
	this->m_AppliedTickCount = ((this->GetCondition()==0)?this->m_AppliedTickCount:GetTickCount());
}

int CKanturuStateInfo::GetCondition()
{
	return this->m_Condition;
}

int CKanturuStateInfo::GetValue()
{
	return this->m_Value;
}

int CKanturuStateInfo::GetAppliedTime()
{
	return this->m_AppliedTickCount;
}

int CKanturuStateInfo::GetElapsedTime()
{
	return (GetTickCount()-this->GetAppliedTime());
}

int CKanturuStateInfo::GetRemainTime()
{
	return ((this->GetCondition()==0)?0:(((this->m_Value-this->GetElapsedTime())<=0)?0:((this->m_Value-this->GetElapsedTime())/1000)));
}

int CKanturuStateInfo::IsTimeOut()
{
	return ((this->GetRemainTime()>0)?0:1);
}
