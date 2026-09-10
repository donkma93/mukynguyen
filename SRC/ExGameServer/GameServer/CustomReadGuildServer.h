#pragma once
#include "Protocol.h"

struct ReadFromLeach
{
	PSWMSG_HEAD header;
	char NameChar[11];
	char GuildName[11];
	BYTE GuildMark[32];
	int aIndex;
};
struct RequestGuilCache
{
	PSBMSG_HEAD header;
	char NameChar[11];
	int aIndex;
};

class CustomReadGuild
{
	public:
	void CGetInfoCharTop(RequestGuilCache* lpMsg, int aIndex);
	void RecvInfoCharTop(ReadFromLeach* lpMsg);
};
extern CustomReadGuild gCustomReadGuild;