#include "Stdafx.h"
#include "ACuoi_HonHoan.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <ZzzLodTerrain.h>
#include <MapManager.h>

cHonHoan gHonHoan;

static HonHoanRenderCache g_HonHoanEffectCache;

SEASON3B::CNewUIHonHoan::CNewUIHonHoan()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;

	gHonHoan.IsData[0] = 1;
	gHonHoan.IsData[2] = 50;
}

SEASON3B::CNewUIHonHoan::~CNewUIHonHoan()
{
	Release();
}

bool SEASON3B::CNewUIHonHoan::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENHONHOAN, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIHonHoan::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUIHonHoan::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 40;
}

bool SEASON3B::CNewUIHonHoan::Update()
{
	return true;
}
bool SEASON3B::CNewUIHonHoan::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	POINT ptExitBtn1 = { m_Pos.x + 248, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON))
	{
		if (CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16) || !CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		{
			gHonHoan.IsData[0] = 1;
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENHONHOAN);
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		return false;
	return true;
}

bool SEASON3B::CNewUIHonHoan::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENHONHOAN);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIHonHoan::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIHonHoan::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIHonHoan::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	this->SendStartLoad();

	RenderImage(IMAGE_BASE_WINDOW_BACK,		m_Pos.x + 1, m_Pos.y + 5, 268.f, 336.f);
	RenderImage(IMAGE_BASE_WINDOW_TOP,		m_Pos.x, m_Pos.y, 270.f, 64.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT,		m_Pos.x, m_Pos.y + 64.f, 21.f, 233.f);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT,	m_Pos.x + 249, m_Pos.y + 64.f, 21.f, 233.f);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM,	m_Pos.x, m_Pos.y + 297.f, 270.f, 45.f);

	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 10, 0xFFEECCE1, 0, 270, 0, RT3_SORT_CENTER, GlobalText[3791]);

	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 40, float(WINDOW_WIDTH) - 65, 120);
	g_pUIForm->RenderBack(m_Pos.x + 135, m_Pos.y + 40, float(WINDOW_WIDTH) - 65, 120);

	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 160, float(WINDOW_WIDTH) + 60, 75);
	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 237, float(WINDOW_WIDTH) + 60, 85);

	RenderText(m_Pos.x, m_Pos.y);

	if (gHonHoan.IsData[1] < gHonHoan.IsData[2])
	{
		m_Btn[0].Render();
	}

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIHonHoan::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 110, m_Pos.y + 288, 52, 26, 1, 0, 1, 1u, GlobalText[4012], "", 0);
}

bool SEASON3B::CNewUIHonHoan::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent() && gHonHoan.IsData[1] < gHonHoan.IsData[2])
	{
		this->SendPackButton();
		PlayBuffer(SOUND_CLICK01);
		gHonHoan.IsData[0] = 1;
		return true;
	}
	return false;
}

void SEASON3B::CNewUIHonHoan::LoadImages()
{
	LoadBitmap("Custom\\ThangCuoi\\RankTitle\\nodata.jpg", IMAGE_HD_LOGO_0, GL_LINEAR);
	for (int i = 0; i < gHonHoan.IsData[2]; i++)
	{
		char IsFile[100];
		sprintf(IsFile, "Custom\\ThangCuoi\\RankTitle\\HonHoan\\%d.jpg", i + 1);
		LoadBitmap(IsFile, IMAGE_HD_LOGO_1 + i, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}
}

void SEASON3B::CNewUIHonHoan::UnloadImages()
{
	for (int i = 0; i < gHonHoan.IsData[2]; i++)
	{
		DeleteBitmap(IMAGE_HD_LOGO_0 + i);
	}
}

GetDataHonHoan* GetStep(int SPK)
{
	std::map<int, GetDataHonHoan>::iterator it = gHonHoan.m_LoadData.find(SPK);
	if (it == gHonHoan.m_LoadData.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

void SEASON3B::CNewUIHonHoan::SendStartLoad()
{
	if (gHonHoan.IsData[0] == 1)
	{
		SendRequestDataSendType(0xD5, 0x01, 1);
		gHonHoan.IsData[0] = 2;
	}
}

void SEASON3B::CNewUIHonHoan::SendPackButton()
{
	SendRequestDataSendType(0xD5, 0x02, 1);
}

void cHonHoan::SendWsClient(BYTE* lpMsg)
{
	this->m_LoadData.clear();
	CountListHonHoan* mRecv = (CountListHonHoan*)lpMsg;
	for (int i = 0; i < mRecv->Count; i++)
	{
		GetDataHonHoan lpInfo = *(GetDataHonHoan*)(((BYTE*)lpMsg) + sizeof(CountListHonHoan) + (sizeof(GetDataHonHoan) * i));
		this->m_LoadData.insert(std::pair<int, GetDataHonHoan>(lpInfo.AllData[0], lpInfo));
	} 
	
	this->IsData[1]			= gRank.m_Data[HeroKey].rHonHoan;
	this->IsCurrentLevel	= GetStep(this->IsData[1]);
	this->IsNextLevel		= GetStep(this->IsData[1] + 1);
}

void SEASON3B::CNewUIHonHoan::RenderHonHoanEffect(CHARACTER* c, OBJECT* o)
{
	if (!g_bRenderImageLogo)
		return;

	if (gMapManager.InChaosCastle() == true)
		return;

	if (!o || o->Kind != KIND_PLAYER || SceneFlag != MAIN_SCENE || !o->Live)
		return;

	int mGetRankLevel = gRank.m_Data[c->Key].rHonHoan;

	if (mGetRankLevel <= 0)
		return;

	if (g_isCharacterBuff(o, eBuff_Cloaking))
		return;

	vec3_t vLight = { 1.0f, 1.0f, 1.0f };
	vec3_t vPos;

	VectorCopy(o->Position, vPos);

	EnableAlphaBlend();

	RenderTerrainAlphaBitmap((SEASON3B::CNewUIHonHoan::IMAGE_HD_LOGO_1 + mGetRankLevel) - 1,
		vPos[0], vPos[1], 2.5f, 2.5f, vLight, -WorldTime * 0.01f);

	DisableAlphaBlend();
}

void SEASON3B::CNewUIHonHoan::RenderText(int a, int b)
{
	if (!gHonHoan.IsCurrentLevel) return;

	if (gHonHoan.IsData[1] < gHonHoan.IsNextLevel->AllData[10] + 1)
	{
		if (gHonHoan.IsData[1] != 0)
		{		
			EnableAlphaBlend();
			RenderBitmap((SEASON3B::CNewUIHonHoan::IMAGE_HD_LOGO_1 + gHonHoan.IsData[1]) - 1, a + 13, b + 41, 114, 113, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
			DisableAlphaBlend();

			IsToolKit.ThisFont(a + 13, b + 162, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, GlobalText[4004], gHonHoan.IsData[1]);
			IsToolKit.ThisFont(a + 16, b + 177 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
			IsToolKit.ThisFont(a + 16, b + 177 + (0 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsCurrentLevel->AllData[6]));
			IsToolKit.ThisFont(a + 16, b + 177 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
			IsToolKit.ThisFont(a + 16, b + 177 + (1 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsCurrentLevel->AllData[7]));
			IsToolKit.ThisFont(a + 16, b + 177 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
			IsToolKit.ThisFont(a + 16, b + 177 + (2 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsCurrentLevel->AllData[8]));
			IsToolKit.ThisFont(a + 16, b + 177 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
			IsToolKit.ThisFont(a + 16, b + 177 + (3 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsCurrentLevel->AllData[9]));
		}
		else
		{
			RenderBitmap((SEASON3B::CNewUIHonHoan::IMAGE_HD_LOGO_0), a + 13, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);

			IsToolKit.ThisFont(a + 13, b + 162, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, GlobalText[4004], gHonHoan.IsData[1]);
			IsToolKit.ThisFont(a + 16, b + 177 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
			IsToolKit.ThisFont(a + 16, b + 177 + (0 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
			IsToolKit.ThisFont(a + 16, b + 177 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
			IsToolKit.ThisFont(a + 16, b + 177 + (1 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
			IsToolKit.ThisFont(a + 16, b + 177 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
			IsToolKit.ThisFont(a + 16, b + 177 + (2 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
			IsToolKit.ThisFont(a + 16, b + 177 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
			IsToolKit.ThisFont(a + 16, b + 177 + (3 * 13), 0x800000FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
		}

		if (gHonHoan.IsData[1] + 1 < gHonHoan.IsNextLevel->AllData[10] + 1)
		{
			EnableAlphaBlend();
			RenderBitmap(SEASON3B::CNewUIHonHoan::IMAGE_HD_LOGO_1 + gHonHoan.IsData[1], a + 138, b + 41, 114.f, 113.f, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
			DisableAlphaBlend();

			IsToolKit.ThisFont(a + 138, b + 162, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, GlobalText[4005], gHonHoan.IsData[1] + 1);
			
			IsToolKit.ThisFont(a + 141, b + 177 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
			IsToolKit.ThisFont(a + 141, b + 177 + (0 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[6]));
			IsToolKit.ThisFont(a + 141, b + 177 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
			IsToolKit.ThisFont(a + 141, b + 177 + (1 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[7]));
			IsToolKit.ThisFont(a + 141, b + 177 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
			IsToolKit.ThisFont(a + 141, b + 177 + (2 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[8]));
			IsToolKit.ThisFont(a + 141, b + 177 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
			IsToolKit.ThisFont(a + 141, b + 177 + (3 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[9]));
		}
	}

	if (gHonHoan.IsData[1] < gHonHoan.IsNextLevel->AllData[10])
	{
		IsToolKit.ThisFont(a + 13, b + 239, 0xFFEECCE1, 0x880015FF, 242, 0, RT3_SORT_CENTER, GlobalText[4006], gHonHoan.IsData[1] + 1);
		//IsToolKit.ThisFont(a + 17, b + 239 + (12 * 1), 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4007], gHonHoan.IsNextLevel->AllData[1], ItemNameBank[gHonHoan.IsNextLevel->AllData[2]]);
		//IsToolKit.ThisFont(a + 17, b + 239 + (12 * 2), 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4008], IsToolKit.QN(gJwBanking.m_Data.ItemBank[gHonHoan.IsNextLevel->AllData[2]]), ItemNameBank[gHonHoan.IsNextLevel->AllData[2]]);
		IsToolKit.ThisFont(a + 17, b + 220 + (12 * 3), 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4009], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[3]));
		//IsToolKit.ThisFont(a + 17, b + 239 + (12 * 4), 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4010], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[4]));
		//IsToolKit.ThisFont(a + 17, b + 239 + (12 * 5), 0xFFEECCE1, 0, 242, 0, RT3_SORT_LEFT, GlobalText[4011], IsToolKit.QN(gHonHoan.IsNextLevel->AllData[5]));
	}
	else
	{
		RenderBitmap((SEASON3B::CNewUIHonHoan::IMAGE_HD_LOGO_0), a + 139, b + 42, 116, 112, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);

		//IsToolKit.ThisFont(a + 138, b + 162, 0xFFEECCE1, 0x00A2E850, 116, 0, RT3_SORT_CENTER, "Level is Max");

		//IsToolKit.ThisFont(a + 141, b + 177 + (0 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4000]);
		//IsToolKit.ThisFont(a + 141, b + 177 + (0 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
		//IsToolKit.ThisFont(a + 141, b + 177 + (1 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4001]);
		//IsToolKit.ThisFont(a + 141, b + 177 + (1 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
		//IsToolKit.ThisFont(a + 141, b + 177 + (2 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4002]);
		//IsToolKit.ThisFont(a + 141, b + 177 + (2 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));
		//IsToolKit.ThisFont(a + 141, b + 177 + (3 * 13), 0xFFEECCE1, 0, 110, 0, RT3_SORT_LEFT, GlobalText[4003]);
		//IsToolKit.ThisFont(a + 141, b + 177 + (3 * 13), 0x00FF00FF, 0, 110, 0, RT3_SORT_RIGHT, GlobalText[3999], IsToolKit.QN(0));

		IsToolKit.ThisFont(a, b + 239 + 12 * 3, 0xFF80C0FF, 0, 270, 0, RT3_SORT_CENTER, GlobalText[4013]);
	}
}
