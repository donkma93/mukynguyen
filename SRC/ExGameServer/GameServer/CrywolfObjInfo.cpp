#include "stdafx.h"
#include "CrywolfObjInfo.h"

CCrywolfObjInfo::CCrywolfObjInfo()
{
	this->Reset();
}

void CCrywolfObjInfo::Reset()
{
	this->m_ObjCount = 0;

	for(int n=0;n < MAX_CRYWOLF_OBJ_INFO;n++)
	{
		this->m_ObjIndex[n] = -1;
	}
}

bool CCrywolfObjInfo::AddObj(int aIndex)
{
	if(this->m_ObjCount < 0 || this->m_ObjCount >= MAX_CRYWOLF_OBJ_INFO)
	{
		return 0;
	}

	this->m_ObjIndex[this->m_ObjCount++] = aIndex;
	return 1;
}
