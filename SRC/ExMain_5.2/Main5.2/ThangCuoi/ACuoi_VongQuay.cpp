#include "Stdafx.h"
#include "ACuoi_VongQuay.h"
#include "NewUISystem.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

cCustomSpin gCustomSpin;

SEASON3B::CNewUISpinPush::CNewUISpinPush()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_iNumCurOpenTab = 0;
	gCustomSpin.StartRoll = 0;
	gCustomSpin.RollNumber = -1;

	memset(&m_EquipmentSlots, 0, sizeof(EQUIPMENT_ITEM) * MAX_EQUIPMENT_INDEX);

	for (int i = 0; i < MAX_EQUIPMENT; ++i)
	{
		this->CharItemInfo[i].Type = -1;
		this->CharItemInfo[i].Level = 0;
	}

	this->m_CharItemInfo.clear();
}


SEASON3B::CNewUISpinPush::~CNewUISpinPush()
{
	Release();
}

bool SEASON3B::CNewUISpinPush::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENSPIN, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUISpinPush::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUISpinPush::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 73;

	SetEquipmentSlotInfo();
}

void SEASON3B::CNewUISpinPush::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], IMAGE_IGS_BUTTON, m_Pos.x + 115, m_Pos.y + 110, 55, 55, 1, 0, 1, 1u, GlobalText[3899], "", 0);
}

void SEASON3B::CNewUISpinPush::LoadImages()
{
	LoadBitmap("ACuoi\\Arrow.tga", DRAW_ARROW_RED, GL_LINEAR);
}

void SEASON3B::CNewUISpinPush::UnloadImages()
{
	DeleteBitmap(DRAW_ARROW_RED);
}

bool SEASON3B::CNewUISpinPush::Update()
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

bool SEASON3B::CNewUISpinPush::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	POINT ptExitBtn1 = { m_Pos.x + 248, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENSPIN);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 80, WINDOW_HEIGHT + 85))
		return false;
	return true;
}

bool SEASON3B::CNewUISpinPush::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENSPIN);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUISpinPush::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUISpinPush::GetKeyEventOrder()
{
	return 3.4;
}

void SEASON3B::CNewUISpinPush::GenerateCharItemInfoFromSpin()
{
	this->m_CharItemInfo.clear();

	for (int SPK = 0; SPK < 12; SPK++)
	{
		ITEM item{};

		item.Type = gCustomSpin.ItemBmd[SPK];
		item.Level = gCustomSpin.ItemLvl[SPK];
		item.ExtOption = gCustomSpin.ItemExc[SPK];

		this->CharItemInfo[SPK] = item;
		this->m_CharItemInfo.push_back(item);

		g_ConsoleDebug->Write(3, "Index %d | Type=%d | Level=%d | Exc=%d",
			SPK,
			gCustomSpin.ItemBmd[SPK],
			gCustomSpin.ItemLvl[SPK],
			gCustomSpin.ItemExc[SPK]);
	}
}


void SEASON3B::CNewUISpinPush::SetEquipmentSlotInfo()
{
	const float radius = 80.0f;
	const float size = 42.0f;
	const float centerX = m_Pos.x + 135.f;
	const float centerY = m_Pos.y + 144.f;

	const float angleDeg[12] = {
		270.f, 240.f, 210.f, 180.f, 150.f, 120.f,
		 90.f,  60.f,  30.f,   0.f, 330.f, 300.f
	};

	for (int i = 0; i < 12; ++i)
	{
		m_EquipmentSlots[i].x = (int)(centerX + cosf(angleDeg[i] * (PI / 180.0f)) * radius - size / 2);
		m_EquipmentSlots[i].y = (int)(centerY + sinf(angleDeg[i] * (PI / 180.0f)) * radius - size / 2);
		m_EquipmentSlots[i].width = (int)size;
		m_EquipmentSlots[i].height = (int)size;
		m_EquipmentSlots[i].dwBgImage = CNewUIViewItem::IMAGE_INVENTORY_ITEM_FAIRY;
	}
}

float GetPositionAddX()
{
	float Ret = 0;
	switch (m_Resolution)
	{
	case 0:		Ret = 340.f; break;
	case 1:		Ret = 340.f; break;
	case 2:		Ret = 340.f; break;
	case 3:		Ret = 210.f; break;
	case 4:		Ret = 169.f; break;
	case 5:		Ret = 130.f; break;
	case 6:		Ret = 53.f; break;
	case 7:		Ret = 53.f; break;
	case 8:		Ret = 53.f; break;
	case 9:		Ret = 53.f; break;
	case 10:	Ret = 53.f; break;
	default:	Ret = 53.f;
		break;
	}
	return Ret;
}

float GetPositionAddY()
{
	float Ret = 0;
	switch (m_Resolution)
	{
	case 0:		Ret = 653.f; break;
	case 1:		Ret = 653.f; break;
	case 2:		Ret = 653.f; break;
	case 3:		Ret = 785.f; break;
	case 4:		Ret = 830.f; break;
	case 5:		Ret = 865.f; break;
	case 6:		Ret = 950.f; break;
	case 7:		Ret = 950.f; break;
	case 8:		Ret = 950.f; break;
	case 9:		Ret = 950.f; break;
	case 10:	Ret = 950.f; break;
	default:	Ret = 950.f;
		break;
	}
	return Ret;
}

bool SEASON3B::CNewUISpinPush::Render()
{
	int NumberInfo = -1;

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderImage(IMAGE_BASE_WINDOW_BACK, m_Pos.x + 1, m_Pos.y + 5, float(WINDOW_WIDTH) + 78, float(WINDOW_HEIGHT) + 104);
	RenderImage(IMAGE_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, float(WINDOW_WIDTH + 80), 64.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) + 1);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT, m_Pos.x + (float(WINDOW_WIDTH) - 21.f) + 80, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) + 1);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM, m_Pos.x, m_Pos.y + float(WINDOW_HEIGHT) + 65, float(WINDOW_WIDTH + 80), 45.f);

	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 10, 0xFFEECCE1, 0, 270, 0, RT3_SORT_CENTER, GlobalText[3786]);

	g_pUIForm->RenderBack(m_Pos.x + 15, m_Pos.y + 40, 240, 210);

	for (int i = 0; i < MAX_EQUIPMENT_INDEX; i++)
	{
		RenderImage(-1
			, m_EquipmentSlots[i].x
			, m_EquipmentSlots[i].y
			, m_EquipmentSlots[i].width
			, m_EquipmentSlots[i].height);

		if (SEASON3B::CheckMouseIn(m_EquipmentSlots[i].x, m_EquipmentSlots[i].y, m_EquipmentSlots[i].width, m_EquipmentSlots[i].height))
		{
			NumberInfo = i;
		}
	}

	EnableAlphaTest();

	glColor4f(1.f, 1.f, 1.f, 0.f);

	float centerX = IsToolKit.GetPositionScreen() - GetPositionAddX();
	float centerY = (GetPositionAddY() - 140) / 2;

	float radius = 80;
	float sizeW = 45.f;
	float sizeH = 45.f;
	float arrowWidth = 140;
	float arrowHeight = 140;

	//g_pUIForm->RenderBack(m_Pos.x + 15, m_Pos.y + 250, 240, 50);

	g_pUIForm->RenderBack(m_Pos.x + 120, m_Pos.y + 50, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 160, m_Pos.y + 60, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 188, m_Pos.y + 92, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 200, m_Pos.y + 130, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 190, m_Pos.y + 170, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 160, m_Pos.y + 200, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 120, m_Pos.y + 208, 35, 35);


	g_pUIForm->RenderBack(m_Pos.x + 80, m_Pos.y + 198, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 48, m_Pos.y + 170, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 40, m_Pos.y + 130, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 48, m_Pos.y + 90, 35, 35);

	g_pUIForm->RenderBack(m_Pos.x + 80, m_Pos.y + 60, 35, 35);


	if (gCustomSpin.StartRoll < 1 && gCustomSpin.RollNumber < 0)
	{
		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.f);

		float arrowAngle = 270;
		RenderBitmapRotated2D(DRAW_ARROW_RED,
			centerX,
			centerY,
			arrowWidth, arrowHeight,
			0.f, 0.f,
			1.0f, 1.0f,
			1.0f,
			arrowAngle);

		DisableAlphaBlend();
	}

	if (gCustomSpin.StartRoll >= 1)
	{
		static float Spin_CurrentAngle = 0.0f;
		static DWORD Spin_LastTimeSmooth = 0;

		DWORD CurrentTime = GetTickCount();
		if (Spin_LastTimeSmooth == 0)
			Spin_LastTimeSmooth = CurrentTime;

		float deltaTime = (CurrentTime - Spin_LastTimeSmooth) / 1000.0f;
		Spin_LastTimeSmooth = CurrentTime;

		float speed = 800.0f;
		Spin_CurrentAngle += speed * deltaTime;
		if (Spin_CurrentAngle >= 360.0f)
			Spin_CurrentAngle -= 360.0f;

		float angleRad = Spin_CurrentAngle * (PI / 180.0f);
		float highlightX = centerX + cosf(angleRad + PI / 2) * radius - 18.f;
		float highlightY = centerY + sinf(angleRad + PI / 2) * radius - 18.f;

		EnableAlphaTest();
		glColor4f(1.0, 1.0, 1.0, 1.0);

		glColor3f(1.0, 1.0, 1.0);


		float arrowAngle = -Spin_CurrentAngle + 270;

		RenderBitmapRotated2D(DRAW_ARROW_RED,
			centerX,
			centerY,
			arrowWidth, arrowHeight,
			0.f, 0.f,
			1.0f, 1.0f,
			1.0f,
			arrowAngle);

		DisableAlphaBlend();
	}

	if (gCustomSpin.RollNumber >= 0)
	{
		int index = gCustomSpin.RollNumber % 12;
		float arrowAngle = index * 30.0f + 270;

		RenderBitmapRotated2D(DRAW_ARROW_RED,
			centerX,
			centerY,
			arrowWidth, arrowHeight,
			0.f, 0.f,
			1.0f, 1.0f,
			1.0f,
			arrowAngle);

		DisableAlphaBlend();
	}

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	//m_Btn[0].Render();

	//IsToolKit.ThisFont(m_Pos.x - 10, m_Pos.y + 252, 0xFFEECCE1, 0xFF0000FF, 100, 0, RT3_SORT_CENTER, GlobalText[3185], IsToolKit.QN(gCustomSpin.ThisCoin));
	IsToolKit.ThisFont(m_Pos.x + 18, m_Pos.y + 248, 0xFFEECCE1, 0xFF0000FF, 232, 0, RT3_SORT_CENTER, GlobalText[3185], IsToolKit.QN(gCustomSpin.ThisCoin)); IsToolKit.ThisFont(m_Pos.x + 18, m_Pos.y + 259, 0xFFEECCE1, 0xFFFFFF00, 232, 0, RT3_SORT_CENTER, "Phần Thưởng", 0);

	gCustomSpin.Render3DItem(m_Pos.x + 1.2, m_Pos.y + 25);

	if (NumberInfo != -1)
	{
		int fixedIndex = (NumberInfo + 6) % 12;

		if (CharItemInfo[fixedIndex].Type != 0x1FFF)
		{
			RenderItemInfo(m_EquipmentSlots[NumberInfo].x + 20, m_EquipmentSlots[NumberInfo].y, &CharItemInfo[fixedIndex], 0, 0, false);
		}
	}

	DisableAlphaBlend();
	return true;
}

bool SEASON3B::CNewUISpinPush::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent())
	{
		gCustomSpin.RollNumber = -1;
		if (gCustomSpin.StartRoll != 1)
		{
			SendRequestDataSend(0xFD, 0x16);
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	return false;
}

void cCustomSpin::LuckySpinRoll()
{
	this->StartRoll = 1;
}
void cCustomSpin::GCLuckySpinInit(const BYTE* Protocol)
{
	auto DataSPK = reinterpret_cast<const cCustomSpinDataUnit*>(Protocol);

	if (DataSPK->result)
	{
		g_pNewUIMenuOption->IsVisible(SEASON3B::INTERFACE_EVENSPIN);
	}
}

void cCustomSpin::GCItemSend(const BYTE* Protocol)
{
	auto DataSPK = reinterpret_cast<const cCustomSpinGetData*>(Protocol);

	for (int SPK = 0; SPK < 12; SPK++)
	{
		ItemBmd[SPK] = DataSPK->ItemBmd[SPK];
		ItemLvl[SPK] = DataSPK->ItemLvl[SPK];
		ItemExc[SPK] = DataSPK->ItemExc[SPK];
	}

	this->ThisCoin = DataSPK->ThisCoin;

	g_pNewUISpinPush->GenerateCharItemInfoFromSpin();

}

void cCustomSpin::LuckySpinData(const BYTE* Protocol)
{
	auto DataSPK = reinterpret_cast<const cCustomSpinDataUpdate*>(Protocol);

	this->StartRoll = 0;
	this->RollNumber = DataSPK->RollNumber;
}

void cCustomSpin::Render3DItem(float xs, float ys) const
{
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(1.3f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor4f(1.f, 1.f, 1.f, 1.f);

	float Width = INVENTORY_SCALE;
	float Height = INVENTORY_SCALE;
	float centerX = xs + 135.f;
	float centerY = ys + 120.f;
	float radius = 80;

	for (int i = 0; i < 12; ++i)
	{
		float angle = (float)(i * 30) * (PI / 180.0f);
		float x = centerX + cosf(-angle + PI / 2) * radius - Width / 2;
		float y = centerY + sinf(-angle + PI / 2) * radius - Height / 2;

		if (g_pNewUISpinPush->CharItemInfo[i].Type >= 7412 && g_pNewUISpinPush->CharItemInfo[i].Type <= 7420)
		{
			y -= 0;
		}

		RenderItem3D(x, y, Width, Height
			, g_pNewUISpinPush->CharItemInfo[i].Type
			, g_pNewUISpinPush->CharItemInfo[i].Level * 8
			, g_pNewUISpinPush->CharItemInfo[i].Option1
			, g_pNewUISpinPush->CharItemInfo[i].ExtOption, true);
	}

	if (this->RollNumber >= 0)
	{
		int AdjustedY = ys + 255;
		if (g_pNewUISpinPush->CharItemInfo[RollNumber].Type >= 7412 && g_pNewUISpinPush->CharItemInfo[RollNumber].Type <= 7420)
		{
			AdjustedY -= 0;
		}

		RenderItem3D(xs + 125, AdjustedY, Width, Height,
			g_pNewUISpinPush->CharItemInfo[RollNumber].Type,
			g_pNewUISpinPush->CharItemInfo[RollNumber].Level * 8,
			g_pNewUISpinPush->CharItemInfo[RollNumber].Option1,
			g_pNewUISpinPush->CharItemInfo[RollNumber].ExtOption,
			true);
	}

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}

