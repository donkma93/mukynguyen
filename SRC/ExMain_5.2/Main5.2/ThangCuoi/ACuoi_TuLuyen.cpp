#include "Stdafx.h"
#include "ACuoi_TuLuyen.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

cTuLuyen gTuLuyen;

SEASON3B::CNewUITuLuyen::CNewUITuLuyen()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	gTuLuyen.IsData[0] = 1;
	gTuLuyen.IsData[2] = 50;

	WINDOW_WIDTH = 270;
	WINDOW_HEIGHT = 200;
}

SEASON3B::CNewUITuLuyen::~CNewUITuLuyen()
{
	Release();
}
bool SEASON3B::CNewUITuLuyen::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENTULUYEN, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUITuLuyen::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUITuLuyen::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUITuLuyen::Update()
{
	return true;
}
bool SEASON3B::CNewUITuLuyen::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 20, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENTULUYEN);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUITuLuyen::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENTULUYEN);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUITuLuyen::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUITuLuyen::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUITuLuyen::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gTuLuyen.SendStartLoad();

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH + 20, (float)WINDOW_HEIGHT, GlobalText[3794]);

	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 40, float(WINDOW_WIDTH) - 145, 120);
	g_pUIForm->RenderBack(m_Pos.x + 135, m_Pos.y + 40, float(WINDOW_WIDTH) - 145, 120);
	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 160, float(WINDOW_WIDTH) - 20, 85);

	gTuLuyen.Render(m_Pos.x, m_Pos.y);

	if (gTuLuyen.IsData[1] < gTuLuyen.IsData[2])
	{
		m_Btn[0].Render();
	}

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUITuLuyen::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 110, m_Pos.y + 211, 52, 26, 1, 0, 1, 1u, GlobalText[4012], "", 0);
}

bool SEASON3B::CNewUITuLuyen::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent() && gTuLuyen.IsData[1] < gTuLuyen.IsData[2])
	{
		gTuLuyen.SendPackButton();
		PlayBuffer(SOUND_CLICK01);
		gTuLuyen.IsData[0] = 1;
		return true;
	}
	return false;
}

void SEASON3B::CNewUITuLuyen::LoadImages()
{
	LoadBitmap("Custom\\ThangCuoi\\RankTitle\\nodata.jpg", IMAGE_HD_LOGO_3, GL_LINEAR);

	for (int i = 0; i < gTuLuyen.IsData[2]; i++)
	{
		char IsFile[100];
		sprintf(IsFile, "Custom\\ThangCuoi\\RankTitle\\TuLuyen\\%d.tga", i + 1);
		LoadBitmap(IsFile, IMAGE_HD_LOGO_START3 + i, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}
}
void SEASON3B::CNewUITuLuyen::UnloadImages()
{
	for (int i = 0; i < gTuLuyen.IsData[2]; i++)
	{
		DeleteBitmap(IMAGE_HD_LOGO_3 + i);
	}
}

GetDataTuLuyen* GetStep(int SPK)
{
	std::map<int, GetDataTuLuyen>::iterator it = gTuLuyen.m_LoadData.find(SPK);
	if (it == gTuLuyen.m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

void cTuLuyen::SendStartLoad()
{
	if (this->IsData[0] == 1)
	{
		SendRequestDataSendType(0xD5, 0x09, 1);
		this->IsData[0] = 2;
	}
}
void cTuLuyen::SendPackButton()
{
	SendRequestDataSendType(0xD5, 0x10, 1);
}

void cTuLuyen::SendWsClient(BYTE* lpMsg)
{
	this->m_LoadData.clear();
	CountListTuLuyen* mRecv = (CountListTuLuyen*)lpMsg;
	for (int i = 0; i < mRecv->Count; i++)
	{
		GetDataTuLuyen lpInfo = *(GetDataTuLuyen*)(((BYTE*)lpMsg) + sizeof(CountListTuLuyen) + (sizeof(GetDataTuLuyen) * i));
		this->m_LoadData.insert(std::pair<int, GetDataTuLuyen>(lpInfo.AllData[0], lpInfo));
	}

	this->IsData[1] = gRank.m_Data[HeroKey].rTuLuyen;
	this->IsCurrentLevel = GetStep(this->IsData[1]);
	this->IsNextLevel = GetStep(this->IsData[1] + 1);
}

void cTuLuyen::Render(int a, int b)
{
	if (!this->IsCurrentLevel) return;

	if (this->IsData[1] < this->IsNextLevel->AllData[10] + 1)
	{
		if (this->IsData[1] != 0)
		{
			RenderBitmap((SEASON3B::CNewUITuLuyen::IMAGE_HD_LOGO_START3 + this->IsData[1]) - 1, a - 9, b + 55, 160, 40, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);

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
			RenderBitmap((SEASON3B::CNewUITuLuyen::IMAGE_HD_LOGO_3), a + 13, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);
		}
		if (this->IsData[1] + 1 < this->IsNextLevel->AllData[10] + 1)
		{
			RenderBitmap(SEASON3B::CNewUITuLuyen::IMAGE_HD_LOGO_START3 + this->IsData[1], a + 116, b + 55, 160, 40, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);

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
		RenderBitmap((SEASON3B::CNewUITuLuyen::IMAGE_HD_LOGO_3), a + 139, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);
		IsToolKit.ThisFont(a, b + 130 + 12 * 5, 0xFF80C0FF, 0, 270, 0, RT3_SORT_CENTER, GlobalText[4013]);
	}
}
