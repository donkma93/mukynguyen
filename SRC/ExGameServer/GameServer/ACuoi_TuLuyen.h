#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"

struct GetDataTuLuyen
{
	int AllData[11];
};

struct SolvedTuLuyen
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};
struct GetMessageTuLuyen
{
	int Index;
	char Message[256];
};
class cTuLuyen
{
public:
	cTuLuyen();
	void LoadConfig(char* FilePath);
	bool Enable;
	bool ThongBao;
	int ThoiGian, GioiHan;
	std::map<int, GetDataTuLuyen> m_LoadData;
	GetDataTuLuyen* ReadConfig(int LvTuLuyen);
	void SendInfoClient(int aIndex);
	bool RequestAccess(int aIndex);
	void AddOption(LPOBJ lpObj, bool flag);

private:
	std::map<int, GetMessageTuLuyen> m_ReadMessage;
	char* GetMessage(int index);
};
extern cTuLuyen gTuLuyen;
