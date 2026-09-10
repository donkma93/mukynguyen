// NewUIMyInventory.cpp: implementation of the CNewUIMyInventory class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIMyInventory.h"
#include "NewUISystem.h"
#include "NewUICustomMessageBox.h"
#include "CSChaosCastle.h"
#include "GOBoid.h"
#include "ZzzEffect.h"
#include "GIPetManager.h"
#include "CSParts.h"
#include "UIJewelHarmony.h"
#include "CDirection.h"
#include "GMCryWolf1st.h"
#include "GMCryingWolf2nd.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "MixMgr.h"
#include "ZzzLodTerrain.h"
#include "CSQuest.h"
#include "UIGuildInfo.h"
#include "UIManager.h"
#include "CSItemOption.h"
#include "MapManager.h"
#include "w_PetProcess.h"
#include "SocketSystem.h"
#include "w_CursedTemple.h"
#include "PortalMgr.h"
#ifdef CSK_FIX_BLUELUCKYBAG_MOVECOMMAND
#include "Event.h"
#endif // CSK_FIX_BLUELUCKYBAG_MOVECOMMAND
#include "GMUnitedMarketPlace.h"
#include "ChangeRingManager.h"
#include "MonkSystem.h"
#include "CharacterManager.h"
#include "ThangCuoi/WideData.h"
#include "ThangCuoi/CustomWing.h"
#include "ThangCuoi/CustomJewel.h"
#include "ThangCuoi/CustomPet.h"
#include"Util.h"
using namespace SEASON3B;

CNewUIMyInventory::CNewUIMyInventory()
{
	m_pNewUIMng = nullptr;
	m_pNewUI3DRenderMng = nullptr;
	m_pNewInventoryCtrl = nullptr;
	m_Pos.x = m_Pos.y = 0;

	memset(&m_EquipmentSlots, 0, sizeof(EQUIPMENT_ITEM) * MAX_EQUIPMENT_INDEX);
	m_iPointedSlot = -1;

	m_MyShopMode = MYSHOP_MODE_OPEN;
	m_RepairMode = REPAIR_MODE_OFF;
	m_dwStandbyItemKey = 0;

	m_bRepairEnableLevel = false;
	m_bMyShopOpen = false;
	m_bCloseExpand = false;
	m_ChangeFormCoin = false;
}

CNewUIMyInventory::~CNewUIMyInventory()
{
	Release();
}

bool CNewUIMyInventory::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng, int x, int y)
{
	if (nullptr == pNewUIMng || nullptr == pNewUI3DRenderMng || nullptr == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(INTERFACE_INVENTORY, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;
	m_pNewUI3DRenderMng->Add3DRenderObj(this, INVENTORY_CAMERA_Z_ORDER);

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;
	if (false == m_pNewInventoryCtrl->Create(STORAGE_TYPE::INVENTORY, m_pNewUI3DRenderMng, g_pNewItemMng, this, x + 15, y + 200, 8, 8, MAX_EQUIPMENT))
	{
		SAFE_DELETE(m_pNewInventoryCtrl);
		return false;
	}

	SetPos(x, y);
	LoadImages();
	SetButtonInfo();
	Show(false);
	return true;
}

void CNewUIMyInventory::Release()
{
	if (m_pNewUI3DRenderMng)
		m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);

	UnequipAllItems();
	DeleteAllItems();

	UnloadImages();

	SAFE_DELETE(m_pNewInventoryCtrl);

	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->Remove3DRenderObj(this);
		m_pNewUI3DRenderMng = nullptr;
	}
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = nullptr;
	}
}

bool CNewUIMyInventory::EquipItem(int iIndex, BYTE* pbyItemPacket)
{
	if (iIndex >= 0 && iIndex < MAX_EQUIPMENT_INDEX && g_pNewItemMng && CharacterMachine)
	{
		ITEM* pTargetItemSlot = &CharacterMachine->Equipment[iIndex];
		if (pTargetItemSlot->Type > 0)
		{
			UnequipItem(iIndex);
		}

		ITEM* pTempItem = g_pNewItemMng->CreateItem(pbyItemPacket);

		if (nullptr == pTempItem)
		{
			return false;
		}

		if (pTempItem->Type == ITEM_HELPER + 4)
			SendRequestPetInfo(PET_TYPE_DARK_HORSE, 0, iIndex);

		if (pTempItem->Type == ITEM_HELPER + 5)
		{
			CreatePetDarkSpirit(Hero);
			SendRequestPetInfo(PET_TYPE_DARK_SPIRIT, 0, iIndex);
		}

		pTempItem->lineal_pos = iIndex;
		pTempItem->ex_src_type = ITEM_EX_SRC_EQUIPMENT;
		memcpy(pTargetItemSlot, pTempItem, sizeof(ITEM));
		g_pNewItemMng->DeleteItem(pTempItem);

		CreateEquippingEffect(pTargetItemSlot);
	}
	return false;
}
void CNewUIMyInventory::UnequipItem(int iIndex)
{
	if (iIndex >= 0 && iIndex < MAX_EQUIPMENT_INDEX && g_pNewItemMng && CharacterMachine)
	{
		ITEM* pEquippedItem = &CharacterMachine->Equipment[iIndex];

		if (pEquippedItem && pEquippedItem->Type != -1)
		{
			if (pEquippedItem->Type == ITEM_HELPER + 4)
			{
				Hero->InitPetInfo(PET_TYPE_DARK_HORSE);
			}
			else if (pEquippedItem->Type == ITEM_HELPER + 5)
			{
				DeletePet(Hero);
				Hero->InitPetInfo(PET_TYPE_DARK_SPIRIT);
			}

			if (pEquippedItem->Type != ITEM_HELPER + 5)
				DeleteEquippingEffectBug(pEquippedItem);

			pEquippedItem->Type = -1;
			pEquippedItem->Level = 0;
			pEquippedItem->Number = -1;
			pEquippedItem->Option1 = 0;
			pEquippedItem->ExtOption = 0;
			pEquippedItem->SocketCount = 0;
			for (int i = 0; i < MAX_SOCKETS; ++i)
			{
				pEquippedItem->SocketSeedID[i] = SOCKET_EMPTY;
				pEquippedItem->SocketSphereLv[i] = 0;
			}
			pEquippedItem->SocketSeedSetOption = 0;
			DeleteEquippingEffect();
		}
	}
}
void CNewUIMyInventory::UnequipAllItems()
{
	if (CharacterMachine)
	{
		for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
		{
			UnequipItem(i);
		}
	}
}

bool CNewUIMyInventory::IsEquipable(int iIndex, ITEM* pItem)
{
	if (pItem == nullptr)
		return false;

	const ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	bool bEquipable = false;
	if (pItemAttr->RequireClass[gCharacterManager.GetBaseClass(Hero->Class)])
		bEquipable = true;

	else if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_MAGIC && pItemAttr->RequireClass[CLASS_WIZARD]
		&& pItemAttr->RequireClass[CLASS_KNIGHT])
		bEquipable = true;

	const BYTE byFirstClass = gCharacterManager.GetBaseClass(Hero->Class);
	const BYTE byStepClass = gCharacterManager.GetStepClass(Hero->Class);
	if (pItemAttr->RequireClass[byFirstClass] > byStepClass)
	{
		return false;
	}

	if (bEquipable == false)
		return false;

	bEquipable = false;
	if (pItemAttr->m_byItemSlot == iIndex)
		bEquipable = true;

	else if (pItemAttr->m_byItemSlot == EQUIPMENT_WEAPON_RIGHT && iIndex == EQUIPMENT_WEAPON_LEFT)
	{
		if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_KNIGHT || gCharacterManager.GetBaseClass(Hero->Class) == CLASS_MAGIC
			|| gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER)
		{
			if (!pItemAttr->TwoHand)
				bEquipable = true;
#ifdef PBG_FIX_EQUIP_TWOHANDSWORD
			else
			{
				bEquipable = false;
				return false;
			}
#endif //PBG_FIX_EQUIP_TWOHANDSWORD
		}
		else if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_SUMMONER &&
			!(pItem->Type >= ITEM_STAFF && pItem->Type <= ITEM_STAFF + MAX_ITEM_INDEX))
			bEquipable = true;
	}
	else if (pItemAttr->m_byItemSlot == EQUIPMENT_RING_RIGHT && iIndex == EQUIPMENT_RING_LEFT)
		bEquipable = true;

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_ELF)
	{
		//ITEM *r = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
		const ITEM* l = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
		if (iIndex == EQUIPMENT_WEAPON_RIGHT && l->Type != ITEM_BOW + 7
			&& (l->Type >= ITEM_BOW && l->Type < ITEM_BOW + MAX_ITEM_INDEX))
		{
			if (pItem->Type != ITEM_BOW + 15)
				bEquipable = false;
		}
	}

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER)
	{
		if (iIndex == EQUIPMENT_GLOVES)
			bEquipable = false;
		else if (pItemAttr->m_byItemSlot == EQUIPMENT_WEAPON_RIGHT)
			bEquipable = g_CMonkSystem.RageEquipmentWeapon(iIndex, pItem->Type);
	}

	if (bEquipable == false)
		return false;

	const DWORD wStrength = CharacterAttribute->ViewStrength + CharacterAttribute->AddStrength;
	const DWORD wDexterity = CharacterAttribute->ViewDexterity + CharacterAttribute->AddDexterity;
	const DWORD wEnergy = CharacterAttribute->ViewEnergy + CharacterAttribute->AddEnergy;
	const DWORD wVitality = CharacterAttribute->ViewVitality + CharacterAttribute->AddVitality;
	const DWORD wCharisma = CharacterAttribute->ViewLeadership + CharacterAttribute->AddCharisma;
	const DWORD wLevel = CharacterAttribute->Level;

	const int iItemLevel = (pItem->Level >> 3) & 15;

	int iDecNeedStrength = 0, iDecNeedDex = 0;

	extern JewelHarmonyInfo* g_pUIJewelHarmonyinfo;
	if (iItemLevel >= pItem->Jewel_Of_Harmony_OptionLevel)
	{
		StrengthenCapability SC;
		g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC, pItem, 0);

		if (SC.SI_isNB)
		{
			iDecNeedStrength = SC.SI_NB.SI_force;
			iDecNeedDex = SC.SI_NB.SI_activity;
		}
	}
	if (pItem->SocketCount > 0)
	{
		for (int i = 0; i < pItem->SocketCount; ++i)
		{
			if (pItem->SocketSeedID[i] == 38)
			{
				const int iReqStrengthDown = g_SocketItemMgr.GetSocketOptionValue(pItem, i);
				iDecNeedStrength += iReqStrengthDown;
			}
			else if (pItem->SocketSeedID[i] == 39)
			{
				const int iReqDexterityDown = g_SocketItemMgr.GetSocketOptionValue(pItem, i);
				iDecNeedDex += iReqDexterityDown;
			}
		}
	}

	if (pItem->RequireStrength - iDecNeedStrength > wStrength)
		return false;
	if (pItem->RequireDexterity - iDecNeedDex > wDexterity)
		return false;
	if (pItem->RequireEnergy > wEnergy)
		return false;
	if (pItem->RequireVitality > wVitality)
		return false;
	if (pItem->RequireCharisma > wCharisma)
		return false;
	if (pItem->RequireLevel > wLevel)
		return false;

	if (pItem->Type == ITEM_HELPER + 5)
	{
		const PET_INFO* pPetInfo = GetPetInfo(pItem);
		const WORD wRequireCharisma = (185 + (pPetInfo->m_wLevel * 15));
		if (wRequireCharisma > wCharisma)
			return false;
	}

	if (gMapManager.WorldActive == WD_7ATLANSE && (pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 3))
	{
		return false;
	}
	if (pItem->Type == ITEM_HELPER + 2 && gMapManager.WorldActive == WD_10HEAVEN)
	{
		return false;
	}
	if (pItem->Type == ITEM_HELPER + 2 && g_Direction.m_CKanturu.IsMayaScene())
	{
		return false;
	}
	if (gMapManager.InChaosCastle() || (Get_State_Only_Elf()
		&& g_isCharacterBuff((&Hero->Object), eBuff_CrywolfHeroContracted)))
	{
		if ((pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 5) || pItem->Type == ITEM_HELPER + 37 || gCustomPet.GetInfoPetType(pItem->Type) == 5 || gCustomPet.GetInfoPetType(pItem->Type) == 6)
			return false;
	}
	else if ((pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 4 || pItem->Type == ITEM_HELPER + 37 || gCustomPet.GetInfoPetType(pItem->Type) == 5 || gCustomPet.GetInfoPetType(pItem->Type) == 6)
		&& Hero->Object.CurrentAction >= PLAYER_SIT1 && Hero->Object.CurrentAction <= PLAYER_SIT_FEMALE2)
	{
		return false;
	}

	return bEquipable;
}

bool CNewUIMyInventory::InsertItem(int iIndex, BYTE* pbyItemPacket) const
{
	if (m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->AddItem(iIndex, pbyItemPacket);
	}

	return false;
}
void CNewUIMyInventory::DeleteItem(int iIndex) const
{
	if (m_pNewInventoryCtrl)
	{
		ITEM* pItem = m_pNewInventoryCtrl->FindItem(iIndex);
		if (pItem != nullptr)
		{
			m_pNewInventoryCtrl->RemoveItem(pItem);
		}
		else
		{
			CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();
			if (pPickedItem)
			{
				if (pPickedItem->GetOwnerInventory() == m_pNewInventoryCtrl)
				{
					if (pPickedItem->GetSourceLinealPos() == iIndex)
					{
						CNewUIInventoryCtrl::DeletePickedItem();
					}
				}
			}
		}
	}
}
void CNewUIMyInventory::DeleteAllItems() const
{
	if (m_pNewInventoryCtrl)
		m_pNewInventoryCtrl->RemoveAllItems();
}

void CNewUIMyInventory::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->SetPos(m_Pos.x + 15, m_Pos.y + 200);
		SetEquipmentSlotInfo();
	}

	m_BtnExit.SetPos(m_Pos.x	+ 8,	m_Pos.y + 391);
	m_BtnRepair.SetPos(m_Pos.x	+ 44,	m_Pos.y + 391);
	m_BtnMyShop.SetPos(m_Pos.x	+ 80,	m_Pos.y + 391);
	m_BtnExpand.SetPos(m_Pos.x	+ 116,	m_Pos.y + 391);
	m_BtnClear.SetPos(m_Pos.x	+ 152,	m_Pos.y + 391);
}

const POINT& CNewUIMyInventory::GetPos() const
{
	return m_Pos;
}

CNewUIMyInventory::REPAIR_MODE CNewUIMyInventory::GetRepairMode() const
{
	return m_RepairMode;
}

void CNewUIMyInventory::SetRepairMode(bool bRepair)
{
	if (bRepair)
	{
		m_RepairMode = REPAIR_MODE_ON;
		if (m_pNewInventoryCtrl)
		{
			m_pNewInventoryCtrl->SetRepairMode(true);
		}
	}
	else
	{
		m_RepairMode = REPAIR_MODE_OFF;
		if (m_pNewInventoryCtrl)
		{
			m_pNewInventoryCtrl->SetRepairMode(false);
		}
	}
}

bool CNewUIMyInventory::UpdateMouseEvent()
{
	if (m_pNewInventoryCtrl && !m_pNewInventoryCtrl->UpdateMouseEvent())
		return false;

	if (true == EquipmentWindowProcess())
		return false;
	if (true == InventoryProcess())
		return false;

	if (true == BtnProcess())
		return false;

	CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();
	if (pPickedItem && SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(0, 0, GetScreenWidth(), (GetWindowsY() - 51)))
	{
		if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_TRADE) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_STORAGE) == true
			|| g_pNewUISystem->IsVisible(INTERFACE_MYSHOP_INVENTORY) == true
#ifdef LEM_ADD_LUCKYITEM
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND) == true
#endif // LEM_ADD_LUCKYITEM
			|| g_pNewUISystem->IsVisible(INTERFACE_PURCHASESHOP_INVENTORY) == true)
		{
			ResetMouseLButton();
			return false;
		}

		CNewUIInventoryCtrl* pOwner = pPickedItem->GetOwnerInventory();
		if (!pOwner && g_pMyInventoryExt)
		{
			pOwner = g_pMyInventoryExt->GetOwnerOf(pPickedItem);
		}
		if (!pOwner)
		{
			ResetMouseLButton();
			return false;
		}

		ITEM* pItemObj = pPickedItem->GetItem();
		if (pItemObj && pItemObj->Jewel_Of_Harmony_Option != 0)
		{
			g_pChatListBox->AddText("", GlobalText[2217], TYPE_ERROR_MESSAGE);

			ResetMouseLButton();
			return false;
		}
		if (pItemObj && IsHighValueItem(pItemObj) == true)
		{
			g_pChatListBox->AddText("", GlobalText[269], TYPE_ERROR_MESSAGE);
			CNewUIInventoryCtrl::BackupPickedItem();

			ResetMouseLButton();
			return false;
		}
		if (pItemObj && IsDropBan(pItemObj))
		{
			g_pChatListBox->AddText("", GlobalText[1915], TYPE_ERROR_MESSAGE);
			CNewUIInventoryCtrl::BackupPickedItem();

			ResetMouseLButton();
			return false;
		}
		if (pItemObj && pItemObj->Type == ITEM_POTION + 28 && gMapManager.IsCursedTemple() == true)
		{
			ResetMouseLButton();
			return false;
		}
		RenderTerrain(true);
		if (RenderTerrainTile(SelectXF, SelectYF, (int)SelectXF, (int)SelectYF, 1.f, 1, true))
		{
			const int iSourceIndex = pPickedItem->GetSourceLinealPos();
			const int tx = (int)(CollisionPosition[0] / TERRAIN_SCALE);
			const int ty = (int)(CollisionPosition[1] / TERRAIN_SCALE);
			if (pPickedItem->GetOwnerInventory() == m_pNewInventoryCtrl)
			{
				if (Hero->Dead == false)
				{
					SendRequestDropItem(iSourceIndex, tx, ty);
				}
			}
			else if (pItemObj && pItemObj->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
			{
				SendRequestDropItem(iSourceIndex, tx, ty);
			}
			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			ResetMouseLButton();
			return false;
		}
	}

	g_csItemOption.SetViewOptionList(false);

	if (CheckMouseIn(m_Pos.x, m_Pos.y + 35, INVENTORY_WIDTH * 0.5f, 15) == true)
	{
		g_csItemOption.SetViewOptionList(true);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if (IsPress(VK_RBUTTON))
		{
			ResetMouseRButton();
			return false;
		}

		if (IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}

	return true;
}

bool CNewUIMyInventory::UpdateKeyEvent()
{
	if (!g_pNewUISystem->IsVisible(INTERFACE_INVENTORY))
	{
		return true;
	}

	if (IsPress(VK_ESCAPE) == true)
	{
		if (g_pNPCShop->IsSellingItem() == false)
		{
			g_pNewUISystem->Hide(INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if (IsPress('L') == true)
	{
		if (m_bRepairEnableLevel == true && g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) == false
			&& g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY) == false
#ifdef LEM_ADD_LUCKYITEM
			&& g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND) == false
#endif // LEM_ADD_LUCKYITEM
			)
		{
			ToggleRepairMode();

			return false;
		}
	}

	if (CanOpenMyShopInterface() == true && IsPress('S'))
	{
		if (m_bMyShopOpen)
		{
			if (m_MyShopMode == MYSHOP_MODE_OPEN)
			{
				ChangeMyShopButtonStateClose();
			}
			else if (m_MyShopMode == MYSHOP_MODE_CLOSE)
			{
				ChangeMyShopButtonStateOpen();
			}
			g_pNewUISystem->Toggle(INTERFACE_MYSHOP_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if (IsPress('K'))
	{
		g_pNewUISystem->Toggle(INTERFACE_INVENTORY_EXT);
		PlayBuffer(SOUND_CLICK01);

		return false;
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT) == false)
	{
		return true;
	}

	if (IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 11, m_Pos.y + 363, 190, 27))
	{
		m_ChangeFormCoin = !m_ChangeFormCoin;
	}

	if (IsRepeat(VK_CONTROL))
	{
		int iHotKey = -1;
		if (IsPress('Q'))
		{
			iHotKey = HOTKEY_Q;
		}
		else if (IsPress('W'))
		{
			iHotKey = HOTKEY_W;
		}
		else if (IsPress('E'))
		{
			iHotKey = HOTKEY_E;
		}
		else if (IsPress('R'))
		{
			iHotKey = HOTKEY_R;
		}

		if (iHotKey != -1)
		{
			const ITEM* pItem = m_pNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);
			if (pItem == nullptr)
			{
				return false;
			}

			if (CanRegisterItemHotKey(pItem->Type) == true)
			{
				const int iItemLevel = (pItem->Level >> 3) & 15;
				g_pMainFrame->SetItemHotKey(iHotKey, pItem->Type, iItemLevel);
				return false;
			}

		}
	}

	return true;
}

bool CNewUIMyInventory::Update()
{

	if (m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->Update())
	{
		return false;
	}

	if (IsVisible())
	{
		m_iPointedSlot = -1;
		for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
		{
			if (CheckMouseIn(m_EquipmentSlots[i].x + 1, m_EquipmentSlots[i].y,
				m_EquipmentSlots[i].width - 4, m_EquipmentSlots[i].height - 4))
			{
				m_iPointedSlot = i;
				break;
			}
		}
	}
	return true;
}

bool CNewUIMyInventory::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderInventoryDetails();
	RenderSetOption();
	RenderSocketOption();
	RenderButtons();

	if (m_pNewInventoryCtrl)
		m_pNewInventoryCtrl->Render();

	RenderEquippedItem();
	DisableAlphaBlend();
	return true;
}

void CNewUIMyInventory::RenderSetOption()
{
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	if (g_csItemOption.GetSetOptionANum() > 0 || g_csItemOption.GetSetOptionBNum() > 0)
	{
		g_pRenderText->SetTextColor(255, 204, 25, 255);
	}
	else
	{
		g_pRenderText->SetTextColor(100, 100, 100, 255);
	}
	//ACuoi Vi tri SetOption
	unicode::t_char strText[128];
	unicode::_sprintf(strText, "[%s]", GlobalText[989]);
	g_pRenderText->RenderText((m_Pos.x + INVENTORY_WIDTH * 0.2f) - 20, m_Pos.y + 35, strText, INVENTORY_WIDTH * 0.3f, 0, RT3_SORT_LEFT);

	if (g_csItemOption.IsViewOptionList() == true)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, -1, ITEM_SET_OPTION);
	}

}
void CNewUIMyInventory::RenderSocketOption()
{
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	if (g_SocketItemMgr.IsSocketSetOptionEnabled())
	{
		g_pRenderText->SetTextColor(255, 204, 25, 255);
	}
	else
	{
		g_pRenderText->SetTextColor(100, 100, 100, 255);
	}

	unicode::t_char strText[128];
	unicode::_sprintf(strText, "[%s]", GlobalText[2651]);
	//ACuoi Vi tri SocketOption
	g_pRenderText->RenderText((m_Pos.x + INVENTORY_WIDTH * 0.4f) + 5, m_Pos.y + 35, strText, INVENTORY_WIDTH * 0.5f, 0, RT3_SORT_RIGHT);
	if (CheckMouseIn(m_Pos.x + INVENTORY_WIDTH * 0.5f, m_Pos.y + 35, INVENTORY_WIDTH * 0.5f, 15) == true)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, -1, ITEM_SOCKET_SET_OPTION);
	}
}

void CNewUIMyInventory::Render3D()
{
	for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
	{
		const ITEM* pEquippedItem = &CharacterMachine->Equipment[i];
		if (pEquippedItem->Type >= 0)
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
			RenderItem3D(
				m_EquipmentSlots[i].x + 1,
				y,
				m_EquipmentSlots[i].width - 4,
				m_EquipmentSlots[i].height - 4,
				pEquippedItem->Type,
				pEquippedItem->Level,
				pEquippedItem->Option1,
				pEquippedItem->ExtOption,
				false);
		}
	}

}

bool CNewUIMyInventory::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

void CNewUIMyInventory::OpenningProcess()
{
	SetRepairMode(false);

	m_MyShopMode = MYSHOP_MODE_OPEN;
	ChangeMyShopButtonStateOpen();

	const WORD wLevel = CharacterAttribute->Level;

	if (wLevel >= 50)
	{
		m_bRepairEnableLevel = true;
	}
	else
	{
		m_bRepairEnableLevel = false;
	}

	if (wLevel >= 6)
	{
		m_bMyShopOpen = true;
	}
	else
	{
		m_bMyShopOpen = false;
	}
	if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP))
	{
		m_bCloseExpand = false;
	}
	else
	{
		m_bCloseExpand = true;
	}
	if (g_QuestMng.IsIndexInCurQuestIndexList(0x1000F))
	{
		if (g_QuestMng.IsEPRequestRewardState(0x1000F))
		{
			SendSatisfyQuestRequestFromClient(0x1000F);
			g_QuestMng.SetEPRequestRewardState(0x1000F, false);
		}
	}
}

void CNewUIMyInventory::ClosingProcess()
{
	m_pNewInventoryCtrl->BackupPickedItem();
	RepairEnable = 0;
	SetRepairMode(false);
}

float CNewUIMyInventory::GetLayerDepth()
{
	return 4.2f;
}

CNewUIInventoryCtrl* CNewUIMyInventory::GetInventoryCtrl() const
{
	return m_pNewInventoryCtrl;
}

ITEM* CNewUIMyInventory::FindItem(int iLinealPos) const
{
	if (m_pNewInventoryCtrl)
	{
		if (ITEM* pItem = m_pNewInventoryCtrl->FindItem(iLinealPos))
			return pItem;
	}

	if (g_pMyInventoryExt)
	{
		if (ITEM* pItem = g_pMyInventoryExt->FindItem(iLinealPos))
			return pItem;
	}

	return nullptr;
}

ITEM* CNewUIMyInventory::FindItemByKey(DWORD dwKey) const
{
	if (m_pNewInventoryCtrl)
		return m_pNewInventoryCtrl->FindItemByKey(dwKey);
	return nullptr;
}

int CNewUIMyInventory::FindItemIndex(short int siType, int iLevel) const
{
	if (m_pNewInventoryCtrl)
	{
		int index = m_pNewInventoryCtrl->FindItemIndex(siType, iLevel);
		if (index != -1)
			return index;
	}

	if (g_pMyInventoryExt)
	{
		int index = g_pMyInventoryExt->FindItemIndex(siType, iLevel);
		if (index != -1)
			return index;
	}

	return -1;
}

int CNewUIMyInventory::FindItemReverseIndex(short sType, int iLevel) const
{
	if (m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->FindItemReverseIndex(sType, iLevel);
	}

	return -1;
}

int CNewUIMyInventory::FindEmptySlot(IN int cx, IN int cy) const
{
	if (m_pNewInventoryCtrl)
		return m_pNewInventoryCtrl->FindEmptySlot(cx, cy, false);
	return -1;
}

int CNewUIMyInventory::FindEmptySlot(ITEM* pItem) const
{
	const ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	if (m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->FindEmptySlot(pItemAttr->Width, pItemAttr->Height, false);
	}

	return -1;
}

void CNewUIMyInventory::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		auto* pMyInventory = (CNewUIMyInventory*)(pClass);

		if (dwParamB == ITEM_SET_OPTION)
		{
			g_csItemOption.RenderSetOptionList(pMyInventory->GetPos().x, pMyInventory->GetPos().y + 7);
		}
		else if (dwParamB == ITEM_SOCKET_SET_OPTION)
		{
			g_SocketItemMgr.RenderToolTipForSocketSetOption(pMyInventory->GetPos().x, pMyInventory->GetPos().y);
		}
		else
		{
			pMyInventory->RenderItemToolTip(dwParamA);
		}
	}
}

void CNewUIMyInventory::SetStandbyItemKey(DWORD dwItemKey)
{
	m_dwStandbyItemKey = dwItemKey;
}

DWORD CNewUIMyInventory::GetStandbyItemKey() const
{
	return m_dwStandbyItemKey;
}
//ACuoi Fix use Item Ext Inventory - Chaos
int CNewUIMyInventory::GetStandbyItemIndex() const
{
	if (ITEM* pItem = GetStandbyItem())
	{
		if (g_pMyInventoryExt)
		{
			CNewUIInventoryCtrl* pExtInv = g_pMyInventoryExt->FindInventoryByItemKey(m_dwStandbyItemKey);
			if (pExtInv)
			{
				int idx = pExtInv->GetIndexByItem(pItem);
				if (idx != -1)
					return idx;
			}
		}
		return m_pNewInventoryCtrl->GetIndexByItem(pItem);
	}
	return -1;
}

ITEM* CNewUIMyInventory::GetStandbyItem() const
{
	if (m_pNewInventoryCtrl)
	{
		ITEM* pItem = m_pNewInventoryCtrl->FindItemByKey(m_dwStandbyItemKey);
		if (pItem)
			return pItem;
	}

	if (g_pMyInventoryExt)
	{
		CNewUIInventoryCtrl* pExtInv = g_pMyInventoryExt->FindInventoryByItemKey(m_dwStandbyItemKey);
		if (pExtInv)
		{
			ITEM* pItem = pExtInv->FindItemByKey(m_dwStandbyItemKey);
			if (pItem)
				return pItem;
		}
	}

	return nullptr;
}
// End
void CNewUIMyInventory::CreateEquippingEffect(ITEM* pItem)
{
	SetCharacterClass(Hero);
	OBJECT* pHeroObject = &Hero->Object;
	if (false == gMapManager.InChaosCastle())
	{
		switch (pItem->Type)
		{
		case ITEM_HELPER:
			CreateMount(MODEL_HELPER, pHeroObject->Position, pHeroObject);
			break;
		case ITEM_HELPER + 2:
			CreateMount(MODEL_UNICON, pHeroObject->Position, pHeroObject);
			if (!Hero->SafeZone)
				CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
			break;
		case ITEM_HELPER + 3:
			CreateMount(MODEL_PEGASUS, pHeroObject->Position, pHeroObject);
			if (!Hero->SafeZone)
				CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
			break;
		case ITEM_HELPER + 4:
			CreateMount(MODEL_DARK_HORSE, pHeroObject->Position, pHeroObject);
			if (!Hero->SafeZone)
				CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
			break;
		case ITEM_HELPER + 37:
			Hero->Helper.Option1 = pItem->Option1;
			if (pItem->Option1 == 0x01)
			{
				CreateMount(MODEL_FENRIR_BLACK, pHeroObject->Position, pHeroObject);
			}
			else if (pItem->Option1 == 0x02)
			{
				CreateMount(MODEL_FENRIR_BLUE, pHeroObject->Position, pHeroObject);
			}
			else if (pItem->Option1 == 0x04)
			{
				CreateMount(MODEL_FENRIR_GOLD, pHeroObject->Position, pHeroObject);
			}
			else
			{
				CreateMount(MODEL_FENRIR_RED, pHeroObject->Position, pHeroObject);
			}

			if (!Hero->SafeZone)
			{
				CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
			}
			break;
		case ITEM_HELPER + 64:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 64, pHeroObject->Position, Hero);
			break;
		case ITEM_HELPER + 65:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 65, pHeroObject->Position, Hero);
			break;
		case ITEM_HELPER + 67:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 67, pHeroObject->Position, Hero);
			break;
		case ITEM_HELPER + 80:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 80, pHeroObject->Position, Hero);
			break;
		case ITEM_HELPER + 106:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 106, pHeroObject->Position, Hero);
			break;
		case ITEM_HELPER + 123:
			ThePetProcess().CreatePet(pItem->Type, MODEL_HELPER + 123, pHeroObject->Position, Hero);
			break;
		}
		//===Custom Pet
		if (gCustomPet.CheckCustomPetByItem(pItem->Type))
		{
			CreateMount(pItem->Type + MODEL_ITEM, pHeroObject->Position, pHeroObject); // KIem tra
		}
	}
	if (Hero->EtcPart <= 0 || Hero->EtcPart > 3)
	{
		if (pItem->Type == ITEM_HELPER + 20 && (pItem->Level >> 3) == 3)
		{
			DeleteParts(Hero);
			Hero->EtcPart = PARTS_LION;
		}
	}
	if (pItem->Type == ITEM_WING + 39 
		|| pItem->Type == ITEM_HELPER + 30 
		|| pItem->Type == ITEM_WING + 130 
		|| (pItem->Type >= ITEM_WING + 49 && pItem->Type <= ITEM_WING + 50) 
		|| (pItem->Type == ITEM_WING + 135) || (pItem->Type == ITEM_WING + 154) 
		|| pItem->Type == ITEM_WING + 40 
		|| pItem->Type == ITEM_WING + 155 
		|| pItem->Type == ITEM_WING + 160 
		|| pItem->Type == ITEM_WING + 162 
		)
	{
		DeleteCloth(Hero, &Hero->Object);
	}
}

void CNewUIMyInventory::DeleteEquippingEffectBug(ITEM* pItem)
{
	if (ThePetProcess().IsPet(pItem->Type) == true)
	{
		ThePetProcess().DeletePet(Hero, pItem->Type);
	}

	switch (pItem->Type)
	{
	case ITEM_HELPER + 30:
	case ITEM_WING + 39:
	case ITEM_WING + 40:
	case ITEM_WING + 130:
	case ITEM_WING + 49:
	case ITEM_WING + 50:
	case ITEM_WING + 135:
	case ITEM_WING + 155:
	case ITEM_WING + 160:
	case ITEM_WING + 162:
		DeleteCloth(Hero, &Hero->Object);
		return;
	}

	if (IsMount(pItem) == true)
	{
		DeleteMount(&Hero->Object);
	}
}

void CNewUIMyInventory::DeleteEquippingEffect()
{
	if (Hero->EtcPart < PARTS_ATTACK_TEAM_MARK)
	{
		DeleteParts(Hero);
		if (Hero->EtcPart > 3)
		{
			Hero->EtcPart = 0;
		}
	}

	SetCharacterClass(Hero);
}

void CNewUIMyInventory::SetEquipmentSlotInfo()
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

void CNewUIMyInventory::SetButtonInfo()
{
	m_BtnExit.ChangeButtonImgState(true, IMAGE_INVENTORY_EXIT_BTN, false);
	m_BtnExit.ChangeButtonInfo(m_Pos.x + 8, m_Pos.y + 391, 36, 29);
	m_BtnExit.ChangeToolTipText(GlobalText[225], true);

	m_BtnRepair.ChangeButtonImgState(true, IMAGE_INVENTORY_REPAIR_BTN, false);
	m_BtnRepair.ChangeButtonInfo(m_Pos.x + 44, m_Pos.y + 391, 36, 29);
	m_BtnRepair.ChangeToolTipText(GlobalText[233], true);

	m_BtnMyShop.ChangeButtonImgState(true, IMAGE_INVENTORY_MYSHOP_OPEN_BTN, false);
	m_BtnMyShop.ChangeButtonInfo(m_Pos.x + 80, m_Pos.y + 391, 36, 29);
	m_BtnMyShop.ChangeToolTipText(GlobalText[1125], true);

	m_BtnExpand.ChangeButtonImgState(true, IMAGE_INVENTORY_EXPAND_BTN, false);
	m_BtnExpand.ChangeButtonInfo(m_Pos.x + 116, m_Pos.y + 391, 36, 29);
	m_BtnExpand.ChangeToolTipText(GlobalText[3322], true);

	m_BtnClear.ChangeButtonImgState(true, IMAGE_INVENTORY_CLEARN_BTN, false);
	m_BtnClear.ChangeButtonInfo(m_Pos.x + 152, m_Pos.y + 391, 36, 29);
	m_BtnClear.ChangeToolTipText(GlobalText[3191], true);
}

void CNewUIMyInventory::LoadImages() const
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_INVENTORY_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_INVENTORY_BACK_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back04.tga", IMAGE_INVENTORY_BACK_TOP2, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_INVENTORY_BACK_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_INVENTORY_BACK_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_INVENTORY_BACK_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_boots.tga", IMAGE_INVENTORY_ITEM_BOOT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_cap.tga", IMAGE_INVENTORY_ITEM_HELM, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_fairy.tga", IMAGE_INVENTORY_ITEM_FAIRY, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_wing.tga", IMAGE_INVENTORY_ITEM_WING, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_weapon(L).tga", IMAGE_INVENTORY_ITEM_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_weapon(R).tga", IMAGE_INVENTORY_ITEM_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_upper.tga", IMAGE_INVENTORY_ITEM_ARMOR, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_gloves.tga", IMAGE_INVENTORY_ITEM_GLOVES, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_lower.tga", IMAGE_INVENTORY_ITEM_PANTS, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_ring.tga", IMAGE_INVENTORY_ITEM_RING, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_necklace.tga", IMAGE_INVENTORY_ITEM_NECKLACE, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_money.tga", IMAGE_INVENTORY_MONEY, GL_LINEAR);
	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_INVENTORY_EXIT_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_repair_00.tga", IMAGE_INVENTORY_REPAIR_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_expansion_btn.tga", IMAGE_INVENTORY_EXPAND_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_Bt_openshop.tga", IMAGE_INVENTORY_MYSHOP_OPEN_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_Bt_closeshop.tga", IMAGE_INVENTORY_MYSHOP_CLOSE_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_clear_inv.tga", IMAGE_INVENTORY_CLEARN_BTN, GL_LINEAR);
	LoadBitmap("ACuoi\\iMoney.tga", BITMAP_START_SPK_BIT + 6, GL_LINEAR);
}

void CNewUIMyInventory::UnloadImages()
{
	DeleteBitmap(IMAGE_INVENTORY_MYSHOP_CLOSE_BTN);
	DeleteBitmap(IMAGE_INVENTORY_MYSHOP_OPEN_BTN);
	DeleteBitmap(IMAGE_INVENTORY_REPAIR_BTN);
	DeleteBitmap(IMAGE_INVENTORY_EXIT_BTN);
	DeleteBitmap(IMAGE_INVENTORY_MONEY);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_NECKLACE);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_RING);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_PANTS);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_GLOVES);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_ARMOR);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_RIGHT);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_LEFT);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_WING);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_FAIRY);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_HELM);
	DeleteBitmap(IMAGE_INVENTORY_ITEM_BOOT);
	DeleteBitmap(IMAGE_INVENTORY_BACK_BOTTOM);
	DeleteBitmap(IMAGE_INVENTORY_BACK_RIGHT);
	DeleteBitmap(IMAGE_INVENTORY_BACK_LEFT);
	DeleteBitmap(IMAGE_INVENTORY_BACK_TOP2);
	DeleteBitmap(IMAGE_INVENTORY_BACK_TOP);
	DeleteBitmap(IMAGE_INVENTORY_BACK);
	DeleteBitmap(IMAGE_INVENTORY_EXPAND_BTN);
	DeleteBitmap(IMAGE_INVENTORY_CLEARN_BTN);
	DeleteBitmap(BITMAP_START_SPK_BIT + 6);
}

void CNewUIMyInventory::RenderFrame() const
{
	const auto x = static_cast<float>(m_Pos.x);
	const auto y = static_cast<float>(m_Pos.y);

	RenderImage(IMAGE_INVENTORY_BACK, x + 2, y + 3, INVENTORY_WIDTH - 4, INVENTORY_HEIGHT - 3);
	RenderImage(IMAGE_INVENTORY_BACK_TOP2, x, y, 190.f, 64.f);
	RenderImage(IMAGE_INVENTORY_BACK_LEFT, x, y + 64, 21.f, 320.f);
	RenderImage(IMAGE_INVENTORY_BACK_RIGHT, x + INVENTORY_WIDTH - 21, y + 64, 21.f, 320.f);
	RenderImage(IMAGE_INVENTORY_BACK_BOTTOM, x, y + INVENTORY_HEIGHT - 45, 190.f, 45.f);
}

void CNewUIMyInventory::RenderEquippedItem()
{
	for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
	{
		if (i == EQUIPMENT_HELM)
		{
			if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_MAGIC)
			{
				continue;
			}
		}
		if ((i == EQUIPMENT_GLOVES) && (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER))
			continue;

		EnableAlphaTest();

		RenderImage(m_EquipmentSlots[i].dwBgImage, m_EquipmentSlots[i].x, m_EquipmentSlots[i].y,
			m_EquipmentSlots[i].width, m_EquipmentSlots[i].height);
		DisableAlphaBlend();

		ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[i];
		if (pEquipmentItemSlot->Type != -1)
		{
			ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pEquipmentItemSlot->Type];
			const int iLevel = (pEquipmentItemSlot->Level >> 3) & 15;
			const int iMaxDurability = calcMaxDurability(pEquipmentItemSlot, pItemAttr, iLevel);

			// 용사/전사의반지 예외처리
			if (i == EQUIPMENT_RING_LEFT || i == EQUIPMENT_RING_RIGHT)
			{
				if (pEquipmentItemSlot->Type == ITEM_HELPER + 20 && iLevel == 1
					|| iLevel == 2)
				{
					continue;
				}
			}

			if ((pEquipmentItemSlot->bPeriodItem == true) && (pEquipmentItemSlot->bExpiredPeriod == false))
				continue;

			if (pEquipmentItemSlot->Durability <= 0)
				glColor4f(1.f, 0.f, 0.f, 0.25f);
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability * 0.2f))
				glColor4f(1.f, 0.15f, 0.f, 0.25f);
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability * 0.3f))
				glColor4f(1.f, 0.5f, 0.f, 0.25f);
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability * 0.5f))
				glColor4f(1.f, 1.f, 0.f, 0.25f);
			else if (IsEquipable(i, pEquipmentItemSlot) == false)
				glColor4f(1.f, 0.f, 0.f, 0.25f);
			else
			{
				continue;
			}

			EnableAlphaTest();
			RenderColor(m_EquipmentSlots[i].x + 1, m_EquipmentSlots[i].y, m_EquipmentSlots[i].width - 4, m_EquipmentSlots[i].height - 4);
			EndRenderColor();
		}
	}

	if (CNewUIInventoryCtrl::GetPickedItem() && m_iPointedSlot != -1)
	{
		ITEM* pItemObj = CNewUIInventoryCtrl::GetPickedItem()->GetItem();
		const ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[m_iPointedSlot];
		if (pItemObj && (pEquipmentItemSlot->Type != -1 || false == IsEquipable(m_iPointedSlot, pItemObj))
			&& !((gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER) && (m_iPointedSlot == EQUIPMENT_GLOVES)))
		{
			glColor4f(0.9f, 0.1f, 0.1f, 0.4f);
			EnableAlphaTest();
			RenderColor(m_EquipmentSlots[m_iPointedSlot].x + 1, m_EquipmentSlots[m_iPointedSlot].y,
				m_EquipmentSlots[m_iPointedSlot].width - 4, m_EquipmentSlots[m_iPointedSlot].height - 4);
			EndRenderColor();
		}
	}

	if (m_iPointedSlot != -1 && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, m_iPointedSlot, 0);
	}
}

void CNewUIMyInventory::RenderButtons()
{
	EnableAlphaTest();

	if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_TRADE) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY) == false
#ifdef LEM_ADD_LUCKYITEM
		&& g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND) == false
#endif // LEM_ADD_LUCKYITEM
		&& g_pNewUISystem->IsVisible(INTERFACE_STORAGE) == false)
	{
		if (m_bRepairEnableLevel == true)
		{
			m_BtnRepair.Render();
		}
		if (m_bMyShopOpen == true)
		{
			m_BtnMyShop.Render();
		}
	}
	m_BtnClear.Render();
	m_BtnExit.Render();
	m_BtnExpand.Render();
	DisableAlphaBlend();
}

void CNewUIMyInventory::RenderInventoryDetails() const
{
	EnableAlphaTest();

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[223], INVENTORY_WIDTH, 0, RT3_SORT_CENTER);

	if (!m_ChangeFormCoin)
	{
		RenderImage(IMAGE_INVENTORY_MONEY, m_Pos.x + 11, m_Pos.y + 364, 170.f, 26.f);
		const DWORD dwZen = CharacterMachine->Gold;
		unicode::t_char Text[256] = { 0, };
		ConvertGold(dwZen, Text);
		g_pRenderText->SetTextColor(getGoldColor(dwZen));
		g_pRenderText->RenderText((int)m_Pos.x + 50, (int)m_Pos.y + 371, Text);
	}
	else
	{
		RenderBitmap(BITMAP_START_SPK_BIT + 6, m_Pos.x + 11, m_Pos.y + 363, 100.f, 16.f, 0, 0, 170.f / 256.f, 21.f / 32.f, 1, 1, 0.0);
		RenderBitmap(BITMAP_START_SPK_BIT + 6, m_Pos.x + 98, m_Pos.y + 363, 100.f, 16.f, 0, 0, 170.f / 256.f, 21.f / 32.f, 1, 1, 0.0);
		RenderBitmap(BITMAP_START_SPK_BIT + 6, m_Pos.x + 11, m_Pos.y + 377, 100.f, 16.f, 0, 0, 170.f / 256.f, 21.f / 32.f, 1, 1, 0.0);
		RenderBitmap(BITMAP_START_SPK_BIT + 6, m_Pos.x + 98, m_Pos.y + 377, 100.f, 16.f, 0, 0, 170.f / 256.f, 21.f / 32.f, 1, 1, 0.0);

		unicode::t_char Text[256] = { 0, };

		const DWORD dwZen = CharacterMachine->Gold;
		ConvertGold(dwZen, Text);
		g_pRenderText->SetTextColor(getGoldColor(dwZen));
		g_pRenderText->RenderText((int)m_Pos.x + 37, (int)m_Pos.y + 366, Text, 54, 0, 4);

		const DWORD dwCoinC = pGetCoin.ThisCoin[0];
		g_pRenderText->SetTextColor(getGoldColor(dwCoinC));
		ConvertGold(dwCoinC, Text);
		g_pRenderText->RenderText((int)m_Pos.x + 126, (int)m_Pos.y + 366, Text, 54, 0, 4);

		const DWORD dwCoinP = pGetCoin.ThisCoin[1];
		g_pRenderText->SetTextColor(getGoldColor(dwCoinP));
		ConvertGold(dwCoinP, Text);
		g_pRenderText->RenderText((int)m_Pos.x + 37, (int)m_Pos.y + 380, Text, 54, 0, 4);

		const DWORD dwCoinG = pGetCoin.ThisCoin[2];
		g_pRenderText->SetTextColor(getGoldColor(dwCoinG));
		ConvertGold(dwCoinG, Text);
		g_pRenderText->RenderText((int)m_Pos.x + 126, (int)m_Pos.y + 380, Text, 54, 0, 4);

		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText((int)m_Pos.x + 20, (int)m_Pos.y + 366, "Zen:", 20, 0, 1);
		g_pRenderText->RenderText((int)m_Pos.x + 110, (int)m_Pos.y + 366, "WC:", 20, 0, 1);
		g_pRenderText->RenderText((int)m_Pos.x + 20, (int)m_Pos.y + 380, "WP:", 20, 0, 1);
		g_pRenderText->RenderText((int)m_Pos.x + 110, (int)m_Pos.y + 380, "GP:", 20, 0, 1);
	}
	DisableAlphaBlend();
}

bool CNewUIMyInventory::EquipmentWindowProcess()
{
	if (m_iPointedSlot != -1 && IsRelease(VK_LBUTTON)) // Move Up item to Slot Equipment
	{
		if (CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem())
		{
			ITEM* pItemObj = pPickedItem->GetItem();
			const int iSourceIndex = pPickedItem->GetSourceLinealPos();
			const int iTargetIndex = m_iPointedSlot;
			if (pItemObj->bPeriodItem && pItemObj->bExpiredPeriod)
			{
				g_pChatListBox->AddText("", GlobalText[2285], SEASON3B::TYPE_ERROR_MESSAGE);
				CNewUIInventoryCtrl::BackupPickedItem();

				ResetMouseLButton();
				return false;
			}

			ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[iTargetIndex];
			if (pEquipmentItemSlot && pEquipmentItemSlot->Type != -1)
			{
				return true;
			}

			if (g_ChangeRingMgr->CheckChangeRing(pPickedItem->GetItem()->Type))
			{
				ITEM* pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
				ITEM* pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

				if (g_ChangeRingMgr->CheckChangeRing(pItemRingLeft->Type) || g_ChangeRingMgr->CheckChangeRing(pItemRingRight->Type))
				{
					g_pChatListBox->AddText("", GlobalText[2285], TYPE_ERROR_MESSAGE);
					CNewUIInventoryCtrl::BackupPickedItem();

					ResetMouseLButton();
					return false;
				}
			}

			if (IsEquipable(iTargetIndex, pItemObj))
			{
				const STORAGE_TYPE sourceType = pPickedItem->GetSourceStorageType();

				if (sourceType == STORAGE_TYPE::INVENTORY && iSourceIndex == iTargetIndex)
				{
					CNewUIInventoryCtrl::BackupPickedItem();
				}
				else
				{
					SendRequestEquipmentItem(sourceType, iSourceIndex, pItemObj, STORAGE_TYPE::INVENTORY, iTargetIndex);
					return true;
				}
			}
		}
		else // pPickedItem == NULL
		{
			if (GetRepairMode() == REPAIR_MODE_ON)
			{
				ITEM* pEquippedItem = &CharacterMachine->Equipment[m_iPointedSlot];

				if (pEquippedItem == NULL)
				{
					return true;
				}

				if (IsRepairBan(pEquippedItem) == true)
				{
					return true;
				}

				if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) && g_pNPCShop->IsRepairShop())
				{
					SendRequestRepair(m_iPointedSlot, 0);
				}
				else if (m_bRepairEnableLevel == true)
				{
					SendRequestRepair(m_iPointedSlot, 1);
				}

				return true;
			}

			ITEM* pEquippedItem = &CharacterMachine->Equipment[m_iPointedSlot];
			if (pEquippedItem->Type >= 0)
			{
				if (gMapManager.WorldActive == WD_10HEAVEN)
				{
					const ITEM* pEquippedPetItem = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
					bool bPicked = true;

					if (m_iPointedSlot == EQUIPMENT_HELPER || m_iPointedSlot == EQUIPMENT_WING)
					{
						if (((m_iPointedSlot == EQUIPMENT_HELPER) && !gCharacterManager.IsEquipedWing()))
						{
							bPicked = false;
						}
						else if (((m_iPointedSlot == EQUIPMENT_WING) && !((pEquippedPetItem->Type == ITEM_HELPER + 3) || (pEquippedPetItem->Type == ITEM_HELPER + 4) || (pEquippedPetItem->Type == ITEM_HELPER + 37) || gCustomPet.GetInfoPetType(pEquippedPetItem->Type) == 5 || gCustomPet.GetInfoPetType(pEquippedPetItem->Type) == 6))
							)
						{
							bPicked = false;
						}
					}

					if (bPicked == true)
					{
						if (CNewUIInventoryCtrl::CreatePickedItem(nullptr, pEquippedItem))
						{
							UnequipItem(m_iPointedSlot);
						}
					}
				}
				else
				{
					if (CNewUIInventoryCtrl::CreatePickedItem(nullptr, pEquippedItem))
					{
						UnequipItem(m_iPointedSlot);
					}
				}
			}
		}
	}

	if (IsRelease(VK_RBUTTON)) // Move down item to Inventory
	{
		const CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

		const int iSourceIndex = m_iPointedSlot;
		if (GetRepairMode() != REPAIR_MODE_ON && EquipmentItem == false
			&& pPickedItem == nullptr
			&& iSourceIndex != -1)
		{
			ResetMouseRButton();

			ITEM* pEquippedItem = &CharacterMachine->Equipment[iSourceIndex];

			if (pEquippedItem->Type >= 0)
			{
				const int emptySlotIndex = FindEmptySlot(pEquippedItem);

				if (emptySlotIndex != -1)
				{
					if (CNewUIInventoryCtrl::CreatePickedItem(nullptr, pEquippedItem))
					{
						CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();
						UnequipItem(iSourceIndex);
						pPickedItem->HidePickedItem();
					}

					SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, iSourceIndex, pEquippedItem, STORAGE_TYPE::INVENTORY, emptySlotIndex);
					return true;
				}
			}
		}
	}
	
	return false;
}

bool CNewUIMyInventory::RepairItemAtMousePoint(CNewUIInventoryCtrl* targetControl)
{
	ITEM* pItem = targetControl->FindItemAtPt(MouseX, MouseY);
	if (pItem == nullptr)
	{
		return true;
	}

	if (IsRepairBan(pItem) == true)
	{
		return true;
	}

	int iIndex = targetControl->GetIndex(pItem->x, pItem->y);

	if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) && g_pNPCShop->IsRepairShop())
	{
		SendRequestRepair(iIndex, 0);
	}
	else
	{
		SendRequestRepair(iIndex, 1);
	}

	return true;
}

bool CNewUIMyInventory::ApplyJewels(CNewUIInventoryCtrl* targetControl, CNewUIPickedItem* pPickedItem, ITEM* pPickItem, const int iSourceIndex, const int iTargetIndex)
{
	auto pItem = targetControl->FindItem(iTargetIndex);
	if (!pItem)
	{
		return false;
	}


	const int	iType = pItem->Type;
	const int	iLevel = (pItem->Level >> 3) & 15;
	const int	iDurability = pItem->Durability;

	bool bSuccess = true;

	if (iType > ITEM_WING + 6
		&& iType != ITEM_HELPER + 30
		&& !(iType >= ITEM_WING + 36 && iType <= ITEM_WING + 43 || gCustomWing.CheckCustomWingByItem(iType))
		&& !(ITEM_WING + 130 <= iType && iType <= ITEM_WING + 134)
		&& !(iType >= ITEM_WING + 49 && iType <= ITEM_WING + 50)
		&& (iType != ITEM_WING + 135))
	{
		bSuccess = false;
	}

	if (iType == ITEM_BOW + 7 || iType == ITEM_BOW + 15)
	{
		bSuccess = false;
	}

	if ((pPickItem->Type == ITEM_POTION + 13 && iLevel >= 6) || (pPickItem->Type == ITEM_POTION + 14 && iLevel >= 9))
	{
		bSuccess = false;
	}

	if (pPickItem->Type == ITEM_POTION + 13 && iType == ITEM_HELPER + 37 && iDurability != 255)
	{
		CFenrirRepairMsgBox* pMsgBox = nullptr;
		CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CFenrirRepairMsgBoxLayout), &pMsgBox);
		pMsgBox->SetSourceIndex(iSourceIndex);

		auto iIndex = targetControl->GetIndex(pItem->x, pItem->y);
		pMsgBox->SetTargetIndex(iIndex);

		pPickedItem->HidePickedItem();
		return true;
	}

	if (pPickItem->Type == ITEM_POTION + 42)
	{
		if (g_SocketItemMgr.IsSocketItem(pItem))
		{
			bSuccess = false;
		}
		else
			if (pItem->Jewel_Of_Harmony_Option != 0)
			{
				bSuccess = false;
			}
			else
			{
				const StrengthenItem strengthitem = g_pUIJewelHarmonyinfo->GetItemType(static_cast<int>(pItem->Type));

				if (strengthitem == SI_None)
				{
					bSuccess = false;
				}
			}
	}

	if (pPickItem->Type == ITEM_POTION + 43 || pPickItem->Type == ITEM_POTION + 44)
	{
		if (g_SocketItemMgr.IsSocketItem(pItem))
		{
			bSuccess = false;
		}
		else if (pItem->Jewel_Of_Harmony_Option == 0)
		{
			bSuccess = false;
		}
	}

	if (gCustomJewel.CheckCustomJewelApplyItem(pPickItem->Type, iType))
	{
		bSuccess = true;
	}

#ifdef LEM_ADD_LUCKYITEM
	if (Check_LuckyItem(pItem->Type))
	{
		bSuccess = false;
		if (pPickItem->Type == ITEM_POTION + 161)
		{
			if (pItem->Jewel_Of_Harmony_Option == 0)	bSuccess = true;
		}
		else if (pPickItem->Type == ITEM_POTION + 160)
		{
			if (pItem->Durability > 0)					bSuccess = true;
		}
	}
#endif // LEM_ADD_LUCKYITEM

	if (bSuccess)
	{
		const int targetIndex = targetControl->GetIndexByItem(pItem);
		SendRequestUse(iSourceIndex, targetIndex);
		PlayBuffer(SOUND_GET_ITEM01);
		return true;
	}

	return false;
}

bool CNewUIMyInventory::TryStackItems(CNewUIInventoryCtrl* targetControl, ITEM* pPickItem, const int iSourceIndex, const int iTargetIndex)
{
	if (ITEM* pItem = targetControl->FindItem(iTargetIndex))
	{
		if (targetControl->AreItemsStackable(pPickItem, pItem))
		{
			SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, iSourceIndex, pPickItem, STORAGE_TYPE::INVENTORY, iTargetIndex);
			return true;
		}
	}

	return false;
}
bool CNewUIMyInventory::TryConsumeItemBank(CNewUIInventoryCtrl* targetControl, ITEM* pItem, const int iIndex)
{
	if (!pItem)
	{
		return false;
	}

	struct ItemPacketMap
	{
		int itemType;
		uint8_t SendData;
	};

	static const ItemPacketMap ItemPacket[] =
	{
		{ GET_ITEM(14, 13), 0x0A }, { GET_ITEM(12, 30), 0x0A },
		{ GET_ITEM(14, 14), 0x0B }, { GET_ITEM(12, 31), 0x0B },
		{ GET_ITEM(14, 16), 0x0C }, { GET_ITEM(12, 136), 0x0C },
		{ GET_ITEM(14, 22), 0x0D }, { GET_ITEM(12, 137), 0x0D },
		{ GET_ITEM(14, 31), 0x0E }, { GET_ITEM(12, 138), 0x0E },
		{ GET_ITEM(14, 41), 0x0F }, { GET_ITEM(12, 139), 0x0F },
		{ GET_ITEM(14, 42), 0x10 }, { GET_ITEM(12, 140), 0x10 },
		{ GET_ITEM(12, 15), 0x11 }, { GET_ITEM(12, 141), 0x11 },
		{ GET_ITEM(14, 43), 0x12 }, { GET_ITEM(12, 142), 0x12 },
		{ GET_ITEM(14, 44), 0x13 }, { GET_ITEM(12, 143), 0x13 }
	};

	for (const auto& Entry : ItemPacket)
	{
		if (pItem->Type == Entry.itemType)
		{
			SendRequestDataSend(0xFD, Entry.SendData);
			PlayBuffer(SOUND_DROP_ITEM01);
			return true;
		}
	}

	return false;
}

const static std::list<eBuffState> secretPotionbufflist =
{
	eBuff_SecretPotion1,
	eBuff_SecretPotion2,
	eBuff_SecretPotion3,
	eBuff_SecretPotion4,
	eBuff_SecretPotion5
};

bool CNewUIMyInventory::TryConsumeItem(CNewUIInventoryCtrl* targetControl, ITEM* pItem, const int iIndex)
{
	if (pItem == nullptr)
	{
		return false;
	}


	if (pItem->Type == ITEM_POTION + 10)
	{
		if (!Teleport)
		{
			SendRequestUse(iIndex, 0);
			return true;
		}
	}

	const auto isApple = pItem->Type == ITEM_POTION + 0;
	const auto isPotion =
		(pItem->Type >= ITEM_POTION + 0 && pItem->Type <= ITEM_POTION + 9)
		|| (pItem->Type >= ITEM_POTION + 35 && pItem->Type <= ITEM_POTION + 40);

	if (isApple || isPotion
		|| (pItem->Type == ITEM_POTION + 20 && ((pItem->Level >> 3) & 15) == 0)
		|| (pItem->Type >= ITEM_POTION + 46 && pItem->Type <= ITEM_POTION + 50)
		|| (pItem->Type == ITEM_POTION + 11 && ((pItem->Level >> 3) & 15) == 14)
		|| (pItem->Type >= ITEM_POTION + 70 && pItem->Type <= ITEM_POTION + 71)
		|| (pItem->Type >= ITEM_POTION + 72 && pItem->Type <= ITEM_POTION + 77)
		|| pItem->Type == ITEM_HELPER + 60
		|| pItem->Type == ITEM_POTION + 94
		|| (pItem->Type >= ITEM_POTION + 85 && pItem->Type <= ITEM_POTION + 87)
		|| (pItem->Type >= ITEM_POTION + 97 && pItem->Type <= ITEM_POTION + 98)
		|| pItem->Type == ITEM_HELPER + 81
		|| pItem->Type == ITEM_HELPER + 82
		|| pItem->Type == ITEM_POTION + 133
		)
	{
		SendRequestUse(iIndex, 0);
		if (isApple)
		{
			PlayBuffer(SOUND_EAT_APPLE01);
		}
		else if (isPotion)
		{
			PlayBuffer(SOUND_DRINK01);
		}

		return true;
	}

	if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
	{
		if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) == eBuffNone) {
			SendRequestUse(iIndex, 0);
			return true;
		}

		CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255));
		return false;
	}

	if ((pItem->Type >= ITEM_HELPER + 54 && pItem->Type <= ITEM_HELPER + 57) || (pItem->Type == ITEM_HELPER + 58 && gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD))
	{
		bool result = true;
		DWORD point[5] = { 0, };

		point[0] = CharacterAttribute->ViewStrength + CharacterAttribute->AddStrength;
		point[1] = CharacterAttribute->ViewDexterity + CharacterAttribute->AddDexterity;
		point[2] = CharacterAttribute->ViewVitality + CharacterAttribute->AddVitality;
		point[3] = CharacterAttribute->ViewEnergy + CharacterAttribute->AddEnergy;
		point[4] = CharacterAttribute->ViewLeadership + CharacterAttribute->AddCharisma;

		static const char nStat[MAX_CLASS][5] =
		{
			{18, 18, 15, 30, 0},
			{28, 20, 25, 10, 0},
			{22, 25, 20, 15, 0},
			{26, 26, 26, 26, 0},
			{26, 20, 20, 15, 25},
			{21, 21, 18, 23, 0},
			{32, 27, 25, 20, 0},
		};

		const auto attributeType = pItem->Type - (ITEM_HELPER + 54);
		const int characterClass = gCharacterManager.GetBaseClass(Hero->Class);
		point[attributeType] -= nStat[characterClass][attributeType];
		if (point[attributeType] < (pItem->Durability * 10))
		{
			g_pMyInventory->SetStandbyItemKey(pItem->Key);
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CUsePartChargeFruitMsgBoxLayout));
			return false;
		}

		SendRequestUse(iIndex, 0);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 58 && gCharacterManager.GetBaseClass(Hero->Class) != CLASS_DARK_LORD)
	{
		CreateOkMessageBox(GlobalText[1905]);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 29 || pItem->Type == ITEM_HELPER + 121)
	{
		if (IsUnitedMarketPlace())
		{
			char	szOutputText[512];
			sprintf(szOutputText, "%s %s", GlobalText[3014], GlobalText[3015]);

			CreateOkMessageBox(szOutputText);
			return true;
		}

		if (Hero->SafeZone == false)
		{
			CreateOkMessageBox(GlobalText[2330]);
			return false;
		}

		SendRequestEventZoneOpenTime(4, ((pItem->Level >> 3) & 15));
		g_pMyInventory->SetStandbyItemKey(pItem->Key);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 46)
	{
		const BYTE byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_DEVILSQUARE);
		SendRequestEventZoneOpenTime(1, byPossibleLevel);
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 47)
	{
		const BYTE byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_BLOODCASTLE);
		SendRequestEventZoneOpenTime(2, byPossibleLevel);
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 48)
	{
		if (Hero->SafeZone || gMapManager.InHellas())
		{
			g_pChatListBox->AddText("", GlobalText[1238], TYPE_ERROR_MESSAGE);
			return false;
		}

		SendRequestUse(iIndex, 0);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 61)
	{
		const BYTE byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_CURSEDTEMPLE);
		SendRequestEventZoneOpenTime(5, byPossibleLevel);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 121)
	{
		if (Hero->SafeZone == false)
		{
			CreateOkMessageBox(GlobalText[2330]);
			return false;
		}

		SendRequestEventZoneOpenTime(4, ((pItem->Level >> 3) & 15));
		g_pMyInventory->SetStandbyItemKey(pItem->Key);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 51)
	{
		SendRequestEventZoneOpenTime(5, ((pItem->Level >> 3) & 15));
		return true;
	}

	if (pItem->Type == ITEM_POTION + 19)
	{
		SendRequestEventZoneOpenTime(1, ((pItem->Level >> 3) & 15));
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 18)
	{
		if (pItem->Level == 0)
		{
			g_pChatListBox->AddText("", GlobalText[2089], TYPE_ERROR_MESSAGE);
		}
		else
		{
			SendRequestEventZoneOpenTime(2, ((pItem->Level >> 3) & 15) - 1);
		}

		return true;
	}

	if ((pItem->Type >= ITEM_ETC + 0 && pItem->Type < ITEM_ETC + MAX_ITEM_INDEX)
		|| (pItem->Type >= ITEM_WING + 7 && pItem->Type <= ITEM_WING + 14)
		|| (pItem->Type >= ITEM_WING + 16 && pItem->Type <= ITEM_WING + 19)
		|| (pItem->Type == ITEM_WING + 20)
		|| (pItem->Type >= ITEM_WING + 21 && pItem->Type <= ITEM_WING + 24)
		|| (pItem->Type == ITEM_WING + 35)
		|| (pItem->Type == ITEM_WING + 44)
		|| (pItem->Type == ITEM_WING + 47)
		|| (pItem->Type == ITEM_WING + 46)
		|| (pItem->Type == ITEM_WING + 45)
		|| (pItem->Type == ITEM_WING + 48)
		|| (pItem->Type == ITEM_ETC + 29)
		|| (pItem->Type == ITEM_ETC + 28)
		)
	{
		bool bReadBookGem = true;

		if ((pItem->Type == ITEM_ETC + 18)
			|| (pItem->Type == ITEM_ETC + 28)
			|| pItem->Type == ITEM_WING + 45
			|| (pItem->Type == ITEM_WING + 46)
			|| (pItem->Type == ITEM_WING + 44)
			)
		{
			if (g_csQuest.getQuestState2(QUEST_CHANGE_UP_3) != QUEST_END)

				bReadBookGem = false;
		}
		if (pItem->Type == ITEM_WING + 48)
		{
			const int Level = CharacterAttribute->Level;
			if (Level < 220)
				bReadBookGem = false;
		}
		if (bReadBookGem)
		{

			DWORD Strength, Energy;
			Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
			Energy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

			if (CharacterAttribute->Level >= ItemAttribute[pItem->Type].RequireLevel &&
				Energy >= pItem->RequireEnergy &&
				Strength >= pItem->RequireStrength)
			{
				SendRequestUse(iIndex, 0);
			}

			return true;
		}

		return false;
	}

	if (pItem->Type == ITEM_HELPER + 15)
	{
		const int Level = CharacterAttribute->Level;

		if (Level >= 10)
		{
			bool bEquipmentEmpty = true;
			for (int i = 0; i < MAX_EQUIPMENT; i++)
			{
				if (CharacterMachine->Equipment[i].Type != -1)
				{
					bEquipmentEmpty = false;
				}
			}
			const int Class = CharacterAttribute->Class;

			if (bEquipmentEmpty == true)
			{
				if (pItem->Level == 32)
				{
					if (gCharacterManager.GetBaseClass(Class) != CLASS_DARK_LORD)
					{
						CreateOkMessageBox(GlobalText[1905]);
						return true;
					}
				}
				g_pMyInventory->SetStandbyItemKey(pItem->Key);
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CUseFruitMsgBoxLayout));
				return true;
			}

			CreateOkMessageBox(GlobalText[1909]);
			return true;
		}

		CreateOkMessageBox(GlobalText[749]);
		return true;
	}

	if (pItem->Type == ITEM_HELPER + 11)
	{
		bool bUse = false;
		const int  Level = (pItem->Level >> 3) & 15;
		switch (Level)
		{
		case 0:
			bUse = true;
			break;
		case 1:
			if (Hero->GuildStatus != G_MASTER)
				bUse = true;
			break;
		}
		if (bUse)
		{
			SendRequestUse(iIndex, 0);
			return true;
		}

		return false;
	}


	if (pItem->Type == ITEM_HELPER + 69)
	{
		if (g_PortalMgr.IsRevivePositionSaved())
		{
			if (g_PortalMgr.IsPortalUsable())
			{
				g_pMyInventory->SetStandbyItemKey(pItem->Key);
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CUseReviveCharmMsgBoxLayout));
			}
			else
			{
				CreateOkMessageBox(GlobalText[2608]);
			}
		}

		return false;
	}

	if (pItem->Type == ITEM_HELPER + 70)
	{
		if (g_PortalMgr.IsPortalUsable())
		{
			if (pItem->Durability == 2)
			{
				if (g_PortalMgr.IsPortalPositionSaved())
				{
					CreateOkMessageBox(GlobalText[2610]);
				}
				else
				{
					g_pMyInventory->SetStandbyItemKey(pItem->Key);
					CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CUsePortalCharmMsgBoxLayout));
				}
			}
			else if (pItem->Durability == 1)
			{
				g_pMyInventory->SetStandbyItemKey(pItem->Key);
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CReturnPortalCharmMsgBoxLayout));
			}
		}
		else
		{
			CreateOkMessageBox(GlobalText[2608]);
		}

		return false;
	}

	if (pItem->Type == ITEM_HELPER + 66)
	{
		g_pMyInventory->SetStandbyItemKey(pItem->Key);
		CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CUseSantaInvitationMsgBoxLayout));
	}
	
	return false;
}

// TODO: This whole logic (and possibly others) should be moved into a 'controller' class or similar.
bool CNewUIMyInventory::HandleInventoryActions(CNewUIInventoryCtrl* targetControl)
{
	CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

	if (g_pNewUISystem->IsVisible(INTERFACE_TRADE) && IsPress(VK_RBUTTON))
	{
		g_pMyInventory->ResetMouseRButton();

		if (g_pTrade && g_pTrade->GetMyInvenCtrl())
		{
			if (g_pTrade->GetMyInvenCtrl()->GetEmptySlotCount() <= 0)
			{
				g_pChatListBox->AddText("", "Trade window is full. Cannot add more items.", SEASON3B::TYPE_ERROR_MESSAGE);
				return false;
			}
		}

		return g_pTrade->ProcessMyInvenItemAutoMove();
	}
	else if (g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY) && IsPress(VK_RBUTTON))
	{
		g_pMyInventory->ResetMouseRButton();
		return g_pMixInventory->ProcessMyInvenItemAutoMove();
	}
	else if (g_pNewUISystem->IsVisible(INTERFACE_MYSHOP_INVENTORY) && IsPress(VK_RBUTTON))
	{
		if (gOffStore.ShopActive)
			return false;

			g_pMyInventory->ResetMouseRButton();
			return g_pMyShopInventory->ProcessMyInvenItemAutoMove();
	}

	if (pPickedItem && IsRelease(VK_LBUTTON))
	{
		ITEM* pPickItem = pPickedItem->GetItem();
		if (pPickItem == nullptr)
		{
			return true;
		}

		const int iSourceIndex = pPickedItem->GetSourceLinealPos();
		const int iTargetIndex = pPickedItem->GetTargetLinealPos(targetControl);

		if (pPickedItem->GetOwnerInventory() == targetControl
			|| g_pMyInventoryExt->GetOwnerOf(pPickedItem)) // Movement between Inventory (and within extensions)
		{
			// Apply Jewels:
			if ((pPickItem->Type == ITEM_POTION + 13
				|| pPickItem->Type == ITEM_POTION + 14
				|| pPickItem->Type == ITEM_POTION + 16
				|| pPickItem->Type == ITEM_POTION + 42
				|| pPickItem->Type == ITEM_POTION + 43
				|| pPickItem->Type == ITEM_POTION + 44
				|| pPickItem->Type == ITEM_POTION + 160
				|| pPickItem->Type == ITEM_POTION + 161
				|| gCustomJewel.CheckCustomJewelByItem(pPickItem->Type)
				) && ApplyJewels(targetControl, pPickedItem, pPickItem, iSourceIndex, iTargetIndex))
			{
				return true;
			}

			if (iTargetIndex != -1 && TryStackItems(targetControl, pPickItem, iSourceIndex, iTargetIndex))
			{
				return true;
			}
		}

		// If nothing of above applied, we try to move the item.
		if (iTargetIndex >= 0 && targetControl->CanMove(iTargetIndex, pPickItem))
		{
			const auto sourceStorageType = pPickedItem->GetSourceStorageType();
			const auto targetStorageType = targetControl->GetStorageType();
			if (iTargetIndex != iSourceIndex)
			{
				return SendRequestEquipmentItem(sourceStorageType, iSourceIndex,
					pPickItem, targetStorageType, iTargetIndex);
			}
			else
			{
				CNewUIInventoryCtrl::BackupPickedItem();
			}
		}
	}
	else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (IsPress(VK_RBUTTON)))
	{
		g_pMyInventory->ResetMouseRButton();

		if (g_pNewUISystem->IsVisible(INTERFACE_STORAGE))
		{
			return g_pStorageInventory->ProcessMyInvenItemAutoMove();
		}

		if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY)
			&& !g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP)
			&& !g_pNewUISystem->IsVisible(INTERFACE_TRADE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY))
		{
			ITEM* pItem = targetControl->FindItemAtPt(MouseX, MouseY);
			if (!pItem)
			{
				return false;
			}

			const int iIndex = targetControl->GetIndexByItem(pItem);

			if (iIndex >= 0 && TryConsumeItemBank(targetControl, pItem, iIndex))
			{
				return true;
			}
		}

		return false;
	}
	else if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (IsPress(VK_RBUTTON)))
	{
		g_pMyInventory->ResetMouseRButton();

		if (g_pNewUISystem->IsVisible(INTERFACE_STORAGE))
		{
			return g_pStorageInventory->ProcessMyInvenItemAutoMove();
		}

		if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY)
			&& !g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP)
			&& !g_pNewUISystem->IsVisible(INTERFACE_TRADE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY))
		{
			ITEM* pItem = targetControl->FindItemAtPt(MouseX, MouseY);
			if (!pItem)
			{
				return false;
			}

			const int iIndex = targetControl->GetIndexByItem(pItem);

			if (iIndex >= 0)
			{
				if (iIndex >= 0)
				{
					int MixID = pItem->Type;
					int A = MixID / 512;
					int B = MixID % 512;

					for (int a = 0; a < 10; a++)
					{
						g_pNewUIRenderColorItem->RenderItem[a] = 0;
					}

					g_pNewUIRenderColorItem->RenderItem[ITEM_SECTION] = A;
					g_pNewUIRenderColorItem->RenderItem[ITEM_INDEX] = B;
					g_pNewUIRenderColorItem->RenderItem[ITEM_LEVEL] = pItem->Level / 8;
					g_ConsoleDebug->Write(3, "%d %d", pItem->Type, pItem->Level);
				}
			}
		}

		return false;
	}
	else if (g_pMyInventory->GetRepairMode() == REPAIR_MODE_OFF && IsPress(VK_RBUTTON))
	{
		// handle right click (item usage etc.)


		g_pMyInventory->ResetMouseRButton();

		if (g_pNewUISystem->IsVisible(INTERFACE_STORAGE))
		{
			const ITEM* pItemObj = targetControl->FindItemAtPt(MouseX, MouseY);
			if (!pItemObj)
				return false;

			int width = ItemAttribute[pItemObj->Type].Width;
			int height = ItemAttribute[pItemObj->Type].Height;

			int col = 0, row = 0;

			if (g_pStorageInventory && g_pStorageInventory->GetInventoryCtrl())
			{
				if (g_pStorageInventory->GetInventoryCtrl()->FindEmptySlot(width, height, col, row))
				{
					return g_pStorageInventory->ProcessMyInvenItemAutoMove();
				}
			}

			if (g_pNewUISystem->IsVisible(INTERFACE_STORAGE_EXT))
			{
				if (g_pStorageInventoryExt && g_pStorageInventoryExt->GetInventoryCtrl())
				{
					if (g_pStorageInventoryExt->GetInventoryCtrl()->FindEmptySlot(width, height, col, row))
					{
						return g_pStorageInventoryExt->ProcessMyInvenItemAutoMove();
					}
				}
			}

			g_pChatListBox->AddText("", GlobalText[375], SEASON3B::TYPE_ERROR_MESSAGE);
			return false;
		}

		if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY)
			&& !g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP)
			&& !g_pNewUISystem->IsVisible(INTERFACE_TRADE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE)
			&& !g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE)
#ifdef LEM_ADD_LUCKYITEM
			&& !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
			&& !g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY))
		{
			ITEM* pItem = targetControl->FindItemAtPt(MouseX, MouseY);
			if (!pItem)
			{
				return false;
			}

			//ACuoi Fix AutoMove Item Check Slot Khieng
			const ITEM* pLeft = &CharacterMachine->Equipment[0];
			const ITEM* pRight = &CharacterMachine->Equipment[1];

			if (pLeft->Type >= 0
				&& ItemAttribute[pLeft->Type].TwoHand == 1
				&& pItem->Type >= ITEM_SWORD && pItem->Type <= ITEM_SHIELD + MAX_ITEM_INDEX
				)
			{
				return false;
			}

			if (pRight->Type >= 0 &&
				ItemAttribute[pRight->Type].TwoHand == 0 &&
				ItemAttribute[pItem->Type].TwoHand == 1)
			{
				return false;
			}

			const int iIndex = targetControl->GetIndexByItem(pItem);
			if (GetKeyState(VK_CONTROL) & 0x8000) //Send Post Item
			{
				PMSG_POSTITEM iMsg = { '\0' };
				strcpy_s(iMsg.chatmsg, (char*)BGetItemName(pItem->Type, pItem->Level));
				memcpy_s(&iMsg.chatid[0], 10, (char*)CharacterAttribute->Name, 10);
				memcpy_s(&iMsg.item_data[0], 107, pItem, 107);
				iMsg.h.set(0x78, sizeof(iMsg));
				BDataSend((LPBYTE)&iMsg, iMsg.h.size);
				return 1;
			}


			if (iIndex >= 0 && TryConsumeItem(targetControl, pItem, iIndex))
			{
				return true;
			}
#ifdef LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
			else if (g_pMyInventory->IsInvenItem(pItem->Type))
			{
#ifdef LJH_FIX_APP_SHUTDOWN_WEQUIPPING_INVENITEM_WITH_CLICKING_MOUSELBTN
				if (MouseLButton || MouseLButtonPop || MouseLButtonPush)
					return false;
#endif //LJH_FIX_APP_SHUTDOWN_WEQUIPPING_INVENITEM_WITH_CLICKING_MOUSELBTN
				if (pItem->Durability == 0)
					return false;

				int iChangeInvenItemStatus = 0;
				(pItem->Durability == 255) ? iChangeInvenItemStatus = 254 : iChangeInvenItemStatus = 255;

				SendRequestEquippingInventoryItem(iIndex, iChangeInvenItemStatus);
			}
#endif //LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
			//-- Equip item
			if (!EquipmentItem)
			{
				const int iSrcIndex = iIndex;

				const ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];

				int nDstIndex = pItemAttr->m_byItemSlot;

				if (nDstIndex >= 0 && nDstIndex < MAX_EQUIPMENT_INDEX)
				{
					const ITEM* pEquipment = &CharacterMachine->Equipment[nDstIndex];

					if (pEquipment && pEquipment->Type != -1)
					{
						//ACuoi Auto di chuyen item tu hom do <-> hom do mo rong + tu dong sap xep
						//const int empty = g_pMyInventory->FindEmptySlot(pItem);
						//if (empty != -1)
						//{
						//	SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, iSrcIndex, pItem, STORAGE_TYPE::INVENTORY, empty);
						//	targetControl->RemoveItem(pItem);
						//	return true;
						//}
						//else
						//{
						//	g_pChatListBox->AddText("", GlobalText[375], TYPE_ERROR_MESSAGE);
						//	return false;
						//}

						if (nDstIndex == EQUIPMENT_WEAPON_RIGHT)
						{
							if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_KNIGHT || gCharacterManager.GetBaseClass(Hero->Class) == CLASS_MAGIC
								|| gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER)
							{
								if (!pItemAttr->TwoHand)
								{
									nDstIndex = EQUIPMENT_WEAPON_LEFT;
									pEquipment = &CharacterMachine->Equipment[nDstIndex];

									if (!(pEquipment && pEquipment->Type != -1))
									{
										goto LABEL_32;
									}
								}
							}
						}
						else if (nDstIndex == EQUIPMENT_RING_RIGHT)
						{
							nDstIndex = EQUIPMENT_RING_LEFT;
							pEquipment = &CharacterMachine->Equipment[nDstIndex];

							if (!(pEquipment && pEquipment->Type != -1))
							{
								goto LABEL_32;
							}
						}

						return true;
					}
				LABEL_32: // wtf...
					if (g_pMyInventory->IsEquipable(nDstIndex, pItem))
					{
						if (CNewUIInventoryCtrl::CreatePickedItem(nullptr, pItem))
						{
							targetControl->RemoveItem(pItem);

							const auto pPickedItem = CNewUIInventoryCtrl::GetPickedItem();
							pPickedItem->HidePickedItem();
						}

						SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, iSrcIndex, pItem, STORAGE_TYPE::INVENTORY, nDstIndex);
					}
				}

				return true;
			}
		}

		return false;
	}
	if (g_pMyInventory->GetRepairMode() == REPAIR_MODE_ON && IsPress(VK_LBUTTON))
	{
		// Repair stuff

		return RepairItemAtMousePoint(targetControl);
	}

	return false;
}

bool CNewUIMyInventory::InventoryProcess() const
{
	if (CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT) == false)
	{
		return false;
	}

	if (m_pNewInventoryCtrl == nullptr)
	{
		return false;
	}

	return HandleInventoryActions(m_pNewInventoryCtrl);
}

bool CNewUIMyInventory::HasAnyItem(CNewUIInventoryCtrl* pCtrl)
{
	for (int i = 0; i < 64; ++i)
	{
		if (pCtrl->FindItem(i) != nullptr)
			return true;
	}
	return false;
}

bool CNewUIMyInventory::BtnProcess()
{
	const POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 7 };

	if (IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(INTERFACE_INVENTORY);
		return true;
	}
	if (m_BtnExit.UpdateMouseEvent())
	{
		if (g_pNewUISystem->IsVisible(INTERFACE_MYSHOP_INVENTORY))
		{
			g_pNewUISystem->Hide(INTERFACE_MYSHOP_INVENTORY);
		}
		if (g_pNewUISystem->IsVisible(INTERFACE_JEWELBANK))
		{
			g_pNewUISystem->Hide(INTERFACE_JEWELBANK);
		}
		g_pNewUISystem->Hide(INTERFACE_INVENTORY);
		return true;
	}

	if (m_BtnExpand.UpdateMouseEvent())
		{
			g_pNewUISystem->Toggle(INTERFACE_INVENTORY_EXT);
			return true;
		}
	if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_TRADE) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_DEVILSQUARE) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_BLOODCASTLE) == false
#ifdef LEM_ADD_LUCKYITEM
		&& g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND) == false
#endif // LEM_ADD_LUCKYITEM
		&& g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY) == false
		&& g_pNewUISystem->IsVisible(INTERFACE_STORAGE) == false)
	{
		

		if (m_bRepairEnableLevel == true && m_BtnRepair.UpdateMouseEvent() == true)
		{
			ToggleRepairMode();
			return true;
		}

		if (m_bMyShopOpen == true && m_BtnMyShop.UpdateMouseEvent() == true)
		{
			if (m_MyShopMode == MYSHOP_MODE_OPEN)
			{
				ChangeMyShopButtonStateClose();
				g_pNewUISystem->Show(INTERFACE_MYSHOP_INVENTORY);
			}
			else if (m_MyShopMode == MYSHOP_MODE_CLOSE)
			{
				ChangeMyShopButtonStateOpen();
				g_pNewUISystem->Hide(INTERFACE_MYSHOP_INVENTORY);
				g_pNewUISystem->Hide(INTERFACE_PURCHASESHOP_INVENTORY);
			}

			return true;
		}
	}
	if (m_BtnClear.UpdateMouseEvent())
	{
		const auto pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();
		if (pMyInvenCtrl && this->HasAnyItem(pMyInvenCtrl))
		{
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CClearInventoryConfirmLayout));
		}
		else
		{
			PlayBuffer(SOUND_CLICK01);
			g_pChatListBox->AddText("", GlobalText[3189], SEASON3B::TYPE_SYSTEM_MESSAGE);
		}
		return true;
	}
	return false;
}

void CNewUIMyInventory::RenderItemToolTip(int iSlotIndex) const
{
	if (m_iPointedSlot != -1)
	{
		ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[iSlotIndex];
		if (pEquipmentItemSlot->Type != -1)
		{
			const int iTargetX = m_EquipmentSlots[iSlotIndex].x + m_EquipmentSlots[iSlotIndex].width / 2;
			const int iTargetY = m_EquipmentSlots[iSlotIndex].y + m_EquipmentSlots[iSlotIndex].height / 2;

			pEquipmentItemSlot->bySelectedSlotIndex = iSlotIndex;

			if (m_RepairMode == REPAIR_MODE_OFF)
			{
				RenderItemInfo(iTargetX, iTargetY, pEquipmentItemSlot, false);
			}
			else
			{
				RenderRepairInfo(iTargetX, iTargetY, pEquipmentItemSlot, false);
			}
		}
	}
}

bool CNewUIMyInventory::CanRegisterItemHotKey(int iType)
{
	switch (iType)
	{
	case ITEM_POTION + 0:
	case ITEM_POTION + 1:
	case ITEM_POTION + 2:
	case ITEM_POTION + 3:
	case ITEM_POTION + 4:
	case ITEM_POTION + 5:
	case ITEM_POTION + 6:
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 35:
	case ITEM_POTION + 36:
	case ITEM_POTION + 37:
	case ITEM_POTION + 38:
	case ITEM_POTION + 39:
	case ITEM_POTION + 40:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		return true;
	}

	return false;
}

bool CNewUIMyInventory::CanOpenMyShopInterface()
{
	if (g_pNewUISystem->IsVisible(INTERFACE_NPCSHOP)
		|| g_pNewUISystem->IsVisible(INTERFACE_STORAGE)
		|| g_pNewUISystem->IsVisible(INTERFACE_MIXINVENTORY)
#ifdef LEM_ADD_LUCKYITEM
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
		|| g_pNewUISystem->IsVisible(INTERFACE_TRADE)
		|| gMapManager.IsCursedTemple()
		)
	{
		return false;
	}
	return true;
}

bool CNewUIMyInventory::IsRepairEnableLevel() const
{
	return m_bRepairEnableLevel;
}

void CNewUIMyInventory::SetRepairEnableLevel(bool bOver)
{
	m_bRepairEnableLevel = bOver;
}

void CNewUIMyInventory::ChangeMyShopButtonStateOpen()
{
	m_MyShopMode = MYSHOP_MODE_OPEN;
	m_BtnMyShop.UnRegisterButtonState();
	m_BtnMyShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_INVENTORY_MYSHOP_OPEN_BTN, 0);
	m_BtnMyShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_INVENTORY_MYSHOP_OPEN_BTN, 1);
	m_BtnMyShop.ChangeImgIndex(IMAGE_INVENTORY_MYSHOP_OPEN_BTN, 0);
	m_BtnMyShop.ChangeToolTipText(GlobalText[1125], true);
}

void CNewUIMyInventory::ChangeMyShopButtonStateClose()
{
	m_MyShopMode = MYSHOP_MODE_CLOSE;
	m_BtnMyShop.UnRegisterButtonState();
	m_BtnMyShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_INVENTORY_MYSHOP_CLOSE_BTN, 0);
	m_BtnMyShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_INVENTORY_MYSHOP_CLOSE_BTN, 1);
	m_BtnMyShop.ChangeImgIndex(IMAGE_INVENTORY_MYSHOP_CLOSE_BTN, 0);
	m_BtnMyShop.ChangeToolTipText(GlobalText[1127], true);
}

void CNewUIMyInventory::LockMyShopButtonOpen()
{
	m_BtnMyShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(100, 100, 100, 255));
	m_BtnMyShop.ChangeTextColor(RGBA(100, 100, 100, 255));
	m_BtnMyShop.Lock();

	m_BtnMyShop.ChangeToolTipText(GlobalText[1125], true);
}

void CNewUIMyInventory::UnlockMyShopButtonOpen()
{
	m_BtnMyShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnMyShop.ChangeTextColor(RGBA(255, 255, 255, 255));
	m_BtnMyShop.UnLock();

	m_BtnMyShop.ChangeToolTipText(GlobalText[1125], true);
}

void CNewUIMyInventory::ToggleRepairMode()
{
	if (m_RepairMode == REPAIR_MODE_OFF)
	{
		SetRepairMode(true);
	}
	else if (m_RepairMode == REPAIR_MODE_ON)
	{
		SetRepairMode(false);
	}
}

bool CNewUIMyInventory::IsItem(short int siType, bool bcheckPick) const
{
	if (bcheckPick == true)
	{
		const CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

		if (pPickedItem)
		{
			const ITEM* pItemObj = pPickedItem->GetItem();

			if (pItemObj->Type == siType) return true;
		}
	}

	const ITEM* pholyitemObj = m_pNewInventoryCtrl->FindTypeItem(siType);

	if (pholyitemObj) return true;

	return false;
}

int CNewUIMyInventory::GetNumItemByKey(DWORD dwItemKey) const
{
	return m_pNewInventoryCtrl->GetNumItemByKey(dwItemKey);
}

int CNewUIMyInventory::GetNumItemByType(short sItemType) const
{
	return m_pNewInventoryCtrl->GetNumItemByType(sItemType);
}

BYTE CNewUIMyInventory::GetDurabilityPointedItem() const
{
	const ITEM* pItem = nullptr;

	if (m_iPointedSlot != -1)
	{
		pItem = &CharacterMachine->Equipment[m_iPointedSlot];
		const BYTE byDurability = pItem->Durability;

		return byDurability;
	}

	pItem = m_pNewInventoryCtrl->FindItemPointedSquareIndex();
	if (pItem != nullptr)
	{
		const BYTE byDurability = pItem->Durability;
		return byDurability;
	}

	return 0;
}

int CNewUIMyInventory::GetPointedItemIndex() const
{
	if (m_iPointedSlot != -1)
	{
		return m_iPointedSlot;
	}

	return m_pNewInventoryCtrl->GetPointedSquareIndex();
}

int CNewUIMyInventory::FindManaItemIndex() const
{
	for (int i = ITEM_POTION + 6; i >= ITEM_POTION + 4; i--)
	{
		const int iIndex = FindItemReverseIndex(i);
		if (iIndex != -1)
		{
			return iIndex;
		}
	}

	return -1;
}

int CNewUIMyInventory::FindHealingItemIndex() const
{
	for (int i = ITEM_LARGE_HEALING_POTION; i >= ITEM_APPLE; i--)
	{
		const int iIndex = FindItemReverseIndex(i);
		if (iIndex != -1)
		{
			return iIndex;
		}
	}

	return -1;
}

void CNewUIMyInventory::ResetMouseLButton()
{
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void CNewUIMyInventory::ResetMouseRButton()
{
	MouseRButton = false;
	MouseRButtonPop = false;
	MouseRButtonPush = false;
}


#ifdef LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
BOOL SEASON3B::CNewUIMyInventory::IsInvenItem(const short sType)
{
	BOOL bInvenItem = FALSE;

	if (FALSE
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM	
		|| (sType == ITEM_HELPER + 128 || sType == ITEM_HELPER + 129 || sType == ITEM_HELPER + 134)
#endif //LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM_PART_2
		|| (sType >= ITEM_HELPER + 130 && sType <= ITEM_HELPER + 133)
#endif //LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM_PART_2
		)
		bInvenItem = TRUE;

	return bInvenItem;
}
#endif //LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY