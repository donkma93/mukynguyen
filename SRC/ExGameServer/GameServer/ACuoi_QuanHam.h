#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"

struct GetDataQuanHam
{
	int AllData[11];
};

struct SolvedQuanHam
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};
struct GetMessageQuanHam
{
	int Index;
	char Message[256];
};
class cQuanHam
{
public:
	cQuanHam();
	void LoadConfig(char* FilePath);
	bool Enable;
	bool ThongBao;
	int ThoiGian, GioiHan;
	std::map<int, GetDataQuanHam> m_LoadData;
	GetDataQuanHam* ReadConfig(int LvQuanHam);
	void SendInfoClient(int aIndex);
	bool RequestAccess(int aIndex);
	void AddOption(LPOBJ lpObj, bool flag);

private:
	std::map<int, GetMessageQuanHam> m_ReadMessage;
	char* GetMessage(int index);
};
extern cQuanHam gQuanHam;
