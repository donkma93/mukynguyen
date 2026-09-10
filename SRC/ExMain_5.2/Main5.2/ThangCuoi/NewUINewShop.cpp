#include "Stdafx.h"
#include "NewUINewShop.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <Util.h>

int SelectTypeShop;
int ViTriMua;
int SeclectClass;
int m_CurrentPage;

SEASON3B::CNewUINewShop::CNewUINewShop()
{
	m_pNewUIMng = nullptr;
	m_pNewUI3DRenderMng = nullptr;
	m_pNewInventoryCtrl = nullptr;

    this->m_Pos = { 0, 0 };

	WINDOW_WIDTH = 480;
	WINDOW_HEIGHT = 150;

	SelectTypeShop = 1;
	ViTriMua = -1;
	SeclectClass = -1;
	m_CurrentPage = 0;

	DataListShop = 0;
	this->ListItemCashShop.clear();
	this->DanhSachCashShop.clear();
}

SEASON3B::CNewUINewShop::~CNewUINewShop()
{
	Release();
}

bool SEASON3B::CNewUINewShop::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (!pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NEWSHOP, this);
	m_pNewUI3DRenderMng = g_pNewUISystem->GetNewUI3DRenderMng();

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;
	m_pNewInventoryCtrl->Create(
		STORAGE_TYPE::UNDEFINED,
		m_pNewUI3DRenderMng,
		g_pNewItemMng,
		this,
		m_Pos.x + 488,
		m_Pos.y + 200,
		8, 8,
		MAX_EQUIPMENT
	);

	m_pNewUI3DRenderMng->Add3DRenderObj(m_pNewInventoryCtrl, INVENTORY_CAMERA_Z_ORDER);

	CreateCharacterPhoto();
	SetPos(x, y);
	InitButtons();
	Show(false);

	return true;
}


void SEASON3B::CNewUINewShop::CreateCharacterPhoto()
{
	m_PhotoChar.Init(0);
	m_PhotoChar.SetOption(UIPHOTOVIEWER_CANCONTROL);
	m_PhotoChar.SetAngle(100.f);
	m_PhotoChar.SetZoom(0.90f);
	m_PhotoChar.SetAutoupdatePlayer(FALSE);
	m_PhotoChar.SetPosition(m_Pos.x + 490, m_Pos.y + 50);
	m_PhotoChar.SetSize(140, 180);
	m_PhotoChar.CopyPlayer();
	m_PhotoChar.SetAnimation(AT_STAND1);
}

void SEASON3B::CNewUINewShop::UpdateCharacterPhoto()
{
	m_PhotoChar.CopyPlayer();
}

void SEASON3B::CNewUINewShop::Release()
{
	SAFE_DELETE(m_pNewInventoryCtrl);
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUINewShop::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - 640) / 2;
	m_Pos.y = y;

	if (m_pNewInventoryCtrl)
		m_pNewInventoryCtrl->SetPos(m_Pos.x + 478, m_Pos.y + 200);

	m_PhotoChar.SetPosition(m_Pos.x + 490, m_Pos.y + 50);
	m_PhotoChar.SetSize(140, 180);
}

bool SEASON3B::CNewUINewShop::Update()
{
	if (m_pNewInventoryCtrl && m_pNewInventoryCtrl->Update() == false)
		return false;

	if (IsVisible())
	{
		UpdateCharacterPhoto();
	}

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->UpdateMouseEvent();
		m_pNewInventoryCtrl->Update();
	}

	return true;
}

bool SEASON3B::CNewUINewShop::UpdateMouseEvent()
{
	if (IsVisible() == false)
		return true;

	if (m_pNewInventoryCtrl && m_pNewInventoryCtrl->UpdateMouseEvent() == false)
		return false;

	for (int i = 0; i < DataListShop; i++)
	{
		if (m_Btn[i].UpdateMouseEvent())
		{
			PlayBuffer(SOUND_CLICK01);

			SelectTypeShop = this->DanhSachCashShop[i].IndexShop;
			SeclectClass = -1;

			SolvedNewShop pMsg;
			pMsg.header.set(0xD5, 0x12, sizeof(pMsg));
			pMsg.ThaoTac = SelectTypeShop;
			DataSend((LPBYTE)&pMsg, pMsg.header.size);

			return true;
		}
	}

	for (int cnt = 0; cnt < 9; cnt++)
	{
		if (m_Buy[cnt].UpdateMouseEvent())
		{
			int index = m_CurrentPage * 9 + cnt;
			if (index < (int)ListItemCashShop.size())
			{
				ViTriMua = index;
				XacNhanMuaShop(this);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}
		}
	}

	if (m_Btn[19].UpdateMouseEvent())
	{
		if (m_CurrentPage > 0) m_CurrentPage--;
	}

	if (m_Btn[18].UpdateMouseEvent())
	{
		int itemsPerPage = 9;
		int totalPages = (ListItemCashShop.size() + itemsPerPage - 1) / itemsPerPage;
		if (m_CurrentPage < totalPages - 1) m_CurrentPage++;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_NEWSHOP);
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, 640, 430))
	{
		if (SEASON3B::IsPress(VK_RBUTTON))
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}

		if (SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}

		return false;
	}

	return true;
}


bool SEASON3B::CNewUINewShop::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_NEWSHOP);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUINewShop::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUINewShop::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUINewShop::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderWindowShop();

	RenderItem3DShop();

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->Render();
		m_pNewInventoryCtrl->Render3D();
	}

	m_PhotoChar.Render();
	
	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUINewShop::SendPacketToProtocol11(BYTE* lpMsg)
{
    DanhSachCashShop.clear();

    auto mRecv = reinterpret_cast<PMSG_CASHSHOP_SEND*>(lpMsg);

    for (int i = 0; i < mRecv->count; i++)
    {
        auto lpInfo = *reinterpret_cast<ListShopSend*>(reinterpret_cast<BYTE*>(lpMsg) + sizeof(PMSG_CASHSHOP_SEND) + (sizeof(ListShopSend) * i));
        DanhSachCashShop.push_back(lpInfo);
    }

    DataListShop = DanhSachCashShop.size();

    for (int i = 0; i < DataListShop; i++)
    {
        g_pUIForm->RenderCustomButton(&m_Btn[i], m_Pos.x + 20, m_Pos.y + 32 + (i * 30), 2, DanhSachCashShop[i].Name);
    }

    const int itemsPerRow = 3;
    const int itemsPerCol = 3;
    for (int cnt = 0; cnt < itemsPerRow * itemsPerCol; cnt++)
    {
        float posX = m_Pos.x + 128 + ((cnt % itemsPerRow) * 122);
        float posY = m_Pos.y + 52 + ((cnt / itemsPerRow) * 121) + 75;
        g_pUIForm->RenderCustomButton(&m_Buy[cnt], posX + 34, posY - 1, 5, GlobalText[2886]); //
    }

    g_pNewUISystem->Toggle(SEASON3B::INTERFACE_NEWSHOP);
}

void SEASON3B::CNewUINewShop::SendPacketToProtocol12(BYTE* lpMsg)
{
	ListItemCashShop.clear();

	auto mRecv = reinterpret_cast<PMSG_LISTITEMSHOP_SEND*>(lpMsg);

	for (int i = 0; i < mRecv->count; i++)
	{
		auto lpInfo = reinterpret_cast<LISTITEMCASHSHOP_SENDINFO*>(reinterpret_cast<BYTE*>(lpMsg) + sizeof(PMSG_LISTITEMSHOP_SEND) + (sizeof(LISTITEMCASHSHOP_SENDINFO) * i));

		INFO_SHOP_ITEM infoItemLocal{};
		infoItemLocal.SizeBMD = lpInfo->SizeBMD;
		infoItemLocal.PriceType = lpInfo->PriceType;
		infoItemLocal.Price = lpInfo->Price;

		for (int n = 0; n < MAX_CLASS; n++)
		{
			infoItemLocal.Class[n] = lpInfo->Class[n];
		}

		infoItemLocal.Index = lpInfo->Index;
		infoItemLocal.TypeItem = lpInfo->TypeItem;

		if (auto item = g_pNewItemMng->CreateItem(lpInfo->Item))
		{
			memcpy(&infoItemLocal.Item, item, sizeof(ITEM));
			infoItemLocal.Item.Durability = lpInfo->Dur;

			if (lpInfo->PeriodTime)
			{
				infoItemLocal.Item.bPeriodItem = 1;
				infoItemLocal.Item.lExpireTime = lpInfo->PeriodTime;
			}
		}

		ListItemCashShop.push_back(infoItemLocal);
	}
}

void SEASON3B::CNewUINewShop::XacNhanMuaShop(LPVOID pClass)
{
	XULY_CGPACKET_SSHOP pMsg;
	memset(&pMsg, 0, sizeof(pMsg));
	pMsg.header.set(0xD5, 0x11, sizeof(pMsg));
	pMsg.Shop = SelectTypeShop;
	pMsg.ViTri = ViTriMua;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);
}

void SEASON3B::CNewUINewShop::SendMenuKey()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NEWSHOP))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NEWSHOP);
		return;
	}
	
	SolvedNewShop pMsg1;
	memset(&pMsg1, 0, sizeof(pMsg1));
	pMsg1.header.set(0xD5, 0x13, sizeof(pMsg1));
	pMsg1.ThaoTac = 1;
	DataSend((LPBYTE)&pMsg1, pMsg1.header.size);

	SolvedNewShop pMsg2;
	memset(&pMsg2, 0, sizeof(pMsg2));
	pMsg2.header.set(0xD5, 0x12, sizeof(pMsg2));
	pMsg2.ThaoTac = 1;
	DataSend((LPBYTE)&pMsg2, pMsg2.header.size);

	g_pNewShop->InsertInventoryItemsFromPlayer();

}

void SEASON3B::CNewUINewShop::InitButtons()
{
	g_pUIForm->RenderCustomButton(&m_Btn[18], m_Pos.x + 82, m_Pos.y + 382, 3, "");
	g_pUIForm->RenderCustomButton(&m_Btn[19], m_Pos.x + 18, m_Pos.y + 382, 4, "");
}

void SEASON3B::CNewUINewShop::RenderWindowShop()
{
	RenderImage(CNewUIInGameShop::IMAGE_IGS_BACK_CT, m_Pos.x, 0, 640, 430);

	for (int n = 0; n < DataListShop; n++)
	{
		m_Btn[n].ChangeText(this->DanhSachCashShop[n].Name);
		m_Btn[n].Render();
	}

	m_Btn[18].Render();
	m_Btn[19].Render();

	g_pUIForm->RenderToolTip(m_Pos.x + 40, m_Pos.y + 388, 40, 12);

	// Laster notice.
	g_pRenderText->SetBgColor(255,0,0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	char zString[11];
	sprintf(zString, "Page: %d", m_CurrentPage + 1);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 390, zString, 40, 0, 3);

	sprintf(zString, "%s", Hero->ID);
	g_pRenderText->RenderText((m_Pos.x + WINDOW_WIDTH ) + 14, m_Pos.y + 23, zString, 129, 0, 3);
	
}

void SEASON3B::CNewUINewShop::RenderItem3DShop()
{
	const int itemsPerPage = 3 * 3;
	int totalItems = ListItemCashShop.size();
	int startIndex = m_CurrentPage * itemsPerPage;

	float WBox = 35.0f;
	float HBox = 35.0f;

	int RenderInfoItem = -1;

	for (int cnt = 0; cnt < itemsPerPage; cnt++)
	{
		int index = startIndex + cnt;
		float posX = m_Pos.x + 128 + ((cnt % 3) * 122);
		float posY = m_Pos.y + 52 + ((cnt / 3) * 121);

		if (index < totalItems)
		{
			if (SeclectClass != -1 && ListItemCashShop[index].Class[SeclectClass] == 0)
				continue;

			g_pNewUISystem->RenderItem3DNew(
				posX, posY, WBox, HBox,
				ListItemCashShop[index].Index,
				ListItemCashShop[index].Item.Level,
				ListItemCashShop[index].Item.Option1,
				ListItemCashShop[index].Item.ExtOption,
				false, 1.1f, false
			);

			unicode::t_char zString[128] = { 0 };
			sprintf(zString, "%s", BGetItemName(ListItemCashShop[index].Index, 0));
			g_pRenderText->SetBgColor(244, 0, 0, 0);
			g_pRenderText->SetTextColor(255, 255, 255, 255);
			g_pRenderText->RenderText(posX - 26, posY - 12, zString, 109, 0, 3);

			sprintf(zString, "%s %s", IsToolKit.QN(this->ListItemCashShop[index].Price), MoneyType[this->ListItemCashShop[index].PriceType]);
			g_pRenderText->RenderText(posX - 26, posY + 60, zString, 109, 0, 3);

			if (CheckMouseIn(posX, posY, 40, 40) == 1)
			{
				RenderInfoItem = index;
			}

			m_Buy[cnt].Render();
		}
		else
		{
			RenderImage(CNewUIInGameShop::IMAGE_IGS_ITEMBOX_LOGO, posX, posY, 57, 57);
		}
	}

	if (RenderInfoItem != -1)
	{
		ITEM tempItem;
		memset(&tempItem, 0, sizeof(ITEM));
		memcpy(&tempItem, &ListItemCashShop[RenderInfoItem].Item, sizeof(tempItem));
		RenderItemInfo(MouseX, MouseY, &tempItem, false, 0, false, false);
	}
}

void SEASON3B::CNewUINewShop::InsertInventoryItemsFromPlayer()
{
	if (!m_pNewInventoryCtrl)
		return;

	m_pNewInventoryCtrl->RemoveAllItems();

	CNewUIInventoryCtrl* pInvCtrl = g_pNewUISystem->GetUI_NewMyInventory()->GetInventoryCtrl();
	if (!pInvCtrl)
		return;

	for (size_t i = 0; i < pInvCtrl->GetNumberOfItems(); ++i)
	{
		ITEM* pItem = pInvCtrl->GetItem(static_cast<int>(i));
		if (pItem)
		{
			ITEM* pItemNew = new ITEM;
			memcpy(pItemNew, pItem, sizeof(ITEM));
			m_pNewInventoryCtrl->AddItem(pItem->x, pItem->y, pItemNew);
		}
	}
}


//Lam chua xong
//void SEASON3B::CNewUINewShop::InsertInventoryItemsFromPlayer()
//{
//	if (!m_pNewInventoryCtrl)
//		return;
//
//	CNewUIInventoryCtrl* pInvCtrl = g_pNewUISystem->GetUI_NewMyInventory()->GetInventoryCtrl();
//	if (!pInvCtrl)
//		return;
//
//	for (size_t i = 0; i < pInvCtrl->GetNumberOfItems(); ++i)
//	{
//		ITEM* pItem = pInvCtrl->GetItem(static_cast<int>(i));
//		if (pItem)
//			m_pNewInventoryCtrl->AddItem(pItem->x, pItem->y, pItem);
//	}
//}