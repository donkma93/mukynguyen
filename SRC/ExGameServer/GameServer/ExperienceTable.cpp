#include "stdafx.h"
#include "ExperienceTable.h"
#include "MemScript.h"
#include "Util.h"

CExperienceTable gExperienceTable;

CExperienceTable::CExperienceTable()
{
	this->m_ExperienceTableInfo.clear();
	this->m_MasterMapMoney.clear();
}

void CExperienceTable::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ExperienceTableInfo.clear();
	this->m_MasterMapMoney.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(lpMemScript->GetToken() == TOKEN_END)
					{
						break;
					}

					if(strcmp("end", lpMemScript->GetString()) == 0)
					{
						break;
					}

					EXPERIENCE_TABLE_INFO info;

					info.MinLevel = lpMemScript->GetNumber();
					info.MaxLevel = lpMemScript->GetAsNumber();
					info.MinMasterLevel = lpMemScript->GetAsNumber();
					info.MaxMasterLevel = lpMemScript->GetAsNumber();
					info.MinReset = lpMemScript->GetAsNumber();
					info.MaxReset = lpMemScript->GetAsNumber();
					info.MinMasterReset = lpMemScript->GetAsNumber();
					info.MaxMasterReset = lpMemScript->GetAsNumber();
					info.ExperienceRate = lpMemScript->GetAsNumber();
					info.MoneyRate = lpMemScript->GetAsNumber();

					this->m_ExperienceTableInfo.push_back(info);
				}
				else if(section == 1)
				{
					if(lpMemScript->GetToken() == TOKEN_END)
					{
						break;
					}

					if(strcmp("end", lpMemScript->GetString()) == 0)
					{
						break;
					}

					int map = lpMemScript->GetNumber();
					int rate = lpMemScript->GetAsNumber();
					this->m_MasterMapMoney[map] = rate;
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CExperienceTable::GetExperienceRate(LPOBJ lpObj)
{
	for(std::vector<EXPERIENCE_TABLE_INFO>::iterator it=this->m_ExperienceTableInfo.begin();it != this->m_ExperienceTableInfo.end();it++)
	{
		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->MinMasterReset != -1 && it->MinMasterReset > lpObj->MasterReset)
		{
			continue;
		}

		if(it->MaxMasterReset != -1 && it->MaxMasterReset < lpObj->MasterReset)
		{
			continue;
		}

		return it->ExperienceRate;
	}

	return 100;
}

int CExperienceTable::GetMasterExperienceRate(LPOBJ lpObj)
{
	for(std::vector<EXPERIENCE_TABLE_INFO>::iterator it=this->m_ExperienceTableInfo.begin();it != this->m_ExperienceTableInfo.end();it++)
	{
		if(it->MinMasterLevel != -1 && it->MinMasterLevel > lpObj->MasterLevel)
		{
			continue;
		}

		if(it->MaxMasterLevel != -1 && it->MaxMasterLevel < lpObj->MasterLevel)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->MinMasterReset != -1 && it->MinMasterReset > lpObj->MasterReset)
		{
			continue;
		}

		if(it->MaxMasterReset != -1 && it->MaxMasterReset < lpObj->MasterReset)
		{
			continue;
		}

		return it->ExperienceRate;
	}

	return 100;
}

int CExperienceTable::GetMoneyDropRate(LPOBJ lpObj)
{
	std::map<int, int>::iterator mapIt = this->m_MasterMapMoney.find(lpObj->Map);

	if (mapIt != this->m_MasterMapMoney.end() && mapIt->second > 0)
	{
		return mapIt->second;
	}

	for (std::vector<EXPERIENCE_TABLE_INFO>::iterator it = this->m_ExperienceTableInfo.begin(); it != this->m_ExperienceTableInfo.end(); it++)
	{
		if (it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if (it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if (it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if (it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if (it->MinMasterReset != -1 && it->MinMasterReset > lpObj->MasterReset)
		{
			continue;
		}

		if (it->MaxMasterReset != -1 && it->MaxMasterReset < lpObj->MasterReset)
		{
			continue;
		}

		return ((it->MoneyRate > 0) ? it->MoneyRate : 100);
	}

	return 100;
}
