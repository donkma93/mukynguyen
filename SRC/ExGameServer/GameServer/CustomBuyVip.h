#pragma once

#define MAX_CUSTOM_BUYVIP 3
#include "Protocol.h"

struct BUYPREMIUM_REQ
{
	PSBMSG_HEAD h;
	// The client sends the selected package as a single byte (C1:F3:F0).
	// Keeping this field as an int made the server read past the packet and
	// occasionally reject a valid VIP purchase before WCoinC was charged.
	BYTE	VipType;
};

struct CUSTOM_BUYVIP_INFO
{
	int Index;
	int Exp;
	int Drop;
	int Days;
	int Coin1;
	int Coin2;
	int Coin3;
	char VipName[32];
};

class CCustomBuyVip
{
public:
	CCustomBuyVip();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_BUYVIP_INFO info);
	CUSTOM_BUYVIP_INFO* GetInfo(int index);
	void BuyVip(int aIndex, BUYPREMIUM_REQ* lpMsg);
	void BuyVipDone(LPOBJ lpObj);
	CUSTOM_BUYVIP_INFO m_CustomBuyVipInfo[MAX_CUSTOM_BUYVIP];
};

extern CCustomBuyVip gCustomBuyVip;
