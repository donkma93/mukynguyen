#include "stdafx.h"
#include <windows.h>
#include "CustomReadGuildClient.h"
#include <wsclientinline.h>

CustomReadGuild gCustomReadGuild;

CustomReadGuild::CustomReadGuild()
{
	Release();
}

void CustomReadGuild::Release()
{
	ClearSession();
}

void CustomReadGuild::ClearSession()
{
	ZeroMemory(this->ReadData.Name, sizeof(this->ReadData.Name));
	ZeroMemory(this->ReadData.Guild, sizeof(this->ReadData.Guild));
}

void CustomReadGuild::GCReqInfoGuild(ReadFromLeach* lpMsg)
{
	this->CacheUserRank = true;
	this->SetItemCharTop = false;
	ZeroMemory(&this->CacheDataUserTop, sizeof(&this->CacheDataUserTop));
	memcpy(&this->CacheDataUserTop, lpMsg, sizeof(this->CacheDataUserTop));
	ClearSession();
	memcpy(&ReadData.Guild[0], &this->CacheDataUserTop.GuildName[0], sizeof(ReadData.Guild));
	memcpy(&ReadData.Name[0], &this->CacheDataUserTop.NameChar[0], 10);
}

void CustomReadGuild::GetInfoGuild(char* Name)
{
	RequestGuilCache pMsg{};
	pMsg.header.set(0xD3, 0x40, sizeof(pMsg));
	pMsg.aIndex = 0;
	memcpy(pMsg.NameChar, Name, sizeof(pMsg.NameChar) - 1);
	DataSend((LPBYTE)&pMsg, pMsg.header.size);
}