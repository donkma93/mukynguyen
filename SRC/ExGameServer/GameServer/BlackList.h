#pragma once

struct BLACKLIST_IP_INFO
{
	char IpAddress[17];
};

struct BLACKLIST_HID_INFO
{
	char HardwareId[MAX_HWID_KEY];
};

class CBlackList
{
	public:
	CBlackList();
	void Load(char* path);
	bool CheckIpAddress(char* IpAddress);
	bool CheckHardwareId(char* HardwareId);
	bool AddBlackList(char* BlackList, char* type);
	std::map<std::string,BLACKLIST_IP_INFO> m_BlackListIpInfo;
	std::map<std::string,BLACKLIST_HID_INFO> m_BlackListHidInfo;
};

extern CBlackList gBlackList;