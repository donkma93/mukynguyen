// ExperienceTable.h: interface for the CExperienceTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include <map>

struct EXPERIENCE_TABLE_INFO
{
	int MinLevel;
	int MaxLevel;
	int MinMasterLevel;
	int MaxMasterLevel;
	int MinReset;
	int MaxReset;
	int MinMasterReset;
	int MaxMasterReset;
	int ExperienceRate;
	int MoneyRate;
};

class CExperienceTable
{
public:
	CExperienceTable();
	void Load(char* path);
	int GetExperienceRate(LPOBJ lpObj);
	int GetMasterExperienceRate(LPOBJ lpObj);
	int GetMoneyDropRate(LPOBJ lpObj);
private:
	std::vector<EXPERIENCE_TABLE_INFO> m_ExperienceTableInfo;
	std::map<int, int> m_MasterMapMoney;
};

extern CExperienceTable gExperienceTable;
