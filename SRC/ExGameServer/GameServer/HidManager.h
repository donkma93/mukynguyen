#pragma once
#include "Protocol.h"

struct PMSG_HARDWAREID_INFO_RECV
{
	PSBMSG_HEAD header;
	char HardwareId[45];
	char ComputerName[32];
	char OSName[32];
	char OSVersion[8];
	char InstallDate[11];
	char SystemArch[8];
	char TotalRAM[32];
	char GPUName[48];
};

struct HARDWARE_ID_INFO
{
	char HardwareId[MAX_HWID_KEY];
	WORD HardwareIdCount;
};

class CHidManager
{
	public:
	bool CheckHardwareId(char* HardwareId);
	void InsertHardwareId(char* HardwareId);
	void RemoveHardwareId(char* HardwareId);
	void CGHardwareIdRecv(PMSG_HARDWAREID_INFO_RECV* lpMsg,int aIndex);
	private:
	std::map<std::string,HARDWARE_ID_INFO> m_HardwareIdInfo;
};

extern CHidManager gHidManager;
