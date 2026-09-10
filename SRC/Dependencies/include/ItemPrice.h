#pragma once
#include "array.h"
#include "..\include\prodef.h"

enum CURR_NAME
{
	ZEN,
	WCOINC,
	WCOINP,
	GOBLINPOINT,
	VPOINT,
	GCOIN
};

struct ITEMPRICE_DATA
{
	WORD	ItemID;
	WORD	ItemIndex;
	BYTE	ItemLevel;
	__int64	BuyPrice;
	__int64	SellPrice;
	CURR_NAME BuyCurrency;
	CURR_NAME SellCurrency;
};

struct PMSG_ITEMPRICE
{
	PBMSG_HEAD2 h;
	ITEMPRICE_DATA data[5];
	int size;
};

class ItemPrice
{
public:	
	void	Init();
	void	ReadList(char * File);

	CURR_NAME GetCurrFromString(char *curr);

	void	Load();
	void	SendData(int aIndex);
	bool	CheckItem(short ItemCode, BYTE ItemLevel, __int64 * BuyPrice, __int64 * SellPrice, CURR_NAME * BuyCurrency, CURR_NAME * SellCurrency);
	bool	CheckItem(short ItemCode, BYTE ItemLevel);
	
	bool	Enabled;
private:
	array<ITEMPRICE_DATA> m_Data;
	
};
extern ItemPrice g_ItemPrice;
