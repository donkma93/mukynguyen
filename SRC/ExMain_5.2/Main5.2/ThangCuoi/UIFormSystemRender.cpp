#include "Stdafx.h"
#include "UIFormSystemRender.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <MapManager.h>
#include <Util.h>

SEASON3B::CUIFormSystemRender::CUIFormSystemRender()
{
    this->m_pNewUIMng = nullptr;
	this->m_Pos.x = m_Pos.y = 0;
}

SEASON3B::CUIFormSystemRender::~CUIFormSystemRender()
{
	Release();
}

bool SEASON3B::CUIFormSystemRender::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_UI_NEWSYSTEM, this);

	this->LoadImages();

	this->SetPos(x, y);

	this->Show(false);

	return true;
}

void SEASON3B::CUIFormSystemRender::Release()
{
	this->UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CUIFormSystemRender::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CUIFormSystemRender::Update()
{
	return true;
}

bool SEASON3B::CUIFormSystemRender::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CUIFormSystemRender::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_UI_NEWSYSTEM);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CUIFormSystemRender::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CUIFormSystemRender::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CUIFormSystemRender::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	DisableAlphaBlend();
	return true;
}

void SEASON3B::CUIFormSystemRender::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\newui_item_form_480_top.tga", IMAGE_BASE_WINDOW_TOP, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_480_bottom.tga", IMAGE_BASE_WINDOW_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_80_top.tga", IMAGE_BASE_WINDOW_TOP_80, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_80_bottom.tga", IMAGE_BASE_WINDOW_BOTTOM_80, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_bank_back.jpg", IMAGE_BASE_WINDOW_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_BASE_WINDOW_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_BASE_WINDOW_RIGHT, GL_LINEAR);
}

void SEASON3B::CUIFormSystemRender::UnloadImages()
{
	for (int i = 0; i < 7; i++)
	{
		DeleteBitmap(IMAGE_BASE_WINDOW_TOP + i);
	}
}

void SEASON3B::CUIFormSystemRender::NewRenderForm(float x, float y, float w, float h, const char* zText)
{
	RenderImage(IMAGE_BASE_WINDOW_BACK, (x + 1), y + 20, float(w - 20), float(h + 50));
	RenderImage(IMAGE_BASE_WINDOW_TOP, x, y, float(w - 97), 64.f);
	RenderImage(IMAGE_BASE_WINDOW_TOP_80, (x) + float(w - 97), y, float(80), 64.f);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM, x, y + (h + 25), float(w - 97), 45.f);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM_80, (x) + float(w - 97), y + (h + 25), float(80), 45.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT, x, y + 64.f, 21.f, h - 39.0f);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT, x + float(w - 38.f), y + 64.f, 21.f, h - 39.0f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(x, y + 10, zText, w - 17, 0, RT3_SORT_CENTER);
}

void SEASON3B::CUIFormSystemRender::RenderBack(int x, int y, int width, int height, float a)
{
	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, a);
	RenderColor(x + 3.f, y + 2.f, width - 7.f, height - 7, 0.0, 0);
	EndRenderColor();

	RenderImage(IMAGE_TABLE_TOP_LEFT, x, y, 14.0, 14.0);
	RenderImage(IMAGE_TABLE_TOP_RIGHT, (x + width) - 14.f, y, 14.0, 14.0);
	RenderImage(IMAGE_TABLE_BOTTOM_LEFT, x, (y + height) - 14.f, 14.0, 14.0);
	RenderImage(IMAGE_TABLE_BOTTOM_RIGHT, (x + width) - 14.f, (y + height) - 14.f, 14.0, 14.0);
	RenderImage(IMAGE_TABLE_TOP_PIXEL, x + 6.f, y, (width - 12.f), 14.0);
	RenderImage(IMAGE_TABLE_RIGHT_PIXEL, (x + width) - 14.f, y + 6.f, 14.0, (height - 14.f));
	RenderImage(IMAGE_TABLE_BOTTOM_PIXEL, x + 6.f, (y + height) - 14.f, (width - 12.f), 14.0);
	RenderImage(IMAGE_TABLE_LEFT_PIXEL, x, (y + 6.f), 14.0, (height - 14.f));
}

void SEASON3B::CUIFormSystemRender::RenderHover(float x, float y, float w, float h, DWORD c)
{
	EnableAlphaTest();

	float r = ((c >> 24) & 0xFF) / 255.0f;
	float g = ((c >> 16) & 0xFF) / 255.0f;
	float b = ((c >> 8) & 0xFF) / 255.0f;
	float a = ((c >> 0) & 0xFF) / 255.0f;

	glColor4f(r, g, b, a);
	RenderColor(x, y, w, h, 0.0f, 0);
	EndRenderColor();
}

void SEASON3B::CUIFormSystemRender::RenderCustomButton(CNewUIButton* m_Btn,float x, float y, BYTE Mode, const char* zString)
{
	if (Mode == 1)
	{
		SetButtonInfo(m_Btn, IMAGE_IGS_BUTTON, x, y, 108, 27, true, false, true, true, zString, "", false);
	}
	else if (Mode == 2)
	{
		SetButtonInfo(m_Btn, CNewUIInGameShop::IMAGE_IGS_CATEGORY_BTN, x, y, 73, 27, true, false, true, true, zString, "", false);
	}
	else if (Mode == 3) // Next
	{
		SetButtonInfo(m_Btn, CNewUIInGameShop::IMAGE_IGS_STORAGE_PAGE_RIGHT, x, y, 20, 22, true, false, true, true, zString, "", false);
	}
	else if (Mode == 4) // Prev
	{
		SetButtonInfo(m_Btn, CNewUIInGameShop::IMAGE_IGS_STORAGE_PAGE_LEFT, x, y, 20, 22, true, false, true, true, zString, "", false);
	}
	else if (Mode == 5) // Button Buy 
	{
		SetButtonInfo(m_Btn, CNewUIInGameShop::IMAGE_IGS_VIEWDETAIL_BTN, x, y, 52, 26, true, false, true, true, zString, "", false);
	}
	else if (Mode == 6)
	{
		SetButtonInfo(m_Btn, IMAGE_IGS_BUTTON, x, y, 20, 20, true, false, true, true, zString, "", false);
	}
}

void SEASON3B::CUIFormSystemRender::RenderToolTip(float x, float y, float w, float h)
{
	EnableAlphaTest();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	RenderColor(x - 1, y - 1, w + 1, 1);
	RenderColor(x - 1, y - 1, 1, h + 1);
	RenderColor(x - 1 + w + 1, y - 1, 1, h + 1);
	RenderColor(x - 1, y - 1 + h + 1, w + 2, 1);

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	RenderColor(x, y, w, h);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void SEASON3B::CUIFormSystemRender::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
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

void SEASON3B::CUIFormSystemRender::RenderButton3D(int GetID, float x, float y, float w, float h, BYTE Mode)
{
	if (Mode == 1)
	{
		if (CheckMouseIn(x, y, w, h) == true && SEASON3B::IsPress(VK_LBUTTON))
		{
			RenderBitmap(GetID, x, y, w, h, 0.0, 0.451f, 0.878, 0.225, 1, 1, 0.0);
		}
		else if (CheckMouseIn(x, y, w, h) == true)
		{
			RenderBitmap(GetID, x, y, w, h, 0.0, 0.2255f, 0.878, 0.225, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(GetID, x, y, w, h, 0.0, 0.0, 0.878, 0.225, 1, 1, 0.0);
		}
	}
}

void SEASON3B::CUIFormSystemRender::RenderButtonBarFrameMain(int GetID, float x, float y, float w, float h, BYTE Mode)
{
	if (Mode == 1)
	{
		if (CheckMouseIn(x, y, w, h) == true)
		{
			RenderBitmap(GetID, x, y, w, h, 0.47f, 0.0f, 0.4f, 0.8, 1.0f, 1.0f, 0.0f);
		}
		else
		{
			RenderBitmap(GetID, x, y, w, h, 0.0205f, 0.0205f, 0.4f, 0.8, 1.0f, 1.0f, 0.0f);
		}
	}
}