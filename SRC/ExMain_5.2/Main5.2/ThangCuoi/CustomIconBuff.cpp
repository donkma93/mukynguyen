#include "stdafx.h"
#include "CustomIconBuff.h"
#include <ZzzInventory.h>
#include <UIControls.h>
#include "WideData.h"

BuffIcon gIconBuff;

BuffIcon::BuffIcon()
{
	this->LoadBuffCount = 0;
	this->UserBuffCount = 0;
	ZeroMemory(&RenderInfoBuff, sizeof(RenderInfoBuff));
	ZeroMemory(&UserBuff, sizeof(UserBuff));
	BuffEffectTRSData_ENG.clear();
	this->hThread = (HANDLE)_beginthreadex(NULL, 0, this->Timer, 0, 0, NULL);
	memset(NewAddressData1, 0, sizeof(NewAddressData1));
	memset(NewAddressData2, 0, sizeof(NewAddressData2));
	memset(NewAddressData3, 0, sizeof(NewAddressData3));
}

BuffIcon::~BuffIcon()
{
	CloseHandle(this->hThread);
}

void BuffIcon::LoadEng(RenderTooltipBuff* info)
{
	for (int n = 0; n < 256; n++)
	{
		if (info[n].IndexBuff <= 0 || info[n].NameBuff[0] == 0)
		{
			continue;
		}

		this->BuffEffectTRSData_ENG.insert(std::pair<int, RenderTooltipBuff>(info[n].IndexBuff, info[n]));
	}
}


void BuffIcon::Recv(PMSG_SEND_BUFF_ICON* DataRecv)
{
	if (DataRecv->state == 0 && DataRecv->effect > 0)
	{
		for (int i = 0; i < MAX_EFFECT_LIST; i++)
		{
			if (this->UserBuff[i].Index == NULL || this->UserBuff[i].Index == DataRecv->effect)
			{
				if (this->UserBuff[i].Index == NULL)
				{
					this->UserBuffCount++;
				}
				this->UserBuff[i].Index = DataRecv->effect;
				this->UserBuff[i].Time = DataRecv->time;
				this->UserBuff[i].MaxTime = DataRecv->time;
				return;
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_EFFECT_LIST; i++)
		{
			if (this->UserBuff[i].Index == DataRecv->effect)
			{
				this->UserBuff[i].Index = NULL;
				this->UserBuff[i].Time = 0;
				this->UserBuff[i].MaxTime = 0;

				this->UserBuffCount--;
				return;
			}
		}
	}
}

unsigned int BuffIcon::Timer(void* Args)
{
	while (true)
	{
		for (int i = 0; i < MAX_EFFECT_LIST; i++)
		{
			if (gIconBuff.UserBuff[i].Index != NULL)
			{
				if (gIconBuff.UserBuff[i].Time > 0)
				{
					gIconBuff.UserBuff[i].Time--;
					if (gIconBuff.UserBuff[i].Time <= 0)
					{
						gIconBuff.UserBuff[i].Index = NULL;
						gIconBuff.UserBuff[i].Time = 0;

						gIconBuff.UserBuffCount--;
					}
				}
			}
			Sleep(1);
		}
		Sleep(999);
	}
	return 0;
}

void BuffIcon::DeleteAllBuff()
{
	this->UserBuffCount = 0;
	for (int i = 0; i < MAX_EFFECT_LIST; i++)
	{
		this->UserBuff[i].Index = NULL;
		this->UserBuff[i].Time = 0;
		this->UserBuff[i].MaxTime = 0;
	}
}

void BuffIcon::FormatBuffTooltip(int BuffIndex, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(RenderInfoBuff[BuffIndex], fmt, args);
	va_end(args);

	strcpy(gIconBuff.BuffEffectTRSData_ENG[BuffIndex].InfoBuff, RenderInfoBuff[BuffIndex]);
}

void BuffIcon::GC_BuffInfo(PMSG_SEND_BUFF_ICON_EX* aRecv)
{
	int BuffIndex = aRecv->btEffectIndex;

	bool continuar = false;

	std::string strSelectedML = "Eng";

	std::map<int, RenderTooltipBuff>::iterator it;

	if (strcmp(strSelectedML.c_str(), "Eng") == 0)
	{
		it = this->BuffEffectTRSData_ENG.find(BuffIndex);

		if (it != this->BuffEffectTRSData_ENG.end())
		{
			continuar = true;
		}
	}

	if (continuar == true)
	{
		switch (BuffIndex)
		{
			case EFFECT_GREATER_DAMAGE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_GREATER_DEFENSE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;
			case EFFECT_ELF_BUFFER:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_MANA_SHIELD:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_GREATER_CRITICAL_DAMAGE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_INFINITY_ARROW:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BP_RECOVERY:
				break;

			case EFFECT_GREATER_LIFE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_GREATER_MANA:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_BLESS_POTION:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SOUL_POTION:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_DISABLE_MAGIC:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CASTLE_GATE_STATE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GUILD_STATE1:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GUILD_STATE2:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GUILD_STATE3:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GUILD_STATE4:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_INVISIBILITY:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GUILD_STATE5:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CASTLE_CROWN_STATE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE1:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE2:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE3:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE4:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE5:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE6:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE7:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GAME_MASTER:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SEAL_OF_ASCENSION1:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SEAL_OF_WEALTH1:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_SEAL_OF_SUSTENANCE1:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ORDER_OF_SPEED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ORDER_OF_SUBLIMATION:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ORDER_OF_PROTECTION:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_HALLOWEEN1:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_HALLOWEEN2:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_HALLOWEEN3:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_HALLOWEEN4:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_HALLOWEEN5:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SEAL_OF_ASCENSION2:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SEAL_OF_WEALTH2:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_SEAL_OF_SUSTENANCE2:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SEAL_OF_MOVEMENT:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SCROLL_OF_QUICK:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_DEFENSE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_DAMAGE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_MAGIC_DAMAGE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_LIFE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_MANA:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_ELIXIR_OF_STRENGTH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_ELIXIR_OF_DEXTERITY:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_ELIXIR_OF_VITALITY:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_ELIXIR_OF_ENERGY:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_ELIXIR_OF_LEADERSHIP:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_POISON:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ICE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ICE_ARROW:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_FIRE_SLASH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_PHYSI_DAMAGE_IMMUNITY:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MAGIC_DAMAGE_IMMUNITY:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_STERN:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MAGIC_DEFENSE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ORDER_OF_RESTRAINT:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE8:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE9:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE10:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE11:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CRYWOLF_STATE12:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_DAMAGE_REFLECT:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SLEEP:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BLIND:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_NEIL:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SAHAMUTT:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_LESSER_DAMAGE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_LESSER_DEFENSE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHERRY_BLOSSOM1:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHERRY_BLOSSOM2:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHERRY_BLOSSOM3:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_SWORD_POWER:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_MAGIC_CIRCLE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_SWORD_SLASH:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_LIGHTNING_STORM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_RED_STORM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_FROZEN_STAB:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_SEAL_OF_LIFE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_SEAL_OF_MANA:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_SCROLL_OF_BATTLE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_STRENGTH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS1:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_CHRISTMAS2:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS3:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS4:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS5:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS6:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_CHRISTMAS7:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_DUEL_ARENA_WATCH:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_TALISMAN_OF_GUARDIAN:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_TALISMAN_OF_PROTECTION:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MASTER_SEAL_OF_ASCENSION:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_MASTER_SEAL_OF_WEALTH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_GLADIATORS_GLORY:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_PARTY_EXPERIENCE_BONUS:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_MAX_AG_BOOST_AURA:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_MAX_SD_BOOST_AURA:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_SCROLL_OF_HEALING:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_HAWK_FIGURINE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GOAT_FIGURINE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_OAK_CHARM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MAPLE_CHARM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GOLDEN_OAK_CHARM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GOLDEN_MAPLE_CHARM:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_WORN_HORSESHOE:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GREATER_IGNORE_DEFENSE_RATE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_FITNESS:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_GREATER_DEFENSE_SUCCESS_RATE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_IRON_DEFENSE:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_GREATER_LIFE_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_GREATER_LIFE_MASTERED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_DEATH_STAB_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_MAGIC_CIRCLE_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_MAGIC_CIRCLE_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_MANA_SHIELD_MASTERED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_FROZEN_STAB_MASTERED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BLESS:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_INFINITY_ARROW_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_BLIND_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_DRAIN_LIFE_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_ICE_STORM_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_EARTH_PRISON:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_GREATER_CRITICAL_DAMAGE_MASTERED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_GREATER_CRITICAL_DAMAGE_EXTENDED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_SWORD_POWER_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_SWORD_POWER_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_SWORD_POWER_MASTERED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_GREATER_DEFENSE_SUCCESS_RATE_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_GREATER_DEFENSE_SUCCESS_RATE_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0], aRecv->iValue[1]);
			}
			break;

			case EFFECT_FITNESS_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_DRAGON_ROAR_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_CHAIN_DRIVER_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_POISON_ARROW:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_POISON_ARROW_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BLESS_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0], aRecv->iValue[0]);
			}
			break;

			case EFFECT_LESSER_DAMAGE_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_LESSER_DEFENSE_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_FIRE_SLASH_ENHANCED:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff, aRecv->iValue[0]);
			}
			break;

			case EFFECT_IRON_DEFENSE_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BLOOD_HOWLING:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_BLOOD_HOWLING_IMPROVED:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_PENTAGRAM_JEWEL_HALF_SD:
			case EFFECT_PENTAGRAM_JEWEL_HALF_MP:
			case EFFECT_PENTAGRAM_JEWEL_HALF_SPEED:
			case EFFECT_PENTAGRAM_JEWEL_HALF_HP:
			case EFFECT_PENTAGRAM_JEWEL_STUN:
			case EFFECT_PENTAGRAM_JEWEL_SLOW:
			case EFFECT_TALISMAN_OF_ASCENSION1:
			case EFFECT_TALISMAN_OF_ASCENSION2:
			case EFFECT_TALISMAN_OF_ASCENSION3:
			case EFFECT_SEAL_OF_ASCENSION3:
			case EFFECT_MASTER_SEAL_OF_ASCENSION2:
			case EFFECT_BLESSING_OF_LIGHT:
			case EFFECT_MASTER_SCROLL_OF_DEFENSE:
			case EFFECT_MASTER_SCROLL_OF_MAGIC_DAMAGE:
			case EFFECT_MASTER_SCROLL_OF_LIFE:
			case EFFECT_MASTER_SCROLL_OF_MANA:
			case EFFECT_MASTER_SCROLL_OF_DAMAGE:
			case EFFECT_MASTER_SCROLL_OF_HEALING:
			case EFFECT_MASTER_SCROLL_OF_BATTLE:
			case EFFECT_MASTER_SCROLL_OF_STRENGTH:
			case EFFECT_MASTER_SCROLL_OF_QUICK:
			{
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
			}
			break;

			case EFFECT_DAME_ITEM_SWICH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2]);
			}
			break;

			case EFFECT_DAME_SET_ITEM_SWICH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_DAME_PENRING_ITEM_SWICH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_DAME_WING_ITEM_SWICH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_GUILD_BUFF:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			case EFFECT_DAME_KHIEN_ITEM_SWICH:
			{
				FormatBuffTooltip(BuffIndex, it->second.InfoBuff,
					aRecv->iValue[0], aRecv->iValue[1], aRecv->iValue[2], aRecv->iValue[3]);
			}
			break;

			default:
				FormatBuffTooltip(BuffIndex, "%s", it->second.InfoBuff);
				break;

		}
	}
}

void BuffIcon::RenderTipBuff(int BuffIndex, int IndexTime, float RenderX, float RenderY)
{
	int TextNum = 0;
	memset(TextList, 0, sizeof(TextList));
	memset(TextListColor, 0, sizeof(int) * 30);
	memset(TextBold, 0, sizeof(int) * 30);

	auto it = gIconBuff.BuffEffectTRSData_ENG.find(BuffIndex);
	if (it == gIconBuff.BuffEffectTRSData_ENG.end())
		return;

	unicode::_sprintf(TextList[TextNum], it->second.NameBuff);
	TextListColor[TextNum] = TEXT_COLOR_BLUE;
	TextBold[TextNum] = true;
	TextNum++;

	if (it->second.InfoBuff[0] != 0)
	{
		char infoText[128] = { 0 };
		strcpy_s(infoText, sizeof(infoText), it->second.InfoBuff);
		char* token = strtok(infoText, "/");
		while (token && TextNum < 30)
		{
			if (strstr(token, "+0") == nullptr && strstr(token, "-1") == nullptr)
			{
				unicode::_sprintf(TextList[TextNum], token);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				TextNum++;
			}
			token = strtok(NULL, "/");
		}
	}

	if (gIconBuff.UserBuff[IndexTime].Time > 0 && gIconBuff.UserBuff[IndexTime].Index != 28)
	{
		time_t t = gIconBuff.UserBuff[IndexTime].Time;
		tm* m = gmtime(&t);
		char timerLine[128] = { 0 };

		if (m->tm_yday > 0)
		{
			sprintf(timerLine, "%d %s %d %s %d %s %d %s",
				m->tm_yday, GlobalText[2298],
				m->tm_hour, GlobalText[2299],
				m->tm_min, GlobalText[2300],
				m->tm_sec, GlobalText[2301]);
		}
		else if (m->tm_hour > 0)
		{
			sprintf(timerLine, "%d %s %d %s %d %s",
				m->tm_hour, GlobalText[2299],
				m->tm_min, GlobalText[2300],
				m->tm_sec, GlobalText[2301]);
		}
		else if (m->tm_min > 0)
		{
			sprintf(timerLine, "%d %s %d %s",
				m->tm_min, GlobalText[2300],
				m->tm_sec, GlobalText[2301]);
		}
		else
		{
			sprintf(timerLine, "%d %s",
				m->tm_sec, GlobalText[2301]);
		}

		unicode::_sprintf(TextList[TextNum], GlobalText[2533], timerLine);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		TextNum++;
	}

	RenderTipTextList(RenderX, RenderY, TextNum, 0, 3, STRP_BOTTOMCENTER, TRUE, FALSE);
}



void BuffIcon::RenderBasicTooltip(float x, float y, const std::vector<std::string>& lines)
{
	if (lines.empty()) return;

	SIZE textSize = { 0, 0 };
	int maxWidth = 0;
	int lineHeight = 0;

	for (const auto& line : lines)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), line.c_str(), (int)line.length(), &textSize);
		if (maxWidth < textSize.cx) maxWidth = textSize.cx;
		if (lineHeight < textSize.cy) lineHeight = textSize.cy;
	}

	const float bgPadding = 4.f;
	const float spacing = 2.0f;
	float boxWidth = maxWidth / gPosWide.x_fScreenRate_x + bgPadding * 2;
	float boxHeight = ((lineHeight + spacing) * lines.size()) / gPosWide.x_fScreenRate_y + bgPadding * 2;

	float fx = x - boxWidth / 2;
	float fy = y;

	if (fx < 0) fx = 0;
	if (fx + boxWidth > WindowWidth / gPosWide.x_fScreenRate_x)
		fx = WindowWidth / gPosWide.x_fScreenRate_x - boxWidth;

	EnableAlphaTest();
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.f, 0.f, 0.f, 0.75f);
	RenderColor(fx, fy, boxWidth, boxHeight);
	glColor4f(0.f, 0.f, 0.f, 1.f);
	RenderColor(fx - 1, fy - 1, boxWidth + 2, 1);
	RenderColor(fx - 1, fy + boxHeight, boxWidth + 2, 1);
	RenderColor(fx - 1, fy - 1, 1, boxHeight + 2);
	RenderColor(fx + boxWidth, fy - 1, 1, boxHeight + 2);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	float cursorY = fy + bgPadding;
	for (const auto& line : lines)
	{
		if (line.empty()) continue;

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(fx + bgPadding, cursorY, line.c_str(), boxWidth - bgPadding * 2, 0, 0, nullptr);
		cursorY += (lineHeight + spacing) / gPosWide.x_fScreenRate_y;
	}

	DisableAlphaBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);
}