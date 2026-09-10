#include "Stdafx.h"
#include "StructSendGs.h"
#include "NewUIRenderNumber.h"
#include "wsclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

CustomJewelBank gJwBanking;
#define MaxY 20

SEASON3B::CNewUIJewelBank::CNewUIJewelBank()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_iNumCurOpenTab = 0;

	for (int SPK = 0; SPK < MAX_ITEM_SLOT; SPK++)
	{
		gJwBanking.m_Data.ItemBank[SPK] = 0;
	}

	gJwBanking.MaxPosition.x = (static_cast<float>((640 - 270)) / 2) + gPosWide.x_GetAddPos;
	gJwBanking.MaxPosition.y = 0;
	gJwBanking.CurPosition.x = MouseX;
	gJwBanking.CurPosition.y = MouseY;
	gJwBanking.DragItem = false;
}

SEASON3B::CNewUIJewelBank::~CNewUIJewelBank()
{
	Release(); 
}

bool SEASON3B::CNewUIJewelBank::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_JEWELBANK, this);

	LoadImages();

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIJewelBank::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIJewelBank::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void SEASON3B::CNewUIJewelBank::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\newui_item_bank_back.jpg", IMAGE_BASE_WINDOW_BACK, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_bank_top.tga", IMAGE_BASE_WINDOW_TOP, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_bank_bottom.tga", IMAGE_BASE_WINDOW_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_iten_countbox.tga", IMAGE_BASE_WINDOW_BOX, GL_LINEAR);
	LoadBitmap("ACuoi\\Banking.tga", IMAGE_BASE_BANK_BOX, GL_LINEAR);
}
void SEASON3B::CNewUIJewelBank::UnloadImages()
{
	DeleteBitmap(IMAGE_BASE_WINDOW_BACK);
	DeleteBitmap(IMAGE_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_BASE_WINDOW_BOTTOM);
	DeleteBitmap(IMAGE_BASE_WINDOW_BOX);
	DeleteBitmap(IMAGE_BASE_BANK_BOX);
}

bool SEASON3B::CNewUIJewelBank::Update()
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

bool SEASON3B::CNewUIJewelBank::UpdateMouseEvent()
{
	POINT ptExitBtn1 = { m_Pos.x + 249, m_Pos.y + 7 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_JEWELBANK);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 80, WINDOW_HEIGHT + 39))
		return false;
	return true;
}

bool SEASON3B::CNewUIJewelBank::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_JEWELBANK);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIJewelBank::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIJewelBank::GetKeyEventOrder()
{
	return 3.4;
}

void CustomJewelBank::GCCustomBankInfoRecv(const BYTE* lpMsg)
{
	auto DataSPK = reinterpret_cast<const PSBMSG_JEWELBANK_RECV*>(lpMsg);

	for (auto SPK = 0; SPK < MAX_ITEM_SLOT; ++SPK)
	{
		this->m_Data.ItemBank[SPK] = DataSPK->ItemBank[SPK];
	}

	this->JewelEnable = DataSPK->JewelEnable;
	this->JewelMaxCount = DataSPK->JewelMaxCount;
}


void SEASON3B::CNewUIJewelBank::Render3DItem()
{
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	float x = m_Pos.x;
	float y = m_Pos.y;

	struct ItemRenderInfo
	{
		float offsetX;
		float offsetY;
		int itemType;
	};

	static const ItemRenderInfo itemRenderList[] =
	{
		{ 83.0f,  37.0f,  GET_ITEM(14, 13) },
		{ 83.0f,  56.0f,  GET_ITEM(14, 14) },
		{ 84.7f,  76.0f,  GET_ITEM(14, 16) },
		{ 83.0f,  97.0f,  GET_ITEM(14, 22) },
		{ 83.5f, 118.0f,  GET_ITEM(14, 31) },
		{ 83.0f, 138.0f,  GET_ITEM(14, 41) },
		{ 83.0f, 158.0f,  GET_ITEM(14, 42) },
		{ 83.0f, 176.0f,  GET_ITEM(12, 15) },
		{ 83.0f, 198.0f,  GET_ITEM(14, 43) },
		{ 83.0f, 219.0f,  GET_ITEM(14, 44) }
	};

	for (const auto& item : itemRenderList)
	{
		RenderItem3D(x + item.offsetX, y + item.offsetY, 20, 20, item.itemType, 0, 0, 0, false);
	}

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}


bool SEASON3B::CNewUIJewelBank::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	if (GetKeyState(VK_RBUTTON) & 0x8000 && GetTickCount() > gJwBanking.ActionTime + 1000)
	{
		if (IsToolKit.IsWorkZone(gJwBanking.MaxPosition.x + gPosWide.x_GetAddPos, gJwBanking.MaxPosition.y, 270, 270))
		{
			if (!gJwBanking.DragItem)
			{
				gJwBanking.DragItem = true;
				gJwBanking.CurPosition.x = MouseX;
				gJwBanking.CurPosition.y = MouseY;
			}
		}
		if (gJwBanking.DragItem)
		{
			gJwBanking.MaxPosition.x += MouseX - gJwBanking.CurPosition.x;
			gJwBanking.MaxPosition.y += MouseY - gJwBanking.CurPosition.y;
			gJwBanking.CurPosition.x = MouseX;
			gJwBanking.CurPosition.y = MouseY;
		}
	}
	else
	{
		if (gJwBanking.DragItem)
		{
			gJwBanking.DragItem = false;
		}
	}

	if (gJwBanking.MaxPosition.x + gPosWide.x_GetAddPos < 0)		gJwBanking.MaxPosition.x = -gPosWide.x_GetAddPos;
	if (gJwBanking.MaxPosition.y < 0)		gJwBanking.MaxPosition.y = 0;

	float MaxCalc = 477.0f;
	float MaxPos = MaxCalc;

	if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY))
	{
		MaxPos = (MaxCalc - WINDOW_WIDTH) - 1;
	}

	if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY_EXT) || g_pNewUISystem->IsVisible(INTERFACE_MYSHOP_INVENTORY))
	{
		MaxPos = (MaxCalc - (WINDOW_WIDTH * 2)) - 1;
	}

	if (g_pNewUISystem->IsVisible(INTERFACE_INVENTORY) && g_pNewUISystem->IsVisible(INTERFACE_INVENTORY_EXT)&& g_pNewUISystem->IsVisible(INTERFACE_MYSHOP_INVENTORY))
	{
		MaxPos = (MaxCalc - (WINDOW_WIDTH * 3)) - 1;
	}

	if (gJwBanking.MaxPosition.x > MaxPos)	gJwBanking.MaxPosition.x = MaxPos;
	if (gJwBanking.MaxPosition.y > 144)		gJwBanking.MaxPosition.y = 144;

	m_Pos.x = gJwBanking.MaxPosition.x + gPosWide.x_GetAddPos;
	m_Pos.y = gJwBanking.MaxPosition.y;

	float CangChieuDai = 24;

	RenderImage(IMAGE_BASE_WINDOW_BACK,		m_Pos.x + 1, m_Pos.y + 5, float(WINDOW_WIDTH) + 78, float(WINDOW_HEIGHT) + 79);
	RenderImage(IMAGE_BASE_WINDOW_TOP,		m_Pos.x, m_Pos.y, float(WINDOW_WIDTH + 80), 64.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT,		m_Pos.x, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) - CangChieuDai);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT,	m_Pos.x + (float(WINDOW_WIDTH) - 21.f) + 80, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) - CangChieuDai);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM,	m_Pos.x, m_Pos.y + float(WINDOW_HEIGHT) + 40, float(WINDOW_WIDTH + 80), 45.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFF00B4FF);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[3769], 270, 0, RT3_SORT_CENTER);

	g_pRenderText->SetTextColor(0xFF00B4FF);

	char ItemCount[255], ItemName[255], GuiAll[255];
	int TypeIDE[10] = { 0, 1, 2, 3, 4 };
	int Counter[5] = { 1, 10, 20, 30, 99 };
	int GetPosX = 152;
	int GetPosY = 21;
	sprintf(GuiAll, "%s", GlobalText[3780]);	

	for (int i = 0; i < 10; ++i)
	{
		sprintf(ItemCount, "%s", IsToolKit.QN(gJwBanking.m_Data.ItemBank[i]));
		sprintf(ItemName, "%s", GlobalText[3770 + i]);

		g_pRenderText->RenderText(m_Pos.x - 110, m_Pos.y + 44 + (i * MaxY), ItemName,  190, 0, RT3_SORT_RIGHT);

		RenderImage(IMAGE_BASE_WINDOW_BOX, m_Pos.x + 105, m_Pos.y + 40 + (i * MaxY), float(32), float(16));

		g_pRenderText->RenderText(m_Pos.x - 57, m_Pos.y + 44 + (i * MaxY), ItemCount, 190, 0, RT3_SORT_RIGHT);		

		int posxx = 0;
		int posyy = i;
		for (int m = 0; m < 5; m++)
		{
			if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(m_Pos.x + (GetPosX - 8) + (GetPosY * m), m_Pos.y + 40 + (MaxY * i), 16, 16))
			{
				SendRequestCustomJewelBank(i, Counter[m]);
			}
		}

		if (i < 10)
		{
			for (int j = (i * 5); j < (i * 5) + 5; j++)
			{
				RenderImage(BITMAP_START_SPK + i, m_Pos.x + (GetPosX - 8) + (GetPosY * posxx), m_Pos.y + 40 + (MaxY * posyy), float(16), float(16));

				if (posxx == 0) g_RenderNumber->RenderNumber(m_Pos.x + GetPosX + (GetPosY * posxx), m_Pos.y + 44 + (MaxY * posyy), 1, 0.78);
				if (posxx == 1) g_RenderNumber->RenderNumber(m_Pos.x + GetPosX + (GetPosY * posxx), m_Pos.y + 44 + (MaxY * posyy), 1 * 10, 0.78);
				if (posxx == 2) g_RenderNumber->RenderNumber(m_Pos.x + GetPosX + (GetPosY * posxx), m_Pos.y + 44 + (MaxY * posyy), 1 * 20, 0.78);
				if (posxx == 3) g_RenderNumber->RenderNumber(m_Pos.x + GetPosX + (GetPosY * posxx), m_Pos.y + 44 + (MaxY * posyy), 1 * 30, 0.78);
				if (posxx == 4) g_RenderNumber->RenderNumber(m_Pos.x + GetPosX + (GetPosY * posxx), m_Pos.y + 44 + (MaxY * posyy), gJwBanking.JewelMaxCount, 0.78);
				posxx++;
			}
		}
	}

	IsToolKit.ThisFont(m_Pos.x + 30, m_Pos.y + 255, 0x00A2E8FF, 0, 190, 0, RT3_SORT_LEFT, GuiAll);
	Render3DItem();
	DisableAlphaBlend();

	return true;
}
