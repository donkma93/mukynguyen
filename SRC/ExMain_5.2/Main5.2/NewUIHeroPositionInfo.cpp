// NewUIHeroPositionInfo.cpp: implementation of the CNewUIHeroPositionInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIHeroPositionInfo.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "MapManager.h"
#include "ThangCuoi\WideData.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "CharacterManager.h"
#include "GameShop/MsgBoxIGSCommon.h"
#include "ThangCuoi\Reconnect.h"
#include "ThangCuoi\UIFormSystemRender.h"

using namespace SEASON3B;

namespace
{
	constexpr float kServerDropdownOffsetX = 60.0f;
	constexpr float kServerDropdownWidth = 118.0f;
	constexpr float kServerDropdownHeight = 18.0f;
	constexpr float kServerDropdownItemHeight = 18.0f;
}

CNewUIHeroPositionInfo::CNewUIHeroPositionInfo()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_AutoHP = false;
	m_CurHeroPosition.x = m_CurHeroPosition.y = 0;
	m_bServerDropdownOpen = false;
	m_dwLastSwitchClick = 0;
}

CNewUIHeroPositionInfo::~CNewUIHeroPositionInfo()
{
	Release();
}

//---------------------------------------------------------------------------------------------
// Create
bool CNewUIHeroPositionInfo::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_HERO_POSITION_INFO, this);


	SetPos(x, y);
	LoadImages();

	//Menu Button //ACuoi
	this->InitRenderButton();

	Show(true);

	return true;
}


void CNewUIHeroPositionInfo::InitRenderButton()
{
	WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.4f));
	if (WindowWidth > 800)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.4f));
	}

#if Ex700
	SetButtonInfo(&m_Btn[0], BITMAP_IGS_MSGBOX_BUTTON, (m_Pos.x + WidenX) + 5, m_Pos.y + 6, 19, 18, 1, 0, 1);
	SetButtonInfo(&m_Btn[1], BITMAP_IGS_MSGBOX_BUTTON, (m_Pos.x + WidenX) + 24, m_Pos.y + 6, 19, 18, 1, 0, 1);
	SetButtonInfo(&m_Btn[2], BITMAP_IGS_MSGBOX_BUTTON, (m_Pos.x + WidenX) + 43, m_Pos.y + 6, 19, 18, 1, 0, 1);
	SetButtonInfo(&m_Btn[3], BITMAP_IGS_MSGBOX_BUTTON, (m_Pos.x + WidenX) + 43, m_Pos.y + 6, 19, 18, 1, 0, 1);
#else
	SetButtonInfo(&m_Btn[1], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 1, m_Pos.x + WidenX + 21, m_Pos.y + 1, 18, 13, 1, 0, 1);
	SetButtonInfo(&m_Btn[2], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2, m_Pos.x + WidenX + 37, m_Pos.y + 1, 18, 13, 1, 0, 1);
	SetButtonInfo(&m_Btn[3], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3, m_Pos.x + WidenX + 37, m_Pos.y + 1, 18, 13, 1, 0, 1);
	SetButtonInfo(&m_Btn[0], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4, m_Pos.x + WidenX + 5, m_Pos.y + 1, 18, 13, 1, 0, 1);

#endif
	g_pUIForm->SetButtonInfo(&m_Btn[8], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos - 12, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Auto Reset", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[10], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 43, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Sự Kiện Boss", "", 0);
	//g_pUIForm->SetButtonInfo(&m_Btn[12], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 98, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Attack", "", 0);
	//g_pUIForm->SetButtonInfo(&m_Btn[14], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 153, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "FaceBook", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[4], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 98, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Xóa Tội", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[5], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 153, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Nhiệm Vụ2,3", "", 0);
	//g_pUIForm->SetButtonInfo(&m_Btn[7], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 318, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Tích Nạp", "", 0);

	//g_pUIForm->SetButtonInfo(&m_Btn[15], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 430, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Zalo", "", 0);
	//g_pUIForm->SetButtonInfo(&m_Btn[13], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 485, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "OffAttack", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[9], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 540, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Auto HP", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[6], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 595, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Auto PK", "", 0);


#if RENDER_ITEM_3D
	g_pUIForm->SetButtonInfo(&m_Btn[11], IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, m_Pos.x + gPosWide.x_GetAddPos + 375, m_Pos.y + 416, 58, 14, 1, 0, 1, 1u, "Admin Color", "", 0);
#endif
}

bool CNewUIHeroPositionInfo::Render()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		return false;
	}

	unicode::t_char szText[255] = { NULL, };

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	if (Hero->PK >= 4)
	{
		m_Btn[4].Render();
	}
	if (IsToolKit.CheckClass())
	{
		m_Btn[5].Render();
	}

	m_Btn[6].Render();
	m_Btn[7].Render();
	m_Btn[8].Render();
	m_Btn[9].Render();
	m_Btn[10].Render();
	m_Btn[12].Render();
	m_Btn[13].Render();
	m_Btn[14].Render();
	m_Btn[15].Render();

#if RENDER_ITEM_3D
	if (!g_pNewUISystem->IsVisible(INTERFACE_RENDER_COLOR))
	{
		m_Btn[11].Render();
	}
#endif
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	RenderImage(IMAGE_TIME, m_Pos.x - 24, m_Pos.y + 15, 170, 15);

	time_t TimeServer, TimeLocal;
	struct tm* ServerT, * LocalT;
	time(&TimeServer);
	time(&TimeLocal);
	// ----
	ServerT = gmtime(&TimeServer);
	// ----
	// -----
	LocalT = localtime(&TimeLocal);

	unicode::_sprintf(szText, "%2d:%02d:%02d", LocalT->tm_hour, LocalT->tm_min, LocalT->tm_sec);
	g_pRenderText->RenderText(m_Pos.x - 4, m_Pos.y + 17, szText, WidenX + 20, 13 - 4, RT3_SORT_CENTER);

#if Ex700

	RenderBitmap(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_07, m_Pos.x + 100, m_Pos.y, 124.0, 24.0, 0.0, 0.285, 0.715, 0.26, 1, 1, 0.0);
	RenderBitmap(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_07, m_Pos.x, m_Pos.y, 124.0, 24.0, 0.0, 0.0, 0.715, 0.28, 1, 1, 0.0);

	unicode::_sprintf(szText, "%s (%d,%d)", gMapManager.GetMapName(gMapManager.WorldActive), Hero->PositionX, Hero->PositionY);

	g_pRenderText->RenderText(m_Pos.x - 5, m_Pos.y + 4, szText, WidenX + 20, 13 - 4, RT3_SORT_CENTER);

	g_pNewUICastleEvent->RenderTime();

	SIZE Btn3D = { 19.f, 19.f };

	g_pUIForm->RenderButton3D(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_08, (m_Pos.x + WidenX) + 4.f, m_Pos.y + 5.f, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButton3D(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_09, (m_Pos.x + WidenX) + Btn3D.cx + 5.f, m_Pos.y + 5.f, Btn3D.cx, Btn3D.cy, 1);

	if (!MUHelper::g_MuHelper.IsActive())
	{
		g_pUIForm->RenderButton3D(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_10, (m_Pos.x + WidenX) + (Btn3D.cx * 2) + 5.f, m_Pos.y + 5.f, Btn3D.cx, Btn3D.cy, 1);
	}
	else
	{
		g_pUIForm->RenderButton3D(CNewUIMainFrameWindow::IMAGE_GAUGE_BTN_11, (m_Pos.x + WidenX) + (Btn3D.cx * 2) + 5.f, m_Pos.y + 5.f, Btn3D.cx, Btn3D.cy, 1);
	}

#else

	m_Btn[0].Render();
	m_Btn[1].Render();
	MUHelper::g_MuHelper.IsActive() ? m_Btn[3].Render() : m_Btn[2].Render();

	if (gMapManager.WorldActive == WD_82NEW_CTC)
	{
		RenderImage(UI_BAR_NEW_TIMER_CTC, m_Pos.x, m_Pos.y, float(256.f), float(25.f));
	}
	else
	{
		RenderImage(IMAGE_HERO_POSITION_INFO_BASE_WINDOW, m_Pos.x, m_Pos.y, float(194.f), float(25.f));
	}

	unicode::_sprintf(szText, "%s", gMapManager.GetMapName(gMapManager.WorldActive));
	g_pRenderText->RenderText(m_Pos.x - 5, m_Pos.y + 4, szText, WidenX + 20, 13 - 4, RT3_SORT_CENTER);

	g_pNewUICastleEvent->RenderTime();
#endif

	//IsToolKit.ThisFont(m_Pos.x + gPosWide.x_GetAddPos - 155, m_Pos.y + 385, 0xFFDE26FF, 0, 200, 0, RT3_SORT_CENTER, "- [Phiên Bản] : [Season 5.2]");


	if (!m_AutoCtrl)
	{
		IsToolKit.ThisFont(m_Pos.x + gPosWide.x_GetAddPos - 175, m_Pos.y + 405, 0xFF0000FF, 0, 200, 0, RT3_SORT_CENTER, "● Auto PK : OFF ●");
	}
	else
	{
		IsToolKit.ThisFont(m_Pos.x + gPosWide.x_GetAddPos - 175, m_Pos.y + 405, 0x11FF00FF, 0, 200, 0, RT3_SORT_CENTER, "● Auto PK : ON ●");
	}
	if (!m_AutoHP)
	{
		IsToolKit.ThisFont(m_Pos.x + gPosWide.x_GetAddPos - 175, m_Pos.y + 420, 0xFF0000FF, 0, 200, 0, RT3_SORT_CENTER, "● Auto HP : OFF ●");
	}
	else
	{
		IsToolKit.ThisFont(m_Pos.x + gPosWide.x_GetAddPos - 175, m_Pos.y + 420, 0x11FF00FF, 0, 200, 0, RT3_SORT_CENTER, "● Auto HP : ON ●");
	}

	this->RenderServerDropdown();

	DisableAlphaBlend();
	return true;
}
void CNewUIHeroPositionInfo::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIHeroPositionInfo::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool CNewUIHeroPositionInfo::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MEMNUOPTION);

		for (int i = 0; i < 11; i++)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_JEWELBANK + i);
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}


	if (m_Btn[1].UpdateMouseEvent()) // Config
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MUHELPER);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[2].UpdateMouseEvent()) // Start
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER))
		{
			CMsgBoxIGSCommon* pMsgBox = NULL;
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CMsgBoxIGSCommonLayout), &pMsgBox);
			pMsgBox->Initialize(GlobalText[3028], GlobalText[3186]);
			return false;
		}
		else if (CharacterAttribute->Level < CharacterAttribute->MuHelperLevel)
		{
			unicode::t_char szText[MAX_TEXT_LENGTH] = { '\0', };
			sprintf(szText, GlobalText[3188], CharacterAttribute->MuHelperLevel);
			CMsgBoxIGSCommon* pMsgBox = NULL;
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CMsgBoxIGSCommonLayout), &pMsgBox);
			pMsgBox->Initialize(GlobalText[3028], szText);
			return false;
		}
		else
		{
			MUHelper::g_MuHelper.Toggle();
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[4].UpdateMouseEvent() && Hero->PK >= 4) //xoa toi
	{
		char Text[1200];
		sprintf(Text, GlobalText[3873]);
		CreateNotice(Text, 1);
		IsToolKit.KeySendState(0xFD, 0x30);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[5].UpdateMouseEvent() && IsToolKit.CheckClass()) //Nhiem Vụ  2 3
	{
		char Text[1200];
		sprintf(Text, GlobalText[3874]);
		CreateNotice(Text, 1);
		IsToolKit.KeySendState(0xFD, 0x31);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[6].UpdateMouseEvent()) //AutoCtrl
	{
		m_AutoCtrl = !m_AutoCtrl;
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[7].UpdateMouseEvent()) //MOCNAP
	{
		g_pNewUISystem->Toggle(INTERFACE_MOCNAP);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[8].UpdateMouseEvent()) //Auto Reset
	{
		g_pNewUISystem->Toggle(INTERFACE_AUTORESET);
		SendRequestResetInfo();
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[9].UpdateMouseEvent()) //AutoHP
	{
		IsToolKit.KeySendState(0xFD, 0x3E);
		m_AutoHP = !m_AutoHP;
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[10].UpdateMouseEvent()) //Thong Tin Boss
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENINVASION))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_EVENINVASION);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_EVENINVASION);
			SendRequestDataSend(0xF7, 0x02);
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}


	if (m_Btn[12].UpdateMouseEvent()) //Attack
	{
		IsToolKit.KeySendState(0xFD, 0x34);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[13].UpdateMouseEvent()) //OffAttack
	{
		IsToolKit.KeySendState(0xFD, 0x35);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[14].UpdateMouseEvent()) // FaceBook
	{
		ShellExecute(0, "open", "https://www.facebook.com/mukynguyen/", 0, 0, SW_SHOWNORMAL);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (m_Btn[15].UpdateMouseEvent()) // Zalo
	{
		ShellExecute(0, "open", "https://zalo.me/g/ckdxgj817", 0, 0, SW_SHOWNORMAL);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
#if RENDER_ITEM_3D
	if (m_Btn[11].UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_RENDER_COLOR);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
#endif
	return false;
}

bool CNewUIHeroPositionInfo::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	float dropX = m_Pos.x + WidenX + kServerDropdownOffsetX;
	float dropY = m_Pos.y + 2.0f;
	float dropW = kServerDropdownWidth;
	float dropH = kServerDropdownHeight;

	if (CheckMouseIn(dropX, dropY, dropW, dropH))
	{
		if (SEASON3B::IsRelease(VK_LBUTTON) && GetTickCount() > m_dwLastSwitchClick + 250)
		{
			m_dwLastSwitchClick = GetTickCount();
			m_bServerDropdownOpen = !m_bServerDropdownOpen;
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if (m_bServerDropdownOpen)
	{
		float itemY = dropY + dropH + 2.0f;
		WORD subPorts[5] = { 55901, 55902, 55903, 55904, 55905 };

		for (int i = 0; i < 5; i++)
		{
			float curItemY = itemY + (i * kServerDropdownItemHeight);
			if (CheckMouseIn(dropX, curItemY, dropW, kServerDropdownItemHeight))
			{
				if (SEASON3B::IsRelease(VK_LBUTTON) && GetTickCount() > m_dwLastSwitchClick + 250)
				{
					m_dwLastSwitchClick = GetTickCount();
					ExecuteSwitchSub(subPorts[i]);
					m_bServerDropdownOpen = false;
					PlayBuffer(SOUND_CLICK01);
				}
				return false;
			}
		}

		if (SEASON3B::IsRelease(VK_LBUTTON) || SEASON3B::IsRelease(VK_RBUTTON))
		{
			m_bServerDropdownOpen = false;
		}
	}

	int Width = HERO_POSITION_INFO_BASEA_WINDOW_WIDTH + WidenX + 73;

	if (CheckMouseIn(m_Pos.x, m_Pos.y, Width, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT) || CheckMouseIn(4, 140, 64, 16))
	{
		return false;
	}

	if    (CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 92, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 143, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 485, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 430, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 600, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 194, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 240, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 375, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 263, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 318, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos - 10, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 549, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 35, m_Pos.y + 416, 58, 14)
		|| CheckMouseIn(m_Pos.x + gPosWide.x_GetAddPos + 40, m_Pos.y + 416, 58, 14)
		&& IsToolKit.CheckClass())
	{
		return false;
	}

	return true;
}

bool CNewUIHeroPositionInfo::UpdateKeyEvent()
{
	return true;
}

bool CNewUIHeroPositionInfo::Update()
{
	if ((IsVisible() == true) && (Hero != NULL))
	{
		m_CurHeroPosition.x = (Hero->PositionX);
		m_CurHeroPosition.y = (Hero->PositionY);
	}

	return true;
}


float CNewUIHeroPositionInfo::GetLayerDepth()
{
	return 4.3f;
}

void CNewUIHeroPositionInfo::OpenningProcess()
{

}

void CNewUIHeroPositionInfo::ClosingProcess()
{

}

void CNewUIHeroPositionInfo::SetCurHeroPosition(int x, int y)
{
	m_CurHeroPosition.x = x;
	m_CurHeroPosition.y = y;
}

void CNewUIHeroPositionInfo::LoadImages()
{
	LoadBitmap("Interface\\Minimap_positionA.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Setup.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 1, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Start.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Stop.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_MenuNew.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_ClearPK.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5, GL_LINEAR);
	LoadBitmap("ACuoi\\freecoin.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 6, GL_LINEAR);
	LoadBitmap("ACuoi\\ACuoinuttion.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 7, GL_LINEAR);
	LoadBitmap("Interface\\Minimap_positionE.tga", UI_BAR_NEW_TIMER_CTC, GL_LINEAR);
	LoadBitmap("Interface\\image_time.tga", IMAGE_TIME, GL_LINEAR);
}

void CNewUIHeroPositionInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW);
	DeleteBitmap(UI_BAR_NEW_TIMER_CTC);
	DeleteBitmap(IMAGE_TIME);

	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 1);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 6);
}

void CNewUIHeroPositionInfo::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect)
{
	m_Btn->ChangeButtonImgState(true, imgindex, overflg, isimgwidth, bClickEffect);
	m_Btn->ChangeButtonInfo(x, y, sx, sy);
}

void CNewUIHeroPositionInfo::RenderServerDropdown()
{
	float dropX = m_Pos.x + WidenX + kServerDropdownOffsetX;
	float dropY = m_Pos.y + 2.0f;
	float dropW = kServerDropdownWidth;
	float dropH = kServerDropdownHeight;

	WORD curPort = g_pReconnect->s_Data.GameServerPort;
	const char* curName = "▼ Server";
	BYTE curR = 255, curG = 255, curB = 255;

	if (curPort == 55901)
	{
		curName = "▼ S1 Boss";
		curR = 255; curG = 215; curB = 0;   // Gold
	}
	else if (curPort == 55902)
	{
		curName = "▼ S2 Thường";
		curR = 255; curG = 255; curB = 255; // White
	}
	else if (curPort == 55903)
	{
		curName = "▼ S3 VIP";
		curR = 0; curG = 220; curB = 255;   // Cyan
	}
	else if (curPort == 55904)
	{
		curName = "▼ S4 R0-190";
		curR = 90; curG = 255; curB = 120;  // Green
	}
	else if (curPort == 55905)
	{
		curName = "▼ S5 Non-PvP";
		curR = 255; curG = 165; curB = 0;   // Orange
	}

	// Render Dropdown Header
	g_pUIForm->RenderBack((int)dropX, (int)dropY, (int)dropW, (int)dropH, 0.85f);

	if (CheckMouseIn(dropX, dropY, dropW, dropH))
	{
		g_pUIForm->RenderHover(dropX + 2, dropY + 2, dropW - 4, dropH - 4, 0x0080C060);
	}

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(curR, curG, curB, 255);
	g_pRenderText->RenderText((int)dropX, (int)dropY + 4, curName, (int)dropW, 0, RT3_SORT_CENTER);

	// Render Dropdown Menu Items when open
	if (m_bServerDropdownOpen)
	{
		float menuY = dropY + dropH + 2.0f;
		float menuH = 5 * kServerDropdownItemHeight;

		g_pUIForm->RenderBack((int)dropX, (int)menuY, (int)dropW, (int)menuH, 0.92f);

		const char* subNames[5] = { "S1 Boss", "S2 Thường", "S3 VIP", "S4 R0-190", "S5 Non-PvP" };
		BYTE subColors[5][3] = { {255, 215, 0}, {255, 255, 255}, {0, 220, 255}, {90, 255, 120}, {255, 165, 0} };
		WORD subPorts[5] = { 55901, 55902, 55903, 55904, 55905 };

		for (int i = 0; i < 5; i++)
		{
			float curItemY = menuY + (i * kServerDropdownItemHeight);
			if (CheckMouseIn(dropX, curItemY, dropW, kServerDropdownItemHeight))
			{
				g_pUIForm->RenderHover(dropX + 2, curItemY + 1, dropW - 4, kServerDropdownItemHeight - 2, 0x0080C080);
			}

			char itemLabel[64];
			if (curPort == subPorts[i])
			{
				sprintf(itemLabel, "● %s", subNames[i]);
			}
			else
			{
				sprintf(itemLabel, "%s", subNames[i]);
			}

			g_pRenderText->SetTextColor(subColors[i][0], subColors[i][1], subColors[i][2], 255);
			g_pRenderText->RenderText((int)dropX, (int)curItemY + 3, itemLabel, (int)dropW, 0, RT3_SORT_CENTER);
		}
	}
}

void CNewUIHeroPositionInfo::ExecuteSwitchSub(WORD targetPort)
{
	if (g_pReconnect->s_Data.GameServerPort == targetPort)
		return;

	char szIp[16] = { 0 };
	if (g_pReconnect->s_Data.GameServerAddress[0] != 0)
	{
		strcpy_s(szIp, g_pReconnect->s_Data.GameServerAddress);
	}
	else if (gProtect && gProtect->m_MainInfo.IpAddress[0] != 0)
	{
		strcpy_s(szIp, gProtect->m_MainInfo.IpAddress);
	}
	else
	{
		strcpy_s(szIp, "127.0.0.1");
	}

	strcpy_s(g_pReconnect->s_Data.GameServerAddress, szIp);
	g_pReconnect->s_Data.GameServerPort = targetPort;

	if (CharacterAttribute)
	{
		memcpy(g_pReconnect->s_Data.ReconnectName, CharacterAttribute->Name, sizeof(g_pReconnect->s_Data.ReconnectName));
	}

	g_pReconnect->ReconnectViewportDestroy();
	PartyNumber = 0;

	g_pReconnect->ReconnectSetInfo(CReconnect::RECONNECT_STATUS_RECONNECT, CReconnect::RECONNECT_PROGRESS_NONE, 300, (gProtect ? gProtect->m_MainInfo.ReconnectTime : 30) * 1000);

	SocketClient.Close();
}
