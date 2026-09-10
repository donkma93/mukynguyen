#include "stdafx.h"
#include "DSProtocol.h"
#include "GuildClass.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"

#include "ServerInfo.h"
#include "SkillManager.h"
#include "ScheduleManager.h"
#include "Notice.h"
#include "Util.h"
#include "CommandManager.h"
#include "GameMain.h"
#include "Guild.h"
#include "ACuoi_CastleEvent.h"
#include "CashShop.h"
#include "Monster.h"
#include "ItemBagManager.h"
#include "Gate.h"

CastleStartGuild gCastleStartGuild;

CastleStartGuild::CastleStartGuild()
{
	this->m_State = CTC_STATE_BLANK;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_RemainTimeWinner = 0;
	this->m_TickCount = GetTickCount();
	this->MinutesLeft = -1;

	this->WinnerNumber = -1;
	this->WinnerTime = 0;
	ZeroMemory(this->WinnerName, sizeof(this->WinnerName));
	ZeroMemory(this->WinnerNameOLD, sizeof(this->WinnerNameOLD));

	for (int n = 0; n < 3; n++)
	{
		IndexCong[n] = -1;
		IndexTru[n] = -1;
	}

	ZeroMemory(&this->b_PhaTru, sizeof(this->b_PhaTru));
	ZeroMemory(&this->b_PhaCong, sizeof(this->b_PhaCong));
	ZeroMemory(&this->b_GuildWin, sizeof(this->b_GuildWin));
	ZeroMemory(&this->GateMove, sizeof(this->GateMove));

	this->m_CTCMinitartTime.clear();

	this->Class = -1;
	this->Map = 0;
	this->X = 0;
	this->Y = 0;
	this->GuildMinPlayer = 0;

	this->Clear();
}

void CastleStartGuild::Clear()
{
	for (int n = 0; n < MAX_CTC_GUILD; n++)
	{
		this->Guild[n].Reset();
	}

	for (int n = 0; n < MAX_CTC_CHAR; n++)
	{
		this->Char[n].Reset();
	}

	this->m_RemainTimeWinner = 0;
	this->WinnerNumber = -1;

	for (int n = 0; n < 3; n++)
	{
		IndexCong[n] = -1;
		IndexTru[n] = -1;
	}

	this->m_Active = 0;
}

void CastleStartGuild::Init()
{
	if (this->m_Enabled == 0)
	{
		this->SetState(CTC_STATE_BLANK);
	}
	else
	{
		this->SetState(CTC_STATE_EMPTY);
	}
}

void CastleStartGuild::Load(char* path)
{
	this->m_CTCMinitartTime.clear();
	ZeroMemory(this->WinnerNameOLD, sizeof(this->WinnerNameOLD));
	this->Clear();

	this->m_Enabled = 0;
	this->m_EventTime = 0;
	this->m_WarningTime = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);
	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}

	pugi::xml_node oCTCMini = file.child("CTCMini");

	pugi::xml_node eCTCMini = oCTCMini.child("EventTime");
	for (pugi::xml_node rCTCMini = eCTCMini.child("Time"); rCTCMini; rCTCMini = rCTCMini.next_sibling())
	{
		CTC_START_TIME info;
		info.Year = rCTCMini.attribute("Year").as_int();
		info.Month = rCTCMini.attribute("Month").as_int();
		info.Day = rCTCMini.attribute("Day").as_int();
		info.DayOfWeek = rCTCMini.attribute("DayOfWeek").as_int();
		info.Hour = rCTCMini.attribute("Hour").as_int();
		info.Minute = rCTCMini.attribute("Minute").as_int();
		info.Second = rCTCMini.attribute("Second").as_int();
		this->m_CTCMinitartTime.push_back(info);
	}

	pugi::xml_node eNPCTalk = oCTCMini.child("NPCTalk");
	for (pugi::xml_node rNPCTalk = eNPCTalk.child("NPC"); rNPCTalk; rNPCTalk = rNPCTalk.next_sibling())
	{
		this->Class = rNPCTalk.attribute("Class").as_int();
		this->Map = rNPCTalk.attribute("Map").as_int();
		this->X = rNPCTalk.attribute("X").as_int();
		this->Y = rNPCTalk.attribute("Y").as_int();
		this->ClassTru = rNPCTalk.attribute("ClassTru").as_int();
		this->ClassGate = rNPCTalk.attribute("ClassGate").as_int();
	}

	pugi::xml_node eCauHinhTime = oCTCMini.child("CauHinhTime");
	for (pugi::xml_node rCTCConfig = eCauHinhTime.child("Config"); rCTCConfig; rCTCConfig = rCTCConfig.next_sibling())
	{
		this->m_Enabled = rCTCConfig.attribute("Enabled").as_int();
		this->m_WarningTime = rCTCConfig.attribute("TimeChuanBi").as_int();
		this->m_EventTime = rCTCConfig.attribute("TimeSuKien").as_int();
		this->WinnerTime = rCTCConfig.attribute("SoTruCanChiem").as_int();
		this->GuildMinPlayer = rCTCConfig.attribute("GuildMinPlayer").as_int();
	}

	pugi::xml_node eCauHinhGate = oCTCMini.child("CauHinhGate");
	for (pugi::xml_node rGateConfig = eCauHinhGate.child("Config"); rGateConfig; rGateConfig = rGateConfig.next_sibling())
	{
		this->GateMove[0] = rGateConfig.attribute("Gate1").as_int();
		this->GateMove[1] = rGateConfig.attribute("Gate2").as_int();
		this->GateMove[2] = rGateConfig.attribute("Gate3").as_int();
		this->GateMove[3] = rGateConfig.attribute("Gate4").as_int();
		this->GateMove[4] = rGateConfig.attribute("Gate5").as_int();
	}

	pugi::xml_node ePhaCong = oCTCMini.child("PhaCong");
	for (pugi::xml_node rPhaCong = ePhaCong.child("Config"); rPhaCong; rPhaCong = rPhaCong.next_sibling())
	{
		b_PhaCong.WCoin = rPhaCong.attribute("WCoin").as_int();
		b_PhaCong.WCoinP = rPhaCong.attribute("WCoinP").as_int();
		b_PhaCong.GobinP = rPhaCong.attribute("GobinP").as_int();
		b_PhaCong.Ruud = rPhaCong.attribute("Ruud").as_int();
		b_PhaCong.IndexBonus = rPhaCong.attribute("BonusAllGuild").as_int();
	}

	pugi::xml_node ePhaTru = oCTCMini.child("PhaTru");
	for (pugi::xml_node rPhaTru = ePhaTru.child("Config"); rPhaTru; rPhaTru = rPhaTru.next_sibling())
	{
		b_PhaTru.WCoin = rPhaTru.attribute("WCoin").as_int();
		b_PhaTru.WCoinP = rPhaTru.attribute("WCoinP").as_int();
		b_PhaTru.GobinP = rPhaTru.attribute("GobinP").as_int();
		b_PhaTru.Ruud = rPhaTru.attribute("Ruud").as_int();
		b_PhaTru.IndexBonus = rPhaTru.attribute("BonusAllGuild").as_int();
	}

	pugi::xml_node eGuidWin = oCTCMini.child("GuidWin");
	for (pugi::xml_node rGuildWin = eGuidWin.child("Config"); rGuildWin; rGuildWin = rGuildWin.next_sibling())
	{
		b_GuildWin.WCoin = rGuildWin.attribute("WCoin").as_int();
		b_GuildWin.WCoinP = rGuildWin.attribute("WCoinP").as_int();
		b_GuildWin.GobinP = rGuildWin.attribute("GobinP").as_int();
		b_GuildWin.Ruud = rGuildWin.attribute("Ruud").as_int();
		b_GuildWin.IndexBonus = rGuildWin.attribute("IndexItemBag").as_int();
	}

	this->YeuCauCoin[0] = b_PhaCong.WCoin;
	this->YeuCauCoin[1] = b_PhaTru.WCoin;
	this->YeuCauCoin[2] = b_GuildWin.WCoin;

	GetPrivateProfileString("GuildWinOLD", "GuildName", "", this->WinnerNameOLD, sizeof(this->WinnerNameOLD), "..\\Data\\Event\\CTCMini\\GuildWin.ini");
}


void CastleStartGuild::MainProc()
{
	DWORD elapsed = GetTickCount() - this->m_TickCount;

	if (elapsed < 1000)
	{
		return;
	}

	this->m_TickCount = GetTickCount();
	this->m_RemainTime = (int)difftime(this->m_TargetTime, time(0));

	if (this->m_Enabled == 0)
	{
		if (gServerDisplayer.EventCTCMini != -1)
		{
			gServerDisplayer.EventCTCMini = -1;
		}
	}
	else
	{
		if (this->m_State == CTC_STATE_EMPTY)
		{
			gServerDisplayer.EventCTCMini = this->m_RemainTime;
		}
		else
		{
			if (gServerDisplayer.EventCTCMini != 0)
			{
				gServerDisplayer.EventCTCMini = 0;
			}
		}
	}

	switch (this->m_State)
	{
		case CTC_STATE_BLANK: this->ProcState_BLANK(); break;
		case CTC_STATE_EMPTY: this->ProcState_EMPTY(); break;
		case CTC_STATE_START: this->ProcState_START(); break;
	}

	if (this->m_RemainTimeWinner > 0)
	{
		if (this->WinnerNumber >= 0)
		{
			if (this->m_RemainTimeWinner == 40)
			{
				GCServerMsgStringSendToMap(MAP_GUILWAR, 0, "[Công Thành chiến Mini] Chuẩn bị tiến hành trao thưởng!");
			}

			if (this->m_RemainTimeWinner == 15)
			{
				GUILD_INFO_STRUCT* lpGuildInfo = gGuildClass.SearchGuild_Number(this->WinnerNumber);

				if (lpGuildInfo != 0)
				{
					LogAdd(LOG_EVENT, "[CTC Mini] Guild Winner: %s", lpGuildInfo->Name);
					gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Guild [%s] đã chiến thắng!", lpGuildInfo->Name);
					gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "Các thành viên tham gia sẽ được thưởng %d WC, %d WP, %d GP, %d Ruud", b_GuildWin.WCoin, b_GuildWin.WCoinP, b_GuildWin.GobinP, b_GuildWin.Ruud);

					for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
					{
						if (gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
						{
							if (strcmp(gObj[n].GuildName, lpGuildInfo->Name) == 0)
							{
								LogAdd(LOG_EVENT, "[CTC Mini] Bonus Guild Member Reward: %s", gObj[n].Name);

								gCashShop.GDCashShopAddPointSaveSend(gObj[n].Index, 0, b_GuildWin.WCoin, b_GuildWin.WCoinP, b_GuildWin.GobinP, b_GuildWin.Ruud, 0);
								gCashShop.CGCashShopPointRecv(gObj[n].Index);

								if (b_GuildWin.IndexBonus > 0)
								{
									gItemBagManager.DropItemByIndexItemBag(b_GuildWin.IndexBonus, &gObj[n]);
								}
							}
						}
					}
				}
			}

			this->m_RemainTimeWinner--;

			if (this->m_RemainTimeWinner <= 0)
			{
				GCServerMsgStringSendToMap(MAP_GUILWAR, 0, "[Công Thành chiến Mini] Đã kết thúc !");
				LogAdd(LOG_EVENT, "[CTC Mini] Finish 1");

				for (int n = 0; n < MAX_CTC_CHAR; n++)
				{
					if (this->Char[n].Index == -1)
					{
						continue;
					}

					gObjMoveGate(this->Char[n].Index, 17);
				}

				this->Clear();
			}
		}
		else
		{
			LogAdd(LOG_EVENT, "[CTC Mini] Finish 2");
			GCServerMsgStringSendToMap(MAP_GUILWAR, 0, "[Công Thành chiến Mini] Đã kết thúc không có Guild chiến thắng !");
			this->m_RemainTimeWinner = 0;
			this->Clear();
		}
	}
}


void CastleStartGuild::ProcState_BLANK()
{

}

void CastleStartGuild::ProcState_EMPTY()
{
	if (this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_WarningTime * 60))
	{
		int minutes = this->m_RemainTime / 60;

		if ((this->m_RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (this->MinutesLeft != minutes)
		{
			this->MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Bắt đầu sau %d phút !", (MinutesLeft + 1));
		}

		if (this->m_Active == 0)
		{
			this->m_Active = 1;
		}
	}

	if (this->m_RemainTime > 0 && this->m_RemainTime <= 5)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Bắt đầu sau %d giây !", m_RemainTime);
	}

	if (this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Sự kiện bắt đầu !");
		this->SetState(CTC_STATE_START);
	}
}


void CastleStartGuild::ProcState_START()
{
	if (this->m_RemainTime > 0 && this->m_RemainTime <= 300)
	{
		int minutes = this->m_RemainTime / 60;

		if (this->MinutesLeft != minutes)
		{
			this->MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Sẽ kết thúc sau %d phút !", (MinutesLeft + 1));
		}
	}

	if (this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Đã kết thúc !");
		this->SetGuildTop();
		this->m_RemainTimeWinner = 40;
		this->m_Active = 0;
		this->SetState(CTC_STATE_EMPTY);
		SetEntranceZone();

		for (int n = 0; n < 3; n++)
		{
			if (gObj[IndexCong[n]].Live)
			{
				gObjDel(IndexCong[n]);
			}

			if (gObj[IndexTru[n]].Live)
			{
				gObjDel(IndexTru[n]);
			}
		}
	}

	for (int ttb = 0; ttb < 3; ttb++)
	{
		if (IndexTru[ttb] != -1 && gObj[IndexTru[ttb]].Live)
		{
			int px = ThemTru[ttb][0];
			int py = ThemTru[ttb][1];

			if (gObj[IndexTru[ttb]].X != px || gObj[IndexTru[ttb]].Y != py)
			{
				gObj[IndexTru[ttb]].X = px;
				gObj[IndexTru[ttb]].Y = py;
			}
		}
	}
}


void CastleStartGuild::SetState(int state)
{
	this->m_State = state;

	switch (this->m_State)
	{
		case CTC_STATE_BLANK:		this->SetState_BLANK();		break;
		case CTC_STATE_EMPTY:		this->SetState_EMPTY();		break;
		case CTC_STATE_START:		this->SetState_START();		break;
	}
}

void CastleStartGuild::SetState_BLANK()
{

}

void CastleStartGuild::SetState_EMPTY()
{
	this->CheckSync();
}

void CastleStartGuild::SetState_START()
{
	this->m_Active = 2;
	this->m_RemainTime = this->m_EventTime * 60;
	this->m_TargetTime = (int)(time(0) + this->m_RemainTime);
	this->TestCTCMini();
	this->SendKillCTCMini(0, 0);
	LogAdd(LOG_EVENT, "[CTC Mini] Bắt Đầu Event Công Thành Chiến Mini");
}

void CastleStartGuild::CheckSync()
{
	if (this->m_CTCMinitartTime.empty() != 0)
	{
		this->SetState(CTC_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;
	CScheduleManager ScheduleManager;

	for (std::vector<CTC_START_TIME>::iterator it = this->m_CTCMinitartTime.begin(); it != this->m_CTCMinitartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(
			it->Year,
			it->Month,
			it->Day,
			it->Hour,
			it->Minute,
			it->Second,
			it->DayOfWeek
		);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(CTC_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));
	this->m_TargetTime = (int)ScheduleTime.GetTime();
}

void CastleStartGuild::CGPacketCTCMini(CTCMINI_CGPACKET* aRecv, int aIndex)
{
	if (!this->m_Enabled)
	{
		return;
	}

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ((GetTickCount() - lpObj->ClickClientSend) < 2000)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn thao tác quá nhanh vui lòng chờ đợi !");
		return;
	}

	GUILD_INFO_STRUCT* lpGuildInfo = lpObj->Guild;

	if (this->m_Active == 0 || this->m_Active == 2)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Chưa đến thời gian sự kiện");
		return;
	}

	if (lpGuildInfo == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Không thấy thông tin Guild của bạn");
		return;
	}

	if (lpObj->GuildNumber == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Bạn chưa vào Guild nên không thể tham gia !");
		return;
	}

	if (lpGuildInfo->TotalCount < this->GuildMinPlayer)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[Công Thành chiến Mini] Guild bạn không đủ người tham gia !");
		return;
	}

	int CongVao = aRecv->CongVao;

	this->SetChar(lpObj->Index, CongVao);
	gObjMoveGate(lpObj->Index, this->GateMove[CongVao]);
	this->SendKillCTCMini(lpObj->Index, 1);
}


void CastleStartGuild::SendKillCTCMini(int aIndex, int Type)
{
	CTCMINIKILL_SENDCLIENT pMsg{};
	pMsg.header.set(0xF3, 0x43, sizeof(pMsg));

	if (Type == 1)
	{
		if (gObjIsConnectedGP(aIndex) == 0)
		{
			return;
		}

		LPOBJ lpObj = &gObj[aIndex];
		pMsg.TimeCTCMini = this->m_RemainTime;

		memset(&pMsg.GNameTru1, 0, sizeof(pMsg.GNameTru1));
		memset(&pMsg.GNameTru2, 0, sizeof(pMsg.GNameTru2));
		memset(&pMsg.GNameTru3, 0, sizeof(pMsg.GNameTru3));

		for (int b = 0; b < 3; b++)
		{
			if (IndexTru[b] != -1 && gObj[IndexTru[b]].GuildNumber == lpObj->GuildNumber)
			{
				pMsg.Tru[b] = 1;
			}
			else if (IndexTru[b] != -1 && gObj[IndexTru[b]].GuildNumber != 0 && gObj[IndexTru[b]].GuildNumber != lpObj->GuildNumber)
			{
				pMsg.Tru[b] = 2;
			}
			else
			{
				pMsg.Tru[b] = 0;
			}

			if (IndexCong[b] != -1 && !gObj[IndexCong[b]].Live)
			{
				pMsg.Cong[b] = 1;
			}
			else
			{
				pMsg.Cong[b] = 0;
			}
		}

		if (strlen(gObj[IndexTru[0]].GuildName) > 1)
		{
			memcpy(pMsg.GNameTru1, gObj[IndexTru[0]].GuildName, 9);
		}

		if (strlen(gObj[IndexTru[1]].GuildName) > 1)
		{
			memcpy(pMsg.GNameTru2, gObj[IndexTru[1]].GuildName, 9);
		}

		if (strlen(gObj[IndexTru[2]].GuildName) > 1)
		{
			memcpy(pMsg.GNameTru3, gObj[IndexTru[2]].GuildName, 9);
		}

		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if (gObjIsConnectedGP(n) == 0 || gObj[n].Map != MAP_GUILWAR)
			{
				continue;
			}

			pMsg.TimeCTCMini = this->m_RemainTime;

			memset(&pMsg.GNameTru1, 0, sizeof(pMsg.GNameTru1));
			memset(&pMsg.GNameTru2, 0, sizeof(pMsg.GNameTru2));
			memset(&pMsg.GNameTru3, 0, sizeof(pMsg.GNameTru3));

			for (int b = 0; b < 3; b++)
			{
				if (IndexTru[b] != -1 && gObj[IndexTru[b]].GuildNumber != 0 && gObj[IndexTru[b]].GuildNumber == gObj[n].GuildNumber)
				{
					pMsg.Tru[b] = 1;
				}
				else if (IndexTru[b] != -1 && gObj[IndexTru[b]].GuildNumber != 0 && gObj[IndexTru[b]].GuildNumber != gObj[n].GuildNumber)
				{
					pMsg.Tru[b] = 2;
				}
				else
				{
					pMsg.Tru[b] = 0;
				}

				if (IndexCong[b] != -1 && !gObj[IndexCong[b]].Live)
				{
					pMsg.Cong[b] = 1;
				}
				else
				{
					pMsg.Cong[b] = 0;
				}
			}

			if (strlen(gObj[IndexTru[0]].GuildName) > 1)
			{
				memcpy(pMsg.GNameTru1, gObj[IndexTru[0]].GuildName, 9);
			}

			if (strlen(gObj[IndexTru[1]].GuildName) > 1)
			{
				memcpy(pMsg.GNameTru2, gObj[IndexTru[1]].GuildName, 9);
			}

			if (strlen(gObj[IndexTru[2]].GuildName) > 1)
			{
				memcpy(pMsg.GNameTru3, gObj[IndexTru[2]].GuildName, 9);
			}

			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void SendInfoCTCMini(int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	GUILD_INFO_STRUCT* lpGuildInfo = gGuildClass.SearchGuild(gCastleStartGuild.WinnerNameOLD);

	INFOCTCMINI_SENDCLIENT pMsg{};
	pMsg.header.set(0xF3, 0x42, sizeof(pMsg));
	pMsg.OpenWindow = 1;
	pMsg.GuildWinOld = 0;

	pMsg.ValueCoin[0] = gCastleStartGuild.YeuCauCoin[0];
	pMsg.ValueCoin[1] = gCastleStartGuild.YeuCauCoin[1];
	pMsg.ValueCoin[2] = gCastleStartGuild.YeuCauCoin[2];

	memcpy(pMsg.GuildNameOld, gCastleStartGuild.WinnerNameOLD, sizeof(pMsg.GuildNameOld));

	if (lpGuildInfo != 0)
	{
		if (lpGuildInfo->Number == lpObj->GuildNumber)
		{
			pMsg.GuildWinOld = 1;
		}
	}

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

bool CastleStartGuild::Dialog(int aIndex, int aNpcIndex)
{
	if (!this->m_Enabled)
	{
		return false;
	}

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpUser = &gObj[aIndex];
	LPOBJ lpNpc = &gObj[aNpcIndex];

	if (lpNpc->Class == this->Class &&
		lpNpc->Map == this->Map &&
		lpNpc->X == this->X &&
		lpNpc->Y == this->Y)
	{
		SendInfoCTCMini(aIndex);
		return true;
	}

	return false;
}

void CastleStartGuild::AddGuild(int gIndex)
{
	LPOBJ lpObj = &gObj[gIndex];

	for (int n = 0; n < MAX_CTC_GUILD; n++)
	{
		if (this->Guild[n].Number >= 0)
		{
			continue;
		}

		this->Guild[n].Number = lpObj->GuildNumber;
		this->Guild[n].Owner = 0;
		memcpy(this->Guild[n].GuildOwner, lpObj->GuildName, sizeof(this->Guild[n].GuildOwner));
		return;
	}
}


bool CastleStartGuild::GetGuild(int gIndex)
{
	for (int n = 0; n < MAX_CTC_GUILD; n++)
	{
		if (this->Guild[n].Number == gIndex)
		{
			return 1;
		}
	}
	return 0;
}

void CastleStartGuild::SetGuildTop()
{
	for (int n = 0; n < MAX_CTC_GUILD; n++)
	{
		if (this->Guild[n].Owner == 1)
		{
			this->WinnerNumber = this->Guild[n].Number;
			memcpy(this->WinnerName, this->Guild[n].GuildOwner, sizeof(this->WinnerName));
			WritePrivateProfileStringA("GuildWinOLD", "GuildName", this->WinnerName, "..\\Data\\Event\\CTCMini\\GuildWin.ini");
		}
	}
}


void CastleStartGuild::SetChar(int cIndex, int CongVao) 
{
	if (this->GetChar(cIndex) == 0)
	{
		this->AddChar(cIndex, CongVao);
	}
}

void CastleStartGuild::AddChar(int cIndex, int CongVao) 
{
	for (int n = 0; n < MAX_CTC_CHAR; n++)
	{
		if (this->Char[n].Index >= 0)
		{
			continue;
		}

		this->Char[n].Index = cIndex;
		memcpy(this->Char[n].Name, gObj[cIndex].Name, sizeof(gObj[cIndex].Name));
		this->Char[n].CongVao = CongVao;
		LogAdd(LOG_RED, "Nhân Vật [%s] Vào Cổng [%d]", this->Char[n].Name, CongVao);
		return;
	}
}

int CastleStartGuild::GetCongVao(int cIndex)
{
	for (int n = 0; n < MAX_CTC_CHAR; n++)
	{
		if (this->Char[n].Index == cIndex)
		{
			return this->Char[n].CongVao;
		}
	}
	return 0;
}

bool CastleStartGuild::GetChar(int cIndex)
{
	for (int n = 0; n < MAX_CTC_CHAR; n++)
	{
		if (this->Char[n].Index == cIndex)
		{
			return 1;
		}
	}
	return 0;
}

void CastleStartGuild::CheckChar()
{
	for (int n = 0; n < MAX_CTC_CHAR; n++)
	{
		if (OBJECT_RANGE(this->Char[n].Index) == 0)
		{
			continue;
		}

		if (strcmp(this->Char[n].Name, gObj[this->Char[n].Index].Name) == 0)
		{
			this->Char[n].Reset();
			return;
		}
	}
	return;
}

bool CastleStartGuild::CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget, int Type)
{
	if (Type == 1) // Attack
	{
		if (lpObj->Map == MAP_GUILWAR)
		{
			if (lpObj->GuildNumber == 0 && lpObj->Type == OBJECT_USER)
			{
				gObjMoveGate(lpObj->Index, 17);
				return 1;
			}

			if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
			{
				if (lpObj->GuildNumber == lpTarget->GuildNumber && lpObj->GuildNumber != 0)
				{
					return 1;
				}
			}
		}
	}
	else // View PK
	{
		if (lpObj->Map == MAP_GUILWAR)
		{
			if (lpTarget->GuildNumber != lpObj->GuildNumber)
			{
				return 1;
			}
		}
	}

	return 0;
}

void CastleStartGuild::SetEntranceZone()
{
	for (int i = 0; i < 5; i++)
	{
		for (int x = XoaDuongVao[i][0]; x <= XoaDuongVao[i][2]; x++)
		{
			for (int y = XoaDuongVao[i][1]; y <= XoaDuongVao[i][3]; y++)
			{
				gMap[MAP_GUILWAR].SetAttr(x, y, 16);
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int x = XoaTru[i][0]; x <= XoaTru[i][2]; x++)
		{
			for (int y = XoaTru[i][1]; y <= XoaTru[i][3]; y++)
			{
				gMap[MAP_GUILWAR].SetAttr(x, y, 6);
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int x = XoaCong[i][0]; x <= XoaCong[i][2]; x++)
		{
			for (int y = XoaCong[i][1]; y <= XoaCong[i][3]; y++)
			{
				gMap[MAP_GUILWAR].SetAttr(x, y, 16);
			}
		}
	}

	for (int n = OBJ_STARTUSERINDEX; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Type != OBJECT_USER || gObj[n].Map != MAP_GUILWAR)
		{
			continue;
		}

		for (int i = 0; i < 5; i++)
		{
			GCMapAttrSend(n, 0, 16, 0, 1, (PMSG_MAP_ATTR*)XoaDuongVao[i]);
		}

		for (int i = 0; i < 3; i++)
		{
			GCMapAttrSend(n, 0, 6, 0, 1, (PMSG_MAP_ATTR*)XoaTru[i]);
			GCMapAttrSend(n, 0, 16, 0, 1, (PMSG_MAP_ATTR*)XoaCong[i]);
		}
	}
}


void CastleStartGuild::DelEntranceZone()
{
	for (int i = 0; i < 5; i++)
	{
		for (int x = XoaDuongVao[i][0]; x <= XoaDuongVao[i][2]; x++)
		{
			for (int y = XoaDuongVao[i][1]; y <= XoaDuongVao[i][3]; y++)
			{
				gMap[MAP_GUILWAR].DelAttr(x, y, 16);
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int x = XoaTru[i][0]; x <= XoaTru[i][2]; x++)
		{
			for (int y = XoaTru[i][1]; y <= XoaTru[i][3]; y++)
			{
				gMap[MAP_GUILWAR].DelAttr(x, y, 6);
			}
		}
	}

	for (int n = OBJ_STARTUSERINDEX; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Type != OBJECT_USER || gObj[n].Map != MAP_GUILWAR)
		{
			continue;
		}

		for (int i = 0; i < 5; i++)
		{
			GCMapAttrSend(n, 0, 16, 1, 1, (PMSG_MAP_ATTR*)XoaDuongVao[i]);
		}

		for (int i = 0; i < 3; i++)
		{
			GCMapAttrSend(n, 0, 6, 1, 1, (PMSG_MAP_ATTR*)XoaTru[i]);
		}
	}
}

void CastleStartGuild::XoaTruChiDinh(BYTE n)
{
	for (int x = XoaCong[n][0]; x <= XoaCong[n][2]; x++)
	{
		for (int y = XoaCong[n][1]; y <= XoaCong[n][3]; y++)
		{
			gMap[MAP_GUILWAR].DelAttr(x, y, 6);
		}
	}
	for (int u = OBJ_STARTUSERINDEX; u < MAX_OBJECT; u++)
	{
		if (gObj[u].Type == OBJECT_USER && gObj[u].Map == MAP_GUILWAR)
		{
			GCMapAttrSend(u, 0, 16, 1, 1, (PMSG_MAP_ATTR*)XoaCong[n]);
		}

	}
}

void CastleStartGuild::TestCTCMini()
{
	SetEntranceZone();
	DelEntranceZone();

	for (int n = 0; n < 3; n++)
	{
		IndexCong[n] = gObjAddMonster(MAP_GUILWAR);

		if (OBJECT_RANGE(IndexCong[n]) == 0)
		{
			return;
		}

		LPOBJ lpMonster = &gObj[IndexCong[n]];

		int px = ThemCong[n][0];
		int py = ThemCong[n][1];

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
		lpMonster->Map = MAP_GUILWAR;
		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;

		if (gObjSetMonster(IndexCong[n], this->ClassGate) == 0)
		{
			gObjDel(IndexCong[n]);
			return;
		}
	}

	for (int n = 0; n < 3; n++)
	{
		IndexTru[n] = gObjAddMonster(MAP_GUILWAR);

		if (OBJECT_RANGE(IndexTru[n]) == 0)
		{
			return;
		}

		LPOBJ lpMonster = &gObj[IndexTru[n]];

		int px = ThemTru[n][0];
		int py = ThemTru[n][1];

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
		lpMonster->Map = MAP_GUILWAR;
		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;

		if (gObjSetMonster(IndexTru[n], ClassTru) == 0)
		{
			gObjDel(IndexTru[n]);
			return;
		}
	}
}

void CastleStartGuild::ThemTruGuild(int n, int GuidNumber)
{
	IndexTru[n] = gObjAddMonster(MAP_GUILWAR);

	if (OBJECT_RANGE(IndexTru[n]) == 0)
	{
		return;
	}

	LPOBJ lpMonster = &gObj[IndexTru[n]];
	GUILD_INFO_STRUCT* lpGuildInfo = gGuildClass.SearchGuild_Number(GuidNumber);

	int px = ThemTru[n][0];
	int py = ThemTru[n][1];

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
	lpMonster->Map = MAP_GUILWAR;

	lpMonster->GuildNumber = GuidNumber;
	memcpy(lpMonster->GuildName, lpGuildInfo->Name, 9); // Set guild name (MAX_GUILD_NAME = 9)

	lpMonster->MonsterDeleteTime = GetTickCount() + 1800000; // Auto-delete after 30 minutes

	if (gObjSetMonster(IndexTru[n], ClassTru) == 0)
	{
		gObjDel(IndexTru[n]);
		return;
	}
}

int CastleStartGuild::SoTruDaChiem(int GuildNumber, int aIndex)
{
	int TruDaChiem = 0;

	for (int n = 0; n < 3; n++) // Count controlled towers
	{
		if (gObj[IndexTru[n]].GuildNumber == GuildNumber)
		{
			TruDaChiem++;
		}
	}

	// If guild not registered, add it
	if (this->GetGuild(GuildNumber) == 0)
	{
		this->AddGuild(aIndex);
	}

	if (TruDaChiem >= 2) // If 2 or more towers are controlled
	{
		for (int n = 0; n < MAX_CTC_GUILD; n++)
		{
			if (Guild[n].Number == GuildNumber)
			{
				this->Guild[n].Owner = 1;

				GCServerMsgStringSendToMap(
					MAP_GUILWAR,
					0,
					"[Công Thành chiến Mini] Guild %s đã chiếm đóng thành công 2 trụ!",
					this->Guild[n].GuildOwner
				);
			}
			else
			{
				this->Guild[n].Owner = 0;
			}
		}
	}

	return TruDaChiem;
}

bool CastleStartGuild::MonsterDie(int aIndexMonster, int aIndexUser)
{
	bool Result = false;

	if (!this->m_Enabled)
	{
		return false;
	}

	LPOBJ lpUser = &gObj[aIndexUser];
	LPOBJ lpMonster = &gObj[aIndexMonster];

	if ((lpMonster->Class == ClassTru || lpMonster->Class == this->ClassGate) && lpMonster->Map == MAP_GUILWAR)
	{
		for (int n = 0; n < 3; n++)
		{
			//==== Handle Tower Destroyed ====
			if (IndexTru[n] == lpMonster->Index)
			{
				gObjDel(IndexTru[n]);
				IndexTru[n] = -1;

				this->ThemTruGuild(n, lpUser->GuildNumber);

				int count = this->SoTruDaChiem(lpUser->GuildNumber, lpUser->Index);

				GCServerMsgStringSendToMap(MAP_GUILWAR, 0, "[Công Thành chiến Mini] Guild %s đã chiếm %d Trụ!", lpUser->GuildName, count);
				LogAdd(LOG_EVENT, "[CTC Mini] Bonus Phá Trụ %s", lpUser->Name);

				GDSetCoinSend(lpUser->Index, +b_PhaTru.WCoin, +b_PhaTru.WCoinP, +b_PhaTru.GobinP, 0, 0, "CTC Mini");
				gCashShop.CGCashShopPointRecv(lpUser->Index);

				if (b_PhaTru.IndexBonus == 1)
				{
					for (int i = OBJECT_START_USER; i < MAX_OBJECT; i++)
					{
						if (gObjIsConnectedGP(i) != 0 && gObj[i].Type == OBJECT_USER && gObj[i].Index != lpUser->Index && gObj[i].Map == MAP_GUILWAR)
						{
							if (strcmp(gObj[i].GuildName, lpUser->Name) == 0)
							{
								LogAdd(LOG_EVENT, "[CTC Mini] Bonus b_PhaTru Member Reward: %s", gObj[i].Name);

								GDSetCoinSend(lpUser->Index, +b_PhaTru.WCoin, +b_PhaTru.WCoinP, +b_PhaTru.GobinP, 0, 0, "CTC Mini");
								gCashShop.CGCashShopPointRecv(gObj[i].Index);
							}
						}
					}
				}

				this->SendKillCTCMini(0, 0);
				Result = true;
			}

			//==== Handle Gate Destroyed ====
			if (IndexCong[n] == lpMonster->Index)
			{
				gObjDel(IndexCong[n]);
				this->XoaTruChiDinh(n);

				GCServerMsgStringSendToMap(MAP_GUILWAR, 0, "[Công Thành chiến Mini] Guild %s đã phá cổng %d!", lpUser->GuildName, n + 1);
				LogAdd(LOG_EVENT, "[CTC Mini] Bonus Phá Cổng %s", lpUser->Name);

				GDSetCoinSend(lpUser->Index, +b_PhaCong.WCoin, +b_PhaCong.WCoinP, +b_PhaCong.GobinP, 0, 0, "CTC Mini");
				gCashShop.CGCashShopPointRecv(lpUser->Index);

				if (b_PhaCong.IndexBonus == 1)
				{
					for (int i = OBJECT_START_USER; i < MAX_OBJECT; i++)
					{
						if (gObjIsConnectedGP(i) != 0 && gObj[i].Type == OBJECT_USER && gObj[i].Index != lpUser->Index && gObj[i].Map == MAP_GUILWAR)
						{
							if (strcmp(gObj[i].GuildName, lpUser->Name) == 0)
							{
								LogAdd(LOG_EVENT, "[CTC Mini] Bonus b_PhaCong Member Reward: %s", gObj[i].Name);

								GDSetCoinSend(lpUser->Index, +b_PhaCong.WCoin, +b_PhaCong.WCoinP, +b_PhaCong.GobinP, 0, 0, "CTC Mini");
								gCashShop.CGCashShopPointRecv(gObj[i].Index);
							}
						}
					}
				}

				this->SendKillCTCMini(0, 0);
				Result = true;
			}
		}
	}

	return Result;
}

bool CastleStartGuild::GetUserRespawnLocation(LPOBJ lpObj, int* gate, int* map, int* x, int* y, int* dir, int* level)
{
	if (this->m_Enabled == 0)
	{
		return false;
	}

	if (this->GetChar(lpObj->Index) == 0)
	{
		return false;
	}

	int CongVao = this->GetCongVao(lpObj->Index);

	// Get gate info for respawn location
	if (gGate.GetGate(this->GateMove[CongVao], gate, map, x, y, dir, level) != 0)
	{
		return true;
	}

	return false;
}

void CastleStartGuild::StartCTCMini()
{
	time_t theTime = time(NULL);
	struct tm* aTime = localtime(&theTime);

	int hour = aTime->tm_hour;
	int minute = aTime->tm_min + 2;

	if (minute >= 60)
	{
		hour++;
		minute -= 60;
	}

	CTC_START_TIME info;

	info.Year = -1; // Any year
	info.Month = -1; // Any month
	info.Day = -1; // Any day
	info.DayOfWeek = -1; // Any day of week
	info.Hour = hour;
	info.Minute = minute;
	info.Second = 0;

	this->m_CTCMinitartTime.push_back(info);

	LogAdd(LOG_EVENT, "[CTC Mini] Bắt Đầu At %02d:%02d:00", hour, minute);

	this->Init(); // Recalculate next event time, state, remain time, etc.
}

