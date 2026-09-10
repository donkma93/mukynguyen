#include "stdafx.h"
#include "CustomBow.h"
#include "MemScript.h"

CCustomBow gCustomBow;

CCustomBow::CCustomBow()
{
	this->Init();
}

void CCustomBow::Init()
{
	for (int i = 0; i < MAX_CUSTOM_BOW; i++)
	{
		this->m_CustomBow[i].ItemIndex = -1;
	}
}

void CCustomBow::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

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

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_BOW_INFO info;
			memset(&info, 0, sizeof(info));

			info.Index		= lpMemScript->GetNumber();
			info.ItemIndex	= lpMemScript->GetAsNumber();
			info.Type		= lpMemScript->GetAsNumber();
			info.Skill		= lpMemScript->GetAsNumber();

			const char* modelName = lpMemScript->GetAsString();
			if (modelName == nullptr || strlen(modelName) == 0)
			{
				continue;
			}
			strcpy_s(info.ModelName, sizeof(info.ModelName), modelName);

			info.ColorR = lpMemScript->GetAsNumber();
			info.ColorG = lpMemScript->GetAsNumber();
			info.ColorB = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomBow::SetInfo(CUSTOM_BOW_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_BOW)
	{
		return;
	}

	this->m_CustomBow[info.Index] = info;

	//printf("ID: %d | ItemID: %d | Type: %d | Skill: %d | File:%s.bmd | %d %d %d\n",
	//	  info.Index
	//	, info.ItemIndex
	//	, info.Type
	//	, info.Skill
	//	, info.ModelName
	//	, info.ColorR
	//	, info.ColorG
	//	, info.ColorB
	//);
}

