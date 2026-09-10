#pragma once

class CRaklionBattleUser
{
public:
	CRaklionBattleUser();
	void ResetData();
	void SetIndex(int aIndex);
	BOOL IsUseData();
	int GetIndex();
private:
	int m_Index;
	int m_IsUse;
};
