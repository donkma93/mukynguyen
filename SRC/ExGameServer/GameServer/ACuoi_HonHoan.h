#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"

struct GetDataHonHoan
{
	int AllData[11];
};

struct SolvedHonHoan
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};
struct GetMessageHonHoan
{
	int Index;
	char Message[256];
};
class cHonHoan
{
	public:
	cHonHoan();
	void LoadConfig(char* FilePath);
	bool Enable;
	bool ThongBao;
	int ThoiGian,GioiHan;
	std::map<int, GetDataHonHoan> m_LoadData;
	GetDataHonHoan* ReadConfig(int LvHonHoan);
	void SendInfoClient(int aIndex);
	bool RequestAccess(int aIndex);
	void AddOption(LPOBJ lpObj, bool flag);

	private:
	std::map<int, GetMessageHonHoan> m_ReadMessage;
	char* GetMessage(int index);
};
extern cHonHoan gHonHoan;
