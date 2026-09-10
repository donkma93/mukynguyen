#include "stdafx.h"
#include "CustomNpcNew.h"
#include "MemScript.h"

CustomModelNpc gCustomModelNpc;

CustomModelNpc::CustomModelNpc()
{
	this->Init();
}

void CustomModelNpc::Init()
{
	for (int n = 0; n < 50; n++)
	{
		this->m_CustomModelInfo[n].Index = -1;
	}
}

void CustomModelNpc::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	int Index1 = 0;
	int Index2 = 0;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int Secction = lpMemScript->GetNumber();

			while (true)
			{
				if (lpMemScript->GetToken() == TOKEN_END)
				{
					break;
				}

				if (strcmp("end", lpMemScript->GetString()) == 0)
				{
					break;
				}

				if (Secction == 0)
				{
					CustomModelNpcInfo info;

					info.Index = Index1++;
					info.MonsterID		= lpMemScript->GetNumber();
					info.Class			= lpMemScript->GetAsNumber();
					info.SetItemID		= lpMemScript->GetAsNumber();
					info.LevelItemID	= lpMemScript->GetAsNumber();
					info.WingItemID		= lpMemScript->GetAsNumber();
					info.CurrentAction	= (short)lpMemScript->GetAsNumber();
					info.Scale			= lpMemScript->GetAsFloatNumber();
					info.BoundingBoxMax = lpMemScript->GetAsFloatNumber();
					info.Weapon[0]		= lpMemScript->GetAsNumber();
					info.Weapon[1]		= lpMemScript->GetAsNumber();
					info.Type			= lpMemScript->GetAsNumber();
					this->SetInfo(info);
				}
			}
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CustomModelNpc::SetInfo(CustomModelNpcInfo info)
{
	if (info.Index < 0 || info.Index >= 50)
	{
		return;
	}
	this->m_CustomModelInfo[info.Index] = info;
}
