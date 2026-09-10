#include "stdafx.h"
#include "CustomIconBuff.h"

#include "MemScript.h"

CBuffIcon gCBuffIcon;

CBuffIcon::CBuffIcon()
{
	this->Init();
}

void CBuffIcon::Init()
{
	for (int n = 0; n < 256; n++)
	{
		this->m_TooltipENG[n].IndexBuff = -1;
	}
}

void CBuffIcon::Load(char* path, int Type) // OK
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

	int CustomItemIndexCount = 0;

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

			RenderTooltipBuff info;

			memset(&info, 0, sizeof(info));

			info.Index = CustomItemIndexCount++;

			info.IndexBuff = lpMemScript->GetNumber();

			info.Arg1 = lpMemScript->GetAsNumber();

			info.Arg2 = lpMemScript->GetAsNumber();

			info.Arg3 = lpMemScript->GetAsNumber();

			strcpy_s(info.NameBuff, lpMemScript->GetAsString());

			info.Arg4 = lpMemScript->GetAsNumber();

			info.Arg5 = lpMemScript->GetAsNumber();

			info.Arg6 = lpMemScript->GetAsNumber();

			strcpy_s(info.InfoBuff, lpMemScript->GetAsString());

			if (Type == 0)
			{
				this->m_TooltipENG[CustomItemIndexCount - 1] = info;
			}

			//printf("[CBuffIcon::Load] Index=%d, IndexBuff=%d, Arg1=%d, Arg2=%d, Arg3=%d, NameBuff=%s, Arg4=%d, Arg5=%d, Arg6=%d, InfoBuff=%s\n",
			//	info.Index,
			//	info.IndexBuff,
			//	info.Arg1,
			//	info.Arg2,
			//	info.Arg3,
			//	info.NameBuff,
			//	info.Arg4,
			//	info.Arg5,
			//	info.Arg6,
			//	info.InfoBuff);

		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}