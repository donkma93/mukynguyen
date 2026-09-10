#include "stdafx.h"
#include "ACuoi_QuanHam.h"
#include "DSProtocol.h"
#include "Notice.h"
#include "Util.h"
#include "ObjectManager.h"
#include "CustomRankUser.h"
#include "ACuoi_JewelBank.h"

cQuanHam gQuanHam;

cQuanHam::cQuanHam()
{
	this->Enable = false;
	this->ThongBao = false;
	this->m_LoadData.clear();
}

void cQuanHam::LoadConfig(char* FilePath)
{
	this->m_LoadData.clear();
	this->Enable = false;
	this->ThongBao = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FilePath);
	if (res.status != pugi::status_ok) {
		ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
		return;
	}

	pugi::xml_node oQuanHam = file.child("QuanHam");

	this->Enable = oQuanHam.attribute("Enable").as_int();
	this->ThongBao = oQuanHam.attribute("ThongBao").as_int();
	this->ThoiGian = oQuanHam.attribute("ThoiGian").as_int();
	this->GioiHan = oQuanHam.attribute("GioiHan").as_int();

	pugi::xml_node Message = oQuanHam.child("Message");
	for (pugi::xml_node msg = Message.child("Msg"); msg; msg = msg.next_sibling())
	{
		GetMessageQuanHam info;
		info.Index = msg.attribute("Index").as_int();
		strcpy_s(info.Message, msg.attribute("Text").as_string());
		this->m_ReadMessage.insert(std::pair<int, GetMessageQuanHam>(info.Index, info));
	}
	pugi::xml_node ConfigQuanHam = oQuanHam.child("ConfigQuanHam");
	for (pugi::xml_node CapDo = ConfigQuanHam.child("CapDo"); CapDo; CapDo = CapDo.next_sibling())
	{
		GetDataQuanHam SaveData;
		SaveData.AllData[0] = CapDo.attribute("LvQuanHam").as_int();
		//SaveData.AllData[1] = CapDo.attribute("YCItemSL").as_int();
		//SaveData.AllData[2] = CapDo.attribute("YCItemID").as_int();
		SaveData.AllData[3] = CapDo.attribute("WcoinC").as_int();
		//SaveData.AllData[4] = CapDo.attribute("YCWP").as_int();
		//SaveData.AllData[5] = CapDo.attribute("YCGP").as_int();
		SaveData.AllData[6] = CapDo.attribute("TangMau").as_int();
		SaveData.AllData[7] = CapDo.attribute("TangSD").as_int();
		SaveData.AllData[8] = CapDo.attribute("TangST").as_int();
		SaveData.AllData[9] = CapDo.attribute("TangPT").as_int();
		SaveData.AllData[10] = this->GioiHan;
		this->m_LoadData.insert(std::pair<int, GetDataQuanHam>(SaveData.AllData[0], SaveData));
	}
}

char* cQuanHam::GetMessage(int index)
{
	std::map<int, GetMessageQuanHam>::iterator it = this->m_ReadMessage.find(index);
	if (it == this->m_ReadMessage.end())
	{
		char Error[256];
		wsprintf(Error, "Could not find message %d!", index);
		return Error;
	}
	else
	{
		return it->second.Message;
	}
}

GetDataQuanHam* cQuanHam::ReadConfig(int LvQuanHam)
{
	std::map<int, GetDataQuanHam>::iterator it = this->m_LoadData.find(LvQuanHam);
	if (it == this->m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}
void cQuanHam::SendInfoClient(int aIndex)
{
	if (!this->Enable)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(0));
		return;
	}
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}
	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	BYTE send[4096];
	CountListQuanHam pMsg{};
	pMsg.header.set(0xF3, 0x72, 0);

	int size = sizeof(pMsg);
	pMsg.Count = 0;

	for (int i = 0; i < this->m_LoadData.size(); i++)
	{
		if (size + sizeof(this->m_LoadData[i]) > 4096)
		{
			LogAdd(LOG_RED, "(%s %d) Data qua dai !!", __FILE__, __LINE__);
			return;
		}
		memcpy(&send[size], &this->m_LoadData[i], sizeof(this->m_LoadData[i]));
		size += sizeof(this->m_LoadData[i]);

		pMsg.Count++;
	}
	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}

void cQuanHam::AddOption(LPOBJ lpObj, bool flag)
{
	if (flag != 0 || !this->Enable || OBJECT_RANGE(lpObj->Index) == 0 || gObjIsConnected(lpObj->Index) == false)
	{
		return;
	}
	GetDataQuanHam* AddValue = this->ReadConfig(lpObj->rQuanHam);
	if (AddValue <= 0)
	{
		return;
	}
	lpObj->MagicDamageMin += AddValue->AllData[8];
	lpObj->MagicDamageMax += AddValue->AllData[8];
	lpObj->PhysiDamageMinLeft += AddValue->AllData[8];
	lpObj->PhysiDamageMinRight += AddValue->AllData[8];
	lpObj->PhysiDamageMaxLeft += AddValue->AllData[8];
	lpObj->PhysiDamageMaxRight += AddValue->AllData[8];

	lpObj->Defense += AddValue->AllData[9];
	lpObj->AddLife += AddValue->AllData[6];
	lpObj->AddShield += AddValue->AllData[7];
}
bool cQuanHam::RequestAccess(int aIndex)
{
	if (!this->Enable)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(0));
		return 0;
	}
	if (OBJECT_RANGE(aIndex) == 0 || gObjIsConnected(aIndex) == false)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];
	GetDataQuanHam* pGetDataQuanHam = this->ReadConfig(lpObj->rQuanHam + 1);

	if (!pGetDataQuanHam)
	{
		return 0;
	}

	int CheckWC = pGetDataQuanHam->AllData[3];
	//int CheckWP = pGetDataQuanHam->AllData[4];
	//int CheckGP = pGetDataQuanHam->AllData[5];

	if (CheckWC > lpObj->ThisCoin[0])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(2), CheckWC, "WC");
		return false;
	}
	//if (CheckWP > lpObj->ThisCoin[1])
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(2), CheckWP, "WP");
	//	return false;
	//}
	//if (CheckGP > lpObj->ThisCoin[2])
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(2), CheckGP, "GP");
	//	return false;
	//}
	if ((GetTickCount() - lpObj->ClickClientSend) < this->ThoiGian * 1000)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(6), this->ThoiGian);
		return false;
	}
	if (lpObj->rQuanHam == this->GioiHan)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(5), this->GioiHan);
		return false;
	}

	//if (lpObj->ItemBank[pGetDataQuanHam->AllData[2]] <= 0 || lpObj->ItemBank[pGetDataQuanHam->AllData[2]] < pGetDataQuanHam->AllData[1])
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(7));
	//	return false;
	//}
	// 
	//gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, pGetDataQuanHam->AllData[2], pGetDataQuanHam->AllData[1]);

	//if (CheckWC > 0 || CheckWP > 0 || CheckGP > 0)
	//{
	//	GDSetCoinSend(lpObj->Index, (CheckWC > 0 ? -CheckWC : 0), (CheckWP > 0 ? -CheckWP : 0), (CheckGP > 0 ? -CheckGP : 0), 0, 0, "rQuanHam");
	//}

	if (CheckWC > 0)
	{
		GDSetCoinSend(lpObj->Index, -CheckWC, 0, 0, 0, 0, "rQuanHam");
	}

	lpObj->rQuanHam++;

	if (this->ThongBao)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, this->GetMessage(4), lpObj->Name, lpObj->rQuanHam);
	}

	gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(3), lpObj->rQuanHam);

	gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GDCharacterInfoSaveSend(lpObj->Index);
	lpObj->ClickClientSend = GetTickCount();

	return 1;
}
