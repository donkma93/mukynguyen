#include "Stdafx.h"
#include "NewUIMenu.h"
#include "NewUISystem.h"
#include "WideData.h"
#include "WSclientinline.h"
#include "CProtect.h"
#include "ZzzToolKit.h"

SEASON3B::CNewUIMenuOption::CNewUIMenuOption()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_iNumCurOpenTab = 0;
	BtnMaxOption = 1;
	m_Button = nullptr;
	for (int i = 0; i < MAX_BUTTON_CREATE; ++i)
	{
		m_ButtonVisible[i] = false;
	}

	WINDOW_WIDTH = 283;
	WINDOW_HEIGHT = 210;
}

SEASON3B::CNewUIMenuOption::~CNewUIMenuOption()
{
	Release();
}

bool SEASON3B::CNewUIMenuOption::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MEMNUOPTION, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIMenuOption::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

int SEASON3B::CNewUIMenuOption::CalcAdjustedY()
{
	int visibleCount = 0;
	for (int i = 0; i < 12; ++i)
	{
		if (m_ButtonVisible[i])
			visibleCount++;
	}

	int formMenuH = 0;

	if (visibleCount <= 2)
	{
		formMenuH = 110;
	}
	else if (visibleCount <= 4)
	{
		formMenuH = 137;
	}
	else if (visibleCount <= 6)
	{
		formMenuH = 164;
	}
	else if (visibleCount <= 8)
	{
		formMenuH = 191;
	}
	else if (visibleCount <= 10)
	{
		formMenuH = 218;
	}
	else // 11–12
	{
		formMenuH = 245;
	}

	int screenHeight = 430;
	int posY = (screenHeight - formMenuH) / 2;

	return posY;
}

void SEASON3B::CNewUIMenuOption::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

void SEASON3B::CNewUIMenuOption::InitButtons()
{
	std::list<unicode::t_string> ltext;
	for (int x = 0; x < BtnMaxOption; ++x)
	{
		ltext.push_back(GlobalText[3843 + x]);
	}

	m_TabBtn.CreateRadioGroup(BtnMaxOption, IMAGE_WINDOW_TAB_BTN, TRUE);
	m_TabBtn.ChangeRadioText(ltext);
	m_TabBtn.ChangeRadioButtonInfo(true, m_Pos.x + 10.f, m_Pos.y + 30.f, 56, 22);
	m_TabBtn.ChangeFrame(m_iNumCurOpenTab);

	for (int i = 0; i < MAX_BUTTON_CREATE; ++i)
	{
		m_ButtonVisible[i] = gProtect->m_MainInfo.MenuButton[i];
	}

	int visibleIndex = 0;

	for (int i = 0; i < MAX_BUTTON_CREATE; ++i)
	{
		if (!m_ButtonVisible[i])
		{
			m_Btn[i].ChangeButtonInfo(0, 0, 0, 0);
			continue;
		}

		int row = visibleIndex / 2;
		bool isLeft = (visibleIndex % 2 == 0);

		int x = isLeft ? (m_Pos.x + 25) : (m_Pos.x + 135);
		int y = m_Pos.y + 60 + row * 28;

		SetButtonInfo(
			&m_Btn[i],
			IMAGE_IGS_BUTTON,
			x,
			y,
			108,
			27,
			true,
			false,
			true,
			true,
			GlobalText[3781 + i],
			"",
			false
		);

		visibleIndex++;
	}
}



void SEASON3B::CNewUIMenuOption::LoadImages()
{
	LoadBitmap("Interface\\newui_btn_empty.tga", IMAGE_IGS_BUTTON, GL_LINEAR);
}

bool SEASON3B::CNewUIMenuOption::Update()
{
	if (IsVisible())
	{
		int iNumCurOpenTab = m_TabBtn.UpdateMouseEvent();

		if (iNumCurOpenTab == RADIOGROUPEVENT_NONE)
			return true;

		m_iNumCurOpenTab = iNumCurOpenTab;
	}
	return true;
}

bool SEASON3B::CNewUIMenuOption::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	POINT ptExitBtn1 = { m_Pos.x + 248, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 50))
		return false;
	return true;
}

bool SEASON3B::CNewUIMenuOption::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIMenuOption::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIMenuOption::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIMenuOption::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT, GlobalText[3841]);
	
	m_TabBtn.Render();
	
	int sTapIndex = m_iNumCurOpenTab;
	switch (sTapIndex)
	{
		case 0:
		{
			for (int x = 0; x < MAX_BUTTON_CREATE; x++)
			{
				m_Btn[x].Render();
			}
		}
		break;
		default:
		break;
	}

	DisableAlphaBlend();
	return true;
}

bool SEASON3B::CNewUIMenuOption::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_RANKING);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[1].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENTITME);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[2].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENRELIFE);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[3].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENRSCHANGE);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[4].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENDANHHIEU);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[5].UpdateMouseEvent())
	{
		SendRequestDataSend(0xFD, 0x17);
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENSPIN);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[6].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_IDLEVEL);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[7].UpdateMouseEvent())
	{
		IsVisibleWindow();
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MOCNAP);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[8].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHANGECLASS);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	//
	if (m_Btn[9].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHANGEPASS);
		g_pNewUIChangePass->ClearTextBoxes();
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[10].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENHONHOAN);
		SendRequestDataSendType(0xD5, 0x01, 1);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[11].UpdateMouseEvent())
	{
		g_pNewShop->SendMenuKey();
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[12].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENQUANHAM);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[13].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_EVENTULUYEN);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[14].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GIFTCODE);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MEMNUOPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}


	return false;
}
void SEASON3B::CNewUIMenuOption::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
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
void SEASON3B::CNewUIMenuOption::UnloadImages()
{
	DeleteBitmap(IMAGE_IGS_BUTTON);
}

