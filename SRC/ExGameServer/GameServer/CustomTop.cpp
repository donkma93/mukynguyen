#include "stdafx.h"
#include "CommandManager.h"
#include "CustomTop.h"
#include "Log.h"
#include "MemScript.h"

#include "Notice.h"
#include "Util.h"

CCustomTop gCustomTop;

CCustomTop::CCustomTop()
{
	this->m_CustomTopInfo.clear();
}

void CCustomTop::Load(char* path)
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

	this->m_CustomTopInfo.clear();

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

			CUSTOMTOP_INFO info;

			info.Index = lpMemScript->GetNumber();

			strcpy_s(info.Name,lpMemScript->GetAsString());

			strcpy_s(info.Title,lpMemScript->GetAsString());

			this->m_CustomTopInfo.insert(std::pair<int,CUSTOMTOP_INFO>(info.Index,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CCustomTop::GetInfo(int index,CUSTOMTOP_INFO* lpInfo)
{
	std::map<int,CUSTOMTOP_INFO>::iterator it = this->m_CustomTopInfo.find(index);

	if(it == this->m_CustomTopInfo.end())
	{
		return 0;
	}
	else
	{
		(*lpInfo) = it->second;
		return 1;
	}
}

bool CCustomTop::GetInfoByName(char* message,CUSTOMTOP_INFO* lpInfo)
{
	for(std::map<int,CUSTOMTOP_INFO>::iterator it=this->m_CustomTopInfo.begin();it != this->m_CustomTopInfo.end();it++)
	{
		if(_stricmp(it->second.Name,message) == 0)
		{
			(*lpInfo) = it->second;
			return 1;
		}
	}

	return 0;
}

void CCustomTop::GetTop(LPOBJ lpObj,int index)
{
	CUSTOMTOP_INFO CustomTopInfo;

	if(this->GetInfo(index,&CustomTopInfo) == 0)
	{
		return;
	}

//	if(CustomTopInfo.MinLevel != -1 && lpObj->Level < CustomTopInfo.MinLevel)
//	{
//		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessageNew.GetMessage(252),CustomTopInfo.MinLevel);
//		return;
//	}

}