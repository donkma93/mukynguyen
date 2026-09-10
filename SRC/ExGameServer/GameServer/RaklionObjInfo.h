#pragma once

#define MAX_RAKLION_OBJ_INDEX 800

class CRaklionObjInfo
{
public:
	CRaklionObjInfo();
	void Reset();
	bool AddObj(int aIndex);
	int GetCount();
private:
	int m_ObjCount;
	int m_ObjIndex[MAX_RAKLION_OBJ_INDEX];
};
