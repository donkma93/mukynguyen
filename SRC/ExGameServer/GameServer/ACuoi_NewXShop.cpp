#include "StdAfx.h"
#include "ACuoi_NewXShop.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Notice.h"
#include "Util.h"
#include "ItemOptionRate.h"
#include "ObjectManager.h"
#include "Guild.h"
#include "Move.h"
#include "Monster.h"
#include "ItemBagManager.h"
#include "Party.h"
#include "CashShop.h"
#include "MapServerManager.h"
#include "ServerInfo.h"
#include "ItemLevel.h"
NewCashShop gNewCashShop;

void NewCashShop::GsSendPackSize11(BYTE* lpMsg, int aIndex)
{
	XULY_CGPACKET_SSHOP* mlpMsg = (XULY_CGPACKET_SSHOP*)lpMsg;
	if (!mlpMsg->Shop) { return; }
	gNewCashShop.MuaItem(aIndex, mlpMsg->Shop, mlpMsg->ViTri);
}
void NewCashShop::GsSendPackSize12(BYTE* lpMsg, int aIndex)
{
	XULY_XSHOP* mlpMsg = (XULY_XSHOP*)lpMsg;
	if (!mlpMsg->ThaoTac) { return; }
	gNewCashShop.SendListShop(aIndex, mlpMsg->ThaoTac);
}
void NewCashShop::GsSendPackSize13(BYTE* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	XULY_XSHOP* mlpMsg = (XULY_XSHOP*)lpMsg;
	if (!mlpMsg->ThaoTac) { return; }
	gNewCashShop.UserSendClientInfo(lpObj->Index);
}
void NewCashShop::LoadFileXML(char* FilePath)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FilePath);
	if (res.status != pugi::status_ok) {
		ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
		return;
	}
	pugi::xml_node oCashShop = file.child("CustomCashShop");
	this->Enable = oCashShop.attribute("Enable").as_int();
	this->Firework = oCashShop.attribute("Firework").as_int();
	this->Notice = oCashShop.attribute("Notice").as_int();

	this->m_MessageInfoBP.clear();
	pugi::xml_node Message = oCashShop.child("MessageInfo");
	for (pugi::xml_node msg = Message.child("Message"); msg; msg = msg.next_sibling())
	{
		MESSAGE_INFO_CASHSHOP info;

		info.Index = msg.attribute("Index").as_int();

		strcpy_s(info.Message, msg.attribute("Text").as_string());

		this->m_MessageInfoBP.insert(std::pair<int, MESSAGE_INFO_CASHSHOP>(info.Index, info));
	}
	this->m_DataCashShop.clear();

	pugi::xml_node ConfigShop = oCashShop.child("ConfigShop");
	int IndexMocNap = 1;
	for (pugi::xml_node DanhMuc = ConfigShop.child("DanhMuc"); DanhMuc; DanhMuc = DanhMuc.next_sibling())
	{
		DATA_CASHSHOP infoData = { 0 };
		infoData.IndexShop = IndexMocNap++;
		strncpy_s(infoData.NameDanhMuc, DanhMuc.attribute("NameShop").as_string(), sizeof(infoData.NameDanhMuc));
		//===ItemNhan
		infoData.ListItemBan.clear();
		pugi::xml_node ItemBan = DanhMuc.child("ItemBan");
		for (pugi::xml_node Item = ItemBan.child("Item"); Item; Item = Item.next_sibling())
		{
			DATA_ITEMCASHSHOP ListItemInfo;
			ListItemInfo.SizeBMD	= Item.attribute("SizeBMD").as_float();
			ListItemInfo.PriceType	= Item.attribute("PriceType").as_int();
			ListItemInfo.Price		= Item.attribute("Price").as_int();
			ListItemInfo.ItemType	= Item.attribute("Type").as_int();
			ListItemInfo.IndexItem	= SafeGetItem(GET_ITEM(Item.attribute("Type").as_int(), Item.attribute("Index").as_int()));
			ListItemInfo.LvItem		= Item.attribute("LvItem").as_int();
			ListItemInfo.Dur		= Item.attribute("Dur").as_int();
			ListItemInfo.Skill		= Item.attribute("Skill").as_int();
			ListItemInfo.Luck		= Item.attribute("Luck").as_int();
			ListItemInfo.Opt		= Item.attribute("Opt").as_int();
			ListItemInfo.Exc		= Item.attribute("Exc").as_int();
			ListItemInfo.Anc		= Item.attribute("Anc").as_int();

			ListItemInfo.SK[0] = Item.attribute("SK1").as_int();
			ListItemInfo.SK[1] = Item.attribute("SK2").as_int();
			ListItemInfo.SK[2] = Item.attribute("SK3").as_int();
			ListItemInfo.SK[3] = Item.attribute("SK4").as_int();
			ListItemInfo.SK[4] = Item.attribute("SK5").as_int();

			ListItemInfo.SKBonus = Item.attribute("SKBonus").as_int();
			ListItemInfo.HSD = Item.attribute("HSD").as_int();

			ListItemInfo.Class[0] = Item.attribute("DW").as_int();
			ListItemInfo.Class[1] = Item.attribute("DK").as_int();
			ListItemInfo.Class[2] = Item.attribute("ELF").as_int();
			ListItemInfo.Class[3] = Item.attribute("MG").as_int();
			ListItemInfo.Class[4] = Item.attribute("DL").as_int();
			ListItemInfo.Class[5] = Item.attribute("SUM").as_int();
			ListItemInfo.Class[6] = Item.attribute("RF").as_int();
			infoData.ListItemBan.push_back(ListItemInfo);
		}
		this->m_DataCashShop.insert(std::pair<int, DATA_CASHSHOP>(infoData.IndexShop, infoData));
	}
}
char* NewCashShop::GetMessage(int index) // OK
{
	std::map<int, MESSAGE_INFO_CASHSHOP>::iterator it = this->m_MessageInfoBP.find(index);

	if (it == this->m_MessageInfoBP.end())
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
void NewCashShop::UserSendClientInfo(int aIndex) //Send Danh Sach Moc Nap Ve Client
{
	if (gObj[aIndex].Type != OBJECT_USER)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	if (gObj[aIndex].IsBot >= 1 || gObj[aIndex].m_OfflineMode != 0 || gObj[aIndex].IsFakeOnline != 0)
	{
		return;
	}
	BYTE send[4096];
	PMSG_CASHSHOP_SEND pMsg = { 0 };
	pMsg.header.set(0xD5, 0x11, 0);
	int size = sizeof(pMsg);
	pMsg.count = 0;

	for (std::map<int, DATA_CASHSHOP>::iterator it = this->m_DataCashShop.begin(); it != this->m_DataCashShop.end(); it++)
	{
		if (it == this->m_DataCashShop.end())
		{
			break;
		}
		ListShopSend info;
		info.IndexShop = it->second.IndexShop;;
		memset(info.Name, 0, sizeof(info.Name));
		memcpy(info.Name, it->second.NameDanhMuc, sizeof(info.Name));
		if ((size + sizeof(info) >= 4096))
		{
			break;
		}
		pMsg.count++;
		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
	}
	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}

void CASHSHOP_ItemByteConvert(BYTE* lpMsg, DATA_ITEMCASHSHOP* Data) // OK
{
	lpMsg[0] = Data->IndexItem & 0xFF;

	lpMsg[1] = 0;
	lpMsg[1] |= Data->LvItem * 8;
	lpMsg[1] |= Data->Skill * 128;
	lpMsg[1] |= Data->Luck * 4;
	lpMsg[1] |= Data->Opt & 3;

	lpMsg[2] = Data->Dur;

	lpMsg[3] = 0;
	lpMsg[3] |= (Data->IndexItem & 0x100) >> 1;
	lpMsg[3] |= ((Data->Opt > 3) ? 0x40 : 0);
	lpMsg[3] |= Data->Exc;

	lpMsg[4] = Data->Anc;

	lpMsg[5] = 0;
	lpMsg[5] |= (Data->IndexItem & 0x1E00) >> 5;
	lpMsg[5] |= ((Data->Exc & 0x80) >> 4);
	lpMsg[5] |= ((Data->HSD & 1) << 2);

	lpMsg[6] = Data->SKBonus;

	lpMsg[7] = Data->SK[0];
	lpMsg[8] = Data->SK[1];
	lpMsg[9] = Data->SK[2];
	lpMsg[10] = Data->SK[3];
	lpMsg[11] = Data->SK[4];
}
#include "ItemManager.h"
#include "ZzzToolKit.h"
void NewCashShop::SendListShop(int aIndex, int Shop) 
{
	if (!this->Enable)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(0));
		return;
	}

	if (gObj[aIndex].Type != OBJECT_USER)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	if (gObj[aIndex].IsBot >= 1 || gObj[aIndex].m_OfflineMode != 0 || gObj[aIndex].IsFakeOnline != 0)
	{
		return;
	}
	BYTE send[4096];
	PMSG_LISTITEMSHOP_SEND pMsg = { 0 };
	// ---
	pMsg.header.set(0xD5, 0x12, 0);
	int size = sizeof(pMsg);
	pMsg.count = 0;
	std::map<int, DATA_CASHSHOP>::iterator it = this->m_DataCashShop.find(Shop);
	if (it == this->m_DataCashShop.end())
	{
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(1));
		return;
	}
	for (std::vector<DATA_ITEMCASHSHOP>::iterator itItem = it->second.ListItemBan.begin(); itItem != it->second.ListItemBan.end(); itItem++)
	{
		if (itItem == it->second.ListItemBan.end())
		{
			break;
		}

		LISTITEMCASHSHOP_SENDINFO info;
		info.SizeBMD = itItem->SizeBMD;
		info.PriceType = itItem->PriceType;
		info.Price = itItem->Price;
		info.TypeItem = itItem->ItemType;
		for (int n = 0; n < MAX_CLASS; n++)
		{
			info.Class[n] = gItemManager.CheckItemRequireClassUnick2(n, itItem->IndexItem);
		}
		info.Index = itItem->IndexItem;
		info.Dur = itItem->Dur;
		CASHSHOP_ItemByteConvert(info.Item, &*itItem);
		time_t t = time(NULL);
		localtime(&t);
		DWORD iTime = (DWORD)t + itItem->HSD * 60;
		if ((itItem->HSD) > 0)
		{
			info.PeriodTime = iTime;
		}
		else
		{
			info.PeriodTime = itItem->HSD;
		}
		if ((size + sizeof(info) >= 4096))
		{
			break;
		}
		pMsg.count++;
		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
	}
	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}

char* m_MoneyType[6] =
{
	"Zen",
	"Wcoin",
	"WcoinP",
	"GP",
	"Ruud",
	"ATM",
};

void NewCashShop::MuaItem(int aIndex, int Shop, int ViTri)
{
	if (!this->Enable)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, this->GetMessage(0)); //
		return;
	}

	if (gObj[aIndex].Type != OBJECT_USER)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	if (gObj[aIndex].IsBot >= 1 || gObj[aIndex].m_OfflineMode != 0 || gObj[aIndex].IsFakeOnline != 0)
	{
		return;
	}
	LPOBJ lpObj = &gObj[aIndex];
	if (lpObj->Interface.type != INTERFACE_NONE || lpObj->Interface.use != 0 || lpObj->Transaction == 1 )
	{
		return;
	}

	if (gItemManager.ChaosBoxHasItem(lpObj) || gItemManager.TradeHasItem(lpObj))
	{
		return;
	}
	std::map<int, DATA_CASHSHOP>::iterator it = this->m_DataCashShop.find(Shop);

	if (it == this->m_DataCashShop.end())
	{
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(1)); 
		return;
	}
	if (gItemManager.CheckItemInventorySpace(lpObj, 4, 4) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, this->GetMessage(6));
		return;
	}

	if (it->second.ListItemBan.empty() == 0)
	{
		switch (it->second.ListItemBan[ViTri].PriceType)
		{
			case 0:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->Money)
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			case 1:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->ThisCoin[0])
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			case 2:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->ThisCoin[1])
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			case 3:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->ThisCoin[2])
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			case 4:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->ThisCoin[3])
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			case 5:
			{
				if (it->second.ListItemBan[ViTri].Price > lpObj->ThisCoin[4])
				{
					gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, this->GetMessage(3), IsToolKit.QN(it->second.ListItemBan[ViTri].Price), m_MoneyType[it->second.ListItemBan[ViTri].PriceType]);
					return;
				}
			}
			break;
			default:
			break;
		}

		switch (it->second.ListItemBan[ViTri].PriceType)
		{
			case 0:
			{
				lpObj->Money -= it->second.ListItemBan[ViTri].Price;
				GCMoneySend(lpObj->Index, lpObj->Money);
			}
			break;
			case 1:
			{
				GDSetCoinSend(lpObj->Index, (it->second.ListItemBan[ViTri].Price > 0) ? (-it->second.ListItemBan[ViTri].Price) : 0, 0, 0, 0,0, "Mua Item Shop");
				gCashShop.CGCashShopPointRecv(lpObj->Index);

			}
			break;
			case 2:
			{
				GDSetCoinSend(lpObj->Index, 0, (it->second.ListItemBan[ViTri].Price > 0) ? (-it->second.ListItemBan[ViTri].Price) : 0, 0, 0, 0, "Mua Item Shop");
				gCashShop.CGCashShopPointRecv(lpObj->Index);
			}
			break;
			case 3:
			{
				GDSetCoinSend(lpObj->Index, 0, 0, (it->second.ListItemBan[ViTri].Price > 0) ? (-it->second.ListItemBan[ViTri].Price) : 0,0,0, "Mua Item Shop");
				gCashShop.CGCashShopPointRecv(lpObj->Index);
			}
			break;
			case 4:
			{
				GDSetCoinSend(lpObj->Index, 0, 0, 0, (it->second.ListItemBan[ViTri].Price > 0) ? (-it->second.ListItemBan[ViTri].Price) : 0, 0, "Mua Item Shop");
				gCashShop.CGCashShopPointRecv(lpObj->Index);
			}
			break;
			case 5:
			{
				GDSetCoinSend(lpObj->Index, 0, 0, 0, 0, (it->second.ListItemBan[ViTri].Price > 0) ? (-it->second.ListItemBan[ViTri].Price) : 0, "Mua Item Shop");
				gCashShop.CGCashShopPointRecv(lpObj->Index);
			}
			break;
			default:
			break;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { it->second.ListItemBan[ViTri].SK[0], it->second.ListItemBan[ViTri].SK[1], it->second.ListItemBan[ViTri].SK[2], it->second.ListItemBan[ViTri].SK[3], it->second.ListItemBan[ViTri].SK[4] };
		time_t t = time(NULL);
		localtime(&t);
		DWORD iTime = (DWORD)t + it->second.ListItemBan[ViTri].HSD * 60;
		if (it->second.ListItemBan[ViTri].HSD > 0)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, (BYTE)lpObj->X, (BYTE)lpObj->Y, it->second.ListItemBan[ViTri].IndexItem, it->second.ListItemBan[ViTri].LvItem, it->second.ListItemBan[ViTri].Dur, it->second.ListItemBan[ViTri].Skill, it->second.ListItemBan[ViTri].Luck, it->second.ListItemBan[ViTri].Opt, -1, it->second.ListItemBan[ViTri].Exc, it->second.ListItemBan[ViTri].Anc, 0, 0, ItemSocketOption, it->second.ListItemBan[ViTri].SKBonus, iTime);
		}
		else
		{
			GDCreateItemSend(lpObj->Index, 0xEB, (BYTE)lpObj->X, (BYTE)lpObj->Y, it->second.ListItemBan[ViTri].IndexItem, it->second.ListItemBan[ViTri].LvItem, it->second.ListItemBan[ViTri].Dur, it->second.ListItemBan[ViTri].Skill, it->second.ListItemBan[ViTri].Luck, it->second.ListItemBan[ViTri].Opt, -1, it->second.ListItemBan[ViTri].Exc, it->second.ListItemBan[ViTri].Anc, 0, 0, ItemSocketOption, it->second.ListItemBan[ViTri].SKBonus, 0);
		}
	}
	UserSendClientInfo(aIndex);
	if (this->Firework == 1)
	{
		GCServerCommandSend(lpObj->Index, 0, lpObj->X, lpObj->Y);
	}
	else if (this->Firework == 2)
	{
		GCServerCommandSend(lpObj->Index, 2, lpObj->X, lpObj->Y);
	}
	else if (this->Firework == 3)
	{
		GCServerCommandSend(lpObj->Index, 58, SET_NUMBERHB(lpObj->Index), SET_NUMBERLB(lpObj->Index));
	}
	char tmp[255];
	char tmp2[255];
	wsprintf(tmp, this->GetMessage(5), lpObj->Name, gItemLevel.GetItemName(it->second.ListItemBan[ViTri].IndexItem, it->second.ListItemBan[ViTri].LvItem));
	if (this->Notice == 1) 
	{ 
		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, tmp);
	}
	else if (this->Notice == 2)
	{ 
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, tmp);
	}
	else if (this->Notice == 3)
	{ 
		wsprintf(tmp2, "%s %s", gServerInfo.m_ServerName, tmp);
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, tmp2);
	}
}



