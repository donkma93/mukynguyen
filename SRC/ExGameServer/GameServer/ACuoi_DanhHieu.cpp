#include "stdafx.h"
#include "ACuoi_DanhHieu.h"
#include "DSProtocol.h"
#include "Notice.h"
#include "Util.h"
#include "ObjectManager.h"
#include "CustomRankUser.h"
#include "ACuoi_JewelBank.h"

cDanhHieu gDanhHieu;

cDanhHieu::cDanhHieu()
{
	this->Enable = false;
	this->ThongBao = false;
	this->m_LoadData.clear();
}

void cDanhHieu::LoadConfig(char* FilePath)
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

	pugi::xml_node oDanhHieu = file.child("DanhHieu");

	this->Enable	= oDanhHieu.attribute("Enable").as_int();
	this->ThongBao	= oDanhHieu.attribute("ThongBao").as_int();
	this->ThoiGian	= oDanhHieu.attribute("ThoiGian").as_int();
	this->GioiHan	= oDanhHieu.attribute("GioiHan").as_int();

	pugi::xml_node Message = oDanhHieu.child("Message");
	for (pugi::xml_node msg = Message.child("Msg"); msg; msg = msg.next_sibling())
	{
		GetMessageDanhHieu info;
		info.Index = msg.attribute("Index").as_int();
		strcpy_s(info.Message, msg.attribute("Text").as_string());
		this->m_ReadMessage.insert(std::pair<int, GetMessageDanhHieu>(info.Index, info));
	}
	pugi::xml_node ConfigDanhHieu	= oDanhHieu.child("ConfigDanhHieu");
	for (pugi::xml_node CapDo		= ConfigDanhHieu.child("CapDo"); CapDo; CapDo = CapDo.next_sibling())
	{
		GetDataDanhHieu SaveData;
		SaveData.AllData[0] = CapDo.attribute("LvDanhHieu").as_int();
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
		this->m_LoadData.insert(std::pair<int, GetDataDanhHieu>(SaveData.AllData[0], SaveData));
	}
}

char* cDanhHieu::GetMessage(int index)
{
	std::map<int, GetMessageDanhHieu>::iterator it = this->m_ReadMessage.find(index);
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

GetDataDanhHieu* cDanhHieu::ReadConfig(int LvDanhHieu)
{
	std::map<int, GetDataDanhHieu>::iterator it = this->m_LoadData.find(LvDanhHieu);
	if (it == this->m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}
void cDanhHieu::SendInfoClient(int aIndex)
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
	CountListDanhHieu pMsg{};
	pMsg.header.set(0xF3, 0x70, 0);

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

void cDanhHieu::AddOption(LPOBJ lpObj, bool flag)
{
	if (flag != 0 || !this->Enable || OBJECT_RANGE(lpObj->Index) == 0 || gObjIsConnected(lpObj->Index) == false)
	{
		return;
	}
	GetDataDanhHieu* AddValue = this->ReadConfig(lpObj->rDanhHieu);
	if(AddValue <= 0)
	{
		return;
	}
	lpObj->MagicDamageMin		+= AddValue->AllData[8];
	lpObj->MagicDamageMax		+= AddValue->AllData[8];
	lpObj->PhysiDamageMinLeft	+= AddValue->AllData[8];
	lpObj->PhysiDamageMinRight	+= AddValue->AllData[8];
	lpObj->PhysiDamageMaxLeft	+= AddValue->AllData[8];
	lpObj->PhysiDamageMaxRight	+= AddValue->AllData[8];

	lpObj->Defense				+= AddValue->AllData[9];
	lpObj->AddLife				+= AddValue->AllData[6];
	lpObj->AddShield			+= AddValue->AllData[7];
}
bool cDanhHieu::RequestAccess(int aIndex)
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
	GetDataDanhHieu* pGetDataDanhHieu = this->ReadConfig(lpObj->rDanhHieu + 1);

	if (!pGetDataDanhHieu)
	{
		return 0;
	}

	int CheckWC = pGetDataDanhHieu->AllData[3];
	//int CheckWP = pGetDataDanhHieu->AllData[4];
	//int CheckGP = pGetDataDanhHieu->AllData[5];

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
	if(lpObj->rDanhHieu == this->GioiHan )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(5), this->GioiHan);
		return false;
	}

	//if (lpObj->ItemBank[pGetDataDanhHieu->AllData[2]] <= 0 || lpObj->ItemBank[pGetDataDanhHieu->AllData[2]] < pGetDataDanhHieu->AllData[1])
	//{
	//	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(7));
	//	return false;
	//}
	// 
	//gCustomJewelBank.GDCustomJewelBankDelJewel(lpObj, pGetDataDanhHieu->AllData[2], pGetDataDanhHieu->AllData[1]);

	//if (CheckWC > 0 || CheckWP > 0 || CheckGP > 0)
	//{
	//	GDSetCoinSend(lpObj->Index, (CheckWC > 0 ? -CheckWC : 0), (CheckWP > 0 ? -CheckWP : 0), (CheckGP > 0 ? -CheckGP : 0), 0, 0, "rDanhHieu");
	//}

	if (CheckWC > 0)
	{
		GDSetCoinSend(lpObj->Index, -CheckWC, 0, 0, 0, 0, "rDanhHieu");
	}

	lpObj->rDanhHieu++;

	if (this->ThongBao)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, this->GetMessage(4), lpObj->Name, lpObj->rDanhHieu);
	}

	gNotice.GCNoticeSend(aIndex,1, 0, 0, 0, 0, 0, this->GetMessage(3), lpObj->rDanhHieu);

	gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GDCharacterInfoSaveSend(lpObj->Index);
	lpObj->ClickClientSend = GetTickCount();

	return 1;
}
