
#include "stdafx.h"
#include "NewUIViewItem.h"
#include "wsclientinline.h"
#include "CharacterManager.h"
#include "WideData.h"
#include "CustomWing.h"
#include "ZzzToolKit.h"

using namespace SEASON3B;
ViewCharItem gViewCharItem;

CNewUIViewItem::CNewUIViewItem()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = m_Pos.y = 0;

	memset(&m_EquipmentSlots, 0, sizeof(EQUIPMENT_ITEM) * MAX_EQUIPMENT_INDEX);

	gViewCharItem.ClearData();
}

void ViewCharItem::ClearData()
{
	for (int i = 0; i < MAX_EQUIPMENT; ++i)
	{
		this->CharItemInfo[i].Type = -1;
		this->CharItemInfo[i].Level = 0;
		this->CharItemInfo[i].Option1 = 0;
	}

	this->m_CharItemInfo.clear();
}

void ViewCharItem::RecvProtocol(BYTE* Recv)
{
	if (!Recv) return;

	auto mRecv = reinterpret_cast<RecvViewItemData*>(Recv);
	this->ClearData();

	pPlayerInfo[0] = mRecv->pPlayerInfo[0];
	pPlayerInfo[1] = mRecv->pPlayerInfo[1];

	for (int n = 0; n < mRecv->Count; n++)
	{
		if (n > MAX_EQUIPMENT) break;

		auto lpInfo = reinterpret_cast<RecvInfoItemData*>(
			reinterpret_cast<BYTE*>(Recv) + sizeof(RecvViewItemData) + (sizeof(RecvInfoItemData) * n)
			);

		auto item = g_pNewItemMng->CreateItem(lpInfo->Item);
		memcpy(&this->CharItemInfo[n], item, sizeof(this->CharItemInfo[n]));
		this->CharItemInfo[n].Durability = lpInfo->Dur;

		if (lpInfo->PeriodTime)
		{
			this->CharItemInfo[n].bPeriodItem = 1;
			this->CharItemInfo[n].lExpireTime = lpInfo->PeriodTime;
		}
	}

	g_pNewUISystem->Toggle(INTERFACE_VIEWITEM);
}


void ViewCharItem::SendRequestViewItem(int aIndex)
{
	if (aIndex != -1)
	{
		SendRequestAction pMsg{};
		pMsg.header.set(0xD3, 0x02, sizeof(pMsg));
		pMsg.Action = aIndex;
		DataSend((LPBYTE)&pMsg, pMsg.header.size);
	}
}

bool CNewUIViewItem::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng, int x, int y)
{
	if (nullptr == pNewUIMng || nullptr == pNewUI3DRenderMng || nullptr == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(INTERFACE_VIEWITEM, this);

	SetPos(x, y);
	Show(false);
	return true;
}

void CNewUIViewItem::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	SetEquipmentSlotInfo();
}

const POINT& CNewUIViewItem::GetPos() const
{
	return m_Pos;
}

bool CNewUIViewItem::UpdateMouseEvent()
{
	if (true == BtnProcess())
		return false;

	return true;
}

bool CNewUIViewItem::UpdateKeyEvent()
{
	if (!g_pNewUISystem->IsVisible(INTERFACE_VIEWITEM))
	{
		return true;
	}

	if (IsPress(VK_ESCAPE) == true)
	{
		if (g_pNPCShop->IsSellingItem() == false)
		{
			g_pNewUISystem->Hide(INTERFACE_VIEWITEM);
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	return true;
}

bool CNewUIViewItem::Update()
{
	return true;
}

bool CNewUIViewItem::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame(m_Pos.x, m_Pos.y);
	RenderInventoryDetails();
	RenderEquippedItem();
	DisableAlphaBlend();
	return true;
}

bool CNewUIViewItem::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

float CNewUIViewItem::GetLayerDepth()
{
	return 4.2f;
}

void CNewUIViewItem::SetEquipmentSlotInfo()
{
	m_EquipmentSlots[EQUIPMENT_HELPER].x = m_Pos.x + 15;
	m_EquipmentSlots[EQUIPMENT_HELPER].y = m_Pos.y + 44;
	m_EquipmentSlots[EQUIPMENT_HELPER].width = 46;
	m_EquipmentSlots[EQUIPMENT_HELPER].height = 46;
	m_EquipmentSlots[EQUIPMENT_HELPER].dwBgImage = IMAGE_INVENTORY_ITEM_FAIRY;

	m_EquipmentSlots[EQUIPMENT_HELM].x = m_Pos.x + 75;
	m_EquipmentSlots[EQUIPMENT_HELM].y = m_Pos.y + 44;
	m_EquipmentSlots[EQUIPMENT_HELM].width = 46;
	m_EquipmentSlots[EQUIPMENT_HELM].height = 46;
	m_EquipmentSlots[EQUIPMENT_HELM].dwBgImage = IMAGE_INVENTORY_ITEM_HELM;

	m_EquipmentSlots[EQUIPMENT_WING].x = m_Pos.x + 120;
	m_EquipmentSlots[EQUIPMENT_WING].y = m_Pos.y + 44;
	m_EquipmentSlots[EQUIPMENT_WING].width = 61;
	m_EquipmentSlots[EQUIPMENT_WING].height = 46;
	m_EquipmentSlots[EQUIPMENT_WING].dwBgImage = IMAGE_INVENTORY_ITEM_WING;

	m_EquipmentSlots[EQUIPMENT_WEAPON_LEFT].x = m_Pos.x + 135;
	m_EquipmentSlots[EQUIPMENT_WEAPON_LEFT].y = m_Pos.y + 87;
	m_EquipmentSlots[EQUIPMENT_WEAPON_LEFT].width = 46;
	m_EquipmentSlots[EQUIPMENT_WEAPON_LEFT].height = 66;
	m_EquipmentSlots[EQUIPMENT_WEAPON_LEFT].dwBgImage = IMAGE_INVENTORY_ITEM_LEFT;

	m_EquipmentSlots[EQUIPMENT_ARMOR].x = m_Pos.x + 75;
	m_EquipmentSlots[EQUIPMENT_ARMOR].y = m_Pos.y + 87;
	m_EquipmentSlots[EQUIPMENT_ARMOR].width = 46;
	m_EquipmentSlots[EQUIPMENT_ARMOR].height = 66;
	m_EquipmentSlots[EQUIPMENT_ARMOR].dwBgImage = IMAGE_INVENTORY_ITEM_ARMOR;

	m_EquipmentSlots[EQUIPMENT_WEAPON_RIGHT].x = m_Pos.x + 15;
	m_EquipmentSlots[EQUIPMENT_WEAPON_RIGHT].y = m_Pos.y + 87;
	m_EquipmentSlots[EQUIPMENT_WEAPON_RIGHT].width = 46;
	m_EquipmentSlots[EQUIPMENT_WEAPON_RIGHT].height = 66;
	m_EquipmentSlots[EQUIPMENT_WEAPON_RIGHT].dwBgImage = IMAGE_INVENTORY_ITEM_RIGHT;

	m_EquipmentSlots[EQUIPMENT_GLOVES].x = m_Pos.x + 15;
	m_EquipmentSlots[EQUIPMENT_GLOVES].y = m_Pos.y + 150;
	m_EquipmentSlots[EQUIPMENT_GLOVES].width = 46;
	m_EquipmentSlots[EQUIPMENT_GLOVES].height = 46;
	m_EquipmentSlots[EQUIPMENT_GLOVES].dwBgImage = IMAGE_INVENTORY_ITEM_GLOVES;

	m_EquipmentSlots[EQUIPMENT_PANTS].x = m_Pos.x + 75;
	m_EquipmentSlots[EQUIPMENT_PANTS].y = m_Pos.y + 150;
	m_EquipmentSlots[EQUIPMENT_PANTS].width = 46;
	m_EquipmentSlots[EQUIPMENT_PANTS].height = 46;
	m_EquipmentSlots[EQUIPMENT_PANTS].dwBgImage = IMAGE_INVENTORY_ITEM_PANTS;

	m_EquipmentSlots[EQUIPMENT_BOOTS].x = m_Pos.x + 135;
	m_EquipmentSlots[EQUIPMENT_BOOTS].y = m_Pos.y + 150;
	m_EquipmentSlots[EQUIPMENT_BOOTS].width = 46;
	m_EquipmentSlots[EQUIPMENT_BOOTS].height = 46;
	m_EquipmentSlots[EQUIPMENT_BOOTS].dwBgImage = IMAGE_INVENTORY_ITEM_BOOT;

	m_EquipmentSlots[EQUIPMENT_RING_LEFT].x = m_Pos.x + 114;
	m_EquipmentSlots[EQUIPMENT_RING_LEFT].y = m_Pos.y + 150;
	m_EquipmentSlots[EQUIPMENT_RING_LEFT].width = 28;
	m_EquipmentSlots[EQUIPMENT_RING_LEFT].height = 28;
	m_EquipmentSlots[EQUIPMENT_RING_LEFT].dwBgImage = IMAGE_INVENTORY_ITEM_RING;

	m_EquipmentSlots[EQUIPMENT_AMULET].x = m_Pos.x + 54;
	m_EquipmentSlots[EQUIPMENT_AMULET].y = m_Pos.y + 87;
	m_EquipmentSlots[EQUIPMENT_AMULET].width = 28;
	m_EquipmentSlots[EQUIPMENT_AMULET].height = 28;
	m_EquipmentSlots[EQUIPMENT_AMULET].dwBgImage = IMAGE_INVENTORY_ITEM_NECKLACE;

	m_EquipmentSlots[EQUIPMENT_RING_RIGHT].x = m_Pos.x + 54;
	m_EquipmentSlots[EQUIPMENT_RING_RIGHT].y = m_Pos.y + 150;
	m_EquipmentSlots[EQUIPMENT_RING_RIGHT].width = 28;
	m_EquipmentSlots[EQUIPMENT_RING_RIGHT].height = 28;
	m_EquipmentSlots[EQUIPMENT_RING_RIGHT].dwBgImage = IMAGE_INVENTORY_ITEM_RING;
}

void CNewUIViewItem::RenderFrame(int pX, int pY) const
{
	const auto x = static_cast<float>(pX);
	const auto y = static_cast<float>(pY);

	float hW = 190.f;
	float hH = 206.f;
	float AddH = 60.f;

	RenderImage(IMAGE_INVENTORY_BACK, x + 2, y + 3, hW - 4, hH + AddH);
	RenderImage(IMAGE_INVENTORY_BACK_TOP2, x, y, hW, 64.f);
	RenderImage(IMAGE_INVENTORY_BACK_LEFT, x, y + 64, 21.f, 130.f + AddH);
	RenderImage(IMAGE_INVENTORY_BACK_RIGHT, x + 169, y + 64, 21.f, 130.f + AddH);
	RenderImage(IMAGE_INVENTORY_BACK_BOTTOM, x, y + 164 + AddH, hW, 45.f);

	g_pUIForm->RenderBack(x + 15, y + 196, 166, AddH);

	IsToolKit.ThisFont(x + 25, y + 205, HEX_COLOR_WHITE, 0, 100, 0, 1, "Name:");
	IsToolKit.ThisFont(x + 70, y + 205, 0x99D9EAFF, 0, 100, 0, 4, "%s", gViewCharItem.CharViewName);

	IsToolKit.ThisFont(x + 25, y + 220, HEX_COLOR_WHITE, 0, 100, 0, 1, "Reset:");
	IsToolKit.ThisFont(x + 70, y + 220, 0x99D9EAFF, 0, 100, 0, 4, "%s / %s", IsToolKit.QN(gViewCharItem.pPlayerInfo[0]), IsToolKit.QN(CharacterAttribute->ViewMaxReset));

	IsToolKit.ThisFont(x + 25, y + 235, HEX_COLOR_WHITE, 0, 100, 0, 1, "Point:");
	IsToolKit.ThisFont(x + 70, y + 235, 0x99D9EAFF, 0, 100, 0, 4, "%s", IsToolKit.QN(gViewCharItem.pPlayerInfo[1]));
}

void CNewUIViewItem::RenderEquippedItem()
{
	int NumberInfo = -1;

	for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
	{
		EnableAlphaTest();

		RenderImage(m_EquipmentSlots[i].dwBgImage, m_EquipmentSlots[i].x, m_EquipmentSlots[i].y, m_EquipmentSlots[i].width, m_EquipmentSlots[i].height);
		
		DisableAlphaBlend();

		if (gViewCharItem.CharItemInfo[i].Type != 0x1FFF)
		{
			float y = 0.f;
			if (i == EQUIPMENT_ARMOR)
			{
				y = m_EquipmentSlots[i].y - 10.f;
			}
			else
			{
				y = m_EquipmentSlots[i].y;
			}

			glColor4f(1.f, 1.f, 1.f, 1.f);
			g_pNewUISystem->RenderItem3DFree(
				m_EquipmentSlots[i].x + 1,
				y,
				m_EquipmentSlots[i].width - 4,
				m_EquipmentSlots[i].height - 4,
				gViewCharItem.CharItemInfo[i].Type,
				gViewCharItem.CharItemInfo[i].Level,
				gViewCharItem.CharItemInfo[i].Option1,
				gViewCharItem.CharItemInfo[i].ExtOption,
				false, 0.3, true);
		}

		if (SEASON3B::CheckMouseIn(m_EquipmentSlots[i].x, m_EquipmentSlots[i].y, m_EquipmentSlots[i].width, m_EquipmentSlots[i].height))
		{
			NumberInfo = i;
		}
	}

	if (NumberInfo != -1 && gViewCharItem.CharItemInfo[NumberInfo].Type != 0x1FFF)
	{
		RenderItemInfo(m_EquipmentSlots[NumberInfo].x + 20, m_EquipmentSlots[NumberInfo].y, &gViewCharItem.CharItemInfo[NumberInfo], 0, 0, false);
	}
}

void CNewUIViewItem::RenderInventoryDetails() const
{
	EnableAlphaTest();
	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 10, 0x00FFFFFF, 0, 190, 0, 3, GlobalText[3660]);
	DisableAlphaBlend();
}

bool CNewUIViewItem::BtnProcess()
{
	const POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 7 };

	if (IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(INTERFACE_VIEWITEM);
		return true;
	}
	return false;
}

