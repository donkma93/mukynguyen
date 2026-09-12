#include "Stdafx.h"
#include "NewUIAccountLevel.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "StructSendGs.h"
#include "WideData.h"

AccountLevelID gGetID;

namespace
{
	constexpr int kVipPackageCount = 3;
	constexpr int kVipDays[kVipPackageCount] = { 7, 15, 30 };
	constexpr int kVipWCoinC[kVipPackageCount] = { 5000, 9000, 15000 };
}

SEASON3B::CNewUIAccountLevel::CNewUIAccountLevel()
{
	gCustomSpin.StartRoll = 0;
	gCustomSpin.RollNumber = -1;
	m_pNewUIMng = nullptr;

	m_Pos.x = 0;
	m_Pos.y = 0;

	WINDOW_WIDTH = 300;
	WINDOW_HEIGHT = 130;
}

SEASON3B::CNewUIAccountLevel::~CNewUIAccountLevel()
{
	Release();
}

bool SEASON3B::CNewUIAccountLevel::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_IDLEVEL, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIAccountLevel::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIAccountLevel::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

void SEASON3B::CNewUIAccountLevel::InitButtons()
{
	int startX = m_Pos.x + 35;
	int buttonWidth = 52;
	int spacing = 28;

	for (int i = 0; i < kVipPackageCount; i++)
	{
		g_pUIForm->SetButtonInfo(&m_Btn[i], IMAGE_IGS_BUTTON, startX + i * (buttonWidth + spacing), m_Pos.y + 145, 52, 26, 1, 0, 1, 1u, GlobalText[3819 + i], "", 0);
	}
}

void SEASON3B::CNewUIAccountLevel::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\ExtendVip.tga", IMAGE_IGS_LOGOV1, GL_LINEAR);

}
void SEASON3B::CNewUIAccountLevel::UnloadImages()
{
	DeleteBitmap(IMAGE_IGS_LOGOV1);
}

bool SEASON3B::CNewUIAccountLevel::Update()
{
	return true;
}

bool SEASON3B::CNewUIAccountLevel::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	float GetPosX = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	float GetPosY = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((GetPosX + WINDOW_WIDTH) - 40, GetPosY + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_IDLEVEL);
	}

	if (CheckMouseIn(GetPosX, GetPosY, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;
	return true;
}

bool SEASON3B::CNewUIAccountLevel::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_IDLEVEL);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIAccountLevel::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIAccountLevel::GetKeyEventOrder()
{
	return 3.4;
}


bool SEASON3B::CNewUIAccountLevel::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT, GlobalText[3787]);

	g_pRenderText->SetBgColor(NULL);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 3;

	g_pUIForm->RenderHover(m_Pos.x + 17, m_Pos.y + 70, 250, iLineHeight, 0x320000FF);
	g_pUIForm->RenderHover(m_Pos.x + 17, m_Pos.y + 70 + iLineHeight, 250, 35, 0x00000080);

	char CreateSpace[255];
	g_pRenderText->SetBgColor(NULL);
	sprintf(CreateSpace, GlobalText[3831], Hero->ID);

	g_pRenderText->SetTextColor(0, 160, 230, 255);
	g_pRenderText->RenderText(m_Pos.x + 17, m_Pos.y + 45, CreateSpace, 270, 0, 1);

	g_pRenderText->SetTextColor(255, 128, 0, 255);
	g_pRenderText->RenderText(m_Pos.x + 17, m_Pos.y + 55, GlobalText[3832], 270, 0, 1);

	g_pRenderText->SetTextColor(255, 238, 204, 255);
	for (int i = 0; i < kVipPackageCount; i++)
	{
		const float rowY = m_Pos.y + 83 + (i * 10);
		char days[24];
		sprintf_s(days, GlobalText[3857], kVipDays[i]);

		g_pRenderText->RenderText(m_Pos.x + 17, rowY, GlobalText[3819 + i], 61, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 80, rowY, GlobalText[3823], 61, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 144, rowY, GlobalText[3826], 61, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 207, rowY, days, 61, 0, RT3_SORT_CENTER);
	}

	g_pRenderText->RenderText(m_Pos.x + 17, m_Pos.y + 70, GlobalText[3815], 61, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + 80, m_Pos.y + 70, GlobalText[3816], 61, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + 144, m_Pos.y + 70, GlobalText[3817], 61, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + 207, m_Pos.y + 70, GlobalText[3818], 61, 0, RT3_SORT_CENTER);

	sprintf(CreateSpace, GlobalText[3830], IsToolKit.QN(pGetCoin.ThisCoin[0]));
	g_pRenderText->SetTextColor(0, 128, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 17, m_Pos.y + 125, CreateSpace, 250, 0, 1);

	m_Btn[0].Render();
	m_Btn[1].Render();
	m_Btn[2].Render();

	for (int i = 0; i < kVipPackageCount; i++)
	{
		char price[32];
		sprintf_s(price, "%s WC", IsToolKit.QN(kVipWCoinC[i]));
		g_pRenderText->RenderText(m_Pos.x + 35 + i * 80, m_Pos.y + 175, price, 52, 0, RT3_SORT_CENTER);
	}

	DisableAlphaBlend();
	return true;
}

bool SEASON3B::CNewUIAccountLevel::BtnProcess()
{
	for (int M = 0; M < kVipPackageCount; M++)
	{
		if (m_Btn[M].UpdateMouseEvent())
		{
			SendRequestDataSendType(0xF3, 0xF0, M + 1);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
	}
	return false;
}
