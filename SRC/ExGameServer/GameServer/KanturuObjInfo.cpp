#include "stdafx.h"
#include "KanturuObjInfo.h"

void CKanturuObjInfo::Reset()
{
	this->m_ObjCount = 0;

	for(int n=0;n < MAX_KANTURU_OBJ_INDEX;n++)
	{
		this->m_ObjIndex[n] = -1;
	}
}

bool CKanturuObjInfo::AddObj(int aIndex)
{
	if(this->m_ObjCount < 0 || this->m_ObjCount >= MAX_KANTURU_OBJ_INDEX)
	{
		return 0;
	}

	this->m_ObjIndex[this->m_ObjCount++] = aIndex;
	return 1;
}

int CKanturuObjInfo::GetCount()
{
	return this->m_ObjCount;
}
