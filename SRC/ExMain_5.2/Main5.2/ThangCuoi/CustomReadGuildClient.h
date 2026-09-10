#pragma once
#include <WSclient.h>

struct ReadFromData
{
	BYTE Result;
	char Guild[9];
	char Name[11];
};

struct ReadFromLeach
{
	PSWMSG_HEAD header;
	char NameChar[11];
	char GuildName[11];
	BYTE GuildMark[32];
};

struct RequestGuilCache
{
	PSBMSG_HEAD header; // C3:F3:03
	char NameChar[11];
	int aIndex;
};

class CustomReadGuild
{
	public:
	CustomReadGuild();
	void Release();
	void ClearSession();
	void GCReqInfoGuild(ReadFromLeach* lpMsg);
	void GetInfoGuild(char* Name);
	ReadFromData ReadData;
	ReadFromLeach CacheDataUserTop;
	bool CacheUserRank;
	bool SetItemCharTop;
};
extern CustomReadGuild gCustomReadGuild;