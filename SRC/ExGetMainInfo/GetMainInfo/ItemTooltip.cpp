#include "stdafx.h"
#include "ItemTooltip.h"
#include "MemScript.h"

nCInfo gItemToolTip;

nCInfo::nCInfo()
{
	this->Init();
}

void nCInfo::Init() // OK
{
	for( int n = 0 ; n < MaxLine ; n++ )
	{
		this->n_TRSTooltipData[n].ItemIndex = -1;
		this->n_TRSTooltipText[n].Index = -1;
	};
}

void nCInfo::Load(char* path, int type) // OK
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

	static int CustomItemIndex1Count = 0;
	static int CustomItemIndex2Count = 0;

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END || strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			if( type == 1 )
			{
				nInformation info;

				memset(&info,0,sizeof(info));

				int Section			= lpMemScript->GetNumber();
				int ID				= lpMemScript->GetAsNumber();
				info.ItemIndex		= (Section * 512) + ID;
				strcpy_s(info.OptionName, lpMemScript->GetAsString());
				info.OptionColor = lpMemScript->GetAsNumber();
				for (int i = 0 ; i < 15 ; i++ )
				{
					info.TextIndex[ i ] = lpMemScript->GetAsNumber();
					info.TextColor[ i ] = lpMemScript->GetAsNumber();
				}
				this->SetInfo1(info, CustomItemIndex1Count);
				CustomItemIndex1Count++;
			}
			else
			{
				nText info1;
				memset(&info1,0,sizeof(info1));
				info1.Index = lpMemScript->GetNumber();
				strcpy_s(info1.Text, lpMemScript->GetAsString());
				this->SetInfo2(info1, CustomItemIndex2Count);
				CustomItemIndex2Count++;
			}
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
	}
	delete lpMemScript;
}

void nCInfo::SetInfo1(nInformation info, int CustomItemIndexCount) // OK
{
	if(info.ItemIndex < 0 || info.ItemIndex >= 7680 || CustomItemIndexCount >= MaxLine)
	{
		return;
	}
	this->n_TRSTooltipData[CustomItemIndexCount] = info;
}

void nCInfo::SetInfo2(nText info, int CustomItemIndexCount) // OK
{
	if(info.Index < 0 || info.Index >= MaxLine)
	{
		return;
	}

	this->n_TRSTooltipText[CustomItemIndexCount] = info;
}
