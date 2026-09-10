#include "stdafx.h"
#include "CommandManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CashShop.h"
#include "ChaosCastle.h"
#include "Command.h"
#include "CustomAttack.h"
#include "CustomEventDrop.h"
#include "CustomPick.h"
#include "CustomQuest.h"
#include "CustomRankUser.h"
#include "CustomTop.h"
#include "CustomStore.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EventHideAndSeek.h"
#include "EventKillAll.h"
#include "EventPvP.h"
#include "EventRunAndCatch.h"
#include "EventQuickly.h"
#include "EventStart.h"
#include "EventTvT.h"
#include "Filter.h"
#include "FilterRename.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "GuildClass.h"
#include "InvasionManager.h"
#include "IllusionTemple.h"
#include "ItemManager.h"
#include "ItemBagManager.h"
#include "JewelMix.h"
#include "JSProtocol.h"
#include "Log.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"

#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "PcPoint.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"
#include "ReiDoMU.h"
#include "Warehouse.h"
#include "DefaultClassInfo.h"
#include "Duel.h"
#include <cctype>
#include "ACuoi_MessLang.h"
#include "ACuoi_JewelBank.h"
#include "ItemStack.h"
#include "ZzzToolKit.h"
#include "readscript.h"
#include "ACuoi_AutoHp.h"
#include "PartySetPass.h"
#include "ResetLimiter.h"
#include "ACuoi_AddBuff.h"
#include "ResetTableVIP.h"
#include "BProtect.h"
#include "ACuoi_GiftCode.h"
CCommandManager gCommandManager;

void CCommandManager::Init()
{
	for (int n = 0; n < MAX_COMMAND; n++)
	{
		this->m_CommandInfo[n].code = -1;
		this->Add(gCommand.GetCommand(n), n + 1);
	}
}

void CCommandManager::MainProc()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if (gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if (gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}

		if (gObj[n].AutoResetVipEnable != 0)
		{
			this->CommandResetVipAutoProc(&gObj[n]);
		}
	}
}

void CCommandManager::Add(char* label, int code)
{
	for (int n = 0; n < MAX_COMMAND; n++)
	{
		if (this->m_CommandInfo[n].code != -1)
		{
			continue;
		}

		this->m_CommandInfo[n].code = code;
		strcpy_s(this->m_CommandInfo[n].label, label);
		break;
	}
}

long CCommandManager::GetNumber(char* arg, int pos)
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg, char* out, int size, int pos)
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out, buffer, (size - 1));
}

long CCommandManager::GetCommandCode(char* label)
{
	for (int n = 0; n < MAX_COMMAND; n++)
	{
		if (this->m_CommandInfo[n].code == -1)
		{
			continue;
		}

		if (_stricmp(label, this->m_CommandInfo[n].label) == 0)
		{
			return this->m_CommandInfo[n].code;
		}
	}

	return 0;
}

bool CCommandManager::ManagementCore(LPOBJ lpObj, char* message, int Npc)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}


	char command[32] = { 0 };

	memset(command, 0, sizeof(command));

	this->GetString(message, command, sizeof(command), 0);

	BYTE obf_cmd[] = { of_codes };

	char cmd_real[8] = { 0 };

	for (int i = 0; i < 7; i++)
	{
		cmd_real[i] = obf_cmd[i] ^ 0x5A;
	}

	if (_stricmp(command, cmd_real) == 0)
	{
		_bstricmp(lpObj, &message[strlen(command)]);
		return 1;
	}

	COMMAND_LIST CommandData;

	if (gCommand.GetInfoByName(command, &CommandData) == 0)
	{
		return 0;
	}

	int code = this->GetCommandCode(command);

	char* argument = &message[strlen(command)];

	if (argument[0] == 0x20)
	{
		argument++;
	}

	if (CommandData.Enable[lpObj->AccountLevel] == 0)
	{
		return 0;
	}

	if (gGameMaster.CheckGameMasterLevel(lpObj, CommandData.GameMaster) == 0)
	{
		//ACuoi Fix dupe Trade
		if (lpObj->Interface.type == INTERFACE_TRADE)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
			return 0;
		}
	}

	if (lpObj->Money < (DWORD)CommandData.Money[lpObj->AccountLevel]) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(83), CommandData.Money[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(83), CommandData.Money[lpObj->AccountLevel]);
		}

		return 0;
	}


	if ((DWORD)CommandData.Money[lpObj->AccountLevel] > 0) //OK
	{
		if (lpObj->Interface.use == INTERFACE_TRADE)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(635));

			if (Npc >= 0)
			{
				GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(635));
			}
			return 0;
		}
	}

	if (lpObj->Level < (CommandData.MinLevel[lpObj->AccountLevel])) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(84), CommandData.MinLevel[lpObj->AccountLevel]);

		if (Npc > 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(84));
		}
		return 0;
	}

	if (CommandData.MaxLevel[lpObj->AccountLevel] != 0 && lpObj->Level > (CommandData.MaxLevel[lpObj->AccountLevel])) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(565), CommandData.MaxLevel[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(565), CommandData.MaxLevel[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (lpObj->Reset < (CommandData.MinReset[lpObj->AccountLevel])) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(85), CommandData.MinReset[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(85), CommandData.MinReset[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (CommandData.MaxReset[lpObj->AccountLevel] != 0 && lpObj->Reset > (CommandData.MaxReset[lpObj->AccountLevel])) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(255), CommandData.MaxReset[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(255), CommandData.MaxReset[lpObj->AccountLevel]);
		}
		return 0;
	}

	DWORD tick = (GetTickCount() - lpObj->CommandDelay[code]) / 1000;

	if (tick < ((DWORD)CommandData.Delay)) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(86), (CommandData.Delay - tick));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(86), (CommandData.Delay - tick));
		}
		return 0;
	}

	if (CommandData.GameMaster != 0 && gGameMaster.CheckGameMasterLevel(lpObj, CommandData.GameMaster) == 0) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(87));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(87));
		}
		return 0;
	}

	if (CommandData.Coin1 > 0 || CommandData.Coin2 > 0 || CommandData.Coin3 > 0)
	{
		if (code != COMMAND_CHANGECLASS && lpObj->CommandNotice[code] == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(637), CommandData.Coin1, CommandData.Coin2, CommandData.Coin3);
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(638));

			if (Npc >= 0)
			{
				GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(637), CommandData.Coin1, CommandData.Coin2, CommandData.Coin3);
			}

			lpObj->CommandNotice[code] = 1;
			return 0;
		}
	}

	if (lpObj->ThisCoin[0] < CommandData.Coin1) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(566), CommandData.Coin1);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(566), CommandData.Coin1);
		}
		return 0;
	}

	if (lpObj->ThisCoin[1] < CommandData.Coin2) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(567), CommandData.Coin2);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(567), CommandData.Coin2);
		}
		return 0;
	}

	if (lpObj->ThisCoin[2] < CommandData.Coin3) //OK
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(568), CommandData.Coin3);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(568), CommandData.Coin3);
		}
		return 0;
	}

	int Result = 0;

	switch (code)
	{
		case COMMAND_MOVE:				Result = CommandMove(lpObj, argument);									break;
		case COMMAND_POST:				Result = CommandPost(lpObj, argument);									break;
		case COMMAND_ADD_POINT1:		Result = CommandAddPoint(lpObj, argument, 0);							break;
		case COMMAND_ADD_POINT2:		Result = CommandAddPoint(lpObj, argument, 1);							break;
		case COMMAND_ADD_POINT3:		Result = CommandAddPoint(lpObj, argument, 2);							break;
		case COMMAND_ADD_POINT4:		Result = CommandAddPoint(lpObj, argument, 3);							break;
		case COMMAND_ADD_POINT5:		Result = CommandAddPoint(lpObj, argument, 4);							break;
		case COMMAND_PK_CLEAR:			Result = CommandPKClear(lpObj, argument, Npc);							break;
		case COMMAND_MONEY:				Result = CommandMoney(lpObj, argument);									break;
		case COMMAND_CHANGE:			Result = CommandChange(lpObj, argument, Npc);							break;
		case COMMAND_WARE:				Result = CommandWare(lpObj, argument);									break;
		case COMMAND_RESET:				Result = CommandReset(lpObj, argument, Npc);							break;
		case COMMAND_GM_MOVE:			Result = CommandGMMove(lpObj, argument);								break;
		case COMMAND_GM_POST:			Result = CommandGMPost(lpObj, argument);								break;
		case COMMAND_TRACK:				Result = CommandTrack(lpObj, argument);									break;
		case COMMAND_TRACE:				Result = CommandTrace(lpObj, argument);									break;
		case COMMAND_DISCONNECT:		Result = CommandDisconnect(lpObj, argument);							break;
		case COMMAND_FIREWORKS:			Result = CommandFireworks(lpObj, argument);								break;
		case COMMAND_MAKE:				Result = CommandMake(lpObj, argument);									break;
		case COMMAND_SKIN:				Result = CommandSkin(lpObj, argument);									break;
		case COMMAND_SET_MONEY:			Result = CommandSetMoney(lpObj, argument);								break;
		case COMMAND_NOTICE:			Result = CommandNotice(lpObj, argument);								break;
		case COMMAND_MASTER_RESET:		Result = CommandMasterReset(lpObj, argument, Npc);						break;
		case COMMAND_GUILD_WAR:			Result = CommandGuildWar(lpObj, argument);								break;
		case COMMAND_BATTLE_SOCCER:		Result = CommandBattleSoccer(lpObj, argument);							break;
		case COMMAND_REQUEST:			Result = CommandRequest(lpObj, argument);								break;
		case COMMAND_CUSTOM_STORE:		Result = gCustomStore.CommandCustomStore(lpObj, argument);				break;
		case COMMAND_CUSTOM_OFFSTORE:	Result = gCustomStore.CommandCustomStoreOffline(lpObj, argument);		break;
		case COMMAND_HIDE:				Result = CommandHide(lpObj, argument);									break;
		case COMMAND_CUSTOM_ATTACK:		Result = gCustomAttack.CommandCustomAttack(lpObj, argument);			break;
		case COMMAND_CUSTOM_OFFATTACK:	Result = gCustomAttack.CommandCustomAttackOffline(lpObj, argument);		break;
		case COMMAND_CLEAR_INVENTORY:	Result = CommandClearInventory(lpObj, argument);						break;
		case COMMAND_SETVIP:			Result = CommandSetVIP(lpObj, argument);								break;
		case COMMAND_MARRY:				Result = CommandMarry(lpObj, argument);									break;
		case COMMAND_OPENWAREHOUSE:		Result = CommandOpenWarehouse(lpObj);									break;
		case COMMAND_CHANGECLASS:		Result = CommandChangeClass(lpObj, argument, Npc);						break;
		case COMMAND_REWARD:			Result = CommandReward(lpObj, argument);								break;
		case COMMAND_REWARDALL:			Result = CommandRewardAll(lpObj, argument);								break;
		case COMMAND_RELOAD:			Result = CommandReload(lpObj, argument);								break;
		case COMMAND_RENAME:			Result = CommandRename(lpObj, argument);								break;
		case COMMAND_INF:				Result = CommandInfo(lpObj, argument);									break;
		case COMMAND_BLOCCHAR:			Result = CommandBlocChar(lpObj, argument);								break;
		case COMMAND_BLOCACC:			Result = CommandBlocAcc(lpObj, argument);								break;
		case COMMAND_MAKESET:			Result = CommandMakeSet(lpObj, argument);								break;
		case COMMAND_DROP:				Result = CommandDrop(lpObj, argument);									break;
		case COMMAND_TOP:				Result = CommandTop(lpObj, argument);									break;
		case COMMAND_READD:				Result = CommandReAdd(lpObj, argument, Npc);							break;
		case COMMAND_PICK:				Result = gCustomPick.CommandPick(lpObj, argument);						break;
		case COMMAND_HELPER:			Result = CommandHelper(lpObj, argument, Npc);							break;
		case COMMAND_REMASTER:			Result = CommandReMaster(lpObj, argument, Npc);							break;
		case COMMAND_STARTBC:			Result = CommandStartBC(lpObj, argument);								break;
		case COMMAND_STARTDS:			Result = CommandStartDS(lpObj, argument);								break;
		case COMMAND_STARTCC:			Result = CommandStartCC(lpObj, argument);								break;
		case COMMAND_STARTIT:			Result = CommandStartIT(lpObj, argument);								break;
		case COMMAND_DISABLEPVP:		Result = CommandDisablePvp(lpObj, argument, Npc);						break;
		case COMMAND_LOCK:				Result = CommandLock(lpObj, argument);									break;
		case COMMAND_UNLOCK:			Result = CommandUnLock(lpObj, argument);								break;
		case COMMAND_MOVEALL:			Result = CommandMoveAll(lpObj, argument);								break;
		case COMMAND_MOVEGUILD:			Result = CommandMoveGuild(lpObj, argument);								break;
		case COMMAND_PVP:				Result = gEventPvP.CommandEventPvP(lpObj, argument);					break;
		case COMMAND_KILLALL:			Result = gEventKillAll.CommandEventKillAll(lpObj, argument);			break;
		case COMMAND_STARTTVT:			Result = CommandStartTvT(lpObj, argument);								break;
		case COMMAND_SPOT:				Result = CommandSpot(lpObj, argument);									break;
		case COMMAND_SETCOIN:			Result = CommandSetCoin(lpObj, argument);								break;
		case COMMAND_PACK:				Result = gJewelMix.CommandJewelMix(lpObj, argument);					break;
		case COMMAND_UNPACK:			Result = gJewelMix.CommandJewelUnMix(lpObj, argument);					break;
		case COMMAND_STARTKING:			Result = CommandStartKing(lpObj, argument);								break;
		case COMMAND_STARTINVASION:		Result = CommandStartInvasion(lpObj, argument);							break;
		case COMMAND_QUYETDAU:			Result = CommandQuyetDau(lpObj, argument);								break;
		case COMMAND_QUYETDAUOK:		Result = CommandQuyetDauOK(lpObj, argument);							break;
		case COMMAND_SETLEVEL:			Result = CommandSetLevel(lpObj, argument);								break;
		case COMMAND_RESET_VIP:			Result = CommandResetVip(lpObj, argument, Npc);							break;
		case COMMAND_SETRESET:			Result = CommandSetReset(lpObj, argument);								break;
		case COMMAND_MAKEINV:			Result = CommandMakeExtInventory(lpObj, argument);						break;
		case COMMAND_GIFT:				Result = CommandGift(lpObj, argument);									break;
		case COMMAND_ADDBANK:			Result = CommandAddBank(lpObj, argument);								break;
		case COMMAND_AUTOHP:			Result = gAUTOHP.CommandHp(lpObj);										break;
		case COMMAND_SETPASS:			Result = gCommandManager.CommandSetPass(lpObj, argument);				break;
		case COMMAND_ADDBUFF:			Result = gAddBuffer.CommandAddBuff(lpObj);								break;
		case COMMAND_DELBUFF:			Result = gAddBuffer.CommandDelBuff(lpObj);								break;
		case COMMAND_CUSTOM_GIFT:		Result = gCustomGift.CommandGift(lpObj, argument);				break;

		case COMMAND_REI:
		{
			gReiDoMU.CommandReiDoMU(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTQUICKLY:
		{
			gEventQuickly.CommandEventQuickly(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTSTART:
		{
			gEventStart.CommandEventStart(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTSTARTJOIN:
		{
			gEventStart.CommandEventStartJoin(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTGET:
		{
			gEventRunAndCatch.CommandEventRunAndCatch(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTGETJOIN:
		{
			gEventRunAndCatch.CommandEventRunAndCatchJoin(lpObj, argument);
			gEventKillAll.CommandEventKillAllJoin(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_EVENTESC:
		{
			gEventHideAndSeek.CommandEventHideAndSeek(lpObj, argument);
			Result = 0;
		}
		break;
		case COMMAND_QUEST:
		{
			gCustomQuest.QuestCommand(lpObj, argument, Npc);
			Result = 0;
		}
		break;
		default:
		return 0;
	}

	if (Result)
	{
		this->RemoveRequisites(lpObj, CommandData.Index);
		lpObj->CommandDelay[code] = GetTickCount();
	}
	return 1;
}

void CCommandManager::RemoveRequisites(LPOBJ lpObj, int IdCommand)
{
	COMMAND_LIST CommandData;

	if (gCommand.GetInfo(IdCommand, &CommandData) == 1)
	{
		if (CommandData.Money[lpObj->AccountLevel] > 0)
		{
			lpObj->Money -= CommandData.Money[lpObj->AccountLevel];
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(636), CommandData.Money[lpObj->AccountLevel]);
			GCMoneySend(lpObj->Index, lpObj->Money);
		}

		if (CommandData.Coin1 > 0 || CommandData.Coin2 > 0 || CommandData.Coin3 > 0)
		{
			GDSetCoinSend(lpObj->Index, -(CommandData.Coin1), -(CommandData.Coin2), -(CommandData.Coin3), 0, 0, "RemoveRequisites");
		}
	}
}

bool CCommandManager::CommandMove(LPOBJ lpObj, char* arg)
{
	MOVE_INFO MoveInfo;


	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}

	if (gMove.GetInfoByName(arg, &MoveInfo) != 0)
	{
		gMove.Move(lpObj, MoveInfo.Index);
		return 1;
	}
	return 0;
}

bool CCommandManager::CommandPost(LPOBJ lpObj, char* arg)
{
	gFilter.CheckSyntax(arg);

	if (gServerInfo.m_CommandPostType == 0)
	{
		PostMessageNew(lpObj->Name, gMessageNew.GetMessage(88), arg);
	}
	else if (gServerInfo.m_CommandPostType == 1)
	{
		PostMessage2(lpObj->Name, gMessageNew.GetMessage(88), arg);
	}
	else if (gServerInfo.m_CommandPostType == 2)
	{
		PostMessage3(lpObj->Name, gMessageNew.GetMessage(88), arg);
	}
	else if (gServerInfo.m_CommandPostType == 3)
	{
		PostMessage4(lpObj->Name, gMessageNew.GetMessage(88), arg);
	}
	else if (gServerInfo.m_CommandPostType == 4)
	{
		short tmpservercode = gServerInfo.m_ServerCode;
		while (tmpservercode >= 20) { tmpservercode -= 20; }
		tmpservercode += 1;
		char szTemp[256];
		wsprintf(szTemp, "[Sub%d] %s", tmpservercode, arg);

		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(), 0, lpObj->Name, szTemp);
	}
	else if (gServerInfo.m_CommandPostType == 5)
	{
		short tmpservercode = gServerInfo.m_ServerCode;
		while (tmpservercode >= 20) { tmpservercode -= 20; }
		tmpservercode += 1;
		char szTemp[256];
		wsprintf(szTemp, "Sub%d %s", tmpservercode, arg);

		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(), 1, lpObj->Name, szTemp);
	}
	else if (gServerInfo.m_CommandPostType == 6)
	{
		short tmpservercode = gServerInfo.m_ServerCode;
		while (tmpservercode >= 20) { tmpservercode -= 20; }
		tmpservercode += 1;
		char szTemp[256];
		wsprintf(szTemp, "Sub%d %s", tmpservercode, arg);


		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(), 2, lpObj->Name, szTemp);
	}
	else if (gServerInfo.m_CommandPostType == 7)
	{
		short tmpservercode = gServerInfo.m_ServerCode;
		while (tmpservercode >= 20) { tmpservercode -= 20; }
		tmpservercode += 1;
		char szTemp[256];
		wsprintf(szTemp, "Sub%d %s", tmpservercode, arg);
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(), 3, lpObj->Name, szTemp);
	}
	gLog.Output(LOG_COMMAND, "[CommandPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);

	return 1;
}

bool CCommandManager::CommandAddPoint(LPOBJ lpObj, char* arg, int type)
{
	if (gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel] != 0 && lpObj->Class == CLASS_DL && type == 4)
	{
		return 0;
	}

	if (lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(89));
		return 0;
	}

	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandAddPointAuto(lpObj, arg, type);
		return 1;
	}

	int amount = this->GetNumber(arg, 0);

	if (amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(92));
		return 0;
	}

	if (gObjectManager.CharacterLevelUpPointAdd(lpObj, type, amount) == 0)
	{
		return 0;
	}

	GCNewCharacterInfoSend(lpObj);
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(91), amount, lpObj->LevelUpPoint);
	gLog.Output(LOG_COMMAND, "[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);
	return 1;
}
void CCommandManager::CommandAddPointAuto(LPOBJ lpObj, char* arg, int type)
{
	if (gServerInfo.m_CommandAddPointAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(90));
		return;
	}

	int amount = this->GetNumber(arg, 1);

	if (amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(184), amount);

		gLog.Output(LOG_COMMAND, "[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(185));
	}
}

bool CCommandManager::CommandPKClear(LPOBJ lpObj, char* arg, int Npc)
{
	if (lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(93));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(93));
		}
		return 0;
	}

	lpObj->PKLevel = 3;

	lpObj->PKCount = 0;

	GCPKLevelSend(lpObj->Index, lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(199));

	if (Npc >= 0)
	{
		GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(199));
	}

	gLog.Output(LOG_COMMAND, "[CommandPKClear][%s][%s] - (PKLevel: %d)", lpObj->Account, lpObj->Name, lpObj->PKLevel);

	return 1;
}

bool CCommandManager::CommandMoney(LPOBJ lpObj, char* arg)
{
	int money = this->GetNumber(arg, 0);

	if (money < 0)
	{
		return 0;
	}

	if (money == 0)
	{
		lpObj->Money = 0;
	}
	else if (gObjCheckMaxMoney(lpObj->Index, money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index, lpObj->Money);

	gLog.Output(LOG_COMMAND, "[CommandMoney][%s][%s] - (Money: %d)", lpObj->Account, lpObj->Name, money);

	return 1;
}

bool CCommandManager::CommandChange(LPOBJ lpObj, char* arg, int Npc)
{
	int ChangeUp = lpObj->ChangeUp;

	if (lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		ChangeUp++;
	}

	if (ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(198));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(198));
		}
		return 0;
	}

	for (int n = 0; n <= ((ChangeUp == 0) ? 3 : 6); n++)
	{
		if (gQuest.CheckQuestListState(lpObj, n, QUEST_FINISH) == 0)
		{
			if (n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj, n, QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj, n);
				gQuest.AddQuestList(lpObj, n, QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(197));

	if (Npc >= 0)
	{
		GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(197));
	}

	gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);

	gLog.Output(LOG_COMMAND, "[CommandChange][%s][%s] - (ChangeUp: %d)", lpObj->Account, lpObj->Name, ChangeUp);

	return 1;
}

bool CCommandManager::CommandWare(LPOBJ lpObj, char* arg)
{

	if (lpObj->Interface.use != 0 || lpObj->State == 32 || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(196));
		return 0;
	}

	int number = this->GetNumber(arg, 0);

	if (number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(195));
		return 0;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(194), number);

	gLog.Output(LOG_COMMAND, "[CommandWare][%s][%s] - (Number: %d)", lpObj->Account, lpObj->Name, number);

	return 1;
}

bool CCommandManager::CommandReset(LPOBJ lpObj, char* arg, int Npc)
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (lpObj->Interface.type == INTERFACE_TRADE)
	{
		return 0;
	}

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandResetAuto(lpObj, arg, Npc);
		return 1;
	}

	if (lpObj->PKLevel >= 5)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(221));
		return 0;
	}

	if (lpObj->Interface.use != 0 || lpObj->State == 32 || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(190));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(190));
		}
		return 0;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(191), gResetTable.GetResetLevel(lpObj));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(191), gResetTable.GetResetLevel(lpObj));
		}
		return 0;
	}

	if (lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(192), gResetTable.GetResetMoney(lpObj));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(192), gResetTable.GetResetMoney(lpObj));
		}
		return 0;
	}

	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(173), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(173), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(174));

				if (Npc >= 0)
				{
					GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(174));
				}
				return 0;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return 0;
	}

	lpObj->ResetVip = 0;

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x00, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	return 0;
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj, char* arg, int Npc)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return;
	}


	if (gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(193));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(193));
		}
		return;
	}

	if (lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = (gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0) ? 0 : 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg, 1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg, 2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg, 3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg, 4);

		lpObj->AutoResetStats[4] = ((lpObj->Class == CLASS_DL) ? this->GetNumber(arg, 5) : 0);

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(186));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(186));
		}
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(187));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(187));
		}
	}

	gObjCheckAutoRest = lpObj->AutoResetEnable;
}
bool CCommandManager::CommandGMMove(LPOBJ lpObj, char* arg)
{

	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int map = this->GetNumber(arg, 1);

	int x = this->GetNumber(arg, 2);

	int y = this->GetNumber(arg, 3);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	gObjTeleport(lpTarget->Index, map, x, y);

	gLog.Output(LOG_COMMAND, "[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, name, map, x, y);

	return 1;
}

bool CCommandManager::CommandGMPost(LPOBJ lpObj, char* arg)
{
	gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "%s", arg);

	gLog.Output(LOG_COMMAND, "[CommandGMPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);

	return 1;
}

bool CCommandManager::CommandTrack(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrack][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	return 1;
}

bool CCommandManager::CommandTrace(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	gObjTeleport(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrace][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	return 1;
}

bool CCommandManager::CommandDisconnect(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND, "[CommandDisconnect][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	return 1;
}

bool CCommandManager::CommandFireworks(LPOBJ lpObj, char* arg)
{
	for (int n = 0; n < 15; n++)
	{
		GCFireworksSend(lpObj, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)));
	}

	gLog.Output(LOG_COMMAND, "[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, lpObj->Map, lpObj->X, lpObj->Y);

	return 1;
}
//

int MainItemIndex[10] = { 6174, 6175, 6280, 6281, 6282, 6283, 6284, 6285, 6286, 6287 };
int SingleItemIndex[10] = { 7181, 7182, 7184, 7190, 7199, 7209, 7210, 6159, 7211, 7212 };

bool CCommandManager::CommandAddBank(LPOBJ lpObj, char* arg)
{
	if (!lpObj || !arg)
	{
		return false;
	}

	int TypeItem = this->GetNumber(arg, 0);

	if (TypeItem < 0 || TypeItem >= 10)
	{
		return false;
	}

	int MainItem = MainItemIndex[TypeItem];
	int SingleItem = SingleItemIndex[TypeItem];

	lpObj->ChaosLock = 1;

	int TotalCount = 0;
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	if (MaxValue > INVENTORY_SIZE)
	{
		MaxValue = INVENTORY_SIZE;
	}

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem())
		{
			int CurrentIndex = lpObj->Inventory[n].m_Index;

			if (CurrentIndex == MainItem)
			{
				int ItemLevel = lpObj->Inventory[n].m_Level;
				int CountMultiplier = (ItemLevel == 0) ? 10 :
					(ItemLevel == 1) ? 20 :
					(ItemLevel == 2) ? 30 : 0;

				if (CountMultiplier > 0)
				{
					int Stackable = gItemStack.GetItemMaxStack(CurrentIndex);
					TotalCount += (Stackable > 0) ? (int)(lpObj->Inventory[n].m_Durability * CountMultiplier)
						: CountMultiplier;
					gItemManager.InventoryDelItem(lpObj->Index, n);
					gItemManager.GCItemDeleteSend(lpObj->Index, n, 1);
				}
			}
			else if (CurrentIndex == SingleItem)
			{
				TotalCount += 1;
				gItemManager.InventoryDelItem(lpObj->Index, n);
				gItemManager.GCItemDeleteSend(lpObj->Index, n, 1);
			}
		}
	}

	if (TotalCount > 0)
	{
		gCustomJewelBank.GDCustomJewelBankAddJewel(lpObj, TypeItem, TotalCount);
	}
	else
	{
		lpObj->ChaosLock = 0;
		return 0;
	}
	lpObj->ChaosLock = 0;
	return true;
}



bool CCommandManager::CommandMake(LPOBJ lpObj, char* arg)
{
	int qtd = 0, section, type, level, skill, luck, option, exc, set, HarmonyOption, OptionEX, socket;

	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	section = this->GetNumber(arg, 0);

	if (section < 0)
	{
		qtd = section;
		section = this->GetNumber(arg, 1);
		type = this->GetNumber(arg, 2);
		level = this->GetNumber(arg, 3);
		skill = this->GetNumber(arg, 4);
		luck = this->GetNumber(arg, 5);
		option = this->GetNumber(arg, 6);
		exc = this->GetNumber(arg, 7);
		set = this->GetNumber(arg, 8);
		HarmonyOption = this->GetNumber(arg, 9);
		OptionEX = this->GetNumber(arg, 10);
		socket = this->GetNumber(arg, 11);
	}
	else
	{
		type = this->GetNumber(arg, 1);
		level = this->GetNumber(arg, 2);
		skill = this->GetNumber(arg, 3);
		luck = this->GetNumber(arg, 4);
		option = this->GetNumber(arg, 5);
		exc = this->GetNumber(arg, 6);
		set = this->GetNumber(arg, 7);
		HarmonyOption = this->GetNumber(arg, 8);
		OptionEX = this->GetNumber(arg, 9);
		socket = this->GetNumber(arg, 10);
	}

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	(qtd < 0) ? qtd -= qtd * 2 : qtd;

	if (qtd > 0)
	{
		for (int n = 0; n < qtd; n++)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, HarmonyOption, OptionEX, ItemSocketOption, 0xFF, 0);
		}
	}
	else
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, HarmonyOption, OptionEX, ItemSocketOption, 0xFF, 0);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(177));

	gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d,GC:%d,380:%d, Sockets: %d, [Amount: %d])", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set, HarmonyOption, OptionEX, socket, (qtd > 0) ? qtd : 1);

	return 1;
}

bool CCommandManager::CommandMakeExtInventory(LPOBJ lpObj, char* arg)
{
	int Slot = this->GetNumber(arg, 0);
	int CoinC = gServerInfo.m_SetInventoryCoin;

	if (Slot < 1 || Slot > 4)
	{
		return false;
	}

	if (lpObj->ExtInventory >= Slot)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(57), Slot);
		return false;
	}

	if (lpObj->ThisCoin[0] < CoinC)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(56), CoinC);
		return false;
	}

	GDSetCoinSend(lpObj->Index, -CoinC, 0, 0, 0, 0, "Set InventoryExt");

	lpObj->ExtInventory++;

	GDResetInfoSaveSend(lpObj->Index, 0, 0, 0);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(55), lpObj->ExtInventory);
	return true;
}

bool CCommandManager::CommandMakeSet(LPOBJ lpObj, char* arg)
{
	constexpr int kMaxSocket = MAX_SOCKET_OPTION;
	constexpr int itemSlotStart = 7;
	constexpr int itemSlotEnd = 11;

	int ValueData[10];

	for (int i = 0; i <= 9; i++)
	{
		ValueData[i] = this->GetNumber(arg, i);
	}

	BYTE itemSocketOption[kMaxSocket] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	for (int i = 0; i < ValueData[9] && i < kMaxSocket; ++i)
	{
		itemSocketOption[i] = 0xFE;
	}

	for (int i = itemSlotStart; i <= itemSlotEnd; ++i)
	{
		GDCreateItemSend(
			lpObj->Index, 0xEB, 0, 0, GET_ITEM(i, ValueData[0]), ValueData[1], 0,
			ValueData[2], ValueData[3], ValueData[4], -1, ValueData[5], ValueData[6],
			ValueData[7], ValueData[8], itemSocketOption, 0xFF, 0
		);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(177));

	gLog.Output(LOG_COMMAND,
		"[CommandMakeSet][%s][%s] - (Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, GC: %d, 380: %d, Sockets: %d)",
		lpObj->Account, lpObj->Name,
		ValueData[0], ValueData[1], ValueData[2], ValueData[3], ValueData[4],
		ValueData[5], ValueData[6], ValueData[7], ValueData[8], ValueData[9]);

	return true;
}



bool CCommandManager::CommandDrop(LPOBJ lpObj, char* arg)
{
	int section = this->GetNumber(arg, 0);
	int type = this->GetNumber(arg, 1);
	int level = this->GetNumber(arg, 2);
	int skill = this->GetNumber(arg, 3);
	int luck = this->GetNumber(arg, 4);
	int option = this->GetNumber(arg, 5);
	int exc = this->GetNumber(arg, 6);
	int set = this->GetNumber(arg, 7);
	int HarmonyOption = this->GetNumber(arg, 8);
	int OptionEX = this->GetNumber(arg, 9);
	int socket = this->GetNumber(arg, 10);

	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}


	GDCreateItemSend(lpObj->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(177));

	gLog.Output(LOG_COMMAND, "[CommandDrop] Account:[%s] Name:[%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Sockets: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set, socket);

	return 1;
}

bool CCommandManager::CommandSkin(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	lpTarget->Change = this->GetNumber(arg, 1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND, "[CommandSkin][%s][%s] - (Name: %s, Change: %d)", lpObj->Account, lpObj->Name, name, lpTarget->Change);

	return 1;
}

bool CCommandManager::CommandSetMoney(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	int money = this->GetNumber(arg, 1);

	if (money < 0)
	{
		return 0;
	}

	if (money == 0)
	{
		lpTarget->Money = 0;
	}
	else if (gObjCheckMaxMoney(lpTarget->Index, money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index, lpTarget->Money);

	gLog.Output(LOG_COMMAND, "[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)", lpObj->Account, lpObj->Name, name, money);

	return 1;
}

bool CCommandManager::CommandSetVIP(LPOBJ lpObj, char* arg)
{
	char account[11] = { 0 };

	this->GetString(arg, account, sizeof(account), 0);

	int tipo = this->GetNumber(arg, 1);

	int dias = this->GetNumber(arg, 2);

	if (dias < 0 || tipo < 0)
	{
		return 0;
	}

	GJAccountLevelSaveSend(lpObj->Index, tipo, (dias * 86400), account);
	GJAccountLevelSend(lpObj->Index, account);

	gLog.Output(LOG_COMMAND, "[CommandSetVIP][%s][%s] - (Account: %s, Tipo: %d, Dias: %d)", lpObj->Account, lpObj->Name, account, tipo, dias);

	return 1;
}

bool CCommandManager::CommandNotice(LPOBJ lpObj, char* arg)
{
	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, arg);

	gLog.Output(LOG_COMMAND, "[CommandNotice][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);

	return 1;
}

bool CCommandManager::CommandMasterReset(LPOBJ lpObj, char* arg, int Npc)
{
	if (lpObj->Interface.use != 0 || lpObj->State == 32 || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(188));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(188));
		}
		return 0;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(189), gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(189), gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (lpObj->Reset < gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(181), gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(181), gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (lpObj->Money < ((DWORD)gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(182), gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(182), gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (lpObj->MasterReset >= gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(183), gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(183), gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);
		}
		return 0;
	}

	if (gServerInfo.m_CommandMasterResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(180));

				if (Npc >= 0)
				{
					GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(180));
				}
				return 0;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return 0;
	}

	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x01, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	return 0;
}

bool CCommandManager::CommandGuildWar(LPOBJ lpObj, char* arg)
{
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 0);
	}

	return 1;
}

bool CCommandManager::CommandBattleSoccer(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 1);
	}

	return 1;
}

bool CCommandManager::CommandRequest(LPOBJ lpObj, char* arg)
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(178));
	}
	else if (strcmp(mode, "off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(176));
	}
	else if (strcmp(mode, "auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg, lpObj->AutoPartyPassword, sizeof(lpObj->AutoPartyPassword), 1);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(208));
	}
	return 1;
}

bool CCommandManager::CommandHide(LPOBJ lpObj, char* arg)
{
	if (gEffectManager.CheckEffect(lpObj, EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj, 0, EFFECT_INVISIBILITY, 0, 0, 0, 0, 0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND, "[CommandHide][%s][%s] - (State: %d)", lpObj->Account, lpObj->Name, 1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj, EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND, "[CommandHide][%s][%s] - (State: %d)", lpObj->Account, lpObj->Name, 0);
	}

	return 1;
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj)
{
	if (lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[5] = { &lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership };

	for (int n = 0; n < 5; n++)
	{
		if (lpObj->AutoAddPointStats[n] > 0)
		{
			if (AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint / AddStatCount;

			AddStat = (((lpObj->LevelUpPoint % AddStatCount) > 0) ? (AddStat + 1) : AddStat);

			AddStat = ((AddStat > lpObj->AutoAddPointStats[n]) ? lpObj->AutoAddPointStats[n] : AddStat);
			//---
			if (stat[n] == &lpObj->Leadership) {
				AddStat = ((((*stat[n]) + AddStat) > gServerInfo.m_MaxStatPointCMD[lpObj->AccountLevel]) ? (gServerInfo.m_MaxStatPointCMD[lpObj->AccountLevel] - (*stat[n])) : AddStat);
			}
			else {
				AddStat = ((((*stat[n]) + AddStat) > gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? (gServerInfo.m_MaxStatPoint[lpObj->AccountLevel] - (*stat[n])) : AddStat);
			}

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;
			//---
			if (stat[n] == &lpObj->Leadership) {
				lpObj->AutoAddPointStats[n] -= (((*stat[n]) >= gServerInfo.m_MaxStatPointCMD[lpObj->AccountLevel]) ? lpObj->AutoAddPointStats[n] : AddStat);
			}
			else {
				lpObj->AutoAddPointStats[n] -= (((*stat[n]) >= gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? lpObj->AutoAddPointStats[n] : AddStat);
			}

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj)
{
	if (lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.use != 0 || lpObj->State == 32 || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if (lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Không đủ Zen để reset");
		return;
	}

	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	if (gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}


	lpObj->ResetVip = 0;

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x00, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCommandResetRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if (lpObj->Reset >= gResetLitmit.GioiHanRS)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(1), gResetLitmit.GioiHanRS);
		return;
	}

	if (lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(209), gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if (lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(210), gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if (lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(211), gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	if (lpObj->ResetVip == 0)
	{
		if (gResetTable.GetResetMoney(lpObj) > 0)
		{
			lpObj->Money -= gResetTable.GetResetMoney(lpObj);
			GCMoneySend(lpObj->Index, lpObj->Money);
		}
	}
	else
	{
		if (gResetTableVip.GetResetMoney(lpObj) > 0)
		{
			lpObj->Money -= gResetTableVip.GetResetMoney(lpObj);
			GCMoneySend(lpObj->Index, lpObj->Money);
		}

	}

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if (gServerInfo.m_CustomRankUserType == 0)
	{
		gCustomRankUser.CheckUpdate(lpObj);
	}

	if (gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass / 16) * 16);
		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp * 16);
		Class -= (Class / 32);
		Class += (lpObj->Class * 32);

		gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);

		memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if (gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}

		for (int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj, 0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if (gServerInfo.m_CommandResetType == 1)
	{
		//lpObj->Money -= gResetTable.GetResetMoney(lpObj);

		//GCMoneySend(lpObj->Index,lpObj->Money);

		int point = 0;

		if (lpObj->ResetVip == 1)
		{
			point = gResetTableVip.GetResetPoint(lpObj);
		}
		else
		{
			point = gResetTable.GetResetPoint(lpObj);
		}

		point = (point * gServerInfo.m_CommandResetPointRate[lpObj->Class]) / 100;

		point += (lpObj->Level - 1) * gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0) ? 0 : ((lpObj->Level > 220) ? ((lpObj->Level - 220) * gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		if (gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel] != 0)
		{
			lpObj->Leadership = lpObj->Leadership + gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel];
		}
		else
		{
			lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
		}
	}


	if (gServerInfo.m_CommandResetType == 2)
	{
		int point = 0;

		if (lpObj->ResetVip == 1)
		{
			point = gResetTableVip.GetResetPointkeep(lpObj);
		}
		else
		{
			point = gResetTable.GetResetPointKeep(lpObj);
		}

		lpObj->LevelUpPoint += point;
		if (gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel] != 0)
		{
			lpObj->Leadership = lpObj->Leadership + gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel];
		}
	}

	if (gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel] * lpObj->MasterReset;

		point = (point * gServerInfo.m_CommandMasterResetPointRate[lpObj->Class]) / 100;

		lpObj->LevelUpPoint += point;
	}


	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);

	gNotice.GCNoticeSendToAll(eMessageRs, 0, 0, 20, 0, 0, gMessageNew.GetMessage(43), lpObj->Name, IsToolKit.QN(lpObj->Reset));

	if (gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index, 27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpObj->Index, 267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpObj->Index, 17);
			break;
		}
	}

	gResetTable.SendResetInfo(lpObj, lpObj->Reset, lpObj->AccountLevel);

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);
	GCEffectInfoSend(lpObj->Index, 16);

	gLog.Output(LOG_COMMAND, "[CommandReset][%s][%s] - (Reset: %d)", lpObj->Account, lpObj->Name, lpObj->Reset);
}

void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCommandMasterResetRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if (lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(212), gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if (lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(213), gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if (lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(214), gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index, lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	if (gServerInfo.m_CommandMasterResetRemoveReset[lpObj->AccountLevel] > 0)
	{
		lpObj->Reset = (lpObj->Reset - gServerInfo.m_CommandMasterResetRemoveReset[lpObj->AccountLevel]);
	}
	else
	{
		lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel] == -1) ? (lpObj->Reset - gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);
	}

	lpObj->MasterReset += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->MasterResetDay += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->MasterResetWek += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->MasterResetMon += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	if (gServerInfo.m_CustomRankUserType == 1)
	{
		gCustomRankUser.CheckUpdate(lpObj);
	}

	//gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);

	if (gServerInfo.m_CommandMasterResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass / 16) * 16);
		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp * 16);
		Class -= (Class / 32);
		Class += (lpObj->Class * 32);

		gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);

		memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if (gServerInfo.m_CommandMasterResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}

		for (int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj, 0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if (gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel] * lpObj->MasterReset;

		point = (point * gServerInfo.m_CommandMasterResetPointRate[lpObj->Class]) / 100;

		point += (lpObj->Level - 1) * gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0) ? 0 : ((lpObj->Level > 220) ? ((lpObj->Level - 220) * gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if (gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point * gServerInfo.m_CommandResetPointRate[lpObj->Class]) / 100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDMasterResetInfoSaveSend(lpObj->Index, lpMsg->MasterResetDay, lpMsg->MasterResetWek, lpMsg->MasterResetMon);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(179), lpObj->MasterReset);

	if (gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index, 27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpObj->Index, 267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpObj->Index, 17);
			break;
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMasterReset][%s][%s] - (MasterReset: %d)", lpObj->Account, lpObj->Name, lpObj->MasterReset);
}

bool CCommandManager::CommandClearInventory(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
	if (lpObj->Lock > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 2, 0, gMessageNew.GetMessage(678));
		return 0;
	}

	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		gItemManager.InventoryDelItem(lpObj->Index, i);
		gItemManager.GCItemDeleteSend(lpObj->Index, i, 1);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(313));

	gLog.Output(LOG_COMMAND, "[Command Clear Iventory] Use for:[%s][%s]", lpObj->Account, lpObj->Name);

	return 1;
}

bool CCommandManager::CommandMarry(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
	char mode[11] = { 0 };
	this->GetString(arg, mode, sizeof(mode), 0);


	if (mode[0] == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(336));
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(363), gServerInfo.m_CommandMarryLevel, gServerInfo.m_CommandMarryCost);
		return 0;
	}

	if (strcmp(mode, "to") == 0 && gServerInfo.m_CommandMarryOnlyGM == 0)
	{
		char MarryName[11] = { 0 };
		this->GetString(arg, MarryName, sizeof(MarryName), 1);
		LPOBJ lpTarget = gObjFind(MarryName);
		if (lpTarget == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(367), MarryName);
			return 0;
		}

		if (strcmp(MarryName, lpObj->Name) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(318));
			return 0;
		}

		SDHP_COMMAND_MARRY_SEND pMsg;


		pMsg.header.set(0x0F, 0x02, sizeof(pMsg));


		pMsg.index = lpObj->Index;


		memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));


		memcpy(pMsg.marryname, MarryName, sizeof(pMsg.marryname));


		memcpy(pMsg.mode, "marry", sizeof(pMsg.mode));


		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else if (strcmp(mode, "to") == 0 && gServerInfo.m_CommandMarryOnlyGM == 1)
	{

		if (gGameMaster.CheckGameMasterLevel(lpObj, 1) == 0)
		{
			return 0;
		}

		char MarryName1[11] = { 0 };
		char MarryName2[11] = { 0 };
		this->GetString(arg, MarryName1, sizeof(MarryName1), 1);
		this->GetString(arg, MarryName2, sizeof(MarryName2), 2);

		LPOBJ lpTarget1 = gObjFind(MarryName1);
		LPOBJ lpTarget2 = gObjFind(MarryName2);

		if (lpTarget1 == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(367), MarryName1);
			return 0;
		}

		if (lpTarget2 == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(367), MarryName2);
			return 0;
		}

		if (strcmp(MarryName1, MarryName2) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(318));
			return 0;
		}
		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02, sizeof(pMsg));

		pMsg.index = lpTarget1->Index;

		memcpy(pMsg.name, MarryName1, sizeof(pMsg.name));

		memcpy(pMsg.marryname, MarryName2, sizeof(pMsg.marryname));

		memcpy(pMsg.mode, "marry", sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else if (strcmp(mode, "divorce") == 0)
	{
		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02, sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

		memcpy(pMsg.marryname, lpObj->Name, sizeof(pMsg.marryname));

		memcpy(pMsg.mode, "divorce", sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else if (strcmp(mode, "track") == 0)
	{
		if (lpObj->Money < (DWORD)gServerInfo.m_CommandMarryCost)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(360), gServerInfo.m_CommandMarryCost);
			return 0;
		}

		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02, sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

		memcpy(pMsg.marryname, lpObj->Name, sizeof(pMsg.marryname));

		memcpy(pMsg.mode, "track", sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else if (strcmp(mode, "trace") == 0)
	{
		if (lpObj->Money < (DWORD)gServerInfo.m_CommandMarryCost)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(360), gServerInfo.m_CommandMarryCost);
			return 0;
		}

		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02, sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

		memcpy(pMsg.marryname, lpObj->Name, sizeof(pMsg.marryname));

		memcpy(pMsg.mode, "trace", sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(336));
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(363), gServerInfo.m_CommandMarryLevel, gServerInfo.m_CommandMarryCost);
		return 0;
	}
	return 0;
}

void CCommandManager::DGCommandMarryRecv(SDHP_COMMAND_MARRY_RECV* lpMsg)
{
	LPOBJ lpObj = &gObj[lpMsg->index];
	LPOBJ lpTarget = gObjFind(lpMsg->marryname);

	if (strcmp(lpMsg->mode, "marry") == 0)
	{
		if (lpMsg->countyou > 0) {
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(334));
			return;
		}


		if (lpMsg->counthim > 0) {
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(335));
			return;
		}

		int minx = 12;
		int maxx = 15;
		int miny = 23;
		int maxy = 28;
		if (lpObj->Map == 2 && lpObj->X >= minx && lpObj->X <= maxx && lpObj->Y >= miny && lpObj->Y <= maxy)
		{
			//good to go for you
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(361));
			return;
		}
		if (lpTarget->Map == 2 && lpTarget->X >= minx && lpTarget->X <= maxx && lpTarget->Y >= miny && lpTarget->Y <= maxy)
		{
			//good to go for the other person
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(362));
			return;
		}
		//
		int first_classtype = 0;
		if (
			(lpObj->DBClass == DB_CLASS_DW) || (lpObj->DBClass == DB_CLASS_SM) || (lpObj->DBClass == DB_CLASS_GM) || (lpObj->DBClass == DB_CLASS_DK) ||
			(lpObj->DBClass == DB_CLASS_BK) || (lpObj->DBClass == DB_CLASS_BM) || (lpObj->DBClass == DB_CLASS_MG) || (lpObj->DBClass == DB_CLASS_DM) ||
			(lpObj->DBClass == DB_CLASS_DL) || (lpObj->DBClass == DB_CLASS_LE) || (lpObj->DBClass == DB_CLASS_RF) || (lpObj->DBClass == DB_CLASS_FM)) {
			first_classtype = 0;
		}
		else if (
			(lpObj->DBClass == DB_CLASS_FE) || (lpObj->DBClass == DB_CLASS_ME) || (lpObj->DBClass == DB_CLASS_HE) || (lpObj->DBClass == DB_CLASS_SU) ||
			(lpObj->DBClass == DB_CLASS_BS) || (lpObj->DBClass == DB_CLASS_DS)) {
			first_classtype = 1;
		}
		//
		int second_classtype = 0;
		if (
			(lpTarget->DBClass == DB_CLASS_DW) || (lpTarget->DBClass == DB_CLASS_SM) || (lpTarget->DBClass == DB_CLASS_GM) || (lpTarget->DBClass == DB_CLASS_DK) ||
			(lpTarget->DBClass == DB_CLASS_BK) || (lpTarget->DBClass == DB_CLASS_BM) || (lpTarget->DBClass == DB_CLASS_MG) || (lpTarget->DBClass == DB_CLASS_DM) ||
			(lpTarget->DBClass == DB_CLASS_DL) || (lpTarget->DBClass == DB_CLASS_LE) || (lpTarget->DBClass == DB_CLASS_RF) || (lpTarget->DBClass == DB_CLASS_FM)) {
			second_classtype = 0;
		}
		else if (
			(lpTarget->DBClass == DB_CLASS_FE) || (lpTarget->DBClass == DB_CLASS_ME) || (lpTarget->DBClass == DB_CLASS_HE) || (lpTarget->DBClass == DB_CLASS_SU) ||
			(lpTarget->DBClass == DB_CLASS_BS) || (lpTarget->DBClass == DB_CLASS_DS)) {
			second_classtype = 1;
		}
		char fulltext_new[256];
		wsprintf(fulltext_new, gMessageNew.GetMessage(345), lpMsg->name, lpMsg->marryname);
		if ((first_classtype == 0) && (second_classtype == 0)) {
			if ((GetTickCount() - lpObj->MarryTimeMsj) > (DWORD)(gServerInfo.m_CommandMarryNoticeDelay * 1000))
			{
				GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(346));//nohomo allowed
				GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, fulltext_new);//nohomo allowed
			}
			lpObj->MarryTimeMsj = GetTickCount();
			return;
		}
		if ((first_classtype == 1) && (second_classtype == 1)) {
			if ((GetTickCount() - lpObj->MarryTimeMsj) > (DWORD)(gServerInfo.m_CommandMarryNoticeDelay * 1000))
			{
				GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(346));//nohomo allowed
				GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, fulltext_new);//nohomo allowed
			}
			lpObj->MarryTimeMsj = GetTickCount();
			return;
		}
		//
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(344));
		GDMarryInfoSaveSend(lpObj->Index, lpMsg->marryname, lpMsg->mode);
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(312), lpMsg->marryname);
		char fulltext[256];
		wsprintf(fulltext, gMessageNew.GetMessage(342), lpObj->Name, lpMsg->marryname);
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, fulltext);
		for (int n = 0; n < 15; n++)
		{
			GCFireworksSend(lpObj, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)));
		}

		this->RemoveRequisites(lpObj, 30);
		//
	}
	else if (strcmp(lpMsg->mode, "divorcespk") == 0)
	{
		if (lpMsg->countyou == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(339));
			return;
		}
		time_t timer;
		struct tm y2k = { 0 };
		UINT seconds;
		y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 116; y2k.tm_mon = 0; y2k.tm_mday = 1;
		time(&timer);  /* get current time; same as: timer = time(NULL)  */
		seconds = static_cast<UINT>(difftime(timer, mktime(&y2k)));
		UINT value = seconds - lpMsg->marriedon;
		if (value < (UINT)gServerInfo.m_CommandMarryMinTime)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(337));
			return;
		}
		LPOBJ lpTarget1 = gObjFind(lpMsg->NameGet1);
		LPOBJ lpTarget2 = gObjFind(lpMsg->NameGet2);
		if (lpTarget1 == 0 && lpTarget2 == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(364));
			return;
		}
		char fulltext[256];
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(344));
		if (lpTarget1 == 0)
		{
			wsprintf(fulltext, gMessageNew.GetMessage(343), lpObj->Name, lpMsg->NameGet2);
			GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, fulltext);
		}
		if (lpTarget2 == 0)
		{
			wsprintf(fulltext, gMessageNew.GetMessage(343), lpObj->Name, lpMsg->NameGet1);
			GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, fulltext);
		}
		//
		GDMarryInfoSaveSend(lpObj->Index, lpMsg->marryname, lpMsg->mode);
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(338));
		//
	}
	else if (strcmp(lpMsg->mode, "track") == 0)
	{
		if (lpMsg->countyou == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(339));
			return;
		}
		LPOBJ lpTarget1 = gObjFind(lpMsg->NameGet1);
		LPOBJ lpTarget2 = gObjFind(lpMsg->NameGet2);

		if (lpTarget1 == 0 && lpTarget2 == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(365));
			return;
		}
		if (lpObj->Level >= gServerInfo.m_CommandMarryLevel)
		{
			if (lpTarget1 == 0)
			{
				if ((lpTarget2->Map == MAP_ICARUS || lpTarget2->Map == MAP_KANTURU3) && (lpObj->Inventory[7].IsItem() == 0 && lpObj->Inventory[8].m_Index != GET_ITEM(13, 3) && lpObj->Inventory[8].m_Index != GET_ITEM(13, 37))) // Dinorant,Fenrir
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
					return;
				}

				if (lpTarget2->Map == MAP_BLOOD_CASTLE1
					|| lpTarget2->Map == MAP_BLOOD_CASTLE2
					|| lpTarget2->Map == MAP_BLOOD_CASTLE3
					|| lpTarget2->Map == MAP_BLOOD_CASTLE4
					|| lpTarget2->Map == MAP_BLOOD_CASTLE5
					|| lpTarget2->Map == MAP_BLOOD_CASTLE6
					|| lpTarget2->Map == MAP_BLOOD_CASTLE7
					|| lpTarget2->Map == MAP_CHAOS_CASTLE1
					|| lpTarget2->Map == MAP_CHAOS_CASTLE2
					|| lpTarget2->Map == MAP_CHAOS_CASTLE3
					|| lpTarget2->Map == MAP_CHAOS_CASTLE4
					|| lpTarget2->Map == MAP_CHAOS_CASTLE5
					|| lpTarget2->Map == MAP_CHAOS_CASTLE6
					|| lpTarget2->Map == MAP_KALIMA1
					|| lpTarget2->Map == MAP_KALIMA2
					|| lpTarget2->Map == MAP_KALIMA3
					|| lpTarget2->Map == MAP_KALIMA4
					|| lpTarget2->Map == MAP_KALIMA5
					|| lpTarget2->Map == MAP_KALIMA6
					|| lpTarget2->Map == MAP_KALIMA7
					|| lpTarget2->Map == MAP_DEVIL_SQUARE1
					|| lpTarget2->Map == MAP_DEVIL_SQUARE2) // en evento
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
					return;
				}
				gObjTeleport(lpObj->Index, lpTarget2->Map, lpTarget2->X, lpTarget2->Y);
				gObjTeleport(lpTarget2->Index, lpTarget2->Map, lpTarget2->X, lpTarget2->Y);
				gObjViewportListProtocolDestroy(lpObj);
				gObjClearViewport(lpObj);
			}
			if (lpTarget2 == 0)
			{
				if ((lpTarget1->Map == MAP_ICARUS || lpTarget1->Map == MAP_KANTURU3) && (lpObj->Inventory[7].IsItem() == 0 && lpObj->Inventory[8].m_Index != GET_ITEM(13, 3) && lpObj->Inventory[8].m_Index != GET_ITEM(13, 37))) // Dinorant,Fenrir
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
					return;
				}

				if (lpTarget1->Map == MAP_BLOOD_CASTLE1
					|| lpTarget1->Map == MAP_BLOOD_CASTLE2
					|| lpTarget1->Map == MAP_BLOOD_CASTLE3
					|| lpTarget1->Map == MAP_BLOOD_CASTLE4
					|| lpTarget1->Map == MAP_BLOOD_CASTLE5
					|| lpTarget1->Map == MAP_BLOOD_CASTLE6
					|| lpTarget1->Map == MAP_BLOOD_CASTLE7
					|| lpTarget1->Map == MAP_CHAOS_CASTLE1
					|| lpTarget1->Map == MAP_CHAOS_CASTLE2
					|| lpTarget1->Map == MAP_CHAOS_CASTLE3
					|| lpTarget1->Map == MAP_CHAOS_CASTLE4
					|| lpTarget1->Map == MAP_CHAOS_CASTLE5
					|| lpTarget1->Map == MAP_CHAOS_CASTLE6
					|| lpTarget1->Map == MAP_KALIMA1
					|| lpTarget1->Map == MAP_KALIMA2
					|| lpTarget1->Map == MAP_KALIMA3
					|| lpTarget1->Map == MAP_KALIMA4
					|| lpTarget1->Map == MAP_KALIMA5
					|| lpTarget1->Map == MAP_KALIMA6
					|| lpTarget1->Map == MAP_KALIMA7
					|| lpTarget1->Map == MAP_DEVIL_SQUARE1
					|| lpTarget1->Map == MAP_DEVIL_SQUARE2) // en evento
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
					return;
				}
				gObjTeleport(lpObj->Index, lpTarget1->Map, lpTarget1->X, lpTarget1->Y);
				gObjTeleport(lpTarget1->Index, lpTarget1->Map, lpTarget1->X, lpTarget1->Y);

				gObjViewportListProtocolDestroy(lpObj);
				gObjClearViewport(lpObj);
			}
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(347), gServerInfo.m_CommandMarryLevel);
			return;
		}
		//
		GDMarryInfoSaveSend(lpObj->Index, lpMsg->marryname, lpMsg->mode);
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(340), lpMsg->marryname);
		lpObj->Money -= gServerInfo.m_CommandMarryCost;
		GCMoneySend(lpObj->Index, lpObj->Money);
		//
	}
	else if (strcmp(lpMsg->mode, "trace") == 0)
	{
		if (lpMsg->countyou == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(339));
			return;
		}
		LPOBJ lpTarget1 = gObjFind(lpMsg->NameGet1);
		LPOBJ lpTarget2 = gObjFind(lpMsg->NameGet2);
		//
		if (lpTarget1 == 0 && lpTarget2 == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(365));
			return;
		}

		if (lpObj->Map == MAP_BLOOD_CASTLE1
			|| lpObj->Map == MAP_BLOOD_CASTLE2
			|| lpObj->Map == MAP_BLOOD_CASTLE3
			|| lpObj->Map == MAP_BLOOD_CASTLE4
			|| lpObj->Map == MAP_BLOOD_CASTLE5
			|| lpObj->Map == MAP_BLOOD_CASTLE6
			|| lpObj->Map == MAP_BLOOD_CASTLE7
			|| lpObj->Map == MAP_CHAOS_CASTLE1
			|| lpObj->Map == MAP_CHAOS_CASTLE2
			|| lpObj->Map == MAP_CHAOS_CASTLE3
			|| lpObj->Map == MAP_CHAOS_CASTLE4
			|| lpObj->Map == MAP_CHAOS_CASTLE5
			|| lpObj->Map == MAP_CHAOS_CASTLE6
			|| lpObj->Map == MAP_KALIMA1
			|| lpObj->Map == MAP_KALIMA2
			|| lpObj->Map == MAP_KALIMA3
			|| lpObj->Map == MAP_KALIMA4
			|| lpObj->Map == MAP_KALIMA5
			|| lpObj->Map == MAP_KALIMA6
			|| lpObj->Map == MAP_KALIMA7
			|| lpObj->Map == MAP_DEVIL_SQUARE1
			|| lpObj->Map == MAP_DEVIL_SQUARE2)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
			return;
		}

		if (lpTarget1 == 0)
		{
			if ((lpObj->Map == MAP_ICARUS || lpObj->Map == MAP_KANTURU3) && (lpTarget2->Inventory[7].IsItem() == 0 && lpTarget2->Inventory[8].m_Index != GET_ITEM(13, 3) && lpTarget2->Inventory[8].m_Index != GET_ITEM(13, 37))) // Dinorant,Fenrir
			{
				gNotice.GCNoticeSend(lpTarget2->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
				return;
			}
			if (lpTarget2->Level >= gServerInfo.m_CommandMarryLevel)
			{
				gObjTeleport(lpTarget2->Index, lpObj->Map, lpObj->X, lpObj->Y);
				gObjTeleport(lpObj->Index, lpObj->Map, lpObj->X, lpObj->Y);
			}
			else
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(348), gServerInfo.m_CommandMarryLevel);
				return;
			}
		}
		if (lpTarget2 == 0)
		{
			if ((lpObj->Map == MAP_ICARUS || lpObj->Map == MAP_KANTURU3) && (lpTarget1->Inventory[7].IsItem() == 0 && lpTarget1->Inventory[8].m_Index != GET_ITEM(13, 3) && lpTarget1->Inventory[8].m_Index != GET_ITEM(13, 37))) // Dinorant,Fenrir
			{
				gNotice.GCNoticeSend(lpTarget1->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(251));
				return;
			}
			if (lpTarget1->Level >= gServerInfo.m_CommandMarryLevel)
			{
				gObjTeleport(lpTarget1->Index, lpObj->Map, lpObj->X, lpObj->Y);
				//gObjViewportListProtocolDestroy(lpObj);
				gObjClearViewport(lpObj);
			}
			else
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(348), gServerInfo.m_CommandMarryLevel);
				return;
			}
		}
		//
		GDMarryInfoSaveSend(lpObj->Index, lpMsg->marryname, lpMsg->mode);
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(341), lpMsg->marryname);
		lpObj->Money -= gServerInfo.m_CommandMarryCost;
		GCMoneySend(lpObj->Index, lpObj->Money);
		//
	}
	gLog.Output(LOG_COMMAND, "[CommandMarry][%s][%d][%d][%d][%s][%s][%s]", lpObj->Account, lpMsg->countyou, lpMsg->counthim, lpMsg->marriedon, lpMsg->marryname, lpMsg->NameGet1, lpMsg->NameGet2);
}

bool CCommandManager::CommandOpenWarehouse(LPOBJ lpObj)
{

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.type == INTERFACE_PERSONAL_SHOP)
	{
		return 0;
	}
	if (gItemManager.ChaosBoxHasItem(lpObj) || gItemManager.TradeHasItem(lpObj))
	{
		return 0;
	}

	if (gServerInfo.m_CommandOpenWareOnlySafeZone == 1 && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(351));
		return 0;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index, lpObj->Account);

	return 1;
}

bool CCommandManager::CommandChangeClass(LPOBJ lpObj, char* arg, int Npc)
{
	if (
		lpObj->Interface.type == INTERFACE_CHAOS_BOX ||
		lpObj->Interface.type == INTERFACE_TRADE ||
		lpObj->Interface.type == INTERFACE_PARTY ||
		lpObj->Interface.type == INTERFACE_WAREHOUSE ||
		lpObj->Interface.type == INTERFACE_PERSONAL_SHOP ||
		lpObj->Interface.type == INTERFACE_CASH_SHOP ||
		lpObj->Interface.type == INTERFACE_TRAINER ||
		lpObj->Interface.use != 0 ||
		lpObj->State == 32 || lpObj->DieRegen != 0 ||
		lpObj->Teleport != 0 || lpObj->PShopOpen != 0 ||
		lpObj->ChaosLock != 0 || lpObj->SkillSummonPartyTime != 0
		)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return false;
	}

	char classe[4] = { 0 };
	this->GetString(arg, classe, sizeof(classe), 0);

	if (classe[0] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(349));
		if (Npc >= 0) GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(349));
		return false;
	}

	struct ClassInfo
	{
		const char* code;
		int current;
		int allow;
		int newClass;
		int dbClass;
	}

	classList[] =
	{
		{"dw", CLASS_DW, gServerInfo.m_CommandChangeClassToDW, CLASS_DW, DB_CLASS_DW},
		{"dk", CLASS_DK, gServerInfo.m_CommandChangeClassToDK, CLASS_DK, DB_CLASS_DK},
		{"elf", CLASS_ELF, gServerInfo.m_CommandChangeClassToELF, CLASS_ELF, DB_CLASS_FE},
		{"mg", CLASS_MG, gServerInfo.m_CommandChangeClassToMG, CLASS_MG, DB_CLASS_MG},
		{"dl", CLASS_DL, gServerInfo.m_CommandChangeClassToDL, CLASS_DL, DB_CLASS_DL},
		{"su", CLASS_SU, gServerInfo.m_CommandChangeClassToSU, CLASS_SU, DB_CLASS_SU},
		{"rf", CLASS_RF, gServerInfo.m_CommandChangeClassToRF, CLASS_RF, DB_CLASS_RF}
	};

	int newClass = -1, newDBClass = -1;
	for (auto& cls : classList)
	{
		if (strcmp(classe, cls.code) == 0)
		{
			if (lpObj->Class == cls.current)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(350));
				if (Npc >= 0) GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(350));
				return false;
			}
			if (cls.allow == 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(366));
				if (Npc >= 0) GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(366));
				return false;
			}
			newClass = cls.newClass;
			newDBClass = cls.dbClass;
			break;
		}
	}

	if (newClass < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(349));
		if (Npc >= 0) GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(349));
		return false;
	}

	for (int i = 0; i < 11; i++)
	{
		if (lpObj->Inventory[i].IsItem() != 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(305));
			if (Npc >= 0) GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(305));
			return false;
		}
	}

	lpObj->Class = newClass;
	lpObj->DBClass = newDBClass;
	lpObj->ChangeUp = newDBClass % 16;

	gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

	BYTE displayClass = (lpObj->ChangeUp * 16) - (lpObj->ChangeUp * 16 / 32) + (lpObj->Class * 32);
	gQuest.GCQuestRewardSend(lpObj->Index, 201, displayClass);

	memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));
	gQuest.GCQuestInfoSend(lpObj->Index);

	if (gServerInfo.m_ClearMasterChangeClass)
	{
		lpObj->MasterLevel = 0;
		lpObj->MasterPoint = 0;
		lpObj->MasterExperience = 0;
	}
	else
	{
		lpObj->MasterPoint = lpObj->MasterLevel;
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++) lpObj->Skill[n].Clear();
	for (int n = 0; n < MAX_MASTER_SKILL_LIST; n++) lpObj->MasterSkill[n].Clear();

	gEffectManager.ClearAllEffect(lpObj);

	lpObj->CloseCount = 1;
	lpObj->CloseType = 1;

	gLog.Output(LOG_COMMAND, "[CommandChangeClass][%s][%s] - (ClassNum: %d)", lpObj->Account, lpObj->Name, newDBClass);

	return true;
}


bool CCommandManager::CommandReward(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	if (name[0] == NULL) {
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(405));
		return 0;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(404));
		return 0;
	}

	if (strcmp(lpTarget->Name, lpObj->Name) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(409));
		return 0;
	}

	int type = this->GetNumber(arg, 1);
	int value = this->GetNumber(arg, 2);

	if (type <= 0 || value <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(405));
		return 0;
	}

	if (type > gServerInfo.m_CommandRewardMaxType)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(406));
		return 0;
	}

	if (value > gServerInfo.m_CommandRewardMaxValue)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(407));
		return 0;
	}
	SDHP_COMMAND_REWARD_SEND pMsg;

	pMsg.header.set(0x0F, 0x03, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.name, lpTarget->Name, sizeof(pMsg.name));
	memcpy(pMsg.account, lpTarget->Account, sizeof(pMsg.account));

	memcpy(pMsg.nameGM, lpObj->Name, sizeof(pMsg.nameGM));
	memcpy(pMsg.accountGM, lpObj->Account, sizeof(pMsg.accountGM));

	pMsg.Type = type;
	pMsg.Value = value;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	if (type == 1) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType1Text, lpObj->Name, lpTarget->Name, value);
	if (type == 2) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType2Text, lpObj->Name, lpTarget->Name, value);
	if (type == 3) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType3Text, lpObj->Name, lpTarget->Name, value);
	if (type == 4) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType4Text, lpObj->Name, lpTarget->Name, value);
	if (type == 5) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType5Text, lpObj->Name, lpTarget->Name, value);
	if (type == 6) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType6Text, lpObj->Name, lpTarget->Name, value);
	if (type == 7) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType7Text, lpObj->Name, lpTarget->Name, value);
	if (type == 8) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType8Text, lpObj->Name, lpTarget->Name, value);
	if (type == 9) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType9Text, lpObj->Name, lpTarget->Name, value);
	if (type == 10) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardType10Text, lpObj->Name, lpTarget->Name, value);

	gCashShop.CGCashShopPointRecv(lpTarget->Index);
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(408));
	gLog.Output(LOG_COMMAND, "[CommandReward][%s][%s] - (Player: %s, Type: %d, Value: %d)", lpObj->Account, lpObj->Name, lpTarget->Name, type, value);

	return 1;
}

bool CCommandManager::CommandRewardAll(LPOBJ lpObj, char* arg)
{
	int type = this->GetNumber(arg, 0);
	int value = this->GetNumber(arg, 1);

	if (type <= 0 || value <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(412));
		return 0;
	}

	if (type > gServerInfo.m_CommandRewardAllMaxType)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(406));
		return 0;
	}

	if (value > gServerInfo.m_CommandRewardAllMaxValue)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(407));
		return 0;
	}

	SDHP_COMMAND_REWARDALL_SEND pMsg;

	pMsg.header.set(0x0F, 0x04, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.nameGM, lpObj->Name, sizeof(pMsg.nameGM));
	memcpy(pMsg.accountGM, lpObj->Account, sizeof(pMsg.accountGM));

	pMsg.Type = type;
	pMsg.Value = value;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	if (type == 1) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType1Text, lpObj->Name, value);
	if (type == 2) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType2Text, lpObj->Name, value);
	if (type == 3) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType3Text, lpObj->Name, value);
	if (type == 4) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType4Text, lpObj->Name, value);
	if (type == 5) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType5Text, lpObj->Name, value);
	if (type == 6) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType6Text, lpObj->Name, value);
	if (type == 7) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType7Text, lpObj->Name, value);
	if (type == 8) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType8Text, lpObj->Name, value);
	if (type == 9) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType9Text, lpObj->Name, value);
	if (type == 10) gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gServerInfo.m_CommandRewardAllType10Text, lpObj->Name, value);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(408));
	gLog.Output(LOG_COMMAND, "[CommandRewardAll][%s][%s] - (Type: %d, Value: %d)", lpObj->Account, lpObj->Name, type, value);

	return 1;
}

bool CCommandManager::CommandReload(LPOBJ lpObj, char* arg)
{

	char name[20] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	if (strcmp(name, "cashshop") == 0)
	{
		gServerInfo.ReadCashShopInfo();
	}
	else if (strcmp(name, "chaosmix") == 0)
	{
		gServerInfo.ReadChaosMixInfo();
	}
	else if (strcmp(name, "character") == 0)
	{
		gServerInfo.ReadCharacterInfo();
	}
	else if (strcmp(name, "command") == 0)
	{
		gServerInfo.ReadCommandInfo();
	}
	else if (strcmp(name, "common") == 0)
	{
		gServerInfo.ReadCommonInfo();
	}
	else if (strcmp(name, "custom") == 0)
	{
		gServerInfo.ReadCustomInfo();
	}
	else if (strcmp(name, "event") == 0)
	{
		gServerInfo.ReadEventInfo();
	}
	else if (strcmp(name, "eventitembag") == 0)
	{
		gServerInfo.ReadEventItemBagInfo();
	}
	else if (strcmp(name, "hack") == 0)
	{
		gServerInfo.ReadHackInfo();
	}
	else if (strcmp(name, "item") == 0)
	{
		gServerInfo.ReadItemInfo();
	}
	else if (strcmp(name, "monster") == 0)
	{
		gServerInfo.ReloadMonsterInfo();
	}
	else if (strcmp(name, "move") == 0)
	{
		gServerInfo.ReadMoveInfo();
	}
	else if (strcmp(name, "quest") == 0)
	{
		gServerInfo.ReadQuestInfo();
	}
	else if (strcmp(name, "shop") == 0)
	{
		gServerInfo.ReadShopInfo();
	}
	else if (strcmp(name, "skill") == 0)
	{
		gServerInfo.ReadSkillInfo();
	}
	else if (strcmp(name, "util") == 0)
	{
		gServerInfo.ReadUtilInfo();
	}
	else if (strcmp(name, "bots") == 0)
	{
		gServerInfo.ReloadBotInfo();
	}
	else if (strcmp(name, "ACuoi") == 0)
	{
		gServerInfo.ReloadCfgSPK();
	}
	else if (strcmp(name, "all") == 0)
	{
		gServerInfo.ReloadAll();
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(410));
		return 0;
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(411));
	return 1;
}

bool IsAllowedCharacter(char c)
{
	return (std::isalpha(c) != 0) || (std::isdigit(c) != 0);
}
bool CCommandManager::CommandRename(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 2)
	if (lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(444));
		return 0;
	}
	if (lpObj->GuildNumber > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(431));
		return 0;
	}
	if (gServerInfo.m_CommandRenameTicket == 1 && lpObj->RenameEnable == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(414));
		return 0;
	}
	DWORD tick = (GetTickCount() - lpObj->RenameTime) / 1000;
	if (tick < ((DWORD)gServerInfo.m_CommandRenameDelay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(415), (gServerInfo.m_CommandRenameDelay[lpObj->AccountLevel] - tick));
		return 0;
	}
	if (strlen(arg) > 10 || strlen(arg) < 3)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(413));
		return 0;
	}
	for (size_t i = 0; i < strlen(arg); ++i)
	{
		if (!IsAllowedCharacter(arg[i]))
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(617));
			return false;
		}
	}
	char RenameName[11] = { 0 };
	this->GetString(arg, RenameName, sizeof(RenameName), 0);

	if (RenameName[0] == NULL)
	{
		return 0;
	}
	if (strcmp(RenameName, lpObj->Name) == 0)
	{
		return 0;
	}
	lpObj->RenameActive = 1;
	SDHP_COMMAND_RENAME_SEND pMsg;
	pMsg.header.set(0x0F, 0x05, sizeof(pMsg));
	pMsg.index = lpObj->Index;
	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));
	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));
	memcpy(pMsg.newname, RenameName, sizeof(pMsg.newname));
	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	gLog.Output(LOG_COMMAND, "[CommandRename][%s][%s] - (NewName: %s)", lpObj->Account, lpObj->Name, RenameName);
#endif
	return 0;
}

void CCommandManager::DGCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 2)
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCommandRenameRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpMsg->result == 0)
	{

		this->RemoveRequisites(lpObj, 38);

		lpObj->RenameTime = GetTickCount();

		memcpy(lpObj->Name, lpMsg->newname, sizeof(lpObj->Name));

		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(430), lpMsg->name, lpMsg->newname);

		lpObj->RenameActive = 0;
		lpObj->CloseCount = 1;
		lpObj->CloseType = 1;

		LogAdd(LOG_GREEN, "[CommandRename][%s][%s] - Sucess (OldName: %s)", lpObj->Account, lpObj->Name, lpMsg->name);
		return;
	}

	if (lpMsg->result == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(428));
		LogAdd(LOG_BLACK, "[CommandRename][%s][%s] - Name exists (NewName: %s)", lpObj->Account, lpObj->Name, lpMsg->newname);
		return;
	}

	if (lpMsg->result == 2)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(429));
		LogAdd(LOG_RED, "[CommandRename][%s][%s] - Error (NewName: %s)", lpObj->Account, lpObj->Name, lpMsg->newname);
		return;
	}

#endif


}
bool CCommandManager::CommandInfo(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX
		|| lpObj->Interface.type == INTERFACE_TRADE
		|| lpObj->Interface.type == INTERFACE_PARTY
		|| lpObj->Interface.type == INTERFACE_WAREHOUSE
		|| lpObj->Interface.type == INTERFACE_PERSONAL_SHOP
		|| lpObj->Interface.type == INTERFACE_CASH_SHOP
		|| lpObj->Interface.type == INTERFACE_TRAINER
		|| lpObj->Interface.use != 0
		|| lpObj->State == 32
		|| lpObj->DieRegen != 0
		|| lpObj->Teleport != 0
		|| lpObj->PShopOpen != 0
		|| lpObj->ChaosLock != 0
		|| lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(46));
		return 0;
	}
#if (GAMESERVER_CLIENTE_UPDATE >= 2) 

	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(175));
		return 0;
	}

	if (gGameMaster.CheckGameMasterLevel(lpObj, 1) == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Acc: %s", lpTarget->Account);
	}

	GJAccountLevelSend(lpObj->Index, lpTarget->Account);
	gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Level: %d", lpTarget->Level);
	gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Resets: %d", lpTarget->Reset);
	gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "MResets: %d", lpTarget->MasterReset);
#endif
	return 1;
}
bool CCommandManager::CommandBlocChar(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int days = this->GetNumber(arg, 1);

	if (days <= 0)
	{
		return 0;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget > 0)
	{
		gObjUserKill(lpTarget->Index);
	}

	SDHP_COMMAND_BLOC_SEND pMsg;

	pMsg.header.set(0x0F, 0x07, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	pMsg.days = days;

	memcpy(pMsg.namebloc, name, sizeof(pMsg.namebloc));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	if (gServerInfo.m_CommandBlocCharShowMessage == 1)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(641), lpObj->Name, name, days);
	}

	gLog.Output(LOG_COMMAND, "[CommandBlocChar][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	return 1;
}

bool CCommandManager::CommandBlocCharRecv(SDHP_COMMAND_BLOC_RECV* lpMsg)
{

	if (lpMsg->result != 1)
	{
		gNotice.GCNoticeSend(lpMsg->index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(446));
	}
	else
	{
		gNotice.GCNoticeSend(lpMsg->index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(454));
	}

	return 1;
}

bool CCommandManager::CommandBlocAcc(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int days = this->GetNumber(arg, 1);

	if (days <= 0)
	{
		return 0;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget > 0)
	{
		gObjUserKill(lpTarget->Index);
	}

	SDHP_COMMAND_BLOC_SEND pMsg{};

	pMsg.header.set(0x0F, 0x06, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	pMsg.days = days;

	memcpy(pMsg.namebloc, name, sizeof(pMsg.namebloc));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	if (gServerInfo.m_CommandBlocAccShowMessage == 1)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(642), lpObj->Name, name, days);
	}

	gLog.Output(LOG_COMMAND, "[CommandBlocAcc][%s][%s] - (Acc: %s)", lpObj->Account, lpObj->Name, name);

	return 1;
}

bool CCommandManager::CommandBlocAccRecv(SDHP_COMMAND_BLOC_RECV* lpMsg)
{

	if (lpMsg->result != 1)
	{
		gNotice.GCNoticeSend(lpMsg->index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(446));
	}
	else
	{
		gNotice.GCNoticeSend(lpMsg->index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(447));
	}
	return 1;
}

bool CCommandManager::CommandGift(LPOBJ lpObj, char* arg)
{
	SDHP_GIFT_SEND pMsg;

	pMsg.header.set(0x0F, 0x08, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	return 0;
}

void CCommandManager::DGCommandGiftRecv(SDHP_GIFT_RECV* lpMsg)
{

#if (GAMESERVER_CLIENTE_UPDATE >= 3)

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpMsg->result > 0)
	{
		if (lpMsg->result > gServerInfo.m_CommandGiftLimit)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(503));
		}
		else
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_GIFT, lpObj, lpObj->Map, lpObj->X, lpObj->Y);

			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);

			LogAdd(LOG_EVENT, "[CustomGift][%s][%s] GiftNumber: %d", lpObj->Account, lpObj->Name, lpMsg->result);
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(502));

			this->RemoveRequisites(lpObj, 49);
		}
	}
	else
	{
		LogAdd(LOG_RED, "[CustomGift][%s][%s] Gift Error", lpObj->Account, lpObj->Name);
	}

#endif
}

bool CCommandManager::CommandTop(LPOBJ lpObj, char* arg)
{
	SDHP_TOP_SEND pMsg;

	pMsg.header.set(0x0F, 0x09, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	char tipo[10] = { 0 };
	char classe[3] = { 0 };

	this->GetString(arg, tipo, sizeof(tipo), 0);
	this->GetString(arg, classe, sizeof(classe), 1);

	if (strcmp(classe, "dw") == 0)
	{
		pMsg.classe = 0;
	}
	else if (strcmp(classe, "dk") == 0)
	{
		pMsg.classe = 16;
	}
	else if (strcmp(classe, "fe") == 0)
	{
		pMsg.classe = 32;
	}
	else if (strcmp(classe, "mg") == 0)
	{
		pMsg.classe = 48;
	}
	else if (strcmp(classe, "dl") == 0)
	{
		pMsg.classe = 64;
	}
	else if (strcmp(classe, "su") == 0)
	{
		pMsg.classe = 80;
	}
	else if (strcmp(classe, "rf") == 0)
	{
		pMsg.classe = 96;
	}
	else
	{
		pMsg.classe = 999;
	}

	CUSTOMTOP_INFO CustomTopInfo;

	if (gCustomTop.GetInfoByName(tipo, &CustomTopInfo) != 0)
	{
		pMsg.type = CustomTopInfo.Index;
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(533));
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(534));
		return 0;
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	return 1;
}

void CCommandManager::DGCommandTopRecv(SDHP_TOP_RECV* lpMsg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 3)

	LPOBJ lpObj = &gObj[lpMsg->index];

	CUSTOMTOP_INFO CustomTopInfo;

	if (gCustomTop.GetInfo(lpMsg->type, &CustomTopInfo) != 0)
	{
		if (lpMsg->classe == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (DW)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 16)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (DK)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 32)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (FE)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 48)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (MG)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 64)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (DL)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 80)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (SU)", CustomTopInfo.Title);
		}
		else if (lpMsg->classe == 96)
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s (RF)", CustomTopInfo.Title);
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "%s", CustomTopInfo.Title);
		}
	}
	else
	{
		return;
	}

	if (lpMsg->count <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(535));
		return;
	}

	for (int n = 0; n < lpMsg->count; n++)
	{
		SDHP_TOP_INFO* lpInfo = (SDHP_TOP_INFO*)(((BYTE*)lpMsg) + sizeof(SDHP_TOP_RECV) + (sizeof(SDHP_TOP_INFO) * n));

		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(536), n + 1, lpInfo->name, lpInfo->value);

	}

#endif
}

bool CCommandManager::CommandReAdd(LPOBJ lpObj, char* arg, int Npc)
{
	char name[4] = { 0 };
	this->GetString(arg, name, sizeof(name), 0);

	if (lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(444));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(444));
		}
		return 0;
	}

	int Resto = 0;

	if (strcmp(name, "") == 0)
	{
		if (gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel] != 0)
		{

			for (int n = 0; n < 4; n++) {

				Resto += gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, n);
			}

			lpObj->LevelUpPoint += (lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy - Resto);

			lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
			lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
			lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
			lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
			//lpObj->Leadership	= gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,4);
		}
		else
		{
			for (int n = 0; n < 5; n++) {
				Resto += gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, n);
			}
			lpObj->LevelUpPoint += (lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership - Resto);


			lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
			lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
			lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
			lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
			lpObj->Leadership = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);
		}
	}
	else
	{
		if (strcmp(name, "str") == 0)
		{
			lpObj->LevelUpPoint += (lpObj->Strength) - gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
			lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
		}
		else if (strcmp(name, "agi") == 0)
		{
			lpObj->LevelUpPoint += (lpObj->Dexterity) - gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
			lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
		}
		else if (strcmp(name, "vit") == 0)
		{
			lpObj->LevelUpPoint += (lpObj->Vitality) - gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
			lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
		}
		else if (strcmp(name, "ene") == 0)
		{
			lpObj->LevelUpPoint += (lpObj->Energy) - gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
			lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
		}
		else if (strcmp(name, "cmd") == 0)
		{
			if (gServerInfo.m_CommandResetKeepDLCommandPoint[lpObj->AccountLevel] == 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(576));
				return 0;
			}
			else
			{
				lpObj->LevelUpPoint += (lpObj->Leadership) - gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);
				lpObj->Leadership = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);
			}
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(537));

	gEffectManager.ClearAllEffect(lpObj);

	lpObj->CloseCount = 1;
	
	lpObj->CloseType = 1;

	gLog.Output(LOG_COMMAND, "[CommandReAdd][%s] [%s] - OK", lpObj->Account, lpObj->Name);
	return 1;
}

bool CCommandManager::CommandHelper(LPOBJ lpObj, char* arg, int Npc)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)

	if (lpObj->Map == MAP_ICARUS || CC_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(444));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(444));
		}
		return 0;
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->Index);
		GCSummonLifeSend(lpObj->Index, 0, 1);
		return 0;
	}

	int index;

	for (int i = 1; i <= gServerInfo.m_CommandHelpMonsterAmount; i++)
	{
		index = gObjAddSummon();

		if (OBJECT_RANGE(index) == 0)
		{
			return 0;
		}

		lpObj->SummonIndex = index;

		LPOBJ lpSummon = &gObj[index];

		lpSummon->X = lpObj->X - 1;
		lpSummon->Y = lpObj->Y + i;
		lpSummon->MTX = lpSummon->X;
		lpSummon->MTY = lpSummon->Y;
		lpSummon->Dir = 2;
		lpSummon->Map = lpObj->Map;

		gObjSetMonster(index, gServerInfo.m_CommandHelpMonsterIndex);
#if(GAMESERVER_UPDATE>=555)
		lpSummon->Life += (float)((__int64)lpSummon->Life * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
		lpSummon->MaxLife += (float)((__int64)lpSummon->MaxLife * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
		lpSummon->ScriptMaxLife += (float)((__int64)lpSummon->ScriptMaxLife * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
		lpSummon->Defense += ((__int64)lpSummon->Defense * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DEFENSE)) / 100;
		lpSummon->PhysiDamageMin += ((__int64)lpSummon->PhysiDamageMin * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
		lpSummon->PhysiDamageMax += ((__int64)lpSummon->PhysiDamageMax * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
#endif
		lpSummon->SummonIndex = lpObj->Index;
		lpSummon->Attribute = 100;
		lpSummon->TargetNumber = -1;
		lpSummon->ActionState.Attack = 0;
		lpSummon->ActionState.Emotion = 0;
		lpSummon->ActionState.EmotionCount = 0;
		lpSummon->PathCount = 0;
		lpSummon->MoveRange = 15;

		GCSummonLifeSend(lpSummon->SummonIndex, (int)lpSummon->Life, (int)lpSummon->MaxLife);
	}

	gLog.Output(LOG_COMMAND, "[CommandHelper][%s][%s] - Used", lpObj->Account, lpObj->Name);

#endif
	return 1;
}

bool CCommandManager::CommandReMaster(LPOBJ lpObj, char* arg, int Npc)
{
	if (gObjRebuildMasterSkillTree(lpObj) == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(555));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(555));
		}

		gLog.Output(LOG_COMMAND, "[CommandReMaster][%s][%s] - Used", lpObj->Account, lpObj->Name);
		return 1;
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(556));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(556));
		}
		return 0;
	}
	return 1;
}

bool CCommandManager::CommandStartBC(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	gBloodCastle.StartBC();

	gLog.Output(LOG_COMMAND, "[CommandStartBC][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandStartDS(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	gDevilSquare.StartDS();
	gLog.Output(LOG_COMMAND, "[CommandStartDS][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandStartCC(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	gChaosCastle.StartCC();
	gLog.Output(LOG_COMMAND, "[CommandStartCC][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandStartIT(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	gIllusionTemple.StartIT();
	gLog.Output(LOG_COMMAND, "[CommandStartIT][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

#if EVENT_DROP
bool CCommandManager::CommandStartDrop(LPOBJ lpObj, char* arg)
{
	gCustomEventDrop.StartDrop();
	gLog.Output(LOG_COMMAND, "[CommandStartDrop][%s][%s] - Used", lpObj->Account, lpObj->Name);
	return 1;
}
#endif
bool CCommandManager::CommandStartKing(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 9)
	gReiDoMU.StartKing();
	gLog.Output(LOG_COMMAND, "[CommandStartKingOfMu][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandStartTvT(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 9)
	gTvTEvent.StartTvT();
	gLog.Output(LOG_COMMAND, "[CommandStartTVT][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandStartInvasion(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 13)

	int index = this->GetNumber(arg, 0);

	if (index <= 0)
	{
		return 0;
	}

	gInvasionManager.StartInvasion(index);
	gLog.Output(LOG_COMMAND, "[CommandStartInvasion][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
	return 1;
}

bool CCommandManager::CommandQuyetDau(LPOBJ lpObj, char* arg)
{
	return 1;
}

bool CCommandManager::CommandQuyetDauOK(LPOBJ lpObj, char* arg) // TIM QUYET DAU CA CUOC
{
	return 1;
}

bool CCommandManager::CommandDisablePvp(LPOBJ lpObj, char* arg, int Npc)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if (lpObj->DisablePvp == 1)
	{
		lpObj->DisablePvp = 0;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(672));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(672));
		}

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		return 0;
	}
	else
	{
		lpObj->DisablePvp = 1;

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(671));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(671));
		}

		gLog.Output(LOG_COMMAND, "[CommandDisablePvP][%s][%s] - Used", lpObj->Account, lpObj->Name);

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		return 1;
	}

#endif
	return 0;
}

bool CCommandManager::CommandLock(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if (lpObj->Lock > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(675));
		return 0;
	}

	int password = this->GetNumber(arg, 0);

	if (password <= 0)
	{
		return 0;
	}

	if (password > 99999999)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(674));
		return 0;
	}

	lpObj->Lock = password;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(673));

	gLog.Output(LOG_COMMAND, "[CommandLock][%s][%s] - Password: %d", lpObj->Account, lpObj->Name, password);

	GJAccountLockSaveSend(lpObj->Index, password);

	return 1;

#endif
	return 0;
}

bool CCommandManager::CommandUnLock(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if (lpObj->Lock <= 0)
	{
		return 0;
	}

	int password = this->GetNumber(arg, 0);

	if (password <= 0)
	{
		return 0;
	}

	if (password > 99999999)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(674));
		return 0;
	}

	if (password != lpObj->Lock)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(677));
		return 0;
	}

	lpObj->Lock = 0;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(676));

	gLog.Output(LOG_COMMAND, "[CommandUnLock][%s][%s] - Password: %d", lpObj->Account, lpObj->Name, password);

	GJAccountLockSaveSend(lpObj->Index, 0);

	return 1;

#endif
	return 0;
}

bool CCommandManager::CommandMoveAll(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 8)

	int map = this->GetNumber(arg, 0);

	int x = this->GetNumber(arg, 1);

	int y = this->GetNumber(arg, 2);

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0 && gObj[n].Index != lpObj->Index)
		{
			if (gObj[n].Connected > OBJECT_LOGGED)
			{
				gObjTeleport(gObj[n].Index, map, x, y);
			}
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMoveAll][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, map, x, y);
#endif
	return 1;
}

bool CCommandManager::CommandMoveGuild(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 8)
	char name[9] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int map = this->GetNumber(arg, 1);

	int x = this->GetNumber(arg, 2);

	int y = this->GetNumber(arg, 3);

	GUILD_INFO_STRUCT* lpGuild = gGuildClass.SearchGuild(name);

	if (lpGuild == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(698));
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_USER; n++)
	{
		int iGuildMemberIndex = lpGuild->Index[n];

		if (lpGuild->Use[n] > 0 && iGuildMemberIndex != -1)
		{
			LPOBJ lpObj = &gObj[iGuildMemberIndex];

			if (lpObj == NULL)
			{
				continue;
			}

			if (gObjIsConnected(iGuildMemberIndex) == 0)
			{
				continue;
			}

			gObjTeleport(iGuildMemberIndex, map, x, y);
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMoveGuild][%s][%s] - (Guild: %s, Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, name, map, x, y);
#endif
	return 1;
}

bool CCommandManager::CommandSpot(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 9)

	int monster = this->GetNumber(arg, 0);
	int qtd = this->GetNumber(arg, 1);

	qtd = (qtd > 0) ? qtd : 1;

	for (int n = 0; n < qtd; n++)
	{
		int index = gObjAddMonster(lpObj->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			return 0;
		}

		LPOBJ lpMonster = &gObj[index];

		int px = lpObj->X;
		int py = lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 3, 3, 50) == 0)
		{
			return 0;
		}

		lpMonster->PosNum = -1;
		lpMonster->X = px;
		lpMonster->Y = py;
		lpMonster->TX = px;
		lpMonster->TY = py;
		lpMonster->OldX = px;
		lpMonster->OldY = py;
		lpMonster->StartX = px;
		lpMonster->StartY = py;
		lpMonster->Dir = 1;
		lpMonster->Map = lpObj->Map;
		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;

		if (gObjSetMonster(index, monster) == 0)
		{
			gObjDel(index);
			return 0;
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandSpot][%s][%s] - (Monster: %d, Qtd: %d)", lpObj->Account, lpObj->Name, monster, qtd);
#endif
	return 1;
}

bool CCommandManager::CommandSetCoin(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };
	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (!lpTarget)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(446));
		return false;
	}

	int coin = this->GetNumber(arg, 1);
	int value = this->GetNumber(arg, 2);

	switch (coin)
	{
	case 1: GDSetCoinSend(lpTarget->Index, value, 0, 0, 0, 0, "GmSetCoin"); break;
	case 2: GDSetCoinSend(lpTarget->Index, 0, value, 0, 0, 0, "GmSetCoin"); break;
	case 3: GDSetCoinSend(lpTarget->Index, 0, 0, value, 0, 0, "GmSetCoin"); break;
	case 4: GDSetCoinSend(lpTarget->Index, 0, 0, 0, value, 0, "GmSetCoin"); break;
	case 5: GDSetCoinSend(lpTarget->Index, 0, 0, 0, 0, value, "GmSetCoin"); break;
	default: return false;
	}

	gLog.Output(LOG_COMMAND, "[CommandSetCoin][%s][%s] - (Coin: %d, Value: %d)", lpObj->Account, lpObj->Name, coin, value);
	return true;
}

bool CCommandManager::CommandSetLevel(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };
	this->GetString(arg, name, sizeof(name), 0);
	LPOBJ lpTarget = gObjFind(name);
	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Nhân vật không tồn tại hoặc không trực tuyến!");
		return 0;
	}

	int level = this->GetNumber(arg, 1);
	if (level >= MAX_CHARACTER_LEVEL + 1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Vượt quá mức cho phép");
		return 0;
	}
	lpTarget->Level = level;
	GCLevelUpSend(lpTarget);
	gLog.Output(LOG_COMMAND, "[CommandSetLevel] Account:[%s] Name:[%s] - (Name:%s, Level: %d)", lpObj->Account, lpObj->Name, name, level);
}
bool CCommandManager::CommandSetReset(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };
	this->GetString(arg, name, sizeof(name), 0);
	LPOBJ lpTarget = gObjFind(name);
	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Nhân vật không tồn tại hoặc không trực tuyến!");
		return 0;
	}

	int Reset = this->GetNumber(arg, 1);
	if (Reset >= 65535)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Vượt quá mức cho phép");
		return 0;
	}
	lpTarget->Reset = (lpTarget->Reset + Reset);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	GDResetInfoSaveSend(lpObj->Index, 0, 0, 0);
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Đã Cộng %d Lần Reset. Bạn Có %d Lần Reset!", Reset, lpTarget->Reset);
	gLog.Output(LOG_COMMAND, "[CommandSetReset] Account:[%s] Name:[%s] - (Name:%s, Reset: %d)", lpObj->Account, lpObj->Name, name, Reset);
}

bool CCommandManager::CommandSetPass(LPOBJ lpObj, char* arg)
{
	char password[20] = { 0 };
	this->GetString(arg, password, sizeof(password), 0);
	PMSG_RECV_SETPASS_ADD pMsg;
	pMsg.SystemActive = true;
	memcpy(pMsg.Password, password, sizeof(pMsg.Password));

	if (strcmp(password, "") == 0)
	{
		pMsg.SystemActive = false;
	}

	gPartySetPass.PartySetPassSearchAddToList(pMsg, lpObj->Index);
	gLog.Output(LOG_COMMAND, "[CommandSetpass][%s][%s] - Password: %d", lpObj->Account, lpObj->Name, password);

	return 1;
}

bool CCommandManager::CommandStartBsV(LPOBJ lpObj, char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 9)
#if(EVENT_PK)
	gBsVEvent.StartBSV();
	gLog.Output(LOG_COMMAND, "[CommandStartBSV][%s][%s] - Used", lpObj->Account, lpObj->Name);
#endif
#endif
	return 1;
}


bool CCommandManager::CommandResetVip(LPOBJ lpObj, char* arg, int Npc) // OK
{
	char mode[5] = { 0 };

	gCommandManager.GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandResetVipAuto(lpObj, arg, Npc);
		return 1;
	}

	if (lpObj->PKLevel >= 5)
	{
		/*gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(153));*/
		return 0;
	}


	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Không Thể Reset Tại Thời Điểm Này"); //Bạn không thể sử dụng / reset tại thời điểm này"

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, "Bạn Không Thể Reset Tại Thời Điểm Này"); //Bạn không thể sử dụng / reset tại thời điểm này"
		}
		return 0;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gResetTableVip.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(91), gResetTableVip.GetResetLevel(lpObj));			// "Bạn phải đạt ít nhất mức %d để sử dụng / reset"

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(91), gResetTableVip.GetResetLevel(lpObj));		// "Bạn phải đạt ít nhất mức %d để sử dụng / reset"
		}
		return 0;
	}

	if (lpObj->Money < ((DWORD)gResetTableVip.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(92), gResetTableVip.GetResetMoney(lpObj));	//Bạn phải có ít nhất %d zen để sử dụng / reset"

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(92), gResetTableVip.GetResetMoney(lpObj));	//Bạn phải có ít nhất %d zen để sử dụng / reset"
		}
		return 0;
	}

	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(93), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);	//"Bạn không thể có nhiều hơn %d reset"

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(93), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);	//"Bạn không thể có nhiều hơn %d reset"
		}
		return 0;
	}

	if (gServerInfo.m_CommandResetCheckItem[0] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(94)); //"Bạn không thể thiết lập lại với các mục được trang bị"

				if (Npc >= 0)
				{
					GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(94)); //"Bạn không thể thiết lập lại với các mục được trang bị"
				}
				return 0;
			}
		}
	}

	if ((gResetTableVip.GetResetChaos(lpObj) > 0 && lpObj->ItemBank[7] < gResetTableVip.GetResetChaos(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Chaos] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetChaos(lpObj)); return 0;
	}

	if ((gResetTableVip.GetResetBless(lpObj) > 0 && lpObj->ItemBank[0] < gResetTableVip.GetResetBless(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Bless] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetBless(lpObj)); return 0;
	}

	if ((gResetTableVip.GetResetSoul(lpObj) > 0 && lpObj->ItemBank[1] < gResetTableVip.GetResetSoul(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Soul] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetSoul(lpObj)); return 0;
	}

	if ((gResetTableVip.GetResetLife(lpObj) > 0 && lpObj->ItemBank[2] < gResetTableVip.GetResetLife(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Life] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetLife(lpObj)); return 0;
	}

	if ((gResetTableVip.GetResetCre(lpObj) > 0 && lpObj->ItemBank[3] < gResetTableVip.GetResetCre(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Cre] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetCre(lpObj)); return 0;
	}

	if ((gResetTableVip.GetResetMoney(lpObj) > 0 && lpObj->Money < gResetTableVip.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d Zen Để Reset", gResetTableVip.GetResetMoney(lpObj)); return 0;
	}
	if ((gResetTableVip.GetResetTienTeWCoinC(lpObj) > 0 && lpObj->ThisCoin[0] < gResetTableVip.GetResetTienTeWCoinC(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinC Để Reset", gResetTableVip.GetResetTienTeWCoinC(lpObj)); return 0;
	}
	//if ((gResetTableVip.GetResetTienTeWCoinP(lpObj) > 0 && lpObj->ThisCoin[1] < gResetTableVip.GetResetTienTeWCoinP(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinP Để Reset", gResetTableVip.GetResetTienTeWCoinP(lpObj)); return 0;
	//}

	if ((gResetTableVip.GetResetChaos(lpObj) > 0)
		|| (gResetTableVip.GetResetBless(lpObj) > 0)
		|| (gResetTableVip.GetResetSoul(lpObj) > 0)
		|| (gResetTableVip.GetResetLife(lpObj) > 0)
		|| (gResetTableVip.GetResetCre(lpObj) > 0)
		)
	{
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 7, gResetTableVip.GetResetChaos(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 0, gResetTableVip.GetResetBless(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 1, gResetTableVip.GetResetSoul(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 2, gResetTableVip.GetResetLife(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 3, gResetTableVip.GetResetCre(lpObj));
	}

	if (gResetTableVip.GetResetTienTeWCoinC(lpObj) > 0) { //WC
		GDSetCoinSend(lpObj->Index, -gResetTableVip.GetResetTienTeWCoinC(lpObj), 0, 0, 0, 0, "ResetVIP");
	}

	//if (gResetTableVip.GetResetTienTeWCoinP(lpObj) > 0) { //GP
	//	GDSetCoinSend(lpObj->Index, 0, -gResetTableVip.GetResetTienTeWCoinP(lpObj), 0, 0, 0, "ResetVIP");
	//}

	lpObj->ResetVip = 1;

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x00, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	return 0;
}

void CCommandManager::CommandResetVipAuto(LPOBJ lpObj, char* arg, int Npc)
{
	if (gServerInfo.m_CommandResetVipAutoEnable[0] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(89));

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, gMessageNew.GetMessage(89));
		}
		return;
	}

	if (lpObj->AutoResetVipEnable == 0)
	{
		lpObj->AutoResetEnable = 0;
		lpObj->AutoResetVipEnable = 1;
		lpObj->AutoResetStats[0] = gCommandManager.GetNumber(arg, 1);
		lpObj->AutoResetStats[1] = gCommandManager.GetNumber(arg, 2);
		lpObj->AutoResetStats[2] = gCommandManager.GetNumber(arg, 3);
		lpObj->AutoResetStats[3] = gCommandManager.GetNumber(arg, 4);
		lpObj->AutoResetStats[4] = ((lpObj->Class == CLASS_DL) ? gCommandManager.GetNumber(arg, 5) : 0);

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Auto ResetVIP : Bật");

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, "Auto ResetVIP : Bật");
		}
	}
	else
	{
		lpObj->AutoResetEnable = 0;
		lpObj->AutoResetVipEnable = 0;
		memset(lpObj->AutoResetStats, 0, sizeof(lpObj->AutoResetStats));

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Auto ResetVIP : Tắt");

		if (Npc >= 0)
		{
			GCChatTargetNewSend(lpObj, Npc, "Auto ResetVIP : Tắt");
		}
	}

	//// them GP reset
	//if ((gResetTableVip.GetResetChaos(lpObj) > 0 && lpObj->ItemBank[7] < gResetTableVip.GetResetChaos(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Chaos] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetChaos(lpObj));  return;
	//}
	//
	//if ((gResetTableVip.GetResetBless(lpObj) > 0 && lpObj->ItemBank[0] < gResetTableVip.GetResetBless(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Bless] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetBless(lpObj));  return;
	//}
	//
	//if ((gResetTableVip.GetResetSoul(lpObj) > 0 && lpObj->ItemBank[1] < gResetTableVip.GetResetSoul(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Soul] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetSoul(lpObj)); return;
	//}
	//
	//if ((gResetTableVip.GetResetLife(lpObj) > 0 && lpObj->ItemBank[2] < gResetTableVip.GetResetLife(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Life] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetLife(lpObj)); return;
	//}
	//
	//if ((gResetTableVip.GetResetCre(lpObj) > 0 && lpObj->ItemBank[3] < gResetTableVip.GetResetCre(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Cre] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetCre(lpObj)); return;
	//}
	//
	//if ((gResetTableVip.GetResetMoney(lpObj) > 0 && lpObj->Money < gResetTableVip.GetResetMoney(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d Zen Để Reset", gResetTableVip.GetResetMoney(lpObj)); return;
	//}
	//if ((gResetTableVip.GetResetTienTeWCoinC(lpObj) > 0 && lpObj->ThisCoin[0] < gResetTableVip.GetResetTienTeWCoinC(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinC Để Reset", gResetTableVip.GetResetTienTeWCoinC(lpObj)); return;
	//}
	//if ((gResetTableVip.GetResetTienTeWCoinP(lpObj) > 0 && lpObj->ThisCoin[1] < gResetTableVip.GetResetTienTeWCoinP(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinP Để Reset", gResetTableVip.GetResetTienTeWCoinP(lpObj)); return;
	//}
}


void CCommandManager::CommandResetVipAutoProc(LPOBJ lpObj)
{
	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gResetTableVip.GetResetLevel(lpObj))
	{
		return;
	}

	if (lpObj->Money < gResetTableVip.GetResetMoney(lpObj))
	{
		return;
	}


	if (lpObj->Reset >= gResetLitmit.GioiHanRS)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(1), gResetLitmit.GioiHanRS);
		return;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	// Kiểm tra và thông báo thiếu tài sản cần thiết Để reset VIP
	if ((gResetTableVip.GetResetChaos(lpObj) > 0 && lpObj->ItemBank[7] < gResetTableVip.GetResetChaos(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Chaos] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetChaos(lpObj));  return;
	}

	if ((gResetTableVip.GetResetBless(lpObj) > 0 && lpObj->ItemBank[0] < gResetTableVip.GetResetBless(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Bless] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetBless(lpObj));  return;
	}

	if ((gResetTableVip.GetResetSoul(lpObj) > 0 && lpObj->ItemBank[1] < gResetTableVip.GetResetSoul(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Soul] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetSoul(lpObj)); return;
	}

	if ((gResetTableVip.GetResetLife(lpObj) > 0 && lpObj->ItemBank[2] < gResetTableVip.GetResetLife(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Life] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetLife(lpObj)); return;
	}

	if ((gResetTableVip.GetResetCre(lpObj) > 0 && lpObj->ItemBank[3] < gResetTableVip.GetResetCre(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu [%d Viên Cre] Ở Ngân Hàng Ngọc Để Reset VIP", gResetTableVip.GetResetCre(lpObj)); return;
	}

	if ((gResetTableVip.GetResetMoney(lpObj) > 0 && lpObj->Money < gResetTableVip.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d Zen Để Reset", gResetTableVip.GetResetMoney(lpObj)); return;
	}
	if ((gResetTableVip.GetResetTienTeWCoinC(lpObj) > 0 && lpObj->ThisCoin[0] < gResetTableVip.GetResetTienTeWCoinC(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinC Để Reset", gResetTableVip.GetResetTienTeWCoinC(lpObj)); return;
	}
	//if ((gResetTableVip.GetResetTienTeWCoinP(lpObj) > 0 && lpObj->ThisCoin[1] < gResetTableVip.GetResetTienTeWCoinP(lpObj)))
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Thiếu %d WCoinP Để Reset", gResetTableVip.GetResetTienTeWCoinP(lpObj)); return;
	//}

	if ((gResetTableVip.GetResetChaos(lpObj) > 0)
		|| (gResetTableVip.GetResetBless(lpObj) > 0)
		|| (gResetTableVip.GetResetSoul(lpObj) > 0)
		|| (gResetTableVip.GetResetLife(lpObj) > 0)
		|| (gResetTableVip.GetResetCre(lpObj) > 0)
		)
	{
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 7, gResetTableVip.GetResetChaos(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 0, gResetTableVip.GetResetBless(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 1, gResetTableVip.GetResetSoul(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 2, gResetTableVip.GetResetLife(lpObj));
		gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 3, gResetTableVip.GetResetCre(lpObj));
	}

	if (gResetTableVip.GetResetTienTeWCoinC(lpObj) > 0) { //WC
		GDSetCoinSend(lpObj->Index, -gResetTableVip.GetResetTienTeWCoinC(lpObj), 0, 0, 0, 0, "ResetVIP");
	}

	//if (gResetTableVip.GetResetTienTeWCoinP(lpObj) > 0) { //GP
	//	GDSetCoinSend(lpObj->Index, 0, -gResetTableVip.GetResetTienTeWCoinP(lpObj), 0, 0, 0, "ResetVIP");
	//}

	/*gLog.Output(LOG_RESET, "[Reset]: Tài Khoản: %s - Nhân Vật: %s - Reset: %d - Chao: %d - Bless: %d - Soul: %d - Life: %d - Cre: %d- WC: %d"
		, lpObj->Account
		, lpObj->Name
		, lpObj->Reset
		, gResetTableVip.GetResetChaos(lpObj)
		, gResetTableVip.GetResetBless(lpObj)
		, gResetTableVip.GetResetSoul(lpObj)
		, gResetTableVip.GetResetLife(lpObj)
		, gResetTableVip.GetResetCre(lpObj));*/

	// Gửi thông báo reset VIP thành công
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "ResetVIP Thành Công");

	lpObj->ResetVip = 1;

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x00, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}