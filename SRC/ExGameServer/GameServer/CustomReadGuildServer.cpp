#include "stdafx.h"
#include "CustomReadGuildServer.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Util.h"

CustomReadGuild gCustomReadGuild;

void CustomReadGuild::CGetInfoCharTop(RequestGuilCache* lpMsg, int aIndex)
{
	if (lpMsg == NULL)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}

	if (GetTickCount() < gObj[aIndex].ClickClientSend + 1000)
	{
		return;
	}

	gObj[aIndex].ClickClientSend = GetTickCount();
	RequestGuilCache pMsg{};
	pMsg.header.set(0xD3, 0x40, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.NameChar, lpMsg->NameChar, sizeof(pMsg.NameChar) - 1);
	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CustomReadGuild::RecvInfoCharTop(ReadFromLeach* lpMsg)
{
	if (lpMsg == NULL)
	{
		return;
	}
	int aIndex = lpMsg->aIndex;

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	BYTE send[4096];

	ReadFromLeach pMsg{};

	pMsg.header.set(0xD3, 0x40, 0);

	int size = sizeof(pMsg);

	memcpy(pMsg.NameChar, lpMsg->NameChar, sizeof(pMsg.NameChar));
	pMsg.aIndex = lpMsg->aIndex;
	
	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));
	memcpy(pMsg.GuildMark, lpMsg->GuildMark, sizeof(pMsg.GuildMark));

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}