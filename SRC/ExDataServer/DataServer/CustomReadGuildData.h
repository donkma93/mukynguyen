#pragma once
#include "DataServerProtocol.h"

struct REQUESTINFO_CHARTOP
{
	PSBMSG_HEAD header;
	char NameChar[11];
	int aIndex;
};

struct DATA_VIEWTOPRANKING
{
	PSWMSG_HEAD header;
	char NameChar[11];
	char GuildName[11];
	BYTE GuildMark[32];
	int aIndex;
};

class CustomReadGuild
{
	public:
	void CGetInfoCharTop(REQUESTINFO_CHARTOP* lpMsg, int index);
};
extern CustomReadGuild gCustomReadGuild;