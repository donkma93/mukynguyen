#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ZzzInterface.h"
#include "ThangCuoi\WideData.h"
using namespace SEASON3B;

#define MAXOPTIONNER 6
bool m_AutoCtrl = false;
bool m_MiniMapOn = true;
float MaxWVolume = 86.8f;

SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bAutoAttack = true;
	m_bWhisperSound = false;
	m_bSlideHelp = true;
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;
	m_bRenderAllEffects = true;
	m_iFontLevel = 0;
	m_bFpsWingMotion = false;
	m_bWidowsSD = false;
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);
	SetPos(x, y);
	LoadImages();
	SetButtonInfo();

	this->ReadValueString();

	Show(false);

	return true;
}

void SEASON3B::CNewUIOptionWindow::ReadValueString()
{
	char FontHeightStr[10];
	char szFpsWingMotion[4] = { 0 };

	GetPrivateProfileStringA("FontConfig", "FontHeight", "0", FontHeightStr, sizeof(FontHeightStr), "./ACuoi.ini");
	int FontValue = atoi(FontHeightStr);
	if (FontValue < 11) FontValue = 11;
	if (FontValue > 17) FontValue = 17;
	m_iFontLevel = FontValue - 11;

	m_bFpsWingMotion = (GetPrivateProfileIntA("FontConfig", "FpsWingMotion", 0, "./ACuoi.ini") != 0);
	m_bWidowsSD = (GetPrivateProfileIntA("FontConfig", "WindowsSD", 0, "./ACuoi.ini") != 0);

}
void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
	SetScreenButton(&m_BtnSet[0], CNewUIInGameShop::IMAGE_IGS_CATEGORY_BTN, m_Pos.x + 177, m_Pos.y + 90, 73, 27, 1, 0, 1, 1u, "Change Size", "", 0);
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y;
}

void ChangeFontSize(BYTE a)
{
		FontHeight = a;
		char FontFace[100];
		char FontWeightStr[10];
		char FontAliasStr[50];

		GetPrivateProfileStringA("FontConfig", "FontName", "Tahoma", FontFace, sizeof(FontFace), "./Config.ini");
		GetPrivateProfileStringA("FontConfig", "FontHeight", "13", FontWeightStr, sizeof(FontWeightStr), "./Config.ini");
		GetPrivateProfileStringA("FontConfig", "FontAlias", "3", FontAliasStr, sizeof(FontAliasStr), "./Config.ini");

		int FontWeight = atoi(FontWeightStr);
		int FontAliasRead = atoi(FontAliasStr);


		int FontW = (FontWeight == 0) ? 100 : 700;
		g_hFont = CreateFont(a, 0, 0, 0, FontW, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, FontAliasRead, DEFAULT_PITCH | FF_DONTCARE, FontFace);

		char FontSizeStr[10];
		sprintf(FontSizeStr, "%d", a);
		WritePrivateProfileStringA("FontConfig", "FontHeight", FontSizeStr, "./Config.ini");

		g_ConsoleDebug->Write(MCD_NORMAL, "Resolution: %d / FontHeight:%d Screen: %dx%d WideRate X:%.2f / WideRate Y:%.2f", m_Resolution, FontHeight, WindowWidth, WindowHeight, gPosWide.x_fScreenRate_x, gPosWide.x_fScreenRate_y);
}

void ChangeSize(int W, int H, float S, BYTE Resolution)
{
	WindowWidth = W;
	WindowHeight = H;
	m_Resolution = Resolution;
	gPosWide.SetupPosScreen(S);
	SetWindowPos(g_hWnd, 0, 0, 0, W, H, 0x116u);
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{
	//if(m_BtnSet[0].UpdateMouseEvent() == true)
	//{
	//	ChangeSize(1024, 576, 1.20f, 2);
	//	return false;
	//}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 248, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_OPTION);
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 43, 15, 15))
	{
		m_AutoCtrl = !m_AutoCtrl;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 65, 15, 15))
	{
		m_bFpsWingMotion = !m_bFpsWingMotion;
		WritePrivateProfileStringA("FontConfig", "FpsWingMotion", m_bFpsWingMotion ? "1" : "0", "./Config.ini");
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 87, 15, 15))
	{
		m_MiniMapOn = !m_MiniMapOn;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 109, 15, 15))
	{
		m_bRenderAllEffects = !m_bRenderAllEffects;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 131, 15, 15))
	{
		g_bGMObservation = !g_bGMObservation;
		g_bRenderNameMonster = !g_bRenderNameMonster;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 153, 15, 15))
	{
		g_bRenderImageLogo = !g_bRenderImageLogo;
	}

	if (CheckMouseIn(m_Pos.x + 20, m_Pos.y + 193, 132, 16))
	{
		int iOldValue = m_iVolumeLevel;
		if(MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel++;
			if(m_iVolumeLevel > 10)
			{
				m_iVolumeLevel = 10;
			}
		}
		else if(MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel--;
			if(m_iVolumeLevel < 0)
			{
				m_iVolumeLevel = 0;
			}
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 30);
			if (x < 0)
			{
				m_iVolumeLevel = 0;
			}
			else
			{
				float fValue = (10.f * x) / 124.f;
				m_iVolumeLevel = (int)fValue + 1;
				if (m_iVolumeLevel > 10)
					m_iVolumeLevel = 10;
			}
		}

		if(iOldValue != m_iVolumeLevel)
		{
			SetEffectVolumeLevel(m_iVolumeLevel);
		}
	}

	if (CheckMouseIn(m_Pos.x + 20, m_Pos.y + 215, 141, 29))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}

	if (CheckMouseIn(m_Pos.x + 170, m_Pos.y + 51, (float)MaxWVolume, 16))
	{
		int iOldFont = m_iFontLevel;
		if (MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iFontLevel++;
			if (m_iFontLevel > 6) m_iFontLevel = 6;
		}
		else if (MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iFontLevel--;
			if (m_iFontLevel < 0) m_iFontLevel = 0;
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 170);
			if (x < 0) m_iFontLevel = 0;
			else m_iFontLevel = (int)((7.f * x) / MaxWVolume);
			if (m_iFontLevel > 6) m_iFontLevel = 6;
		}

		if (iOldFont != m_iFontLevel)
		{
			int NewFontHeight = 11 + m_iFontLevel;
			ChangeFontSize(NewFontHeight);
		}
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 170, m_Pos.y + 70, 15, 15))
	{
		m_bWidowsSD = !m_bWidowsSD;
		WritePrivateProfileStringA("FontConfig", "WindowsSD", m_bWidowsSD ? "1" : "0", "./Config.ini");
		//SetWindowPos(g_hWnd, 0, 0, 0, 1024, 768, 0x116u);
	}
		
	if (CheckMouseIn(m_Pos.x, m_Pos.y, 270, 269) == true)
	{
		return false;
	}
	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderButtons();
	RenderContents();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()	//. 10.5f
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()	// 10.f;
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{
	
}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{

}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_button_close.tga", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);	
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_BTN_CLOSE);
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK, m_Pos.x + 1, m_Pos.y + 5, 268.f, 259.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, 270.f, 64.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, 156.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_RIGHT, m_Pos.x + 249, m_Pos.y + 64.f, 21.f, 156.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_BOTTOM, m_Pos.x, m_Pos.y + 220.f, 270.f, 45.f);

	for (int x = 0; x < MAXOPTIONNER; x++)
	{
		RenderImage(IMAGE_OPTION_LINE, m_Pos.x + 21, m_Pos.y + 60 + ( x * 22), 143, 2.f);
	}
}

void SEASON3B::CNewUIOptionWindow::RenderContents()
{
	for (int x = 0; x < MAXOPTIONNER; x++)
	{
		RenderImage(IMAGE_OPTION_POINT, m_Pos.x + 20.f, m_Pos.y + 46.f + (x * 22), 10.f, 10.f);
	}

	RenderImage(IMAGE_OPTION_POINT, m_Pos.x + 20.f, m_Pos.y + 179, 10.f, 10.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[3657], 270, 0, 3);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 46, GlobalText[3658]);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 68, GlobalText[387]);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 90, GlobalText[3187]);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 112, GlobalText[3745]);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 134, GlobalText[3746]);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 157, GlobalText[3767]);

	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 179, GlobalText[3768]);

	if (m_bWidowsSD)
	{
		g_pRenderText->RenderText(m_Pos.x + 188, m_Pos.y + 72, GlobalText[2015]);
	}
	else
	{
		g_pRenderText->SetTextColor(255, 255, 0, 255);
		g_pRenderText->RenderText(m_Pos.x + 188, m_Pos.y + 72, GlobalText[1999]);
	}

	BYTE Value[7] = { 11, 12, 13, 14, 15, 16, 17 };

	for (int i = 0; i < 7; i++)
	{
		SEASON3B::RenderNumber(m_Pos.x + 178 + (i * 12.2), m_Pos.y + 55.5, Value[i], 0.77);
	}

	IsToolKit.ThisFont(m_Pos.x + 170, m_Pos.y + 43, 0xFFFFFFFF, 200, 87, 10, 3, "Font");

	if (CheckMouseIn(m_Pos.x + 170, m_Pos.y + 70, 50, 15))
	{
		int TextNum = 0;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;

		for (int i = 1995; i <= 1998; i++)
		{
			sprintf(TextList[TextNum++], "%s", GlobalText[i]);
		}
		RenderTipTextList(MouseX + 100, MouseY, TextNum, 0);
	}
}
void SEASON3B::CNewUIOptionWindow::RenderButtons()
{
	//m_BtnSet[0].Render();

	if (m_AutoCtrl){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 43, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 43, 15, 15, 0, 15.f);}

	if (m_bFpsWingMotion){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 65, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 65, 15, 15, 0, 15.f);}
	
	if (m_MiniMapOn){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 87, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 87, 15, 15, 0, 15.f);}

	if (m_bRenderAllEffects){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 109, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 109, 15, 15, 0, 15.f);}
	
	if (!g_bGMObservation){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 131, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 131, 15, 15, 0, 15.f);}

	if (g_bRenderImageLogo){
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 153, 15, 15, 0, 0);}else{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 153, 15, 15, 0, 15.f);}

	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 20, m_Pos.y + 193, 124.f, 16.f);
	if (m_iVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 20, m_Pos.y + 193, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
	}

	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 20, m_Pos.y + 215, 141.f, 29.f);
	if (m_iRenderLevel >= 0)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 20, m_Pos.y + 215, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
	}

	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 170, m_Pos.y + 51, MaxWVolume, 16.f);
	if (m_iFontLevel >= 0)
	{
		glColor3f(1.0f, 0.5167, 0.0f);
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 170, m_Pos.y + 51, MaxWVolume * (m_iFontLevel + 1) / 7.f, 16.f);
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	if (m_bWidowsSD) {
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 170, m_Pos.y + 70, 15, 15, 0, 0);
	}
	else {
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 170, m_Pos.y + 70, 15, 15, 0, 15.f);
	}
}

void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetFpsWingMotion(bool bWing)
{
	m_bFpsWingMotion = bWing;
}

bool SEASON3B::CNewUIOptionWindow::IsFpsWingMotion()
{
	return m_bFpsWingMotion;
}

void SEASON3B::CNewUIOptionWindow::SetWidowsSD(bool bSd)
{
	m_bWidowsSD = bSd;
}

bool SEASON3B::CNewUIOptionWindow::IsWidowsSD()
{
	return m_bWidowsSD;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderAllEffects(bool bRenderAllEffects)
{
	m_bRenderAllEffects = bRenderAllEffects;
}
bool SEASON3B::CNewUIOptionWindow::GetRenderAllEffects()
{
	return m_bRenderAllEffects;
}
void SEASON3B::CNewUIOptionWindow::SetScreenButton(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
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