#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"

struct GetDataDanhHieu
{
	int AllData[11];
};

struct SolvedDanhHieu
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};
struct GetMessageDanhHieu
{
	int Index;
	char Message[256];
};
class cDanhHieu
{
	public:
	cDanhHieu();
	void LoadConfig(char* FilePath);
	bool Enable;
	bool ThongBao;
	int ThoiGian,GioiHan;
	std::map<int, GetDataDanhHieu> m_LoadData;
	GetDataDanhHieu* ReadConfig(int LvDanhHieu);
	void SendInfoClient(int aIndex);
	bool RequestAccess(int aIndex);
	void AddOption(LPOBJ lpObj, bool flag);

	private:
	std::map<int, GetMessageDanhHieu> m_ReadMessage;
	char* GetMessage(int index);
};
extern cDanhHieu gDanhHieu;
