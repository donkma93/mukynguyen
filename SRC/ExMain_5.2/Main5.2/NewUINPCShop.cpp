// NewUINPCShop.cpp: implementation of the CNewUINPCShop class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUINPCShop.h"
#include "NewUIMyInventory.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "GambleSystem.h"

using namespace SEASON3B;

SEASON3B::CNewUINPCShop::CNewUINPCShop()
{
	Init();
}

SEASON3B::CNewUINPCShop::~CNewUINPCShop()
{
	Release();
}

void SEASON3B::CNewUINPCShop::Init()
{
	m_pNewUIMng = NULL;
	m_pNewInventoryCtrl = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_dwShopState = SHOP_STATE_BUYNSELL;
	m_iTaxRate = 0;
	m_bRepairShop = false;
	m_bIsNPCShopOpen = false;
	m_dwStandbyItemKey = 0;
	m_bSellingItem = false;
	ResetMoss();
}

bool SEASON3B::CNewUINPCShop::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NPCSHOP, this);

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;
	if (false == m_pNewInventoryCtrl->Create(STORAGE_TYPE::UNDEFINED, g_pNewUI3DRenderMng, g_pNewItemMng, this, x + 15, y + 50, 8, 15))
	{
		SAFE_DELETE(m_pNewInventoryCtrl);
		return false;
	}

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->SetToolTipType(TOOLTIP_TYPE_NPC_SHOP);
	}

	SetPos(x, y);

	LoadImages();

	SetButtonInfo();

	Show(false);

	return true;
}

void SEASON3B::CNewUINPCShop::Release()
{
	UnloadImages();

	SAFE_DELETE(m_pNewInventoryCtrl);

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUINPCShop::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->SetPos(m_Pos.x + 15, m_Pos.y + 50); 
	}
}

bool SEASON3B::CNewUINPCShop::UpdateMouseEvent()
{
	if (GambleSystem::Instance().IsGambleShop())
		return UpdateMossMouseEvent();

	// The NPC shop has a higher UI layer than the player inventory. Handle the
	// right-click here so the event cannot be consumed by another UI first.
	if ((SEASON3B::IsPress(VK_RBUTTON) || SEASON3B::IsRelease(VK_RBUTTON))
		&& m_bSellingItem == false)
	{
		CNewUIInventoryCtrl* pMyInventoryCtrl = g_pMyInventory->GetInventoryCtrl();
		if (pMyInventoryCtrl && pMyInventoryCtrl->CheckPtInRect(MouseX, MouseY))
		{
			ITEM* pItem = pMyInventoryCtrl->FindItemAtPt(MouseX, MouseY);
			if (pItem && pItem->Jewel_Of_Harmony_Option == 0 && IsSellingBan(pItem) == false)
			{
				if (CharacterMachine->Gold + ItemValue(pItem) <= 2000000000)
				{
					const bool bHighValueItem = IsHighValueItem(pItem);
					const int iIndex = pMyInventoryCtrl->GetIndexByItem(pItem);
					if (iIndex >= 0 && CNewUIInventoryCtrl::CreatePickedItem(pMyInventoryCtrl, pItem))
					{
						pMyInventoryCtrl->RemoveItem(pItem);
						if (bHighValueItem)
						{
							SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CHighValueItemCheckMsgBoxLayout));
							CNewUIInventoryCtrl::GetPickedItem()->HidePickedItem();
							return false;
						}
						SendRequestSell(iIndex);
						MouseRButton = false;
						MouseRButtonPop = false;
						MouseRButtonPush = false;
						return false;
					}
				}
				else
				{
					g_pChatListBox->AddText("", GlobalText[3148], SEASON3B::TYPE_SYSTEM_MESSAGE);
					return false;
				}
			}
		}
	}

	if (m_pNewInventoryCtrl)
	{
		if (false == m_pNewInventoryCtrl->UpdateMouseEvent())
		{
			return false;
		}

		if (InventoryProcess() == true)
		{
			return false;
		}

		if (m_pNewInventoryCtrl->CheckPtInRect(MouseX, MouseY) == true)
		{
			ITEM* pItem = m_pNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if ((m_bIsNPCShopOpen == true) && (pItem) && (SEASON3B::IsRelease(VK_LBUTTON)))
			{
				int iIndex = (pItem->y * m_pNewInventoryCtrl->GetNumberOfColumn()) + pItem->x;
				GambleSystem& _gambleSys = GambleSystem::Instance();

				if (_gambleSys.IsGambleShop())
				{
					_gambleSys.SetBuyItemInfo(iIndex, ItemValue(pItem, 0));
					g_pNPCShop->SetStandbyItemKey(pItem->Key);

					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGambleBuyMsgBoxLayout));

					return false;
				}
				if (BuyCost == 0)
				{
					SendRequestBuy(iIndex, ItemValue(pItem, 0));
				}

				return false;
			}
			if (SEASON3B::IsRelease(VK_LBUTTON))
			{
				m_bIsNPCShopOpen = true;
				return false;
			}
			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				return false;
			}
		}
	}

	if (BtnProcess() == true)
	{
		return false;
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, NPCSHOP_WIDTH, NPCSHOP_HEIGHT))
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
	}

	return true;
}

bool SEASON3B::CNewUINPCShop::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP) == false)
	{
		return true;
	}

	if (!GambleSystem::Instance().IsGambleShop() && SEASON3B::IsRepeat(VK_SHIFT) && SEASON3B::IsPress('L'))
	{
		SendRequestRepair(255, 0);
		return false;
	}
	if (SEASON3B::IsPress('L'))
	{
		if (m_bRepairShop && CNewUIInventoryCtrl::GetPickedItem() == NULL)
		{
			ToggleState();
			return false;
		}
	}


	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true && m_bSellingItem == false)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCSHOP);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}
	return true;
}

bool SEASON3B::CNewUINPCShop::Update()
{
	if (GambleSystem::Instance().IsGambleShop())
	{
		// Moss delivers its reward asynchronously through ReceiveModifyItem.
		if (m_bMossPending && GetTickCount() - m_dwMossRequestTime > 15000)
		{
			m_bMossTimedOut = true;
		}
		return true;
	}
	if (m_bRepairShop)
	{
		RepairAllGold();
	}
	if (m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->Update())
	{
		return false;
	}
	return true;
}
#include "_enum.h"
#include "ThangCuoi\WideData.h"
bool SEASON3B::CNewUINPCShop::Render()
{
	if (GambleSystem::Instance().IsGambleShop())
	{
		RenderMoss();
		return true;
	}

	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderTexts();
	RenderButton();
	RenderRepairMoney();

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->Render();
	}

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUINPCShop::RenderFrame()
{
	RenderImage(IMAGE_NPCSHOP_BACK, m_Pos.x + 2, m_Pos.y + 3, 186.f, 426.f);
	RenderImage(IMAGE_NPCSHOP_TOP, m_Pos.x, m_Pos.y, 190.f, 64.f);
	RenderImage(IMAGE_NPCSHOP_LEFT, m_Pos.x, m_Pos.y + 64, 21.f, 320.f);
	RenderImage(IMAGE_NPCSHOP_RIGHT, m_Pos.x + 190 - 21, m_Pos.y + 64, 21.f, 320.f);
	RenderImage(IMAGE_NPCSHOP_BOTTOM, m_Pos.x, m_Pos.y + 429 - 45, 190.f, 45.f);
}

void SEASON3B::CNewUINPCShop::RenderTexts()
{
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(220, 220, 220, 255);

	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[230], NPCSHOP_WIDTH, 0, RT3_SORT_CENTER);

	unicode::t_char strText[256];
	unicode::_sprintf(strText, GlobalText[1623], m_iTaxRate);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 33, strText, NPCSHOP_WIDTH, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUINPCShop::RenderButton()
{
	if (m_bRepairShop)
	{
		m_BtnRepair.Render();
		m_BtnRepairAll.Render();
	}
}

void SEASON3B::CNewUINPCShop::RenderRepairMoney()
{
	if (m_bRepairShop)
	{
		RenderImage(IMAGE_NPCSHOP_REPAIR_MONEY, m_Pos.x + 10, m_Pos.y + 355, 170.f, 24.f);
		g_pRenderText->SetBgColor(255, 255, 255, 0);
		g_pRenderText->SetTextColor(255, 220, 150, 255);
		unicode::t_char strText[256];
		ConvertGold(AllRepairGold, strText);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 362, GlobalText[239]);
		g_pRenderText->SetTextColor(getGoldColor(AllRepairGold));
		g_pRenderText->RenderText(m_Pos.x + 100, m_Pos.y + 362, strText);
	}
}

float SEASON3B::CNewUINPCShop::GetLayerDepth()
{
	return 4.55;
}

void SEASON3B::CNewUINPCShop::LoadImages()
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_NPCSHOP_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back04.tga", IMAGE_NPCSHOP_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_NPCSHOP_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_NPCSHOP_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_NPCSHOP_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_repair_00.tga", IMAGE_NPCSHOP_BTN_REPAIR, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_money2.tga", IMAGE_NPCSHOP_REPAIR_MONEY, GL_LINEAR);
}

void SEASON3B::CNewUINPCShop::UnloadImages()
{
	DeleteBitmap(IMAGE_NPCSHOP_BACK);
	DeleteBitmap(IMAGE_NPCSHOP_TOP);
	DeleteBitmap(IMAGE_NPCSHOP_LEFT);
	DeleteBitmap(IMAGE_NPCSHOP_LEFT);
	DeleteBitmap(IMAGE_NPCSHOP_BOTTOM);
	DeleteBitmap(IMAGE_NPCSHOP_BTN_REPAIR);
	DeleteBitmap(IMAGE_NPCSHOP_REPAIR_MONEY);
}

void SEASON3B::CNewUINPCShop::SetTaxRate(int iTaxRate)
{
	m_iTaxRate = iTaxRate;
}

int SEASON3B::CNewUINPCShop::GetTaxRate()
{
	return m_iTaxRate;
}

bool SEASON3B::CNewUINPCShop::InsertItem(int iIndex, BYTE* pbyItemPacket)
{
	if (m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->AddItem(iIndex, pbyItemPacket);
	}

	return false;
}

bool SEASON3B::CNewUINPCShop::InventoryProcess()
{
	CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

	if (!m_pNewInventoryCtrl)	return false;
	if (!pPickedItem)			return false;
	ITEM* pItem = pPickedItem->GetItem();

#ifdef LEM_ADD_LUCKYITEM
	if (IsSellingBan(pItem))	m_pNewInventoryCtrl->SetSquareColorNormal(1.0f, 0.0f, 0.0f);
	else	m_pNewInventoryCtrl->SetSquareColorNormal(0.1f, 0.4f, 0.8f);
#endif // LEM_ADD_LUCKYITEM

	if (SEASON3B::IsRelease(VK_LBUTTON) == true && m_pNewInventoryCtrl->CheckPtInRect(MouseX, MouseY) == true && m_bSellingItem == false)
	{
		if (CharacterMachine->Gold + ItemValue(pItem) > 2000000000)
		{
			g_pChatListBox->AddText("", GlobalText[3148], SEASON3B::TYPE_SYSTEM_MESSAGE);

			return true;
		}

		if (pItem && pItem->Jewel_Of_Harmony_Option != 0)
		{
			g_pChatListBox->AddText("", GlobalText[2211], SEASON3B::TYPE_ERROR_MESSAGE);

			return true;
		}
		if (pItem && IsSellingBan(pItem) == true)
		{
			g_pChatListBox->AddText("", GlobalText[668], SEASON3B::TYPE_ERROR_MESSAGE);
			m_pNewInventoryCtrl->BackupPickedItem();

			return true;
		}
		if (pItem && IsHighValueItem(pItem) == true)
		{
			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CHighValueItemCheckMsgBoxLayout));
			pPickedItem->HidePickedItem();

			return true;
		}

		if (pPickedItem->GetSourceStorageType() == STORAGE_TYPE::INVENTORY)
		{
			int iSourceIndex = pPickedItem->GetSourceLinealPos();
			SendRequestSell(iSourceIndex);

			return true;
		}
	}

	return false;
}

bool SEASON3B::CNewUINPCShop::BtnProcess()
{
	POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 7 };

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12) && m_bSellingItem == false)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCSHOP);

		return true;
	}

	if (m_bRepairShop)
	{
		if (m_BtnRepair.UpdateMouseEvent() == true)
		{
			ToggleState();

			return true;
		}
		if (m_BtnRepairAll.UpdateMouseEvent() == true)
		{
			SendRequestRepair(255, 0);

			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUINPCShop::OpenningProcess()
{
	ResetMoss();
	if (m_pNewInventoryCtrl)
	{
		if (GambleSystem::Instance().IsGambleShop())
			m_pNewInventoryCtrl->HideInventory();
		else
			m_pNewInventoryCtrl->ShowInventory();
	}
	if (SEASON3B::IsRepeat(VK_LBUTTON))
	{
		m_bIsNPCShopOpen = false;
	}
	else
	{
		m_bIsNPCShopOpen = true;
	}
}

void SEASON3B::CNewUINPCShop::ClosingProcess()
{
	SendExitInventory();

	m_dwShopState = SHOP_STATE_BUYNSELL;
	m_iTaxRate = 0;
	m_bRepairShop = false;
	m_dwStandbyItemKey = 0;

	m_bIsNPCShopOpen = false;

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->RemoveAllItems();
	}

	GambleSystem::Instance().SetGambleShop(false);
	ResetMoss();
	if (m_pNewInventoryCtrl)
		m_pNewInventoryCtrl->ShowInventory();
	m_bSellingItem = false;
}

void SEASON3B::CNewUINPCShop::SetButtonInfo()
{
	m_BtnRepair.ChangeButtonImgState(true, IMAGE_NPCSHOP_BTN_REPAIR, false);
	m_BtnRepair.ChangeButtonInfo(m_Pos.x + 54, m_Pos.y + 390, 36, 29);
	m_BtnRepair.ChangeToolTipText(GlobalText[233], true);

	m_BtnRepairAll.ChangeButtonImgState(true, IMAGE_NPCSHOP_BTN_REPAIR, false);
	m_BtnRepairAll.ChangeButtonInfo(m_Pos.x + 98, m_Pos.y + 390, 36, 29);
	m_BtnRepairAll.ChangeToolTipText(GlobalText[237], true);
}

void SEASON3B::CNewUINPCShop::SetRepairShop(bool bRepair)
{
	m_bRepairShop = bRepair;
}

bool SEASON3B::CNewUINPCShop::IsRepairShop()
{
	return m_bRepairShop;
}

void SEASON3B::CNewUINPCShop::ToggleState()
{
	if (m_dwShopState == SHOP_STATE_BUYNSELL)
	{
		m_dwShopState = SHOP_STATE_REPAIR;

		g_pMyInventory->SetRepairMode(true);
	}
	else
	{
		m_dwShopState = SHOP_STATE_BUYNSELL;
		g_pMyInventory->SetRepairMode(false);
	}
}

DWORD SEASON3B::CNewUINPCShop::GetShopState()
{
	return m_dwShopState;
}


int SEASON3B::CNewUINPCShop::GetPointedItemIndex()
{
	if (GambleSystem::Instance().IsGambleShop())
		return -1;
	return m_pNewInventoryCtrl->GetPointedSquareIndex();
}

void SEASON3B::CNewUINPCShop::SetStandbyItemKey(DWORD dwItemKey)
{
	m_dwStandbyItemKey = dwItemKey;
}

DWORD SEASON3B::CNewUINPCShop::GetStandbyItemKey() const
{
	return m_dwStandbyItemKey;
}

int SEASON3B::CNewUINPCShop::GetStandbyItemIndex()
{
	ITEM* pItem = GetStandbyItem();
	if (pItem)
		return pItem->y * m_pNewInventoryCtrl->GetNumberOfColumn() + pItem->x;
	return -1;
}

ITEM* SEASON3B::CNewUINPCShop::GetStandbyItem()
{
	if (m_pNewInventoryCtrl)
		return m_pNewInventoryCtrl->FindItemByKey(m_dwStandbyItemKey);
	return NULL;
}

void SEASON3B::CNewUINPCShop::SetSellingItem(bool bFlag)
{
	m_bSellingItem = bFlag;
}

bool SEASON3B::CNewUINPCShop::IsSellingItem()
{
	return m_bSellingItem;
}

namespace
{
	const int MOSS_WIDTH = 250;
	const int MOSS_HEIGHT = 429;
	const int MOSS_ROWS = 5;

	const char* MossBoxName(int type)
	{
		switch (type - (ITEM_HELPER + 71))
		{
		case 0: return u8"H\u1ed9p Ki\u1ebfm";
		case 1: return u8"H\u1ed9p G\u1eady";
		case 2: return u8"H\u1ed9p Cung";
		case 3: return u8"H\u1ed9p Tr\u01b0\u1ee3ng";
		case 4: return u8"H\u1ed9p Khuy\u1ec3n";
		default: return "Moss";
		}
	}

	void MossPanel(float x, float y, float width, float height, bool active = false)
	{
		EnableAlphaTest();
		glColor4f(active ? .32f : .22f, active ? .26f : .18f, .10f, 1.f);
		RenderColor(x, y, width, height);
		glColor4f(active ? .10f : .025f, active ? .12f : .03f, active ? .14f : .035f, .96f);
		RenderColor(x + 1, y + 1, width - 2, height - 2);
		EndRenderColor();
		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	void MossText(int x, int y, int width, const char* text, bool gold = false)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		if (gold)
			g_pRenderText->SetTextColor(235, 200, 95, 255);
		else
			g_pRenderText->SetTextColor(170, 166, 154, 255);
		g_pRenderText->RenderText(x, y, text, width, 0, RT3_SORT_CENTER);
	}
}

void SEASON3B::CNewUINPCShop::ResetMoss()
{
	m_iMossSelection = 0;
	m_bMossPending = false;
	m_bMossHasReward = false;
	m_bMossTimedOut = false;
	m_dwMossRequestTime = 0;
	memset(&m_MossReward, 0, sizeof(m_MossReward));
}

int SEASON3B::CNewUINPCShop::GetMossItemCount()
{
	int count = 0;
	if (m_pNewInventoryCtrl)
	{
		for (int i = 0; i < (int)m_pNewInventoryCtrl->GetNumberOfItems(); ++i)
		{
			ITEM* item = m_pNewInventoryCtrl->GetItem(i);
			if (item && item->Type >= ITEM_HELPER + 71 && item->Type <= ITEM_HELPER + 75)
				++count;
		}
	}
	return count;
}

ITEM* SEASON3B::CNewUINPCShop::GetMossItem(int index)
{
	if (m_pNewInventoryCtrl)
	{
		for (int i = 0; i < (int)m_pNewInventoryCtrl->GetNumberOfItems(); ++i)
		{
			ITEM* item = m_pNewInventoryCtrl->GetItem(i);
			if (item && item->Type >= ITEM_HELPER + 71 && item->Type <= ITEM_HELPER + 75)
			{
				if (index-- == 0)
					return item;
			}
		}
	}
	return NULL;
}

bool SEASON3B::CNewUINPCShop::BeginMossPurchase()
{
	if (!GambleSystem::Instance().IsGambleShop() || m_bMossPending || BuyCost != 0
		|| !GetStandbyItem() || CNewUIInventoryCtrl::GetPickedItem())
		return false;
	m_bMossPending = true;
	m_bMossHasReward = false;
	m_bMossTimedOut = false;
	m_dwMossRequestTime = GetTickCount();
	return true;
}

void SEASON3B::CNewUINPCShop::ReceiveMossReward(BYTE* pbyItemPacket)
{
	if (!GambleSystem::Instance().IsGambleShop() || !m_bMossPending || !pbyItemPacket)
		return;
	ITEM* item = g_pNewItemMng->CreateItem(pbyItemPacket);
	if (item)
	{
		// Keep an independent copy: the inventory item may subsequently move.
		m_MossReward = *item;
		g_pNewItemMng->DeleteItem(item);
		m_bMossHasReward = true;
		m_bMossPending = false;
		m_bMossTimedOut = false;
	}
}

bool SEASON3B::CNewUINPCShop::UpdateMossMouseEvent()
{
	// Grow to the left, keeping the same right edge as the standard shop.
	const int x = m_Pos.x - (MOSS_WIDTH - NPCSHOP_WIDTH);
	const int y = m_Pos.y;
	const bool inside = CheckMouseIn(x, y, MOSS_WIDTH, MOSS_HEIGHT);
	if (inside && SEASON3B::IsPress(VK_LBUTTON)
		&& CheckMouseIn(x + 222, y + 10, 18, 18) && !m_bSellingItem)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCSHOP);
		PlayBuffer(SOUND_CLICK01);
		return false;
	}
	if (SEASON3B::IsRelease(VK_LBUTTON))
	{
		const bool ready = m_bIsNPCShopOpen;
		m_bIsNPCShopOpen = true;
		if (inside && ready && !m_bMossPending && BuyCost == 0 && !CNewUIInventoryCtrl::GetPickedItem())
		{
			const int count = GetMossItemCount();
			const int first = (m_iMossSelection / MOSS_ROWS) * MOSS_ROWS;
			for (int row = 0; row < MOSS_ROWS && first + row < count; ++row)
			{
				if (CheckMouseIn(x + 10, y + 48 + row * 29, 82, 28))
				{
					m_iMossSelection = first + row;
					m_bMossHasReward = false;
					PlayBuffer(SOUND_CLICK01);
					return false;
				}
			}
			if (count > MOSS_ROWS && CheckMouseIn(x + 10, y + 200, 82, 22))
			{
				m_iMossSelection = first + MOSS_ROWS < count ? first + MOSS_ROWS : 0;
				m_bMossHasReward = false;
				return false;
			}
			ITEM* item = GetMossItem(m_iMossSelection);
			if (item && CheckMouseIn(x + 104, y + 221, 134, 25))
			{
				const DWORD cost = ItemValue(item, 0);
				const ULONGLONG total = cost + ((ULONGLONG)cost * m_iTaxRate) / 100;
				if (CharacterMachine->Gold < total)
					return false;
				const int slot = item->y * m_pNewInventoryCtrl->GetNumberOfColumn() + item->x;
				GambleSystem::Instance().SetBuyItemInfo(slot, cost);
				SetStandbyItemKey(item->Key);
				SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGambleBuyMsgBoxLayout));
				PlayBuffer(SOUND_CLICK01);
				return false;
			}
		}
	}
	if (inside)
	{
		MouseRButton = MouseRButtonPop = MouseRButtonPush = false;
		return false;
	}
	return true;
}

void SEASON3B::CNewUINPCShop::RenderMoss()
{
	const int x = m_Pos.x - (MOSS_WIDTH - NPCSHOP_WIDTH);
	const int y = m_Pos.y;
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	RenderImage(IMAGE_NPCSHOP_BACK, x + 2, y + 3, MOSS_WIDTH - 4.f, MOSS_HEIGHT - 3.f);
	RenderImage(IMAGE_NPCSHOP_TOP, x, y, MOSS_WIDTH, 64.f);
	RenderImage(IMAGE_NPCSHOP_LEFT, x, y + 64, 21.f, 320.f);
	RenderImage(IMAGE_NPCSHOP_RIGHT, x + MOSS_WIDTH - 21, y + 64, 21.f, 320.f);
	RenderImage(IMAGE_NPCSHOP_BOTTOM, x, y + MOSS_HEIGHT - 45, MOSS_WIDTH, 45.f);
	MossPanel(x + 8, y + 8, 234, 23, true);
	// Blue inset title bar, using the client's existing frame textures.
	glColor4f(.025f, .18f, .27f, .9f);
	RenderColor(x + 11, y + 10, 209, 18);
	EndRenderColor();
	EnableAlphaTest();
	MossText(x + 12, y + 14, 207, "MOSS MERCHANT", true);
	MossPanel(x + 222, y + 10, 18, 18, CheckMouseIn(x + 222, y + 10, 18, 18));
	MossText(x + 222, y + 13, 18, "X", true);

	const int count = GetMossItemCount();
	if (m_iMossSelection >= count)
		m_iMossSelection = 0;
	const int first = (m_iMossSelection / MOSS_ROWS) * MOSS_ROWS;
	for (int row = 0; row < MOSS_ROWS && first + row < count; ++row)
	{
		ITEM* item = GetMossItem(first + row);
		const bool selected = m_iMossSelection == first + row;
		MossPanel(x + 10, y + 48 + row * 29, 82, 28, selected);
		MossText(x + 11, y + 57 + row * 29, 80, MossBoxName(item->Type), selected);
	}
	if (count > MOSS_ROWS)
	{
		MossPanel(x + 10, y + 200, 82, 22);
		MossText(x + 10, y + 206, 82, u8"Trang ti\u1ebfp >");
	}

	ITEM* selected = GetMossItem(m_iMossSelection);
	if (!selected)
	{
		MossText(x + 100, y + 90, 140, u8"Ch\u01b0a c\u00f3 h\u1ed9p Moss");
		DisableAlphaBlend();
		return;
	}
	MossPanel(x + 98, y + 48, 140, 22);
	MossText(x + 99, y + 55, 138, MossBoxName(selected->Type), true);
	MossPanel(x + 133, y + 76, 72, 72);
	MossPanel(x + 98, y + 154, 140, 31);
	MossText(x + 99, y + 159, 138, u8"Nh\u1eadn 1 ph\u1ea7n th\u01b0\u1edfng");
	MossText(x + 99, y + 172, 138, u8"ng\u1eabu nhi\u00ean v\u1edbi gi\u00e1 sau");
	MossPanel(x + 98, y + 189, 140, 27);
	MossPanel(x + 102, y + 192, 23, 21);
	MossText(x + 126, y + 191, 108, "Zen", true);
	char price[64] = { 0 };
	ConvertTaxGold(ItemValue(selected, 0), price);
	MossText(x + 126, y + 203, 108, price);
	const DWORD cost = ItemValue(selected, 0);
	const bool affordable = CharacterMachine->Gold >= cost + ((ULONGLONG)cost * m_iTaxRate) / 100;
	const bool enabled = affordable && !m_bMossPending && BuyCost == 0 && !CNewUIInventoryCtrl::GetPickedItem();
	MossPanel(x + 104, y + 221, 134, 25, enabled);
	MossText(x + 104, y + 229, 134, m_bMossPending ? u8"\u0110ang ch\u1edd..." :
		(affordable ? u8"M\u1ede h\u1ed9p" : u8"Kh\u00f4ng \u0111\u1ee7 Zen"), enabled);

	if (m_bMossPending && !m_bMossTimedOut)
	{
		const int phase = (GetTickCount() / 80) % 12;
		for (int i = 0; i < 12; ++i)
		{
			const float angle = i * 6.2831853f / 12.f;
			const float light = .25f + ((i + 12 - phase) % 12) / 16.f;
			glColor4f(.12f, .46f * light, .85f * light, 1.f);
			RenderColor(x + 168 + cosf(angle) * 8, y + 260 + sinf(angle) * 8, 2, 2);
		}
	}
	else
	{
		glColor4f(.32f, .31f, .27f, 1.f);
		RenderColor(x + 166, y + 250, 4, 13);
		for (int i = 0; i < 6; ++i)
		{
			RenderColor(x + 158 + i, y + 257 + i, 3, 2);
			RenderColor(x + 175 - i, y + 257 + i, 3, 2);
		}
	}
	EndRenderColor();
	EnableAlphaTest();
	MossPanel(x + 133, y + 277, 72, 78, m_bMossHasReward);
	MossText(x + 99, y + 361, 138, u8"Ph\u1ea7n th\u01b0\u1edfng", m_bMossHasReward);
	if (!m_bMossHasReward)
		MossText(x + 134, y + 308, 70, "?", true);
	if (m_bMossTimedOut)
	{
		MossText(x + 12, y + 383, 226, u8"Ch\u01b0a nh\u1eadn \u0111\u01b0\u1ee3c k\u1ebft qu\u1ea3.");
		MossText(x + 12, y + 397, 226, u8"\u0110\u00f3ng v\u00e0 m\u1edf l\u1ea1i Moss \u0111\u1ec3 th\u1eed l\u1ea1i.");
	}
	else
		MossText(x + 12, y + 397, 226, m_bMossHasReward ? u8"V\u1eadt ph\u1ea9m \u0111\u00e3 v\u00e0o t\u00fai \u0111\u1ed3." :
			u8"Ch\u1ecdn h\u1ed9p v\u00e0 nh\u1ea5n M\u1edf h\u1ed9p.");

	g_pNewUISystem->RenderItem3DFree(x + 139, y + 82, 60, 60,
		selected->Type, selected->Level, selected->Option1, selected->ExtOption, false, 1.f, false);
	g_pNewUISystem->RenderItem3DFree(x + 104, y + 192, 19, 19,
		ITEM_POTION + 15, 0, 0, 0, false, 1.f, false);
	if (m_bMossHasReward)
	{
		g_pNewUISystem->RenderItem3DFree(x + 139, y + 281, 60, 69,
			m_MossReward.Type, m_MossReward.Level, m_MossReward.Option1, m_MossReward.ExtOption, false, 1.f, false);
	}
	if (m_bMossHasReward && CheckMouseIn(x + 133, y + 277, 72, 78))
		RenderItemInfo(MouseX, MouseY, &m_MossReward, false);
	DisableAlphaBlend();
}
