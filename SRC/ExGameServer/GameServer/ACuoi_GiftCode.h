// CustomGift.h: interface for the CCustomGift class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include "Protocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//


struct SDHP_COMMAND_GIFT_SEND
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	char GiftCode[32];
	int GioiHan;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_GIFT_RECV
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	BYTE Result;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct GIFT_LIST_ITEM_INFO
{
	int Index;
	int MaxCount;
	int ItemIndex;
	int Level;
	int Dur;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketCount;
	int Duration;
};

struct GIFT_LIST_BUFF_INFO
{
	int Index;
	int Effect;
	int Type;
	int Power[4];
	int Duration;
};


struct GIFT_LIST_INFO
{
	int Index;
	int Class;
	int Enable[MAX_ACCOUNT_LEVEL];
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int LevelUpPoint;
	DWORD Money;
	int Coin1;
	int Coin2;
	int Coin3;
	char Code[32];
	int GioiHan;
};

struct MESSAGE_INFO_GIFT
{
	int Index;
	char Message[256];
};

class CCustomGift
{
public:
	CCustomGift();
	virtual ~CCustomGift();
	void LoadConfig(char* FilePath);
	bool CommandGift(LPOBJ lpObj, char* arg);
	void CheckGiftCode(LPOBJ lpObj);
	void GiftCodeSend(LPOBJ lpObj, const char* giftCode);
	void DGCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg);
	GIFT_LIST_INFO* GetConfigListInfo(int Info);
	bool EnableGift;

private:
	char m_CurrentCode[32];
	std::map<int, MESSAGE_INFO_GIFT> m_MessageInfoBP;
	char* GetMessage(int index);
	std::map<int, GIFT_LIST_INFO> m_GiftListInfo;
	std::multimap<int, GIFT_LIST_ITEM_INFO> ItemList;
	std::multimap<int, GIFT_LIST_BUFF_INFO> BuffList;
};

extern CCustomGift gCustomGift;