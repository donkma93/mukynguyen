#include "stdafx.h"
#include "ChaosMachine.h"
#include "WSclient.h"
#include "Util.h"
#include "wsclientinline.h"

DWORD MixRate = 0;
DWORD MixMoney = 0;
DWORD MixTax = 0;

void InitChaosBox() // OK
{
	(0xE8, 0x0082D1C3, &PrintPlayerChaosRate);
	(0xE8, 0x0082D2E6, &PrintPlayerChaosRate);
	(0xE8, 0x0082D325, &PrintPlayerChaosRate);
	(0xE8, 0x0082D361, &PrintPlayerChaosRate);
	(0xE8, 0x0082D39D, &PrintPlayerChaosRate);
	(0xE8, 0x0082D4A2, &PrintPlayerChaosMoney);
	(0xE8, 0x0082D4D1, &PrintPlayerChaosMoney);
}

void ChaosBoxMainProc() // OK
{
	if (ChaosBoxMixCheck() != 0)
	{
		PMSG_CHAOS_MIX_RATE_SEND pMsg;
		pMsg.header.set(0x8F, sizeof(pMsg));
		pMsg.type = pChaosMixIndex;
		DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
}

bool ChaosBoxMixCheck() // OK
{
	if (pChaosMixIndex > 57)
	{
		return 1;
	}

	switch (pChaosMixIndex)
	{
	case CHAOS_MIX_CHAOS_ITEM:
	case CHAOS_MIX_DEVIL_SQUARE:
	case CHAOS_MIX_PLUS_ITEM_LEVEL1:
	case CHAOS_MIX_PLUS_ITEM_LEVEL2:
	case CHAOS_MIX_DINORANT:
	case CHAOS_MIX_FRUIT:
	case CHAOS_MIX_WING1:
	case CHAOS_MIX_BLOOD_CASTLE:
	case CHAOS_MIX_WING2:
	case CHAOS_MIX_PET1:
	case CHAOS_MIX_PET2:
	case CHAOS_MIX_PLUS_ITEM_LEVEL3:
	case CHAOS_MIX_PLUS_ITEM_LEVEL4:
	case CHAOS_MIX_WING3:
	case CHAOS_MIX_PIECE_OF_HORN:
	case CHAOS_MIX_BROKEN_HORN:
	case CHAOS_MIX_HORN_OF_FENRIR:
	case CHAOS_MIX_HORN_OF_FENRIR_UPGRADE:
	case CHAOS_MIX_SHIELD_POTION1:
	case CHAOS_MIX_SHIELD_POTION2:
	case CHAOS_MIX_SHIELD_POTION3:
	case CHAOS_MIX_JEWEL_OF_HARMONY_ITEM_PURITY:
	case CHAOS_MIX_JEWEL_OF_HARMONY_ITEM_SMELT:
	case CHAOS_MIX_ITEM_380:
	case CHAOS_MIX_ILLUSION_TEMPLE:
	case CHAOS_MIX_FEATHER_OF_CONDOR:
	case CHAOS_MIX_WING4:
	case CHAOS_MIX_SOCKET_ITEM_CREATE_SEED:
	case CHAOS_MIX_SOCKET_ITEM_CREATE_SEED_SPHERE:
	case CHAOS_MIX_PLUS_ITEM_LEVEL5:
	case CHAOS_MIX_PLUS_ITEM_LEVEL6:
	case CHAOS_MIX_LUCKY_ITEM_REFINE:
		return 1;
	}

	return 0;
}

void ChaosBoxConvertMoney(int money, char* target) // OK
{
	if (money >= 1000000000)
	{
		wsprintf(target, "%d,%03d,%03d,%03d", money / 1000000000, money % 1000000000 / 1000000, money % 1000000 / 1000, money % 1000);
	}
	else if (money >= 1000000)
	{
		wsprintf(target, "%d,%03d,%03d", money % 1000000000 / 1000000, money % 1000000 / 1000, money % 1000);
	}
	else if (money > 1000)
	{
		wsprintf(target, "%d,%03d", money % 1000000 / 1000, money % 1000);
	}
	else
	{
		wsprintf(target, "%d", money % 1000);
	}
}

void PrintPlayerChaosRate(char* a, char* b, char* c, int d) // OK
{
	if (ChaosBoxMixCheck() != 0)
	{
		d = MixRate;
	}

	char MixLog[255];
	sprintf(MixLog, "%d", pChaosMixIndex);

	wsprintf(a, b, c, d);
}

void PrintPlayerChaosMoney(char* a, char* b, char* c, char* d) // OK
{
	if (ChaosBoxMixCheck() != 0)
	{
		ChaosBoxConvertMoney(MixMoney, c);
		ChaosBoxConvertMoney((MixMoney + MixTax), d);
	}
	wsprintf(a, b, c, d);
}
void GCChaosMixRateRecv(PMSG_CHAOS_MIX_RATE_RECV* lpMsg) // OK
{
	MixRate = lpMsg->rate;
	MixMoney = lpMsg->money;
	MixTax = lpMsg->tax;
}