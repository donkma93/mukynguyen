#include "stdafx.h"
#include "ACuoi_HonHoan.h"
#include "DSProtocol.h"
#include "Notice.h"
#include "Util.h"
#include "ObjectManager.h"
#include "CustomRankUser.h"
#include "ACuoi_JewelBank.h"

cHonHoan gHonHoan;

cHonHoan::cHonHoan()
{
	this->Enable = false;
	this->ThongBao = false;
	this->m_LoadData.clear();
}

void cHonHoan::LoadConfig(char* FilePath)
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

	pugi::xml_node oHonHoan = file.child("HonHoan");

	this->Enable = oHonHoan.attribute("Enable").as_int();
	this->ThongBao = oHonHoan.attribute("Enable").as_int();
	this->ThoiGian = oHonHoan.attribute("ThoiGian").as_int();
	this->GioiHan = oHonHoan.attribute("GioiHan").as_int();

	pugi::xml_node Message = oHonHoan.child("Message");
	for (pugi::xml_node msg = Message.child("Msg"); msg; msg = msg.next_sibling())
	{
		GetMessageHonHoan info;
		info.Index = msg.attribute("Index").as_int();
		strcpy_s(info.Message, msg.attribute("Text").as_string());
		this->m_ReadMessage.insert(std::pair<int, GetMessageHonHoan>(info.Index, info));
	}
	pugi::xml_node ConfigHonHoan = oHonHoan.child("ConfigHonHoan");
	for (pugi::xml_node CapDo = ConfigHonHoan.child("CapDo"); CapDo; CapDo = CapDo.next_sibling())
	{
		GetDataHonHoan SaveData;
		SaveData.AllData[0] = CapDo.attribute("LvHonHoan").as_int();
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
		this->m_LoadData.insert(std::pair<int, GetDataHonHoan>(SaveData.AllData[0], SaveData));
	}
}

char* cHonHoan::GetMessage(int index)
{
	std::map<int, GetMessageHonHoan>::iterator it = this->m_ReadMessage.find(index);
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

GetDataHonHoan* cHonHoan::ReadConfig(int LvHonHoan)
{
	std::map<int, GetDataHonHoan>::iterator it = this->m_LoadData.find(LvHonHoan);
	if (it == this->m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}
void cHonHoan::SendInfoClient(int aIndex)
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
	CountListHonHoan pMsg{};
	pMsg.header.set(0xF3, 0x71, 0);

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

void cHonHoan::AddOption(LPOBJ lpObj, bool flag)
{
	if (flag != 0 || !this->Enable || OBJECT_RANGE(lpObj->Index) == 0 || gObjIsConnected(lpObj->Index) == false)
	{
		return;
	}
	GetDataHonHoan* AddValue = this->ReadConfig(lpObj->rHonHoan);
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
bool cHonHoan::RequestAccess(int aIndex)
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
	GetDataHonHoan* pGetDataHonHoan = this->ReadConfig(lpObj->rHonHoan + 1);

	if (!pGetDataHonHoan)
	{
		return 0;
	}

	int CheckWC = pGetDataHonHoan->AllData[3];
	//int CheckWP = pGetDataHonHoan->AllData[4];
	//int CheckGP = pGetDataHonHoan->AllData[5];

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
	if (lpObj->rHonHoan == this->GioiHan)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(5), this->GioiHan);
		return false;
	}

	//if (lpObj->ItemBank[pGetDataHonHoan->AllData[2]] <= 0 || lpObj->ItemBank[pGetDataHonHoan->AllData[2]] < pGetDataHonHoan->AllData[1])
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(7));
	//	return false;
	//}
	//
	//gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, pGetDataHonHoan->AllData[2], pGetDataHonHoan->AllData[1]);

	//if (CheckWC > 0 || CheckWP > 0 || CheckGP > 0)
	//{
	//	GDSetCoinSend(lpObj->Index, (CheckWC > 0 ? -CheckWC : 0), (CheckWP > 0 ? -CheckWP : 0), (CheckGP > 0 ? -CheckGP : 0), 0, 0, "rHonHoan");
	//}
	if (CheckWC > 0)
	{
		GDSetCoinSend(lpObj->Index, -CheckWC, 0, 0, 0, 0, "rHonHoan");
	}

	lpObj->rHonHoan++;

	if (this->ThongBao)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, this->GetMessage(4), lpObj->Name, lpObj->rHonHoan);
	}

	gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(3), lpObj->rHonHoan);

	gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GDCharacterInfoSaveSend(lpObj->Index);
	lpObj->ClickClientSend = GetTickCount();

	return 1;
}
