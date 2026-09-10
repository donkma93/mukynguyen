#include "Stdafx.h"
#include "NewUiAutoReset.h"
#include "WSclientinline.h"
#include <LoginWin.h>

SEASON3B::CNewUIAutoReset::CNewUIAutoReset()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_bResetVip = false;
	rsthuong = 0;
	rsvip = 0;
	WINDOW_WIDTH = 270;
	WINDOW_HEIGHT = 250;
}

SEASON3B::CNewUIAutoReset::~CNewUIAutoReset()
{
	Release();
}
bool SEASON3B::CNewUIAutoReset::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_AUTORESET, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIAutoReset::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUIAutoReset::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIAutoReset::Update()
{
	return true;
}
bool SEASON3B::CNewUIAutoReset::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 20, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_AUTORESET);
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 90, m_Pos.y + 223, 15, 15))
	{
		m_bResetVip = !m_bResetVip;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 107, m_Pos.y + 237, 52, 26))
	{
		if (m_bResetVip == true)
		{
			SendRequestResetInfoAuto(0, 1);
		}
		else
		{
			SendRequestResetInfoAuto(1, 0);
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIAutoReset::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_AUTORESET);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIAutoReset::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIAutoReset::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIAutoReset::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH + 20, (float)WINDOW_HEIGHT, "Hệ Thống Auto Reset");

	g_pUIForm->RenderBack(m_Pos.x + 8, m_Pos.y + 30, float(WINDOW_WIDTH) - 10, 70);
	g_pUIForm->RenderBack(m_Pos.x + 8, m_Pos.y + 100, float(WINDOW_WIDTH) - 10, 125);
	//g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 160, float(WINDOW_WIDTH) - 20, 85);

	RenderText();
	m_Btn[0].Render();
	InitButtons();
	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIAutoReset::RenderText()
{
	char textrs[150];

	if (m_bResetVip) {
		RenderImage(CLoginWin::IMAGE_LIST::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 90, m_Pos.y + 223, 15, 15, 0, 0);
	}
	else {
		RenderImage(CLoginWin::IMAGE_LIST::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 90, m_Pos.y + 223, 15, 15, 0, 15.f);
	}

	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 226, 0x00FF90FF, 0, 200, 0, RT3_SORT_LEFT, "Kích Hoạt Reset VIP");
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 270, 0xFF3300FF, 0, 200, 0, RT3_SORT_LEFT, "[Lưu Ý]");
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 280, 0x00FFDDFF, 0, 200, 0, RT3_SORT_LEFT, "- Kiểm Tra Kĩ Các Điều Kiện Reset VIP Ngọc Và WcoinC");
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 290, 0x00FFDDFF, 0, 230, 0, RT3_SORT_LEFT, "- Reset VIP Ngọc Sẽ Được Trừ Trực Tiếp Ở Ngân Hàng Ngọc");
	IsToolKit.ThisFont(m_Pos.x + 75, m_Pos.y + 165, 0xFF3300FF, 0, 200, 0, RT3_SORT_LEFT, "● Nguyên Liệu Cần Khi Reset Vip ●");

	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 35, 0x00FF90FF, 0, 200, 0, RT3_SORT_CENTER, "__-=Điều Kiện Reset Thường=-__");
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 45, 0x00FFDDFF, 0, 200, 0, RT3_SORT_CENTER, "Giới Hạn Reset Hôm Nay Của Bạn");
	sprintf(textrs, "%d / %d", m_AutoResetInfo.m_Reset, m_AutoResetInfo.m_GHRS);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 55, 0xFF3300FF, 0, 200, 0, RT3_SORT_CENTER, textrs);

	sprintf(textrs, "Thưởng Point : %d", m_AutoResetInfo.m_Point);
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 75, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Level : %d", m_AutoResetInfo.m_Level);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 20, m_Pos.y + 75, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Zen : %d", m_AutoResetInfo.m_Money);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH - 70, m_Pos.y + 75, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	float PosY = 0;
	PosY = 70;
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 35 + PosY, 0x00FF90FF, 0, 200, 0, RT3_SORT_CENTER, "__-=Điều Kiện Reset Vip=-__");
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 45 + PosY, 0x00FFDDFF, 0, 200, 0, RT3_SORT_CENTER, "Giới Hạn Reset Hôm Nay Của Bạn");

	sprintf(textrs, "%d / %d", m_AutoResetInfo.m_Reset, m_AutoResetInfo.m_GHRS);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 100, m_Pos.y + 55 + PosY, 0xFF3300FF, 0, 200, 0, RT3_SORT_CENTER, textrs);

	sprintf(textrs, "Thưởng Point : %d", m_AutoResetInfo.m_VipPoint);
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 75 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);
	sprintf(textrs, "Level : %d", m_AutoResetInfo.m_VipLevel);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 20, m_Pos.y + 75 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Zen : %d", m_AutoResetInfo.m_VipMoney);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH - 70, m_Pos.y + 75 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);
	//--------------------------------------------------------------------------------------------------------------------
	sprintf(textrs, "WCoinC : %d", m_AutoResetInfo.m_Coin[0]);
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 110 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	//sprintf(textrs, "WCoinP : %d", m_AutoResetInfo.m_Coin[1]);
	//IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 20, m_Pos.y + 95 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);
	//--------------------------------------------------------------------------------------------------------------------
	sprintf(textrs, "Bless : %d Viên", m_AutoResetInfo.m_Ngoc[0]);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 20, m_Pos.y + 110 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);
	
	sprintf(textrs, "Soul : %d Viên", m_AutoResetInfo.m_Ngoc[1]);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH - 70, m_Pos.y + 110 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Life : %d Viên", m_AutoResetInfo.m_Ngoc[2]);
	IsToolKit.ThisFont(m_Pos.x + 16, m_Pos.y + 130 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Cre : %d Viên", m_AutoResetInfo.m_Ngoc[3]);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH - 70, m_Pos.y + 130 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	sprintf(textrs, "Chaos : %d Viên", m_AutoResetInfo.m_Ngoc[4]);
	IsToolKit.ThisFont(m_Pos.x + WINDOW_WIDTH / 2 - 20, m_Pos.y + 130 + PosY, 0xFFDE26FF, 0, 110, 0, RT3_SORT_LEFT, textrs);

	//--------------------------------------------------------------------------------------------------------------------
}
void SEASON3B::CNewUIAutoReset::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 107, m_Pos.y + 237, 52, 26, 1, 0, 1, 1u, "Auto", "", 0);

	//g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 200, m_Pos.y + 211, 52, 26, 1, 0, 1, 1u, GlobalText[4012], "", 0);
}

bool SEASON3B::CNewUIAutoReset::BtnProcess()
{
	m_Btn[0].Render();
	return false;
}

void SEASON3B::CNewUIAutoReset::LoadImages()
{
}
void SEASON3B::CNewUIAutoReset::UnloadImages()
{
}

void SEASON3B::CNewUIAutoReset::GCResetAutoInfo(const BYTE* lpMsg)
{
	if (!recv) return;
	auto Data2 = reinterpret_cast<const RESET_INFO_RECV*>(lpMsg);

	m_AutoResetInfo.m_ResetThuong = Data2->ResetThuong;
	m_AutoResetInfo.m_ResetVip = Data2->ResetVip;
	m_AutoResetInfo.m_GHRS = Data2->GHRS;
	m_AutoResetInfo.m_Reset = Data2->Reset;
	m_AutoResetInfo.m_Level = Data2->Level;
	m_AutoResetInfo.m_Money = Data2->Money;
	m_AutoResetInfo.m_Point = Data2->Point;
	m_AutoResetInfo.m_VipLevel = Data2->VipLevel;
	m_AutoResetInfo.m_VipMoney = Data2->VipMoney;
	m_AutoResetInfo.m_VipPoint = Data2->VipPoint;
	m_AutoResetInfo.m_Coin[0] = Data2->Coin[0];
	//m_AutoResetInfo.m_Coin[1] = Data2->Coin[1];
	m_AutoResetInfo.m_Ngoc[0] = Data2->Ngoc[0];
	m_AutoResetInfo.m_Ngoc[1] = Data2->Ngoc[1];
	m_AutoResetInfo.m_Ngoc[2] = Data2->Ngoc[2];
	m_AutoResetInfo.m_Ngoc[3] = Data2->Ngoc[3];
	m_AutoResetInfo.m_Ngoc[4] = Data2->Ngoc[4];
}