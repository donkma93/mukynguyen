#include "StdAfx.h"
#include "user.h"
#include "ACuoi_Relife.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Path.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"

#include "Notice.h"
#include "CustomRankUser.h"
#include "ObjectManager.h"
#include "ACuoi_JewelBank.h"
#include "CommandManager.h"
#include "ServerInfo.h"
#include "ItemBagManager.h"
#include "ACuoi_MessLang.h"

TaiSinh gTaiSinh;

void TaiSinh::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);
	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}
	pugi::xml_node oTaiSinh = file.child("TaiSinh");
	pugi::xml_node eEnabled = oTaiSinh.child("Enabled");
	for (pugi::xml_node rEnabled = eEnabled.child("Config"); rEnabled; rEnabled = rEnabled.next_sibling()){

		this->m_Enabled		= rEnabled.attribute("BatTat").as_int();
		this->m_TaiSinh		= rEnabled.attribute("TaiSinh").as_int();
		this->ThoiGian		= rEnabled.attribute("ThoiGian").as_int();
		this->RelogChar		= rEnabled.attribute("ChonLaiNv").as_int();
		this->GioiHanRelife = rEnabled.attribute("GioiHan").as_int();
	}
	pugi::xml_node eDieuKienLevel = oTaiSinh.child("DieuKienLevel");
	for (pugi::xml_node rDieuKienLevel = eDieuKienLevel.child("Config"); rDieuKienLevel; rDieuKienLevel = rDieuKienLevel.next_sibling()){

		this->m_DieuKienLevel		= rDieuKienLevel.attribute("LevelReq").as_int();
		this->m_AfterRelifeLevel	= rDieuKienLevel.attribute("AfterRelifeLevel").as_int();
		this->m_AfterRelifeReset	= rDieuKienLevel.attribute("AfterRelifeReset").as_int();
	}
	pugi::xml_node eDieuKienReset = oTaiSinh.child("DieuKienResets");
	for (pugi::xml_node rDieuKienReset = eDieuKienReset.child("Config"); rDieuKienReset; rDieuKienReset = rDieuKienReset.next_sibling()){

		char MaxChar[20];
		for (int i = 0; i < MAXSIZERL; ++i)
		{
			sprintf_s(MaxChar, "Lan%d", i);
			this->Spk_DieuKien[i] = rDieuKienReset.attribute(MaxChar).as_int();
		}
	}
	pugi::xml_node eDiemPoinTaiSinh = oTaiSinh.child("DiemTaiSinh");
	for (pugi::xml_node rDiemPoinTaiSinh = eDiemPoinTaiSinh.child("Config"); rDiemPoinTaiSinh; rDiemPoinTaiSinh = rDiemPoinTaiSinh.next_sibling())
	{
		char MaxChar[20];
		for (int i = 0; i < MAXSIZERL; ++i)
		{
			sprintf_s(MaxChar, "Lan%d", i);
			this->Spk_DiemPoin[i] = rDiemPoinTaiSinh.attribute(MaxChar).as_int();
		}
	}
	
	pugi::xml_node eDieuKienNgoc = oTaiSinh.child("DieuKienNgoc");
	for (pugi::xml_node rDieuKienNgoc = eDieuKienNgoc.child("Config"); rDieuKienNgoc; rDieuKienNgoc = rDieuKienNgoc.next_sibling())
	{
		for (int i = 0; i < MAXSIZERL; ++i)
		{
			sprintf_s(this->MaxCharChaos, "SlotChaos_%d",i);
			this->g_DieuKienJw[i].CheckEventJw[0] = rDieuKienNgoc.attribute(this->MaxCharChaos).as_int();

			sprintf_s(this->MaxCharBless, "SlotBless_%d",i);
			this->g_DieuKienJw[i].CheckEventJw[1] = rDieuKienNgoc.attribute(this->MaxCharBless).as_int();

			sprintf_s(this->MaxCharSoul, "SlotSoul_%d",i);
			this->g_DieuKienJw[i].CheckEventJw[2] = rDieuKienNgoc.attribute(this->MaxCharSoul).as_int();

			sprintf_s(this->MaxCharLife, "SlotLife_%d",i);
			this->g_DieuKienJw[i].CheckEventJw[3] = rDieuKienNgoc.attribute(this->MaxCharLife).as_int();

			sprintf_s(this->MaxCharCrea, "SlotCre_%d",i);
			this->g_DieuKienJw[i].CheckEventJw[4] = rDieuKienNgoc.attribute(this->MaxCharCrea).as_int();
		}
	}
}

void TaiSinh::GetProcRelife(LPOBJ lpObj, RelifeRequest* aRecv, BYTE CheckDk)
{
	if (aRecv->Number == CheckDk + 1)
	{
		if (this->m_Enabled == 0)
		{
			return;
		}
		if (lpObj->MasterReset >= this->GioiHanRelife)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(30), lpObj->MasterReset, this->GioiHanRelife);
			return;
		}
		if (lpObj->Level < this->m_DieuKienLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(31), this->m_DieuKienLevel);
			return;
		}
		if (lpObj->Reset < this->Spk_DieuKien[CheckDk])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(32), this->Spk_DieuKien[CheckDk]);
			return;
		}
		if (lpObj->ItemBank[7] < this->g_DieuKienJw[CheckDk].CheckEventJw[0])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(33), this->g_DieuKienJw[CheckDk].CheckEventJw[0]);
			return;
		}
		if (lpObj->ItemBank[0] < this->g_DieuKienJw[CheckDk].CheckEventJw[1])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(34), this->g_DieuKienJw[CheckDk].CheckEventJw[1]);
			return;
		}
		if (lpObj->ItemBank[1] < this->g_DieuKienJw[CheckDk].CheckEventJw[2])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(35), this->g_DieuKienJw[CheckDk].CheckEventJw[2]);
			return;
		}
		if (lpObj->ItemBank[2] < this->g_DieuKienJw[CheckDk].CheckEventJw[3])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(36), this->g_DieuKienJw[CheckDk].CheckEventJw[3]);
			return;
		}
		if (lpObj->ItemBank[3] < this->g_DieuKienJw[CheckDk].CheckEventJw[4])
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(37), this->g_DieuKienJw[CheckDk].CheckEventJw[4]);
			return;
		}
		if ((GetTickCount() - lpObj->ClickClientSend) < this->ThoiGian * 1000)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(38), this->ThoiGian);
			return;
		}
		if (lpObj->Reset >= this->Spk_DieuKien[CheckDk])
		{
			lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
			lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
			lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
			lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
			lpObj->Leadership = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);

			lpObj->Level = this->m_AfterRelifeLevel;

			if (lpObj->Reset > this->Spk_DieuKien[CheckDk])
			{
				lpObj->Reset = lpObj->Reset - this->Spk_DieuKien[CheckDk];
				if (lpObj->Reset)
				{
					lpObj->LevelUpPoint = this->Spk_DiemPoin[CheckDk] + (lpObj->Reset * this->m_AfterRelifeUpPoint);
				}
			}
			else
			{
				lpObj->Reset = this->m_AfterRelifeReset;
				lpObj->LevelUpPoint = this->Spk_DiemPoin[CheckDk];
			}
			lpObj->MasterReset = lpObj->MasterReset + 1;
			gItemBagManager.DropItemBySpecialValue(ITEM_RELIFE + CheckDk, lpObj, lpObj->Map, lpObj->X, lpObj->Y);

			gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 7, this->g_DieuKienJw[CheckDk].CheckEventJw[0]);
			gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 0, this->g_DieuKienJw[CheckDk].CheckEventJw[1]);
			gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 1, this->g_DieuKienJw[CheckDk].CheckEventJw[2]);
			gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 2, this->g_DieuKienJw[CheckDk].CheckEventJw[3]);
			gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, 3, this->g_DieuKienJw[CheckDk].CheckEventJw[4]);

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(39), lpObj->Name, CheckDk + 1);

			if (this->RelogChar)
			{
				lpObj->CloseCount = 1;
				lpObj->CloseType = 1;
			}

			GDResetInfoSaveSend(lpObj->Index, 0, 0, 0);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
			GCNewCharacterInfoSend(lpObj);
			GDCharacterInfoSaveSend(lpObj->Index);
		}
		lpObj->ClickClientSend = GetTickCount();
	}
}
void TaiSinh::RunRelifeToProc(LPOBJ lpObj, RelifeRequest *aRecv)
{
	for (int X = 0; X < MAXSIZERL; X++)
	{
		this->GetProcRelife(lpObj, aRecv, X);
	}
	if (aRecv->Number == 100)
	{
		this->TaiSinhCheckDataSend(lpObj->Index);
		this->TaiSinhDiemDataSend(lpObj->Index);
		this->DieuKienNgoc01DataSend(lpObj->Index);
	}
}

struct CheckRelifeUpdate
{
	PBMSG_HEAD3 h;
	int m_TaiSinh;
	int m_DieuKienLevel;
	int CheckDieuKien[MAXSIZERL];
	int ResetLife;
};

void TaiSinh::TaiSinhCheckDataSend(int aIndex)
{
	CheckRelifeUpdate rSend{};
	LPOBJ lpObj = &gObj[aIndex];
	rSend.h.set((LPBYTE)&rSend, 0xF3, 0x6B, sizeof(rSend));
	rSend.m_TaiSinh = gTaiSinh.m_TaiSinh;
	rSend.m_DieuKienLevel = gTaiSinh.m_DieuKienLevel;

	for (int i = 0; i < MAXSIZERL; i++)
	{
		rSend.CheckDieuKien[i] = gTaiSinh.Spk_DieuKien[i];
	}

	rSend.ResetLife = lpObj->MasterReset;
	DataSend(aIndex, (LPBYTE)&rSend, sizeof(rSend));
}

struct CheckUpPointUpdate
{
	PBMSG_HEAD3 h;
	int DiemPointTaiSinh[MAXSIZERL];
};

void TaiSinh::TaiSinhDiemDataSend(int aIndex)
{
	CheckUpPointUpdate rSend{};
	LPOBJ lpObj = &gObj[aIndex];
	rSend.h.set((LPBYTE)&rSend, 0xF3, 0x6C, sizeof(rSend));

	for (int i = 0; i < MAXSIZERL; i++)
	{
		rSend.DiemPointTaiSinh[i] = gTaiSinh.Spk_DiemPoin[i];
	}
	DataSend(aIndex, (LPBYTE)&rSend, sizeof(rSend));
}

struct CheckJewelRelifeUpdate
{
	PBMSG_HEAD3 h;
	int m_DieuKienJw[MAXSIZERL][MAXSUBSIZE];
};

void TaiSinh::DieuKienNgoc01DataSend(int aIndex)
{
	CheckJewelRelifeUpdate rSend{};
	LPOBJ lpObj = &gObj[aIndex];
	rSend.h.set((LPBYTE)&rSend, 0xF3, 0x6D, sizeof(rSend));

	for (int i = 0; i < MAXSIZERL; i++)
	{
		rSend.m_DieuKienJw[i][0] = gTaiSinh.g_DieuKienJw[i].CheckEventJw[0];
		rSend.m_DieuKienJw[i][1] = gTaiSinh.g_DieuKienJw[i].CheckEventJw[1];
		rSend.m_DieuKienJw[i][2] = gTaiSinh.g_DieuKienJw[i].CheckEventJw[2];
		rSend.m_DieuKienJw[i][3] = gTaiSinh.g_DieuKienJw[i].CheckEventJw[3];
		rSend.m_DieuKienJw[i][4] = gTaiSinh.g_DieuKienJw[i].CheckEventJw[4];
	}

	DataSend(aIndex, (LPBYTE)&rSend, sizeof(rSend));
}
