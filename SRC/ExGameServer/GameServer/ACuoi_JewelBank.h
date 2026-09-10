#pragma once

#include "Protocol.h"
#include "User.h"
#define MAX_ITEM_SLOT						10
struct PSBMSG_JEWELBANK_RECV
{
	PSBMSG_HEAD header; // C1:F3:F4
	int slot;
};

struct PSBMSG_JEWELBANKWITHDRAW_RECV
{
	PSBMSG_HEAD header; // C1:F3:F4
	int type;
	int count;
};

struct PSBMSG_JEWELBANK_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:F3:F4
};

struct PSBMSG_JEWELBANK_SEND
{

	PSBMSG_HEAD h;
	int ItemBank[MAX_ITEM_SLOT];
	BYTE Enable;
	BYTE JewelMaxCount;
	BYTE MaxPage;

};

struct SDHP_CUSTOM_JEWELBANK_SEND
{
	PSBMSG_HEAD header;
	WORD index;
	char account[11];
	WORD type;
	DWORD count;
};

struct SDHP_CUSTOM_JEWELBANK_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F5
	WORD index;
	char account[11];
};


struct SDHP_CUSTOM_JEWELBANK_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F5
	WORD index;
	int ItemBank[MAX_ITEM_SLOT];
};

struct MESSAGE_INFO_JWBANK
{
	int Index;
	char Message[256];
};

class CCustomJewelBank
{
public:
	bool Enable;
	BYTE MaxPage;
	WORD JewelMaxCount;


	void LoadConfig(char* FilePath);
	int GetJewelSimpleType(int ItemIndex);
	int GetJewelSimpleIndex(int type);
	int GetJewelBundleIndex(int type);
	void JewelBankRecv(PSBMSG_JEWELBANK_RECV* lpMsg, int aIndex);
	void JewelBankWithDrawRecv(PSBMSG_JEWELBANKWITHDRAW_RECV* lpMsg, int aIndex);
	void CustomJewelBankInfoSend(int index);
	void CustomJewelBankInfoRecv(SDHP_CUSTOM_JEWELBANK_INFO_RECV* lpMsg);
	void GCCustomJewelBankInfoSend(LPOBJ lpObj);
	void GDCustomJewelBankAddJewel(LPOBJ lpObj, int type, int qtd);
	void GDCustomJewelBankDelJewel(LPOBJ lpObj, int type, int qtd);
private:
	std::map<int, MESSAGE_INFO_JWBANK> m_MessageInfoBP;
	char* GetMessage(int index);
};

extern CCustomJewelBank gCustomJewelBank;
