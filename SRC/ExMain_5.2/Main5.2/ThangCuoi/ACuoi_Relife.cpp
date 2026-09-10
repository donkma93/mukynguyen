#include "Stdafx.h"
#include "ACuoi_Relife.h"

#include "NewUISystem.h"
#include "NewUIMuHelper.h"
#include "CharacterManager.h"
#include "DSPlaySound.h"
#include "MemScript.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

SpkRelife G_SpkRelife;
BYTE StartPacket = 1;

SEASON3B::CNewUIRelife::CNewUIRelife()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	BtnMaxOption = 1;
}

SEASON3B::CNewUIRelife::~CNewUIRelife()
{
	Release();
}

bool SEASON3B::CNewUIRelife::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENRELIFE, this);

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIRelife::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRelife::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIRelife::Update()
{
	return true;
}

bool SEASON3B::CNewUIRelife::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENRELIFE);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIRelife::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENRELIFE);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRelife::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRelife::GetKeyEventOrder()
{
	return 3.4;
}

void SpkRelife::SendMenuButton()
{
	if (StartPacket == 1)
	{
		SendRequestDataSendType(0xFD, 0x08, 100);
		StartPacket = 2;
	}
}
void SpkRelife::GetRelifeDataDraw(float x, float y, int SlotJw1, int SlotJw2, int SlotJw3, int SlotJw4, int SlotJw5, int gmCheckDieuKien, int gmCheckRelife) //OK SPK
{
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->SetTextColor(CLRDW_YELLOW);
	g_pRenderText->RenderText(x + 15, y + 65,			GlobalText[3770], 85, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 15, y + 65 + 18,		GlobalText[3771], 85, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 15, y + 65 + 18 * 2,	GlobalText[3772], 85, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 15, y + 65 + 18 * 3,	GlobalText[3773], 85, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 15, y + 65 + 18 * 4,	GlobalText[3777], 85, 0, RT3_SORT_RIGHT);

	char GetValueItem[10][255];

	wsprintf(GetValueItem[0], GlobalText[3866], IsToolKit.QN(SlotJw1));
	wsprintf(GetValueItem[1], GlobalText[3866], IsToolKit.QN(SlotJw2));
	wsprintf(GetValueItem[2], GlobalText[3866], IsToolKit.QN(SlotJw3));
	wsprintf(GetValueItem[3], GlobalText[3866], IsToolKit.QN(SlotJw4));
	wsprintf(GetValueItem[4], GlobalText[3866], IsToolKit.QN(SlotJw5));

	wsprintf(GetValueItem[5], GlobalText[3867], CharacterAttribute->Level, this->m_DieuKienLevel);
	wsprintf(GetValueItem[6], GlobalText[3869], IsToolKit.QN(gmCheckRelife));
	wsprintf(GetValueItem[7], GlobalText[3860], this->ResetLife);
	wsprintf(GetValueItem[8], GlobalText[3865], this->ResetLife + 1);
	wsprintf(GetValueItem[9], GlobalText[3867], CharacterAttribute->ViewReset, gmCheckDieuKien);	

	g_pRenderText->SetTextColor(CLRDW_DARKYELLOW);
	g_pRenderText->RenderText(x + 25, y + 65 + 18 * 0, GetValueItem[0], 175, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 25, y + 65 + 18 * 1, GetValueItem[1], 175, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 25, y + 65 + 18 * 2, GetValueItem[2], 175, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 25, y + 65 + 18 * 3, GetValueItem[3], 175, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 25, y + 65 + 18 * 4, GetValueItem[4], 175, 0, RT3_SORT_RIGHT);

	g_pRenderText->SetBgColor(0, 162, 232, 200);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->RenderText(x + 16, y + 5 + 15 * 10, GetValueItem[8], 200, 0, 3);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 6), GlobalText[3861],	 200, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 6), GetValueItem[5], 175, 0, RT3_SORT_RIGHT);

	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 7), GlobalText[3868], 200, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 7), GetValueItem[9], 175, 0, RT3_SORT_RIGHT);

	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 8), GlobalText[3863], 200, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 8), GetValueItem[6], 175, 0, RT3_SORT_RIGHT);
	g_pRenderText->RenderText(x + 25, y + 83 + (14 * 9), GlobalText[3864], 200, 0, RT3_SORT_LEFT);

	if (this->ResetLife == 0)
	{
		g_pRenderText->RenderText(x, y + 41, GlobalText[3859], 232, 0, RT3_SORT_CENTER);
	}
	else
	{
		g_pRenderText->RenderText(x, y + 41, GetValueItem[7], 232, 0, RT3_SORT_CENTER);
	}
}

bool SEASON3B::CNewUIRelife::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	G_SpkRelife.SendMenuButton();

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, GlobalText[3783]);

	g_pUIForm->RenderBack(m_Pos.x + 13, m_Pos.y + 36, float(WINDOW_WIDTH - 43) , 22);
	g_pUIForm->RenderBack(m_Pos.x + 13, m_Pos.y + 58, float(WINDOW_WIDTH - 43) , WINDOW_HEIGHT);

	for (int G = 0; G < 10; G++)
	{
		if (G_SpkRelife.ResetLife == G)
		{
			G_SpkRelife.GetRelifeDataDraw(m_Pos.x, m_Pos.y,
				G_SpkRelife.CheckDieuKienJw[G][0],
				G_SpkRelife.CheckDieuKienJw[G][1],
				G_SpkRelife.CheckDieuKienJw[G][2],
				G_SpkRelife.CheckDieuKienJw[G][3],
				G_SpkRelife.CheckDieuKienJw[G][4],
				G_SpkRelife.mCheckDieuKien[G],
				G_SpkRelife.mCheckDiemTaiSinh[G]);
		}
	}
	m_Btn[0].Render();

	this->Render3DItem();
	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIRelife::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIMenuOption::IMAGE_IGS_BUTTON, m_Pos.x + 62.5, m_Pos.y + 221, 108, 27, 1, 0, 1, 1u, GlobalText[3844], "", 0);
}

bool SEASON3B::CNewUIRelife::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent())
	{
		for (int resetIndex = 0; resetIndex < 10; resetIndex++)
		{
			if (G_SpkRelife.ResetLife == resetIndex)
			{
				SendRequestDataSendType(0xFD, 0x08, resetIndex + 1);
				g_ConsoleDebug->Write(MCD_RECEIVE, "RelifePacket %d / %d", resetIndex, G_SpkRelife.ResetLife);
			}
		}
		PlayBuffer(SOUND_CLICK01);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_EVENRELIFE);

		return true;
	}
	return false;
}

void SEASON3B::CNewUIRelife::Render3DItem()
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

	int JwBless = GET_ITEM(14, 13);
	int JwSoul	= GET_ITEM(14, 14);
	int JwLife	= GET_ITEM(14, 16);
	int JwCrea	= GET_ITEM(14, 22);
	int JwChaos = GET_ITEM(12, 15);	

	float x = m_Pos.x;
	float y = m_Pos.y + 22;
	float Width = (float)ItemAttribute[JwBless].Width * INVENTORY_SCALE;
	float Height = (float)ItemAttribute[JwBless].Height * INVENTORY_SCALE;

	RenderItem3D(x + 121, y + 42, Width, Height, JwBless, 0, 0, 0, false);
	RenderItem3D(x + 121, y + 59, Width, Height, JwSoul, 0, 0, 0, false);
	RenderItem3D(x + 122, y + 77, Width, Height, JwLife, 0, 0, 0, false);
	RenderItem3D(x + 121, y + 96, Width, Height, JwCrea, 0, 0, 0, false);
	RenderItem3D(x + 122, y + 108, Width, Height, JwChaos, 0, 0, 0, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}

void SpkRelife::TaiSinhCheckRecv(const BYTE* SPK)
{
	auto DataSPK = reinterpret_cast<const RelifeCheckUpdate*>(SPK);

	this->m_TaiSinh = DataSPK->m_TaiSinh;
	this->m_DieuKienLevel = DataSPK->m_DieuKienLevel;

	for (auto x = 0; x < MAXSIZERL; ++x)
	{
		this->mCheckDieuKien[x] = DataSPK->CheckDieuKien[x];
	}

	this->ResetLife = DataSPK->ResetLife;
}

void SpkRelife::TaiSinhDiemRecv(const BYTE* SPK)
{
	auto DataSPK = reinterpret_cast<const RelifePointUpdate*>(SPK);

	for (auto i = 0; i < MAXSIZERL; ++i)
	{
		this->mCheckDiemTaiSinh[i] = DataSPK->CheckDiemTaiSinh[i];
	}
}

void SpkRelife::SaveFormGs(const BYTE* SPK)
{
	auto DataSPK = reinterpret_cast<const RelifeCheckReqInfoPart1*>(SPK);

	for (auto i = 0; i < MAXSIZERL; ++i)
	{
		for (auto j = 0; j < 5; ++j)
		{
			this->CheckDieuKienJw[i][j] = DataSPK->CheckDieuKienNgocVal[i][j];
		}
	}
}
