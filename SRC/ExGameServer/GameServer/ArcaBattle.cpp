#include "stdafx.h"
#include "ArcaBattle.h"
#include "GameMain.h"
#include "Guild.h"
#include "Map.h"
#include "MemScript.h"
#include "Monster.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ScheduleManager.h"
#include "Util.h"

CArcaBattle gArcaBattle;


void CArcaBattle::ChkAuraUserHover()
{
	int iHoverUserIndex_2; //ebp-24
	int iAurReleaseCnt; //ebp-28
	int iHoverUserIndex_1; //ebp-2c
	int iUserIndex; //ebp-30
	int iObeliskIndex; //ebp-34

	for(int n=0;n < MAX_ARCA_BATTLE_OBELISK;n++)
	{
		this->m_ObeliskStateInfo[n].AuraState = 0;

		for(int i=0;i < MAX_ARCA_BATTLE_OBELISK_AURA;i++)
		{
			this->m_ObeliskStateInfo[n].AuraStateInfo[i].State = 0;
			this->m_ObeliskStateInfo[n].AuraStateInfo[i].UserIndex = -1;
		}
	}
	
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(gObj[n].Map == MAP_ACHERON2 || gObj[n].Map == MAP_DEVENTER2)
			{
				for(int i=0;i < MAX_ARCA_BATTLE_OBELISK;i++)
				{
					for(int o=0;o < MAX_ARCA_BATTLE_OBELISK_AURA;o++)
					{
						if(this->m_ObeliskInfo[i].AuraInfo[o].X == gObj[n].X && this->m_ObeliskInfo[i].AuraInfo[o].Y == gObj[n].Y)
						{
							this->m_ObeliskStateInfo[i].AuraStateInfo[o].State = 1;
							this->m_ObeliskStateInfo[i].AuraStateInfo[o].UserIndex = n;
						}
					}
				}
			}
		}
	}
	
	for(int n=0;n < MAX_ARCA_BATTLE_OBELISK;n++)
	{
		iAurReleaseCnt		= 0;
		iHoverUserIndex_1	= 0;
		iHoverUserIndex_2	= 0;
		
		for(int i=0;i < MAX_ARCA_BATTLE_OBELISK_AURA;i++)
		{
			if(this->m_ObeliskStateInfo[n].AuraStateInfo[i].State == 1)
			{
				iAurReleaseCnt++;
			}
		}
		
		if( iAurReleaseCnt == 3 )
		{
			this->m_ObeliskStateInfo[n].AuraState = 2; //#DEFINE
			
			for(int i=0;i < MAX_ARCA_BATTLE_OBELISK_AURA;i++)
			{
				iHoverUserIndex_1 = this->m_ObeliskStateInfo[n].AuraStateInfo[i+0].UserIndex;
				iHoverUserIndex_2 = this->m_ObeliskStateInfo[n].AuraStateInfo[i+1].UserIndex;
				
				if( gObj[iHoverUserIndex_1].GuildNumber != gObj[iHoverUserIndex_2].GuildNumber )
				{
					this->m_ObeliskStateInfo[n].AuraState = 1;
				}
			}
			
			if(this->m_ObeliskStateInfo[n].AuraState == 2)
			{
				iUserIndex = this->m_ObeliskStateInfo[n].AuraStateInfo[0].UserIndex;
				
				if(this->m_ObeliskStateInfo[n].AuraReleaseGuildNum != gObj[iUserIndex].GuildNumber)
				{
					iObeliskIndex = this->m_ObeliskStateInfo[n].MonsterIndex;
					
					if( gObj[iObeliskIndex].Live == 1 )
					{
						if( this->m_ObeliskStateInfo[n].ObeliskState == 1 )
						{
							gObj[iObeliskIndex].Life = (float)this->m_ObeliskInfo[n].OccupyLife;
						}
						else
						{
							gObj[iObeliskIndex].Life = (float)this->m_ObeliskInfo[n].Life;
						}
					}
				}
				
				this->m_ObeliskStateInfo[n].AuraReleaseGuildNum = gObj[iUserIndex].GuildNumber;
				
				for(int i=0;i < MAX_ARCA_BATTLE_OBELISK_AURA;i++)
				{
					this->m_ObeliskStateInfo[n].AuraStateInfo[i].State = 2;
				}
			}
		}
		else
		{
			if( iAurReleaseCnt > 0 )
			{
				this->m_ObeliskStateInfo[n].AuraState = 1;
			}
		}
	}

	PMSG_ARCA_BATTLE_OBELISK_INFO_SEND pMsg;

	pMsg.header.set(0xF8,0x37,sizeof(pMsg));

	for(int n=0;n < MAX_ARCA_BATTLE_OBELISK;n++)
	{
		for(int i=0;i < MAX_ARCA_BATTLE_OBELISK_AURA;i++)
		{
			int iIndex	= this->m_ObeliskStateInfo[n].AuraStateInfo[i].Index;
			LPOBJ obj			= &gObj[iIndex];
			
			for( int n = 0; n < MAX_VIEWPORT; n++ )
			{
				if( obj->VpPlayer2[n].type == OBJECT_USER && obj->VpPlayer2[n].state != 0 )
				{
					int iTarObjIndex			= obj->VpPlayer2[n].index;
					pMsg.ObeliskAttribute	= (BYTE)this->m_ObeliskInfo[n].Attr;
					
					for(int o=0;o < MAX_ARCA_BATTLE_OBELISK_AURA;o++)
					{
						pMsg.ObeliskAuraState[o] = this->m_ObeliskStateInfo[n].AuraStateInfo[o].State;
					}
					
					DataSend(gObj[iTarObjIndex].Index, (LPBYTE)&pMsg, sizeof(PMSG_ARCA_BATTLE_OBELISK_INFO_SEND));
				}
			}
		}
	}
}

bool CArcaBattle::RegenObelisk()
{
	for( int i = 0; i < MAX_ARCA_BATTLE_OBELISK; i++ )
	{
		int iObeliskIndex = this->m_ObeliskStateInfo[i].MonsterIndex;
		
		if( !OBJECT_RANGE(iObeliskIndex) )
		{
			return false;
		}
		
		if( gObj[iObeliskIndex].Live == 1 )
		{
			this->m_ObeliskInfo[i].CreatedTime = GetTickCount();
		}
		else
		{
			if( GetTickCount() - (DWORD)this->m_ObeliskInfo[i].CreatedTime >= (DWORD)this->m_ObeliskInfo[i].RespawnTime )
			{
				int result = gObjAddMonster(this->m_ObeliskInfo[i].Map);
				
				if( result >= 0 )
				{
					gObjSetMonster(result, this->m_ObeliskInfo[i].MonsterClass);
					
					gObj[result].Class			= (WORD)this->m_ObeliskInfo[i].MonsterClass;
					gObj[result].Map		= (BYTE)this->m_ObeliskInfo[i].Map;
					gObj[result].X				= (WORD)this->m_ObeliskInfo[i].X;
					gObj[result].Y				= (WORD)this->m_ObeliskInfo[i].Y;
					gObj[result].TX				= (WORD)this->m_ObeliskInfo[i].X;
					gObj[result].TY				= (WORD)this->m_ObeliskInfo[i].Y;
					gObj[result].OldX			= (WORD)this->m_ObeliskInfo[i].X;
					gObj[result].OldY			= (WORD)this->m_ObeliskInfo[i].Y;
					gObj[result].StartX			= (BYTE)this->m_ObeliskInfo[i].X;
					gObj[result].StartY			= (BYTE)this->m_ObeliskInfo[i].Y;
					gObj[result].Dir			= 1;
					gObj[result].Live			= true;
					gObj[result].DieRegen		= 0;
					gObj[result].MaxRegenTime	= 0;
					gObj[result].MaxLife		= (float)this->m_ObeliskInfo[i].OccupyLife;
					gObj[result].Life			= (float)this->m_ObeliskInfo[i].OccupyLife;
					
					this->m_ObeliskStateInfo[i].MonsterIndex = result;
				}
			}
		}
	}
	
	return true;
}

bool CArcaBattle::IsEnableAttackObelisk(LPOBJ lpObj,int MonsterClass)
{
	int i;
	ARCA_BATTLE_OBELISK_STATE_INFO * pObelskState = NULL;
	
	for( i = 0; i < MAX_ARCA_BATTLE_OBELISK; i++ )
	{
		if( this->m_ObeliskInfo[i].MonsterClass == MonsterClass )
		{
			pObelskState = &this->m_ObeliskStateInfo[i];
		}
	}
	
	if( pObelskState == NULL )
	{
		return true;
	}
	
	if( this->m_State != AB_STATE_START )
	{
		return false;
	}
	
	if( pObelskState->OccupyGuildNum == lpObj->GuildNumber )
	{
		return false;
	}
	
	if( pObelskState->AuraState == 2 && pObelskState->AuraReleaseGuildNum == lpObj->GuildNumber )
	{
		return true;
	}
	
	return false;
}

bool CArcaBattle::IsPkEnable(LPOBJ lpObj,LPOBJ lpTarget)
{
	GUILD_INFO_STRUCT * lpGuildInfo	= lpObj->Guild;
	GUILD_INFO_STRUCT * lpTarGuildInfo	= lpTarget->Guild;
	
	if( this->m_State != AB_STATE_START )
	{
		return true;
	}
	
	if( lpGuildInfo == NULL )
	{
		return false;
	}
	
	if( lpTarGuildInfo == NULL )
	{
		return false;
	}

	if (lpObj->Map != 83 && lpObj->Map != 84)
	{
		return false;
	}
	
	if( lpGuildInfo->Number != lpTarGuildInfo->Number )
	{
		return false;
	}
	
	return true;
}


void CArcaBattle::CGArcaBattleGuildMasterRegisterRecv(int aIndex)
{
	SDHP_ARCA_BATTLE_GUILD_MASTER_REGISTER_SEND pMsg{};

	pMsg.header.set(0x1B,0x00,sizeof(pMsg));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CArcaBattle::CGArcaBattleGuildMemberRegisterRecv(int aIndex)
{
	SDHP_ARCA_BATTLE_GUILD_MEMBER_REGISTER_SEND pMsg{};

	pMsg.header.set(0x1B,0x00,sizeof(pMsg));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CArcaBattle::CGArcaBattleRegisterMarkRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_ARCA_BATTLE_REGISTER_MARK_ERROR_SEND pMsg;

	pMsg.header.set(0xF8,0x47,sizeof(pMsg));

	int iArcaBattleState = this->m_State;

	if ( iArcaBattleState < 3 || iArcaBattleState > 9 )
	{
		if(lpObj->Guild == 0)
		{
			pMsg.result = 15; //NO AUTHORITHA
			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		if(lpObj->GuildStatus != 32 && lpObj->GuildStatus != 64 && lpObj->GuildStatus != 128)
		{
			pMsg.result = 15; //NO AUTHORITHA
			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		if(lpObj->Guild->Count < 10) //MIN GUILD NUM
		{
			pMsg.result = 16; //NOT ENOUGH MEMBAS
			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}
	}
	else
	{
		pMsg.result = 5; //NO TIME
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
}


void CArcaBattle::GCArcaBattleOccupyGuildSend(LPOBJ lpObj,int gate)
{
	PMSG_ARCA_BATTLE_OCCUPY_GUILD_SEND pMsg{};

	pMsg.header.set(0xF8,0x3D,sizeof(pMsg));

	if( gate == 418 || gate == 422 )
	{
		for( int i = 0; i < MAX_ARCA_BATTLE_OBELISK; i++ )
		{
			if(gate == 418 && this->m_GuildInfo[i].ObeliskGroup == 1  ||	gate == 422 && this->m_GuildInfo[i].ObeliskGroup == 2 )	//Maybe it obelist attr (dark, earth...)
			{
				memcpy(pMsg.GuildName, this->m_GuildInfo[i].GuildName, 9); //MAX_GUILD_NAME define
				DataSend(lpObj->Index, (LPBYTE)&pMsg, sizeof(PMSG_ARCA_BATTLE_OCCUPY_GUILD_SEND));
				break;
			}
		}
	}
}
