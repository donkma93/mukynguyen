#include "Stdafx.h"
#include "NewUIRenderDameBoss.h"
#include "NewUISystem.h"
#include "WSclientinline.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ThangCuoi\WideData.h"
#include "GuildCache.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include <CharacterManager.h>

CNewUIRenderDameBossExt gDmgBoss;

SEASON3B::CNewUIRenderDameBoss::CNewUIRenderDameBoss()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;

	gDmgBoss.DamageClear();

	gDmgBoss.MaxPosition.x = (static_cast<float>((640 - 150)) / 2) + gPosWide.x_GetAddPos;
	gDmgBoss.MaxPosition.y = 0;
}

SEASON3B::CNewUIRenderDameBoss::~CNewUIRenderDameBoss()
{
	Release();
}

bool SEASON3B::CNewUIRenderDameBoss::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENDAMEBOSS, this);

	LoadImages();

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIRenderDameBoss::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRenderDameBoss::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos + 200;
	m_Pos.y = y;

	gDmgBoss.m_PosDmg.x = x + gPosWide.x_GetAddPos + 200;
	gDmgBoss.m_PosDmg.y = y;
}

void SEASON3B::CNewUIRenderDameBoss::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\DameTop-Render.tga", IMAGE_BASE_WINDOW_DMG_TOP_1, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\DameTop-Render_Add.tga", IMAGE_BASE_WINDOW_DMG_TOP_2, GL_LINEAR);
}

void SEASON3B::CNewUIRenderDameBoss::UnloadImages()
{
	DeleteBitmap(IMAGE_BASE_WINDOW_DMG_TOP_1);
	DeleteBitmap(IMAGE_BASE_WINDOW_DMG_TOP_2);
}

bool SEASON3B::CNewUIRenderDameBoss::Update()
{
	return true;
}

bool SEASON3B::CNewUIRenderDameBoss::UpdateMouseEvent()
{
	POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENDAMEBOSS);
	}

	//if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT + 70))
	//	return false;
	return true;
}

bool SEASON3B::CNewUIRenderDameBoss::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENDAMEBOSS);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRenderDameBoss::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRenderDameBoss::GetKeyEventOrder()
{
	return 3.4;
}


bool SEASON3B::CNewUIRenderDameBoss::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gDmgBoss.Initz();

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIRenderDameBoss::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
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

void CNewUIRenderDameBossExt::DamageClear()
{
	for (int n = 0; n < 10; n++)
	{
		memset(SavedValue[n].szName, 0, sizeof(SavedValue[n].szName));
		SavedValue[n].Kill = 0;
	}
	this->CalcString;
}

int GetOffsetX(int count)
{
	switch (count)
	{
		case 2: return -10;
		case 3: return -20;
		case 4: return -30;
		case 5: return -40;
		case 6: return -50;
		case 7: return -60;
		case 8: return -70;
		case 9: return -80;
		case 10: return -90;
		default: return 0;
	}
}

bool CNewUIRenderDameBossExt::SetWindows()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_JEWELBANK)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADE)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_STORAGE)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY_EXT)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYSHOP_INVENTORY)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_STORAGE_EXT)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER_EXT)
	 || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER_SKILL_LIST)
	)
	{
		return false;
	}
	return true;
}
void CNewUIRenderDameBossExt::Initz()
{
	if (!SetWindows())
		return;

	if (Hero->SafeZone)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_EVENDAMEBOSS);
			return;
	}

	float ScaleHeight = (gDmgBoss.CalcString * 10) + 15;

	switch (m_Resolution)
	{
		case 6:		
			gDmgBoss.m_PosDmg.y = 394.f; 
			gDmgBoss.m_PosDmg.x = gDmgBoss.MaxPosition.x + gPosWide.x_GetAddPos + 130; break;
			break;
		case 7:		
			gDmgBoss.m_PosDmg.y = 395.f; 
			gDmgBoss.m_PosDmg.x = gDmgBoss.MaxPosition.x + gPosWide.x_GetAddPos + 130; 
			break;
		case 8:		
			gDmgBoss.m_PosDmg.y = 395.f; 
			gDmgBoss.m_PosDmg.x = gDmgBoss.MaxPosition.x + gPosWide.x_GetAddPos + 100; break;
		case 9:		
			gDmgBoss.m_PosDmg.y = 395.f; 
			gDmgBoss.m_PosDmg.x = gDmgBoss.MaxPosition.x + gPosWide.x_GetAddPos + 55; break;
		default:	
		gDmgBoss.m_PosDmg.y = 393.f; 
		gDmgBoss.m_PosDmg.x = gDmgBoss.MaxPosition.x + gPosWide.x_GetAddPos + 130; break;
		break;
	}
	
	float tooltipWidth = 148.0f;
	float lineHeight = 10.0f;
	float baseHeight = 15.0f;
	float tooltipHeight = baseHeight + (gDmgBoss.CalcString - 1) * lineHeight;

	IsToolKit.RenderToolTip(
		gDmgBoss.m_PosDmg.x - 235,
		(gDmgBoss.m_PosDmg.y + 20.0f)- (tooltipHeight - baseHeight),
		tooltipWidth,
		tooltipHeight
	);

	int offsetX = GetOffsetX(gDmgBoss.CalcString);
	DWORD ColorTextRank = 0xC0C0C0FF;

	for (int i = 0; i < gDmgBoss.CalcString; i++)
	{
		if (i % 2)
		{
			ColorTextRank = 0xC0C0C0FF;
		}
		else
		{
			ColorTextRank = 0xC0C0C0FF;
		}
		if (i < 3)
		{
			ColorTextRank = 0xFFCC66FF;
		}
	
		int nameOffsetX = 4;

		CHARACTER* pChar = nullptr;
		const char* szTargetName = gDmgBoss.SavedValue[i].szName;

		for (int j = 0; j < MAX_CHARACTERS_CLIENT; ++j)
		{
			CHARACTER* c = &CharactersClient[j];
			if (c->Key != 0 && strncmp(c->ID, szTargetName, sizeof(c->ID)) == 0)
			{
				pChar = c;
				break;
			}
		}

		if (pChar && pChar->GuildMarkIndex >= 0 &&
			(pChar->GuildMarkIndex == MARK_EDIT || g_GuildCache.IsExistGuildMark(GuildMark[pChar->GuildMarkIndex].Key)))
		{
			EnableAlphaTest();
			glColor4f(1.f, 1.f, 1.f, 1.f);
			CreateGuildMark(pChar->GuildMarkIndex);
			RenderBitmap(BITMAP_GUILD, gDmgBoss.m_PosDmg.x - 233, gDmgBoss.m_PosDmg.y + 25 + offsetX + (i * 10), 8, 8);
		}

		IsToolKit.ThisFont(gDmgBoss.m_PosDmg.x - 223, gDmgBoss.m_PosDmg.y + 25 + offsetX + (i * 10), ColorTextRank, 0, 150, 0, 1, "Top%d - %s", i + 1, gDmgBoss.SavedValue[i].szName);
		IsToolKit.ThisFont(gDmgBoss.m_PosDmg.x - 237, gDmgBoss.m_PosDmg.y + 25 + offsetX + (i * 10), ColorTextRank, 0, 150, 0, 4, "%s", IsToolKit.QN(gDmgBoss.SavedValue[i].Kill));
	}
}
void CNewUIRenderDameBossExt::DamageGetInfo(const BYTE* lpMsg)
{
	auto mRecv = reinterpret_cast<const RecvLifeCount*>(lpMsg);

	CalcString = 0;
	IndexMonster = 0;
	MaxLife = 0;
	MinLife = 0;
	Level = 0;
	Rate = 0;

	for (int n = 0; n < 10; n++)
	{
		memset(SavedValue[n].szName, 0, sizeof(SavedValue[n].szName));
		SavedValue[n].Kill = 0;
	}

	for (int n = 0; n < mRecv->count; n++)
	{
		if (n >= 10) break;

		auto lpInfo = reinterpret_cast<const GetValueFromGs*>(
			lpMsg + sizeof(RecvLifeCount) + sizeof(GetValueFromGs) * n
			);

		memcpy(SavedValue[n].szName, lpInfo->szName, sizeof(SavedValue[n].szName));
		SavedValue[n].Kill = lpInfo->Kill;

		CalcString++;

		IndexMonster = lpInfo->monsterid;
		MaxLife = lpInfo->MaxLife;
		Level = lpInfo->Level;
		MinLife = lpInfo->MinLife;
		Rate = lpInfo->Rate;
	}
}


void CNewUIRenderDameBossExt::SendProtocolClient(BYTE* ReceiveBuffer)
{
	auto* mRecv = (SolvedDameBoss*)ReceiveBuffer;

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EVENDAMEBOSS))
	{
		return;
	}
	else
	{
		g_pNewUIMenuOption->Toggle(SEASON3B::INTERFACE_EVENDAMEBOSS);
	}
}