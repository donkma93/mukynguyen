#include "stdafx.h"
#include "CustomEventTime.h"
#include "MemScript.h"

CEventTimeText gEventTimeText;

CEventTimeText::CEventTimeText()
{
	this->Init();
}

void CEventTimeText::Init()
{
	for(int n=0;n < MAX_EVENTTIME;n++)
	{
		this->m_CustomEventInfo[n].Index = -1;
	}
}

void CEventTimeText::Load(char* path)
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

			CUSTOM_EVENT_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = lpMemScript->GetNumber();

			strcpy_s(info.Name,lpMemScript->GetAsString());

			strcpy_s(info.Map,lpMemScript->GetAsString());

			strcpy_s(info.Gate,lpMemScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CEventTimeText::SetInfo(CUSTOM_EVENT_INFO info)
{
	if(info.Index < 0 || info.Index >= MAX_EVENTTIME)
	{
		return;
	}
	this->m_CustomEventInfo[info.Index] = info;
}
