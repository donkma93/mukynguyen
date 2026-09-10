#include "Stdafx.h"
#include "NewUIRenderCoin.h"
#include "WSclientinline.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ThangCuoi\WideData.h"
#include "ThangCuoi\StructSendGs.h"

POINT MaxPosition;
POINT CurPosition;
bool  DragItem;
DWORD ActionTime;

SEASON3B::CNewUIRenderCoin::CNewUIRenderCoin()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;

	MaxPosition.x = IsToolKit.GetPositionScreen() - 130;
	MaxPosition.y = 5;

	CurPosition.x = MouseX;
	CurPosition.y = MouseY;

	DragItem = false;
}

SEASON3B::CNewUIRenderCoin::~CNewUIRenderCoin()
{
	Release();
}

bool SEASON3B::CNewUIRenderCoin::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENCOIN, this);

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIRenderCoin::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRenderCoin::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIRenderCoin::Update()
{
	return true;
}

bool SEASON3B::CNewUIRenderCoin::UpdateMouseEvent()
{
	if (CheckMouseIn(m_Pos.x, m_Pos.y, 160, 150))
		return false;
	return true;
}

bool SEASON3B::CNewUIRenderCoin::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENCOIN);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRenderCoin::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRenderCoin::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIRenderCoin::Render()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MEMNUOPTION) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_JEWELBANK) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_RANKING) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENTITME) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENRELIFE) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENRSCHANGE) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GIFTCODE) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENDANHHIEU) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENQUANHAM) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENTULUYEN) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENHONHOAN) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENSPIN) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_IDLEVEL) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOCNAP) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CASTLEEVENT) == true
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER) == true
		)
	{
		return false;
	}

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	if (GetKeyState(VK_RBUTTON) & 0x8000 && GetTickCount() > ActionTime + 1000)
	{
		if (IsToolKit.IsWorkZone(MaxPosition.x, MaxPosition.y, 123.f, 65.f))
		{
			if (!DragItem)
			{
				DragItem = true;
				CurPosition.x = MouseX;
				CurPosition.y = MouseY;
			}
		}
		if (DragItem)
		{
			MaxPosition.x += MouseX - CurPosition.x;
			MaxPosition.y += MouseY - CurPosition.y;
			CurPosition.x = MouseX;
			CurPosition.y = MouseY;
		}
	}
	else
	{
		if (DragItem)
		{
			DragItem = false;
		}
	}

	if (MaxPosition.x < 0)		MaxPosition.x = 0;
	if (MaxPosition.y < 0)		MaxPosition.y = 0;

	if (MaxPosition.x > 723)		MaxPosition.x = 723;
	if (MaxPosition.y > 391)		MaxPosition.y = 391;

	IsToolKit.RenderToolTip(MaxPosition.x, MaxPosition.y, 120, (MAX_COIN_POINT * 12) + 2);

	char GetValue[255];

	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	for (int i = 0; i < 5; i++)
	{
		g_pRenderText->RenderText(MaxPosition.x + 5, MaxPosition.y + (12 * i) + 2, GlobalText[3810 + i], 120, 0, 1);
	}

	g_pRenderText->SetTextColor(255, 255, 0, 255);

	for (int i = 0; i < 5; i++)
	{
		sprintf(GetValue, "%s", IsToolKit.QN(pGetCoin.ThisCoin[i]));
		g_pRenderText->RenderText(MaxPosition.x + 5, MaxPosition.y + (12 * i) + 2, GetValue, 110, 0, 4);
	}

	DisableAlphaBlend();
	return true;
}
