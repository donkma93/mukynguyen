#include "StdAfx.h"
#include "ItemPrice.h"
#include "logproc.h"
#include "MemScript.h"
#include "..\common\winutil.h"
#include "GameMain.h"
#include "ExUtilits.h"

ItemPrice	g_ItemPrice;

void ItemPrice::Init()
{
	Enabled = false;
	m_Data.clear();
}

void ItemPrice::ReadList(char * lpszFileName)
{
	Init();

	Enabled = Ex_GetInt(0, 1, 1, "ItemPrice", "Enabled", lpszFileName);
	if (!Enabled)
		return;

	FILE *file = fopen(lpszFileName, "r");

	if (file == NULL)
	{
		Enabled = false;
		LogAdd("[ItemPrice] Can't find %s config file", lpszFileName);
		return;
	}

	char Buff[256];
	int Flag = 0;

	while (!feof(file))
	{
		fgets(Buff, 256, file);

		if (Ex_IsBadFileLine(Buff, Flag))
			continue;

		if (Flag == 1)
		{
			int n[5];
			char buyCurr[16], sellCurr[16];
			sscanf(Buff, "%d %d %d %d %d \"%16[^\"]\" \"%16[^\"]\"", &n[0], &n[1], &n[2], &n[3], &n[4], &buyCurr, &sellCurr);

			ITEMPRICE_DATA temp;
			temp.ItemID		= n[0];
			temp.ItemIndex	= n[1];
			temp.ItemLevel	= n[2];
			temp.BuyPrice	= n[3];
			temp.SellPrice	= n[4];

			temp.BuyCurrency = GetCurrFromString(buyCurr);
			temp.SellCurrency = GetCurrFromString(sellCurr);

			m_Data.add(temp);
		}
	}
	m_Data.optimize();

	fclose(file);
	LogAdd("[ItemPrice] [%d] Items loaded from list", m_Data.size());
}

CURR_NAME ItemPrice::GetCurrFromString(char *curr)
{
	// compare without case
	if (_strcmpi(curr, "zen") == 0)
		return CURR_NAME::ZEN;
	else if (_strcmpi(curr, "wcoinc") == 0)
		return CURR_NAME::WCOINC;
	else if (_strcmpi(curr, "wcoinp") == 0)
		return CURR_NAME::WCOINP;
	else if (_strcmpi(curr, "goblinpoint") == 0)
		return CURR_NAME::GOBLINPOINT;
	else if (_strcmpi(curr, "vpoint") == 0)
		return CURR_NAME::VPOINT;
	else if (_strcmpi(curr, "gcoinkm") == 0)
		return CURR_NAME::GCOIN;
	else
		return CURR_NAME::ZEN;
}

void ItemPrice::Load()
{
	this->ReadList("..\\Data\\Custom\\Item\\ItemPrice.txt");
}

void ItemPrice::SendData(int aIndex)
{
	int packetSize;
	int dataPos = 0;
	do 
	{
		packetSize = 0;

		PMSG_ITEMPRICE pMsg = { 0 };
		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x77;
		pMsg.h.subcode = 0x01;
		pMsg.h.size = sizeof(PMSG_ITEMPRICE); //0x44

		for (int i = dataPos; i < m_Data.size(); i++)
		{
			pMsg.data[packetSize].ItemID		= m_Data[i].ItemID;
			pMsg.data[packetSize].ItemIndex		= m_Data[i].ItemIndex;
			pMsg.data[packetSize].ItemLevel		= m_Data[i].ItemLevel;
			pMsg.data[packetSize].BuyPrice		= m_Data[i].BuyPrice;
			pMsg.data[packetSize].SellPrice		= m_Data[i].SellPrice;
			pMsg.data[packetSize].BuyCurrency	= m_Data[i].BuyCurrency;
			pMsg.data[packetSize].SellCurrency	= m_Data[i].SellCurrency;

			dataPos++;
			packetSize++;

			if (packetSize == 5)
				break;
		}
		pMsg.size = packetSize;

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	} while (packetSize == 5);
}

bool ItemPrice::CheckItem(short ItemCode, BYTE ItemLevel, __int64 * BuyPrice, __int64 * SellPrice, CURR_NAME * BuyCurrency, CURR_NAME * SellCurrency)
{
	for( int i = 0; i < m_Data.size(); i++ )
	{
		if( ItemCode != ITEMGET(m_Data[i].ItemID, m_Data[i].ItemIndex) )
			continue;

		if( m_Data[i].ItemLevel != -1 && ItemLevel != m_Data[i].ItemLevel )
			continue;

		*BuyPrice		= m_Data[i].BuyPrice;
		*SellPrice		= m_Data[i].SellPrice;
		*BuyCurrency	= m_Data[i].BuyCurrency;
		*SellCurrency	= m_Data[i].SellCurrency;
		return true;
	}

	return false;
}

bool ItemPrice::CheckItem(short ItemCode, BYTE ItemLevel)
{
	for (int i = 0; i < m_Data.size(); i++)
	{
		if (ItemCode != ITEMGET(m_Data[i].ItemID, m_Data[i].ItemIndex))
			continue;

		if (m_Data[i].ItemLevel != -1 && ItemLevel != m_Data[i].ItemLevel)
			continue;

		return true;
	}

	return false;
}