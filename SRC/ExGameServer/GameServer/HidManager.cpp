#include "stdafx.h"
#include "HidManager.h"
#include "BlackList.h"
#include "Log.h"

#include "ServerInfo.h"
#include "Util.h"
#include "User.h"
#include "SocketManager.h"
CHidManager gHidManager;

bool CHidManager::CheckHardwareId(char* HardwareId)
{
	std::map<std::string,HARDWARE_ID_INFO>::iterator it = this->m_HardwareIdInfo.find(std::string(HardwareId));

	if(it == this->m_HardwareIdInfo.end())
	{
		return ((gServerInfo.m_MaxConnectionPerHID==0)?0:1);
	}
	else
	{
		return ((it->second.HardwareIdCount>=gServerInfo.m_MaxConnectionPerHID)?0:1);
	}
}

void CHidManager::InsertHardwareId(char* HardwareId)
{
	HARDWARE_ID_INFO info;

	strcpy_s(info.HardwareId,HardwareId);

	info.HardwareIdCount = 1;

	std::map<std::string,HARDWARE_ID_INFO>::iterator it = this->m_HardwareIdInfo.find(std::string(HardwareId));

	if(it == this->m_HardwareIdInfo.end())
	{
		this->m_HardwareIdInfo.insert(std::pair<std::string,HARDWARE_ID_INFO>(std::string(HardwareId),info));
	}
	else
	{
		it->second.HardwareIdCount++;
	}
}

void CHidManager::RemoveHardwareId(char* HardwareId)
{
	std::map<std::string,HARDWARE_ID_INFO>::iterator it = this->m_HardwareIdInfo.find(std::string(HardwareId));

	if(it != this->m_HardwareIdInfo.end())
	{
		if((--it->second.HardwareIdCount) == 0)
		{
			this->m_HardwareIdInfo.erase(it);
		}
	}
}

void CHidManager::CGHardwareIdRecv(PMSG_HARDWAREID_INFO_RECV* lpMsg, int aIndex)
{
	if (!OBJMAX_RANGE(aIndex)) return;

	char HardwareId[MAX_HWID_KEY] = { 0 };
	char ComputerName[32] = { 0 };
	char OSName[32] = { 0 };
	char OSVersion[8] = { 0 };
	char InstallDate[11] = { 0 };
	char SystemArch[8] = { 0 };
	char TotalRAM[32] = { 0 };
	char GPUName[48] = { 0 };

	LPOBJ lpObj = &gObj[aIndex];

	memcpy(HardwareId, lpMsg->HardwareId, sizeof(lpMsg->HardwareId));
	memcpy(ComputerName, lpMsg->ComputerName, sizeof(lpMsg->ComputerName));
	memcpy(OSName, lpMsg->OSName, sizeof(lpMsg->OSName));
	memcpy(OSVersion, lpMsg->OSVersion, sizeof(lpMsg->OSVersion));
	memcpy(InstallDate, lpMsg->InstallDate, sizeof(lpMsg->InstallDate));
	memcpy(SystemArch, lpMsg->SystemArch, sizeof(lpMsg->SystemArch));
	memcpy(TotalRAM, lpMsg->TotalRAM, sizeof(lpMsg->TotalRAM));
	memcpy(GPUName, lpMsg->GPUName, sizeof(lpMsg->GPUName));

	if (HardwareId[8] != '-' || HardwareId[17] != '-' || HardwareId[26] != '-' || HardwareId[35] != '-')
	{
		LogAdd(LOG_RED, "[AntiThangCuoi] Invalid HWID format: %s (aIndex: %d)", HardwareId, aIndex);
		gObjDel(aIndex);
		return;
	}

	if (strlen(HardwareId) <= 0 || strlen(HardwareId) > 44 || strlen(HardwareId) != 44)
	{
		LogAdd(LOG_RED, "[AntiThangCuoi] Invalid HWID length: %s (length: %zu)", HardwareId, strlen(HardwareId));
		gObjDel(aIndex);
		return;
	}

	if (gBlackList.CheckHardwareId(HardwareId) == 0)
	{
		LogAdd(LOG_RED, "[AntiThangCuoi] Blocked HWID (blacklist): %s", HardwareId);
		gObjDel(aIndex);
		return;
	}

	if (this->CheckHardwareId(HardwareId) == 0)
	{
		LogAdd(LOG_RED, "[AntiThangCuoi] HWID duplicated in session: %s", HardwareId);
		gObjDel(aIndex);
		return;
	}

	gObj[aIndex].ClientVerify = 1;

	strcpy_s(gObj[aIndex].HardwareId, HardwareId);
	strcpy_s(gObj[aIndex].ComputerName, ComputerName);
	strcpy_s(gObj[aIndex].OSName, OSName);
	strcpy_s(gObj[aIndex].OSVersion, OSVersion);
	strcpy_s(gObj[aIndex].InstallDate, InstallDate);
	strcpy_s(gObj[aIndex].SystemArch, SystemArch);
	strcpy_s(gObj[aIndex].TotalRAM, TotalRAM);
	strcpy_s(gObj[aIndex].GPUName, GPUName);

	//LogAdd(LOG_BLUE, "ComputerName     : %s", ComputerName);
	//LogAdd(LOG_BLUE, "OS Name          : %s", OSName);
	//LogAdd(LOG_BLUE, "OS Version       : %s", OSVersion);
	//LogAdd(LOG_BLUE, "Installed On     : %s", InstallDate);
	//LogAdd(LOG_BLUE, "Architecture     : %s", SystemArch);
	//LogAdd(LOG_BLUE, "TotalRAM         : %s", TotalRAM);
	//LogAdd(LOG_BLUE, "GPUName          : %s", GPUName);

	this->InsertHardwareId(HardwareId);
}


