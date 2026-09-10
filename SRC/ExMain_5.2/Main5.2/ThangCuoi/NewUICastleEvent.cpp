#include "Stdafx.h"
#include "NewUICastleEvent.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <MapManager.h>

SEASON3B::CNewUICastleEvent::CNewUICastleEvent()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;

	IsToolKit.StartTick = 0;
	IsToolKit.EnterSerectArea = 0;
	IsToolKit.EventCurrentTime = 0;

	memset(IsToolKit.GuildNameOld, 0, sizeof(IsToolKit.GuildNameOld));
	memset(IsToolKit.ValueCoin, 0, sizeof(IsToolKit.ValueCoin));
	memset(IsToolKit.EventGate, 0, sizeof(IsToolKit.EventGate));
	memset(IsToolKit.EventClassMonster, 0, sizeof(IsToolKit.EventClassMonster));
}

SEASON3B::CNewUICastleEvent::~CNewUICastleEvent()
{
	Release();
}

bool SEASON3B::CNewUICastleEvent::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CASTLEEVENT, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUICastleEvent::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUICastleEvent::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 40;
}

bool SEASON3B::CNewUICastleEvent::Update()
{
	return true;
}

void SEASON3B::CNewUICastleEvent::CGMoveCTCMini(int Type)
{
	if (Type > 0)
	{
		CTCMINI_CGPACKET pMsg{};
		pMsg.header.set(0xF3, 0x39, sizeof(pMsg));
		pMsg.CongVao = Type - 1;
		DataSend((LPBYTE)&pMsg, pMsg.header.size);
	}
}

bool SEASON3B::CNewUICastleEvent::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	POINT ptExitBtn1 = { m_Pos.x + 248, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON))
	{
		if (CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))// || !CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CASTLEEVENT);
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		return false;

	return true;
}

bool SEASON3B::CNewUICastleEvent::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CASTLEEVENT);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUICastleEvent::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUICastleEvent::GetKeyEventOrder()
{
	return 3.4;
}

void SEASON3B::CNewUICastleEvent::RenderText()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	IsToolKit.ThisFont(m_Pos.x + 18, m_Pos.y + 37, 0xFFEECCE1, 0x590000FF, 232, 0, RT3_SORT_CENTER, GlobalText[762], Hero->ID);

	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 60, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s +%d %s", GlobalText[773], IsToolKit.ValueCoin[0], GlobalText[774]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 60, 0x2AA147FF, 0, 270, 0, RT3_SORT_LEFT, "%s +%d", GlobalText[773], IsToolKit.ValueCoin[0]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 60, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s", GlobalText[773]);

	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 71, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s +%d %s", GlobalText[773], IsToolKit.ValueCoin[1], GlobalText[775]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 71, 0x2AA147FF, 0, 270, 0, RT3_SORT_LEFT, "%s +%d", GlobalText[773], IsToolKit.ValueCoin[1]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 71, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s", GlobalText[773]);


	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 82, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s +%d %s", GlobalText[773], IsToolKit.ValueCoin[2], GlobalText[776]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 82, 0x2AA147FF, 0, 270, 0, RT3_SORT_LEFT, "%s +%d", GlobalText[773], IsToolKit.ValueCoin[2]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 82, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, "%s", GlobalText[773]);

	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 97, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, GlobalText[766]);
	IsToolKit.ThisFont(m_Pos.x + 23, m_Pos.y + 108, 0xFFEECCE1, 0, 270, 0, RT3_SORT_LEFT, GlobalText[767]);

	IsToolKit.ThisFont(m_Pos.x + 140, m_Pos.y + 142, 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, "%s: %s %s", GlobalText[763], IsToolKit.GuildNameOld, GlobalText[764]);
	IsToolKit.ThisFont(m_Pos.x + 140, m_Pos.y + 142, 0xD18B00FF, 0, 110, 0, RT3_SORT_LEFT, "%s: %s", GlobalText[763], IsToolKit.GuildNameOld);
	IsToolKit.ThisFont(m_Pos.x + 140, m_Pos.y + 142, 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, "%s:", GlobalText[763]);

	DisableAlphaBlend();
}

void SEASON3B::CNewUICastleEvent::RenderTime()
{
	if (gMapManager.WorldActive != WD_82NEW_CTC)
	{
		return;
	}

	DWORD elapsedTick = (GetTickCount() - IsToolKit.StartTick) / 1000;
	int remainTime = IsToolKit.EventCurrentTime - elapsedTick;

	if (remainTime < 0)
	{
		remainTime = 0;
	}

	int hours = remainTime / 3600;
	int minutes = (remainTime / 60) % 60;
	int seconds = remainTime % 60;

	DWORD color = 0xFFDC00FF;

	if (remainTime < 60)
	{
		color = 0xFF0000FF;
	}
	else if (remainTime < 120)
	{
		color = 0xFA7A02FF;
	}

	IsToolKit.ThisFont(m_Pos.x - 125, m_Pos.y - 36, color, 0, 63, 0, 3, "%02d:%02d:%02d", hours, minutes, seconds);
}



bool SEASON3B::CNewUICastleEvent::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderImage(IMAGE_BASE_WINDOW_BACK,		m_Pos.x + 1, m_Pos.y + 5, 268.f, 306.f);
	RenderImage(IMAGE_BASE_WINDOW_TOP,		m_Pos.x, m_Pos.y, 270.f, 64.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT,		m_Pos.x, m_Pos.y + 64.f, 21.f, 203.f);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT,	m_Pos.x + 249, m_Pos.y + 64.f, 21.f, 203.f);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM,	m_Pos.x, m_Pos.y + 267.f, 270.f, 45.f);

	g_pUIForm->RenderBack(m_Pos.x + 15, m_Pos.y + 35, 240, 100);
	g_pUIForm->RenderBack(m_Pos.x + 15, m_Pos.y + 135, 240, 155);

	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 10, 0xFFEECCE1, 0, 270, 0, RT3_SORT_CENTER, GlobalText[761]);

	for (int i = 0; i < 4; i++)
	{
		m_Btn[i].Render();
	}

	if(IsToolKit.EnterSerectArea)
	{
		m_Btn[4].Render();
	}

	EnableAlphaBlend();
	RenderBitmap(IMAGE_HD_LOGO_0, m_Pos.x + 100, m_Pos.y + 147.f, 180.f, 140.f, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
	DisableAlphaBlend();

	this->RenderText();

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUICastleEvent::InitButtons()
{
	for (int i = 0; i < 5; i++)
	{
		g_pUIForm->SetButtonInfo(&m_Btn[i], SEASON3B::CNewUIMenuOption::IMAGE_IGS_BUTTON
			, m_Pos.x + 23
			, m_Pos.y + 141.f + (28.f * i)
			, 108, 27, true, false, true, true, GlobalText[768 + i], "", false);
	}
}

bool SEASON3B::CNewUICastleEvent::BtnProcess()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_Btn[i].UpdateMouseEvent())
		{
			CGMoveCTCMini(i + 1);
			PlayBuffer(SOUND_CLICK01);
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CASTLEEVENT);
			return true;
		}
	}

	if (m_Btn[4].UpdateMouseEvent() && IsToolKit.EnterSerectArea)
	{
		CGMoveCTCMini(5);
		PlayBuffer(SOUND_CLICK01);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_CASTLEEVENT);
		return true;
	}
	return false;
}


void SEASON3B::CNewUICastleEvent::LoadImages()
{
	LoadBitmap("ACuoi\\CTCMiniMap.jpg", IMAGE_HD_LOGO_0, GL_LINEAR);
}

void SEASON3B::CNewUICastleEvent::UnloadImages()
{
	DeleteBitmap(IMAGE_HD_LOGO_0);
}
