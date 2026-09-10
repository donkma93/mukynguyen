#include "Stdafx.h"
#include "ACuoi_QuanHam.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

cQuanHam gQuanHam;

SEASON3B::CNewUIQuanHam::CNewUIQuanHam()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	gQuanHam.IsData[0] = 1;
	gQuanHam.IsData[2] = 50;

	WINDOW_WIDTH = 270;
	WINDOW_HEIGHT = 200;
}

SEASON3B::CNewUIQuanHam::~CNewUIQuanHam()
{
	Release();
}
bool SEASON3B::CNewUIQuanHam::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENQUANHAM, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();	

	Show(false);

	return true;
}

void SEASON3B::CNewUIQuanHam::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUIQuanHam::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIQuanHam::Update()
{
	return true;
}
bool SEASON3B::CNewUIQuanHam::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 20, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENQUANHAM);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIQuanHam::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENQUANHAM);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIQuanHam::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIQuanHam::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIQuanHam::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gQuanHam.SendStartLoad();
	
	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH + 20, (float)WINDOW_HEIGHT, GlobalText[3793]);

	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 40, float(WINDOW_WIDTH) - 145, 120);
	g_pUIForm->RenderBack(m_Pos.x + 135, m_Pos.y + 40, float(WINDOW_WIDTH) - 145, 120);
	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 160, float(WINDOW_WIDTH) - 20, 85);

	gQuanHam.Render(m_Pos.x, m_Pos.y);

	if (gQuanHam.IsData[1] < gQuanHam.IsData[2])
	{
		m_Btn[0].Render();
	}

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIQuanHam::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 110, m_Pos.y + 211, 52, 26, 1, 0, 1, 1u, GlobalText[4012], "", 0);
}

bool SEASON3B::CNewUIQuanHam::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent() && gQuanHam.IsData[1] < gQuanHam.IsData[2])
	{
		gQuanHam.SendPackButton();
		PlayBuffer(SOUND_CLICK01);
		gQuanHam.IsData[0] = 1;
		return true;
	}
	return false;
}

void SEASON3B::CNewUIQuanHam::LoadImages()
{
	LoadBitmap("Custom\\ThangCuoi\\RankTitle\\nodata.jpg", IMAGE_HD_LOGO_2, GL_LINEAR);

	for (int i = 0; i < gQuanHam.IsData[2]; i++)
	{
		char IsFile[100];
		sprintf(IsFile, "Custom\\ThangCuoi\\RankTitle\\QuanHam\\%d.tga", i + 1);
		LoadBitmap(IsFile, IMAGE_HD_LOGO_START2 + i, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}
}
void SEASON3B::CNewUIQuanHam::UnloadImages()
{
	for (int i = 0; i < gQuanHam.IsData[2]; i++)
	{
		DeleteBitmap(IMAGE_HD_LOGO_2 + i);
	}
}

GetDataQuanHam* GetStep(int SPK)
{
	std::map<int, GetDataQuanHam>::iterator it = gQuanHam.m_LoadData.find(SPK);
	if (it == gQuanHam.m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

void cQuanHam::SendStartLoad()
{
	if (this->IsData[0] == 1)
	{
		SendRequestDataSendType(0xD5, 0x07, 1);
		this->IsData[0] = 2;
	}
}
void cQuanHam::SendPackButton()
{
	SendRequestDataSendType(0xD5, 0x08, 1);
}

void cQuanHam::SendWsClient(BYTE* lpMsg)
{
	this->m_LoadData.clear();
	CountListQuanHam* mRecv = (CountListQuanHam*)lpMsg;
	for (int i = 0; i < mRecv->Count; i++)
	{
		GetDataQuanHam lpInfo = *(GetDataQuanHam*)(((BYTE*)lpMsg) + sizeof(CountListQuanHam) + (sizeof(GetDataQuanHam) * i));
		this->m_LoadData.insert(std::pair<int, GetDataQuanHam>(lpInfo.AllData[0], lpInfo));
	}

	this->IsData[1]			= gRank.m_Data[HeroKey].rQuanHam;
	this->IsCurrentLevel	= GetStep(this->IsData[1]);
	this->IsNextLevel		= GetStep(this->IsData[1] + 1);
}

void cQuanHam::Render(int a, int b)
{
	if (!this->IsCurrentLevel) return;

	if (this->IsData[1] < this->IsNextLevel->AllData[10] + 1)
	{
		if (this->IsData[1] != 0)
		{
			RenderBitmap((SEASON3B::CNewUIQuanHam::IMAGE_HD_LOGO_START2 + this->IsData[1]) - 1, a + 38, b + 60, 80, 50, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);

			IsToolKit.ThisFont(a + 13, b + 47, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, GlobalText[4004], this->IsData[1]);
			
			IsToolKit.ThisFont(a + 16, b + 100 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
			IsToolKit.ThisFont(a + 16, b + 100 + (0 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsCurrentLevel->AllData[6]));
			IsToolKit.ThisFont(a + 16, b + 100 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
			IsToolKit.ThisFont(a + 16, b + 100 + (1 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsCurrentLevel->AllData[7]));
			IsToolKit.ThisFont(a + 16, b + 100 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
			IsToolKit.ThisFont(a + 16, b + 100 + (2 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsCurrentLevel->AllData[8]));
			IsToolKit.ThisFont(a + 16, b + 100 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
			IsToolKit.ThisFont(a + 16, b + 100 + (3 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsCurrentLevel->AllData[9]));
		}
		else
		{
			RenderBitmap((SEASON3B::CNewUIQuanHam::IMAGE_HD_LOGO_2), a + 13, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);
		}
		if (this->IsData[1] + 1 < this->IsNextLevel->AllData[10] + 1)
		{
			RenderBitmap(SEASON3B::CNewUIQuanHam::IMAGE_HD_LOGO_START2 + this->IsData[1], a + 165, b + 60, 80, 50, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);

			IsToolKit.ThisFont(a + 138, b + 47, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, GlobalText[4005], this->IsData[1] + 1);
			
			IsToolKit.ThisFont(a + 141, b + 100 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
			IsToolKit.ThisFont(a + 141, b + 100 + (0 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsNextLevel->AllData[6]));
			IsToolKit.ThisFont(a + 141, b + 100 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
			IsToolKit.ThisFont(a + 141, b + 100 + (1 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsNextLevel->AllData[7]));
			IsToolKit.ThisFont(a + 141, b + 100 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
			IsToolKit.ThisFont(a + 141, b + 100 + (2 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsNextLevel->AllData[8]));
			IsToolKit.ThisFont(a + 141, b + 100 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
			IsToolKit.ThisFont(a + 141, b + 100 + (3 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(this->IsNextLevel->AllData[9]));
		}
	}

	if (IsData[1] < IsNextLevel->AllData[10])
	{
		IsToolKit.ThisFont(a + 13, b + 162, 0xFFEECCE1, 0x880015FF, 242, 0, RT3_SORT_CENTER, GlobalText[4006], IsData[1] + 1);
		
	//	IsToolKit.ThisFont(a + 17, b + 162 + 12 * 1, 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4007], IsNextLevel->AllData[1], ItemNameBank[IsNextLevel->AllData[2]]);
	//	IsToolKit.ThisFont(a + 17, b + 162 + 12 * 2, 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4008], IsToolKit.QN(gJwBanking.m_Data.ItemBank[IsNextLevel->AllData[2]]), ItemNameBank[IsNextLevel->AllData[2]]);
	
		IsToolKit.ThisFont(a + 17, b + 142 + 12 * 3, 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4009], IsToolKit.QN(IsNextLevel->AllData[3]));
		//IsToolKit.ThisFont(a + 17, b + 162 + 12 * 4, 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4010], IsToolKit.QN(IsNextLevel->AllData[4]));
		//IsToolKit.ThisFont(a + 17, b + 162 + 12 * 5, 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4011], IsToolKit.QN(IsNextLevel->AllData[5]));
	}
	else
	{
		RenderBitmap((SEASON3B::CNewUIQuanHam::IMAGE_HD_LOGO_2), a + 139, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);
		IsToolKit.ThisFont(a, b + 130 + 12 * 5, 0xFF80C0FF, 0, 270, 0, RT3_SORT_CENTER, GlobalText[4013]);
	}
}
