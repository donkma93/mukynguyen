#pragma once
#include "DefaultClassInfo.h"
#include "User.h"
#include "Protocol.h"
#include "SkillManager.h"

struct MESSAGE_INFO_CASHSHOP
{
	int Index;
	char Message[256];
};
struct DATA_ITEMCASHSHOP
{
	float SizeBMD;
	int PriceType;
	int Price;
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
	BYTE ItemType;
};
struct DATA_CASHSHOP
{
	int IndexShop;
	char NameDanhMuc[90];
	std::vector<DATA_ITEMCASHSHOP> ListItemBan;
};
struct XULY_XSHOP
{
	PSBMSG_HEAD header;
	DWORD ThaoTac;
};
struct PMSG_CASHSHOP_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct ListShopSend
{
	int IndexShop;
	char Name[90];
};

struct PMSG_LISTITEMSHOP_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct XULY_CGPACKET_SSHOP
{
	PSBMSG_HEAD header;
	DWORD Shop;
	DWORD ViTri;
};

struct LISTITEMCASHSHOP_SENDINFO
{
	float SizeBMD;
	int PriceType;
	int Price;
	int Class[MAX_CLASS];
	short Index;
	BYTE TypeItem;
	BYTE Dur;
	BYTE Item[12];
	int  PeriodTime;
};
class NewCashShop
{
	public:
	void LoadFileXML(char* FilePath);
	void UserSendClientInfo(int aIndex);
	void SendListShop(int aIndex, int Shop);
	void MuaItem(int aIndex, int Shop, int ViTri);

	void GsSendPackSize11(BYTE* lpMsg, int aIndex);
	void GsSendPackSize12(BYTE* lpMsg, int aIndex);
	void GsSendPackSize13(BYTE* lpMsg, int aIndex);

	private:
	int Enable;
	int Firework;
	int Notice;
	std::map<int, MESSAGE_INFO_CASHSHOP> m_MessageInfoBP;
	std::map<int, DATA_CASHSHOP> m_DataCashShop;
	char* GetMessage(int index);
}; extern NewCashShop gNewCashShop;