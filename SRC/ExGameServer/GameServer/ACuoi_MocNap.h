#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"
#include "SkillManager.h"

struct MESSAGE_INFO_CBMOCNAP
{
	int Index;
	char Message[256];
};
struct DATA_CBITEMMOCNAP
{
	float SizeBMD;
	int Count;
	int IndexItem;
	int LvItem;
	int Dur;
	int Skill;
	int Luck;
	int Opt;
	int Exc;
	int Anc;
	int SK[MAX_SOCKET_OPTION];
	int SKBonus;
	int HSD;
	int Class[MAX_CLASS];
	BYTE GetTypeItem;
};
struct DATA_CBMONAP
{
	int IndexMocNap;
	int GiaTriNap;
	int WC;
	int WP;
	int GP;
	int Ruud;
	int CoinAtm;
	std::vector<DATA_CBITEMMOCNAP> ListItemNhan;
};

struct PMSG_CBMONAP_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	int NhanMocNap;
	int TongNap;
};

struct ListMocNapSend
{
	int IndexMocNap;
	int GiaTriNap;
};

struct PMSG_CBLISTTHUONG_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	int WC;
	int WP;
	int GP;
	int Ruud;
	int CoinAtm;
};
struct LISTITEMMOCNAP_SENDINFO
{
	float SizeBMD;
	int Count;
	short Index;
	BYTE GetTypeItem;
	BYTE Dur;
	BYTE Item[12];
	int  PeriodTime;
};

struct XULY_CGPITEMTRADE
{
	PSBMSG_HEAD header; // C3:F3:03
	DWORD ThaoTac;

};
class gBMocNap
{
	public:
	void LoadFileXML(char* FilePath);
	void gBMocNap::UserSendClientInfo(int aIndex);
	void gBMocNap::SendListNhanThuong(int aIndex, int MocNap);
	void gBMocNap::NhanThuongMocNap(int aIndex, int MocNap);
	int Enable;
	int Firework;
	int Notice;
	std::map<int, MESSAGE_INFO_CBMOCNAP> m_MessageInfoBP;
	std::map<int, DATA_CBMONAP> m_DataMocNap;
	char* GetMessage(int index);
};

extern gBMocNap gCBMocNap;
