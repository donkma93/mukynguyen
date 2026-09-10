#include "stdafx.h"
#include "BlackList.h"
#include "MemScript.h"
#include "Util.h"
#include <fstream>
#include "Path.h"
using namespace std;

CBlackList gBlackList;

CBlackList::CBlackList()
{
	this->m_BlackListIpInfo.clear();

	this->m_BlackListHidInfo.clear();
}

void CBlackList::Load(char* path)
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

	this->m_BlackListIpInfo.clear();

	this->m_BlackListHidInfo.clear();

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
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_IP_INFO info;

					memset(&info,0,sizeof(info));

					strcpy_s(info.IpAddress,lpMemScript->GetString());

					this->m_BlackListIpInfo.insert(std::pair<std::string,BLACKLIST_IP_INFO>(std::string(info.IpAddress),info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_HID_INFO info;

					memset(&info,0,sizeof(info));

					strcpy_s(info.HardwareId,lpMemScript->GetString());

					this->m_BlackListHidInfo.insert(std::pair<std::string,BLACKLIST_HID_INFO>(std::string(info.HardwareId),info));
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

bool CBlackList::CheckIpAddress(char* IpAddress)
{
	std::map<std::string,BLACKLIST_IP_INFO>::iterator it = this->m_BlackListIpInfo.find(std::string(IpAddress));

	if(it == this->m_BlackListIpInfo.end())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CBlackList::CheckHardwareId(char* HardwareId)
{
	std::string filename = gPath.GetFullPath("BlackList.txt");
	std::ifstream file(filename);
	if (!file.is_open())
		return true;

	std::string line;
	bool inSection1 = false;

	while (std::getline(file, line))
	{
		std::string trimmed = line;
		trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

		if (trimmed == "1") 
		{
			inSection1 = true;
			continue;
		}
		else if (trimmed == "end")
		{
			inSection1 = false;
			continue;
		}

		if (inSection1 && trimmed == HardwareId)
		{
			return false;
		}
	}
	return true; 
}


bool CBlackList::AddBlackList(char* BlackList, char* type)
{
	const char* prefix1 = "IP: ";
	const char* prefix2 = "HWID: ";

	if (strncmp(BlackList, prefix1, strlen(prefix1)) == 0)
	{
		BlackList += strlen(prefix1);
	}
	else if (strncmp(BlackList, prefix2, strlen(prefix2)) == 0)
	{
		BlackList += strlen(prefix2);
	}

	std::string filename = gPath.GetFullPath("BlackList.txt");
	std::ifstream finput(filename);
	if (!finput)
	{
		MessageBoxA(0, "Cannot open BlackList.txt for reading", "AntiThangCuoi", MB_ICONERROR | MB_OK);
		return false;
	}

	std::vector<std::string> lines;
	std::string line;
	bool inTargetSection = false;
	bool alreadyExists = false;
	size_t insertPos = -1;

	while (std::getline(finput, line))
	{
		std::string trimmed = line;
		trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

		if (trimmed == type)
		{
			inTargetSection = true;
			insertPos = lines.size() + 1;
		}
		else if (trimmed == "end")
		{
			inTargetSection = false;
		}
		else if (inTargetSection && trimmed == BlackList)
		{
			alreadyExists = true;
		}

		lines.push_back(line);
	}
	finput.close();

	if (alreadyExists)
	{
		char msg[512];
		sprintf_s(msg, sizeof(msg), "%s already exists:\n\n%s\n\nFile:\n%s",
			(type[0] == '0') ? "IP" : "HWID", BlackList, filename.c_str());
		MessageBoxA(0, msg, "AntiThangCuoi", MB_ICONWARNING | MB_OK);
		return false;
	}

	if (insertPos == -1)
	{
		char msg[256];
		sprintf_s(msg, "Section %s not found in BlackList.txt", type);
		MessageBoxA(0, msg, "AntiThangCuoi", MB_ICONERROR | MB_OK);
		return false;
	}

	lines.insert(lines.begin() + insertPos, BlackList);

	std::ofstream foutput(filename);
	if (!foutput)
	{
		MessageBoxA(0, "Cannot open BlackList.txt for writing", "AntiThangCuoi", MB_ICONERROR | MB_OK);
		return false;
	}

	for (const auto& l : lines)
	{
		foutput << l << "\n";
	}
	foutput.close();

	return true;
}

