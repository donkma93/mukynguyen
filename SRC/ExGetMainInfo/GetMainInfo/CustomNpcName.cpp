#include "stdafx.h"
#include "CustomNpcName.h"
#include "MemScript.h"

IsCustomNameNPC gNPCName;

void IsCustomNameNPC::Init()
{
	for (int n = 0; n < MAX_CUSTOM_NPCNAME; n++)
	{
		this->m_CustomNpcName[n].Index = -1;
	}
}

void IsCustomNameNPC::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			IsNameData IsGetInfo;
			memset(&IsGetInfo,0,sizeof(IsGetInfo));
			static int CustomIndexCount = 0;

			IsGetInfo.Index = CustomIndexCount++;
			IsGetInfo.MonsterID = lpMemScript->GetNumber();
			IsGetInfo.Map = lpMemScript->GetAsNumber();
			IsGetInfo.X = lpMemScript->GetAsNumber();
			IsGetInfo.Y = lpMemScript->GetAsNumber();
			strcpy_s(IsGetInfo.Name,lpMemScript->GetAsString());

			this->SetInfo(IsGetInfo);
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void IsCustomNameNPC::SetInfo(IsNameData IsGetInfo)
{
	if(IsGetInfo.Index < 0 || IsGetInfo.Index >= MAX_CUSTOM_NPCNAME)
	{
		return;
	}
	this->m_CustomNpcName[IsGetInfo.Index] = IsGetInfo;
}