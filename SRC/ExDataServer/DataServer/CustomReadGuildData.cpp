#include "stdafx.h"
#include "CustomReadGuildData.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"

CustomReadGuild gCustomReadGuild;

void CustomReadGuild::CGetInfoCharTop(REQUESTINFO_CHARTOP* lpMsg, int index)
{
	DATA_VIEWTOPRANKING pMsg{};
	pMsg.header.set(0xD3, 0x40, sizeof(pMsg));
	memcpy(pMsg.NameChar, lpMsg->NameChar, sizeof(pMsg.NameChar));
	pMsg.aIndex = lpMsg->aIndex;

	LogAdd(LOG_RED, "[CGetInfoCharTop] Start processing character: %s (Index: %d)", lpMsg->NameChar, index);

	if (gQueryManager.ExecQuery("SELECT G_Name FROM GuildMember WHERE Name='%s'", lpMsg->NameChar) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		memset(pMsg.GuildName, 0xFF, sizeof(pMsg.GuildName));
		memset(pMsg.GuildMark, 0xFF, sizeof(pMsg.GuildMark));
	}
	else
	{
		memset(pMsg.GuildName, 0xFF, sizeof(pMsg.GuildName));
		gQueryManager.GetAsString("G_Name", pMsg.GuildName, sizeof(pMsg.GuildName));
		gQueryManager.Close();

		if (gQueryManager.ExecQuery("SELECT G_Mark,Number FROM Guild WHERE G_Name='%s'", pMsg.GuildName) != 0 && gQueryManager.Fetch() != SQL_NO_DATA)
		{
			memset(pMsg.GuildMark, 0xFF, sizeof(pMsg.GuildMark));
			gQueryManager.GetAsBinary("G_Mark", pMsg.GuildMark, sizeof(pMsg.GuildMark));
			gQueryManager.Close();
		}
		else
		{
			memset(pMsg.GuildMark, 0xFF, sizeof(pMsg.GuildMark));
			gQueryManager.Close();
		}
	}
	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}
