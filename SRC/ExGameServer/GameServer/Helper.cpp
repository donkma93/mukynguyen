#include "stdafx.h"
#include "Helper.h"
#include "CastleSiege.h"
#include "GameMain.h"
#include "Map.h"
#include "MapManager.h"

#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"
#include "ACuoi_MessLang.h"

CHelper gHelper;

void CHelper::MainProc() 
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if(lpObj->HelperDelayTime != 0)
		{
			int HelperStage = 0;

			if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if((HelperStage=((GetTickCount()-lpObj->HelperTotalTime)/12000000)) >= MAX_HELPER_STAGE)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if((GetTickCount()-lpObj->HelperDelayTime) >= ((DWORD)(gServerInfo.m_HelperActiveDelay*60000)))
			{
				if(lpObj->Money < ((DWORD)((lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage])))
				{
					this->DisableHelper(lpObj);
					continue;
				}

				lpObj->HelperDelayTime = GetTickCount();
				lpObj->Money -= (lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage];
				GCMoneySend(lpObj->Index,lpObj->Money);
				this->GCHelperStartSend(lpObj->Index,((GetTickCount()-lpObj->HelperTotalTime)/60000),((lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage]),0);
			}
		}
	}
}

void CHelper::DisableHelper(LPOBJ lpObj)
{
	if(lpObj->HelperDelayTime == 0)
	{
		return;
	}

	lpObj->HelperDelayTime = 0;
	lpObj->HelperTotalTime = 0;
	this->GCHelperStartSend(lpObj->Index,0,0,1);
}

void CHelper::CGHelperDataRecv(PMSG_HELPER_DATA_RECV* lpMsg,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	this->GDHelperDataSaveSend(aIndex,lpMsg->data);
	//LogAdd(LOG_RED, "[CGHelperDataRecv] Received MU Helper data from %s", gObj[aIndex].Name);
}

void CHelper::CGHelperStartRecv(PMSG_HELPER_START_RECV* lpMsg,int aIndex)
{
	if(gServerInfo.m_HelperSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Level < gServerInfo.m_HelperActiveLevel)
	{
		return;
	}

	if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		return;
	}

	//Fix helper + custom attack
	if(lpObj->AttackCustom == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessageNew.GetMessage(564));
		return;
	}

	if(lpMsg->type == 0)
	{
		lpObj->HelperDelayTime = GetTickCount();
		lpObj->HelperTotalTime = GetTickCount();
		this->GCHelperStartSend(aIndex,0,0,0);
	}
	else
	{
		lpObj->HelperDelayTime = 0;
		lpObj->HelperTotalTime = 0;
		this->GCHelperStartSend(aIndex,0,0,1);
	}
}

void CHelper::GCHelperStartSend(int aIndex, DWORD consumeMoney, DWORD money, DWORD pause)
{
	PMSG_HELPER_START_SEND pMsg{};
	pMsg.header.set(0xBF, 0x51, sizeof(pMsg));
	pMsg.ConsumeMoney = consumeMoney;
	pMsg.Money = money;
	pMsg.Pause = pause;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CHelper::DGHelperDataRecv(SDHP_HELPER_DATA_RECV* lpMsg)
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_HELPER_DATA_SEND pMsg;
	pMsg.header.set(0xAE,sizeof(pMsg));
	pMsg.result = lpMsg->result;
	memcpy(pMsg.data,lpMsg->data,sizeof(pMsg.data));

	//LogAdd(LOG_RED, "[Read] [%d](%s) %d/%d/%d/%d/%d"
	//	, lpMsg->index
	//	, lpMsg->name
	//	, lpMsg->data[0]
	//	, lpMsg->data[1]
	//	, lpMsg->data[2]
	//	, lpMsg->data[3]
	//	, lpMsg->data[4]
	//);

	DataSend(lpMsg->index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CHelper::GDHelperDataSend(int aIndex)
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_HELPER_DATA_SEND pMsg;
	pMsg.header.set(0x17,0x00,sizeof(pMsg));
	pMsg.index = aIndex;
	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));
	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CHelper::GDHelperDataSaveSend(int aIndex,BYTE* data)
{
	SDHP_HELPER_DATA_SAVE_SEND pMsg;
	pMsg.header.set(0x17,0x30,sizeof(pMsg));
	pMsg.index = aIndex;
	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));
	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));
	memcpy(pMsg.data,data,sizeof(pMsg.data));
	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}
