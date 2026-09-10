// NewUIMyShopInventory.cpp: implementation of the CNewUIMyShopInventory class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIMyShopInventory.h"
#include "NewUISystem.h"
#include "NewUICustomMessageBox.h"
#include "wsclientinline.h"
#include "PersonalShopTitleImp.h"
#include "ThangCuoi/ZzzToolKit.h"

const int iMAX_SHOPTITLE_MULTI = 26;

namespace
{
	void RenderText( const char* text, int x, int y, int sx, int sy, DWORD color, DWORD backcolor, int sort, HFONT hFont = g_hFont )
	{
		g_pRenderText->SetFont(hFont);

		DWORD backuptextcolor = g_pRenderText->GetTextColor();
		DWORD backuptextbackcolor = g_pRenderText->GetBgColor();

		g_pRenderText->SetTextColor(color);
		g_pRenderText->SetBgColor(backcolor);
		g_pRenderText->RenderText(x, y, text, sx, sy, sort);

		g_pRenderText->SetTextColor(backuptextcolor);
		g_pRenderText->SetBgColor(backuptextbackcolor);
	}
};

using namespace SEASON3B;

SEASON3B::CNewUIMyShopInventory::CNewUIMyShopInventory() : m_SourceIndex(-1), m_TargetIndex(-1), m_EnablePersonalShop( false ) , MAX_SHOPTITLE_UTF16(min(26, (int)(MAX_SHOPTITLE/g_pMultiLanguage->GetNumByteForOneCharUTF8())))
{
	m_pNewUIMng = NULL;
	m_pNewInventoryCtrl = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_EditBox = NULL;
	m_Button = NULL;
	m_bIsEnableInputValueTextBox = false;
}

SEASON3B::CNewUIMyShopInventory::~CNewUIMyShopInventory() 
{ 
	Release(); 
}

bool SEASON3B::CNewUIMyShopInventory::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MYSHOP_INVENTORY, this);

	SetPos(x, y);

	LoadImages();

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;
	if(false == m_pNewInventoryCtrl->Create(STORAGE_TYPE::MYSHOP, g_pNewUI3DRenderMng, g_pNewItemMng, this, m_Pos.x+16, m_Pos.y+90, 8, 4, MAX_MY_INVENTORY_EX_INDEX))
	{
		SAFE_DELETE(m_pNewInventoryCtrl);
		return false;
	}

	m_pNewInventoryCtrl->SetToolTipType( TOOLTIP_TYPE_MY_SHOP );

	m_Button = new CNewUIButton[MYSHOPINVENTORY_MAXBUTTONCOUNT];

	m_Button[MYSHOPINVENTORY_EXIT].ChangeButtonImgState( true, IMAGE_MYSHOPINVENTORY_EXIT_BTN, false );
	m_Button[MYSHOPINVENTORY_EXIT].ChangeButtonInfo( m_Pos.x+13, m_Pos.y+391, 36, 29 );
	m_Button[MYSHOPINVENTORY_EXIT].ChangeToolTipText(GlobalText[1002], true);

	m_Button[MYSHOPINVENTORY_CLOSE].ChangeButtonImgState( true, IMAGE_MYSHOPINVENTORY_CLOSE, false );
	m_Button[MYSHOPINVENTORY_CLOSE].ChangeButtonInfo( m_Pos.x+53, m_Pos.y+391, 36, 29 );
	m_Button[MYSHOPINVENTORY_CLOSE].ChangeToolTipText(GlobalText[1108], true);

	m_EditBox = new CUITextInputBox;

	m_EditBox->Init( g_hWnd, 200, 14, iMAX_SHOPTITLE_MULTI - 1 );
	m_EditBox->SetPosition(m_Pos.x+50, m_Pos.y+55);
	m_EditBox->SetTextColor(255, 255, 230, 210);
	m_EditBox->SetBackColor(0, 0, 0, 25);
	m_EditBox->SetFont(g_hFont);

	ChangeEditBox( UISTATE_HIDE );
	ChangePersonal(m_EnablePersonalShop);
	
	InitButtons();

	Show(false);

	return true;
}


void SEASON3B::CNewUIMyShopInventory::Release()
{
	SAFE_DELETE(m_pNewInventoryCtrl);
	SAFE_DELETE_ARRAY(m_Button);
	SAFE_DELETE(m_EditBox);

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}

	UnloadImages();
}

void SEASON3B::CNewUIMyShopInventory::LoadImages()
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_MYSHOPINVENTORY_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_MYSHOPINVENTORY_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_MYSHOPINVENTORY_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_MYSHOPINVENTORY_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_MYSHOPINVENTORY_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_MYSHOPINVENTORY_EXIT_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_Box_openTitle.tga", IMAGE_MYSHOPINVENTORY_EDIT, GL_LINEAR);
	LoadBitmap("Interface\\newui_Bt_openshop.tga", IMAGE_MYSHOPINVENTORY_OPEN, GL_LINEAR);
	LoadBitmap("Interface\\newui_Bt_closeshop.tga", IMAGE_MYSHOPINVENTORY_CLOSE, GL_LINEAR);

}

void SEASON3B::CNewUIMyShopInventory::UnloadImages()
{
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_CLOSE);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_OPEN);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_EDIT);	
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_EXIT_BTN);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_BOTTOM);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_RIGHT);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_LEFT);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_TOP);
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_BACK);
}

void SEASON3B::CNewUIMyShopInventory::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->SetPos(m_Pos.x + 16, m_Pos.y + 90);
	}
	if (m_Button)
	{
		m_Button[MYSHOPINVENTORY_EXIT].SetPos(m_Pos.x + 13, m_Pos.y + 391);
		m_Button[MYSHOPINVENTORY_CLOSE].SetPos(m_Pos.x + 53, m_Pos.y + 391);

		if (m_Btn)
		{
			int btnIndexList[] = { 0, 1, 2, 3, 4, 5, 7 };
			int charIDList[] = { 1, 2, 3, 4, 5, 6, 8 };

			int startX = m_Pos.x + 13;
			int startY = m_Pos.y + 177;
			int stepX = 58;
			int stepY = 28;

			int curX = startX;
			int curY = startY;
			int count = 0;

			for (int i = 0; i < 7; ++i)
			{
				if (gProtect->m_MainInfo.ButtonCharracter[charIDList[i]])
				{
					m_Btn[btnIndexList[i]].SetPos(curX, curY);
					++count;
					curX += stepX;

					if (count % 3 == 0)
					{
						curX = startX;
						curY += stepY;
					}
				}
			}

			m_Btn[6].SetPos(m_Pos.x + 13, m_Pos.y + 355);
		}
	}
}

void SEASON3B::CNewUIMyShopInventory::GetTitle( unicode::t_string& titletext )
{
	auto* pwszTitle = new wchar_t[ iMAX_SHOPTITLE_MULTI ];
	m_EditBox->GetText( pwszTitle, iMAX_SHOPTITLE_MULTI );
	g_pMultiLanguage->ConvertWideCharToStr(titletext, pwszTitle, CP_UTF8);
	delete [] pwszTitle;
}

bool SEASON3B::CNewUIMyShopInventory::InsertItem(int iIndex, BYTE* pbyItemPacket)
{
	if(m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->AddItem(iIndex, pbyItemPacket);
	}

	return false;
}

void SEASON3B::CNewUIMyShopInventory::DeleteItem(int iIndex)
{
	if(m_pNewInventoryCtrl)
	{
		ITEM* pItem = m_pNewInventoryCtrl->FindItem(iIndex);
		if(pItem != NULL)
			m_pNewInventoryCtrl->RemoveItem(pItem);
	}
}

void SEASON3B::CNewUIMyShopInventory::DeleteAllItems()
{
	if(m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->RemoveAllItems();
	}
}

ITEM* SEASON3B::CNewUIMyShopInventory::FindItem(int iLinealPos)
{
	if(m_pNewInventoryCtrl)
		return m_pNewInventoryCtrl->FindItem(iLinealPos);
	return NULL;
}

void SEASON3B::CNewUIMyShopInventory::ChangePersonal( bool state )
{
	m_EnablePersonalShop = state;
}

void SEASON3B::CNewUIMyShopInventory::OpenButtonLock()
{
	m_Button[MYSHOPINVENTORY_OPEN].ChangeImgColor(BUTTON_STATE_UP, RGBA(100, 100, 100, 255));
	m_Button[MYSHOPINVENTORY_OPEN].ChangeTextColor(RGBA(100, 100, 100, 255));
	m_Button[MYSHOPINVENTORY_OPEN].Lock();
	m_Button[MYSHOPINVENTORY_OPEN].ChangeToolTipText(GlobalText[1107], true);
}

void SEASON3B::CNewUIMyShopInventory::OpenButtonUnLock()
{
	m_Button[MYSHOPINVENTORY_OPEN].ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_Button[MYSHOPINVENTORY_OPEN].ChangeTextColor(RGBA(255, 255, 255, 255));
	m_Button[MYSHOPINVENTORY_OPEN].UnLock();
	m_Button[MYSHOPINVENTORY_OPEN].ChangeToolTipText(GlobalText[1107], true);
}

const bool SEASON3B::CNewUIMyShopInventory::IsEnablePersonalShop() const
{
	return m_EnablePersonalShop;
}

void SEASON3B::CNewUIMyShopInventory::ChangeEditBox(const UISTATES type)
{
	m_EditBox->SetState(type);

	if( type == UISTATE_NORMAL )
	{
		m_EditBox->GiveFocus();
	}

	m_EditBox->SetText(NULL);
}

bool SEASON3B::CNewUIMyShopInventory::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYSHOP_INVENTORY) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYSHOP_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIMyShopInventory::MyShopInventoryProcess()
{
	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT) == false)
	{
		return false;
	}

	CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

	if ((m_EnablePersonalShop || gOffStore.ShopActive) && pPickedItem && IsRelease(VK_LBUTTON))
	{
		g_pChatListBox->AddText("", GlobalText[1426], SEASON3B::TYPE_ERROR_MESSAGE);
		CNewUIInventoryCtrl::BackupPickedItem(); 
		return true;
	}

	if(m_pNewInventoryCtrl && pPickedItem && IsRelease(VK_LBUTTON))
	{
		ITEM* pItemObj = pPickedItem->GetItem();
		int iSourceIndex = pPickedItem->GetSourceLinealPos();
		int iTargetIndex = pPickedItem->GetTargetLinealPos(m_pNewInventoryCtrl);

		if(iTargetIndex == -1)
		{
			return true;
		}

		if (pPickedItem->GetOwnerInventory() == g_pMyInventory->GetInventoryCtrl() || g_pMyInventoryExt->GetOwnerOf(pPickedItem))
		{
			if(IsPersonalShopBan(pItemObj) == true)
			{
				g_pChatListBox->AddText("", GlobalText[2226], SEASON3B::TYPE_ERROR_MESSAGE);
				return true;
			}

			if (m_pNewInventoryCtrl->CanMove(iTargetIndex, pItemObj))
			{
				int srcIndex = pPickedItem->GetSourceLinealPos();
				SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, srcIndex, pItemObj, STORAGE_TYPE::MYSHOP, iTargetIndex);
				g_pMyInventory->GetInventoryCtrl()->RemoveItem(pItemObj); 

				pPickedItem->HidePickedItem();
				return true;
			}
		}
		else if(pPickedItem->GetOwnerInventory() == NULL)
		{
			if(IsPersonalShopBan(pItemObj) == true)
			{
				g_pChatListBox->AddText("", GlobalText[2226], SEASON3B::TYPE_ERROR_MESSAGE);
				return true;
			}
			
			if(m_pNewInventoryCtrl->CanMove(iTargetIndex, pItemObj))
			{
				ChangeSourceIndex(iSourceIndex);
				ChangeTargetIndex(iTargetIndex);
				
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(CPersonalShopItemValueMsgBoxLayout));
				SetInputValueTextBox(true);
				
				pPickedItem->HidePickedItem();
				return true;
			}
		}
		else if(pPickedItem->GetOwnerInventory() == m_pNewInventoryCtrl)
		{
			if(m_pNewInventoryCtrl->CanMove(iTargetIndex, pItemObj))
			{
				ChangeSourceIndex(iSourceIndex);
				ChangeTargetIndex(iTargetIndex);
				SendRequestEquipmentItem(STORAGE_TYPE::MYSHOP, iSourceIndex, pItemObj,
				                         STORAGE_TYPE::MYSHOP, iTargetIndex);
				return true;
			}
		}
	}
	
	else if (m_pNewInventoryCtrl && !pPickedItem &&
		(GetAsyncKeyState(VK_LCONTROL) & 0x8000) && IsPress(VK_RBUTTON))
	{
		if (m_EnablePersonalShop || gOffStore.ShopActive)
		{
			g_pChatListBox->AddText("", GlobalText[1426], SEASON3B::TYPE_ERROR_MESSAGE);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}

		int iCurSquareIndex = m_pNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);
		if (iCurSquareIndex != -1)
		{
			ChangeSourceIndex(iCurSquareIndex);
			ChangeTargetIndex(-1);
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CPersonalShopItemValueMsgBoxLayout));
			SetInputValueTextBox(true);
			return true;
		}
	}
	else if (m_pNewInventoryCtrl && !pPickedItem && IsPress(VK_RBUTTON))
	{
		if (gOffStore.ShopActive)
			return false;

		g_pMyInventory->ResetMouseRButton();

		ITEM* pItem = m_pNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);
		if (pItem)
		{
			int dstIndex = g_pMyInventory->FindEmptySlot(pItem);
			if (dstIndex != -1)
			{
				int srcIndex = m_pNewInventoryCtrl->GetIndexByItem(pItem);
				SendRequestEquipmentItem(STORAGE_TYPE::MYSHOP, srcIndex, pItem, STORAGE_TYPE::INVENTORY, dstIndex);
				m_pNewInventoryCtrl->RemoveItem(pItem);

				PlayBuffer(SOUND_GET_ITEM01);
				return false;
			}
			else
			{
				g_pChatListBox->AddText("", GlobalText[441], SEASON3B::TYPE_ERROR_MESSAGE);
			}
		}
	}

	return false;
}

bool SEASON3B::CNewUIMyShopInventory::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if(m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->UpdateMouseEvent())
	{
		return false;
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if( MyShopInventoryProcess() == true)
		{
			return false;
		}
		
		POINT ptExitBtn1 = { m_Pos.x+169, m_Pos.y+7 };

		if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYSHOP_INVENTORY);
			return false;
		}
		
		if( SEASON3B::IsRelease(VK_LBUTTON) 
			&& CheckMouseIn(m_EditBox->GetPosition_x(), m_EditBox->GetPosition_y(), m_EditBox->GetWidth(), m_EditBox->GetHeight()) )
		{
			ChangeEditBox( UISTATE_NORMAL );
		}
		
		if(SEASON3B::IsRelease(VK_LBUTTON) 
			&& CheckMouseIn(m_EditBox->GetPosition_x(), m_EditBox->GetPosition_y(), m_EditBox->GetWidth(), m_EditBox->GetHeight()) == false )
		{
			SetFocus(g_hWnd);
		}
	}
	
	m_EditBox->DoAction();

	for ( int i = 0; i < MYSHOPINVENTORY_MAXBUTTONCOUNT; ++i )
	{
		if( m_Button[i].UpdateMouseEvent() )
		{
			switch(i)
			{
				case 0:
				{
					g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYSHOP_INVENTORY);
				}
				return false;
				case 2:
				{
					SendRequestDestoryPersonalShop();
					
					g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYSHOP_INVENTORY);
					g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY);
					g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY_EXT);
				}
			    return false;
			}
		}
	}
	
	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if(SEASON3B::IsPress(VK_RBUTTON))
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}

		if(SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}
	
	return true;
}

bool SEASON3B::CNewUIMyShopInventory::Update()
{
	if(m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->Update())
	{
		return false;
	}

	return true;
}

void SEASON3B::CNewUIMyShopInventory::RenderFrame()
{
	RenderImage(IMAGE_MYSHOPINVENTORY_BACK, m_Pos.x + 2, m_Pos.y + 3, 186.f, 426.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_TOP, m_Pos.x, m_Pos.y, INVENTORY_WIDTH, 64.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_LEFT, m_Pos.x, m_Pos.y+64, 21.f, 320.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_RIGHT, m_Pos.x + INVENTORY_WIDTH - 21, m_Pos.y + 64, 21.f, 320.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_BOTTOM, m_Pos.x, m_Pos.y + INVENTORY_HEIGHT - 45, INVENTORY_WIDTH, 45.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_EDIT, m_Pos.x+12, m_Pos.y+49, 169.f, 26.f);

	unicode::t_char Text[100] = {};
	sprintf( Text, GlobalText[1102]);
	RenderText(Text, m_Pos.x, m_Pos.y + 10, INVENTORY_WIDTH, 0, 0xFF49B0FF, 0x00000000, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIMyShopInventory::RenderTextInfo()
{
	unicode::t_char Text[100];

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[370]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 245, 0, 0, RGBA(255, 45, 47, 255), 0x00000000, RT3_SORT_LEFT, g_hFont);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[1109]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 260, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[1111]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 272, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[1112]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 284, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[1113]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 296, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[1114]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 308, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT);

	memset(&Text, 0, sizeof(unicode::t_char) * 100);
	sprintf(Text, GlobalText[3370]);
	RenderText(Text, m_Pos.x + 30, m_Pos.y + 323, 0, 0, RGBA(255, 45, 47, 255), 0x00000000, RT3_SORT_LEFT);
}

void SEASON3B::CNewUIMyShopInventory::InitButtons()
{
	if (gProtect->m_MainInfo.ButtonCharracter[1])
		SetButtonInfo(&m_Btn[0], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "Bless", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[2])
		SetButtonInfo(&m_Btn[1], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "Soul", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[3])
		SetButtonInfo(&m_Btn[2], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "Chaos", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[4])
		SetButtonInfo(&m_Btn[3], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "WC", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[5])
		SetButtonInfo(&m_Btn[4], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "WP", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[6])
		SetButtonInfo(&m_Btn[5], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "WG", "", 0);
	SetButtonInfo(&m_Btn[6], IMAGE_IGS_BUTTON, m_Pos.x + 13, m_Pos.y + 355, 52, 26, 1, 0, 1, 1u, "Off-Shop", "", 0);
	if (gProtect->m_MainInfo.ButtonCharracter[8])
		SetButtonInfo(&m_Btn[7], IMAGE_IGS_BUTTON, 0, 0, 52, 26, 1, 0, 1, 1u, "Zen", "", 0);
}


void SEASON3B::CNewUIMyShopInventory::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
{
	m_Btn->ChangeButtonImgState(1, imgindex, overflg, isimgwidth, bClickEffect);
	m_Btn->ChangeButtonInfo(x, y, sx, sy);

	m_Btn->ChangeText(btname);
	m_Btn->ChangeToolTipText(tooltiptext, istoppos);

	if (MoveTxt)
	{
		m_Btn->MoveTextPos(0, -1);
	}
}

bool SEASON3B::CNewUIMyShopInventory::BtnProcess()
{
	for (int Send = 0; Send <= 7; Send++)
	{
		if (m_Btn[Send].UpdateMouseEvent())
		{
			if ((m_EnablePersonalShop || gOffStore.ShopActive) && Send != 6)
			{
				g_pChatListBox->AddText("", GlobalText[1426], SEASON3B::TYPE_ERROR_MESSAGE);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			if (Send == 7)
			{
				unicode::t_string strTitle;
				GetTitle(strTitle);

				if (IsExistUndecidedPrice() == false && strTitle.size() > 0)
				{
					if (m_EnablePersonalShop == false)
					{
						SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CPersonalshopCreateMsgBoxLayout));
					}
					else
					{
						SendRequestCreatePersonalShop(const_cast<char*>(strTitle.c_str()));
						g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYSHOP_INVENTORY);
						g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY);
						g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY_EXT);
					}
				}
				else
				{
					g_pChatListBox->AddText("", GlobalText[1119], SEASON3B::TYPE_ERROR_MESSAGE);
				}
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			CMyShopBtnConfirmLayout::m_ButtonIndex = Send;
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CMyShopBtnConfirmLayout));
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
	}

	return false;
}


bool SEASON3B::CNewUIMyShopInventory::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();

	RenderTextInfo();

	if(m_EditBox) 
	{
		m_EditBox->Render();
	}

	if(m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->Render();
	}

	for(int i=0; i<MYSHOPINVENTORY_MAXBUTTONCOUNT; ++i)
	{
		m_Button[i].Render();
	}

	int btnIndexList[] = { 0, 1, 2, 3, 4, 5, 7 };
	int charIDList[] = { 1, 2, 3, 4, 5, 6, 8 };

	for (int i = 0; i < 7; ++i)
	{
		if (gProtect->m_MainInfo.ButtonCharracter[charIDList[i]])
		{
			m_Btn[btnIndexList[i]].Render();
		}
	}


	m_Btn[6].Render();


	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIMyShopInventory::ClosingProcess()
{
	CNewUIInventoryCtrl::BackupPickedItem();
	g_pMyInventory->ChangeMyShopButtonStateOpen();
	SetFocus(g_hWnd);
}

int SEASON3B::CNewUIMyShopInventory::GetPointedItemIndex()
{
	int iPointedItemIndex = m_pNewInventoryCtrl->GetPointedSquareIndex();

	if (iPointedItemIndex != -1)
	{
		iPointedItemIndex += MAX_MY_INVENTORY_EX_INDEX;
	}

	return iPointedItemIndex;
}

void SEASON3B::CNewUIMyShopInventory::ResetSubject()
{
	if(m_EditBox)
	{
		m_EditBox->SetText(NULL);
	}
}

bool SEASON3B::CNewUIMyShopInventory::IsEnableInputValueTextBox()
{
	return m_bIsEnableInputValueTextBox;
}

void SEASON3B::CNewUIMyShopInventory::SetInputValueTextBox(bool bIsEnable)
{
	m_bIsEnableInputValueTextBox = bIsEnable;
}

bool SEASON3B::CNewUIMyShopInventory::ProcessMyInvenItemAutoMove()
{
	if (gOffStore.ShopActive && m_EnablePersonalShop)
	{
		g_pChatListBox->AddText("", GlobalText[1433], SEASON3B::TYPE_ERROR_MESSAGE);
		return false;
	}

	if (g_pPickedItem && g_pPickedItem->GetItem())
		return false;

	const auto pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();
	ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(MouseX, MouseY);
	if (!pItemObj)
		return false;

	if (IsPersonalShopBan(pItemObj))
	{
		g_pChatListBox->AddText("", GlobalText[2226], SEASON3B::TYPE_ERROR_MESSAGE);
		return false;
	}

	ITEM_ATTRIBUTE* pAttr = &ItemAttribute[pItemObj->Type];
	const int dstIndex = m_pNewInventoryCtrl->FindEmptySlot(pAttr->Width, pAttr->Height, false);
	if (dstIndex == -1)
	{
		g_pChatListBox->AddText("", GlobalText[441], SEASON3B::TYPE_ERROR_MESSAGE);
		return false;
	}

	const int srcIndex = pMyInvenCtrl->GetIndexByItem(pItemObj);
	SendRequestEquipmentItem(STORAGE_TYPE::INVENTORY, srcIndex, pItemObj, STORAGE_TYPE::MYSHOP, dstIndex);
	pMyInvenCtrl->RemoveItem(pItemObj);
	PlayBuffer(SOUND_GET_ITEM01);

	return true;
}
